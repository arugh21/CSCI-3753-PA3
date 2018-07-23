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

#include "simulator.h"

struct ptrackinfo {
	int firstchance;
	int timestamp;
};

void pageit(Pentry q[MAXPROCESSES]) {

    /* This file contains the stub for a predictive pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static struct ptrackinfo pageinfo [MAXPROCESSES][MAXPROCPAGES];
    /* Local vars */
    int proctmp;
    int pagetmp;
    int minproc = 0;
    int minpage = 0;
    int ticktmp = 0;

    /* initialize static vars on first run */
    if(!initialized){
	/* Init complex static vars here */
	for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++){
		for(pagetmp=0;pagetmp<MAXPROCPAGES;pagetmp++){
			pageinfo[proctmp][pagetmp].firstchance = 0;
			pageinfo[proctmp][pagetmp].timestamp = 0;
		}
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
	//Check if page is already swapped in and attempt to swap it in
		if(!q[proctmp].pages[pagetmp]){
			if(!pagein(proctmp,pagetmp)){
				minproc = -1;
				minpage = -1;
				ticktmp = tick;
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
					break;
				}
				if(pageinfo[minproc][minpage].firstchance == 0){
					pageinfo[minproc][minpage].firstchance = 1;
					break;
				}
				if(!pageout(minproc,minpage)){
					break;
				}
				//printf("Page %d, process %d swapped out",minproc,minpage);
			}
			else{
				pageinfo[proctmp][pagetmp].timestamp = tick;
			}
			
		}
		else{
			pageinfo[proctmp][pagetmp].timestamp = tick;
			pageinfo[proctmp][pagetmp].firstchance = 0;
		}
	}
    }


    /* advance time for next pageit iteration */
    tick++;
}
