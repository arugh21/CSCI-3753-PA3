/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>

#include "simulator.h"

struct ptrackinfo {
	int firstchance;
	int timestamp;
};

int LRUevict(int tick, struct ptrackinfo pageinfo[MAXPROCESSES][MAXPROCPAGES],Pentry q[MAXPROCESSES]){
	int minproc = -1;
	int minpage = -1;
	int ticktmp = tick;

	for(int i=0;i<MAXPROCESSES;i++){
		for(int j=0;j<MAXPROCPAGES;j++){
			//printf("Timestamp: %d",pageinfo[i][j].timestamp);
			if((pageinfo[i][j].timestamp < ticktmp) && q[i].pages[j]){
				ticktmp = pageinfo[i][j].timestamp;
				minproc = i;
				minpage = j;
			}
		}
	}
	if(minproc == -1 || minpage == -1){
		return -1;
	}
	if(pageinfo[minproc][minpage].firstchance == 0){
		pageinfo[minproc][minpage].firstchance = 1;
		return -1;
	}
	if(!pageout(minproc,minpage)){
		return -1;
	}
	return 0;
}
				

void pageit(Pentry q[MAXPROCESSES]) {

    /* This file contains the stub for a predictive pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static struct ptrackinfo pageinfo [MAXPROCESSES][MAXPROCPAGES];
    static int recordingno[MAXPROCESSES];

    /* Local vars */
    int proctmp;
    int pagetmp;
    bool printline = false;

    FILE* write_file;
    write_file = fopen("sequences.txt","a");

    /* initialize static vars on first run */
    if(!initialized){
	/* Init complex static vars here */
	for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++){
		for(pagetmp=0;pagetmp<MAXPROCPAGES;pagetmp++){
			pageinfo[proctmp][pagetmp].firstchance = 0;
			pageinfo[proctmp][pagetmp].timestamp = 0;
		}
		recordingno[proctmp] = -1;
	}
	initialized = 1;
    }


    /* TODO: Predictive paging */
    for(proctmp=0;proctmp<MAXPROCESSES;proctmp++){
	if(q[proctmp].active){
		//Find current active process, check to ensure no actual page fault
		pagetmp = q[proctmp].pc/PAGESIZE;
		if(pagetmp > MAXPROCPAGES){
			printf("Page Fault");
			exit(EXIT_FAILURE);
		}
		
		if(!(pagetmp == recordingno[proctmp])){
			fprintf(write_file,"%d %d,",proctmp,pagetmp);
			recordingno[proctmp] = pagetmp;
			printline = true;
		}
	//Check if page is already swapped in and attempt to swap it in
		if(!q[proctmp].pages[pagetmp]){
			if(!pagein(proctmp,pagetmp)){
				if (!LRUevict(tick,pageinfo,q)){
						break;
				}
			}
			else{
				pageinfo[proctmp][pagetmp].timestamp = tick;
			}
			
		}
		else{
			pageinfo[proctmp][pagetmp].timestamp = tick;
			pageinfo[proctmp][pagetmp].firstchance = 0;
		}
		if(!q[proctmp].pages[pagetmp+1]){
			if(!pagein(proctmp,(pagetmp+1))){
				if(!LRUevict(tick,pageinfo,q)){
					break;
				}
			}
		}
		else{
			pageinfo[proctmp][pagetmp+1].timestamp = tick;
			pageinfo[proctmp][pagetmp+1].firstchance = 0;
		}
	}
    }
    if(printline){
	    fprintf(write_file,"\n");
	    printline = false;
    }
    fclose(write_file);
    /* advance time for next pageit iteration */
    tick++;
}
