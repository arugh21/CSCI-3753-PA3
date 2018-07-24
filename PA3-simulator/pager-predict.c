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
#include <stdbool.h>

#include "pgenfunction.h"
#include "simulator.h"

struct ptrackinfo {
	int firstchance;
	int timestamp;
};

int pintegrate(int process, int page, float pdist[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES]);

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

int pagepredict(int process, int page, float pdist[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES]){
	int nextlikelypage=-1;
	float maxP=0;
	for(int i=0;i<MAXPROCPAGES;i++){
		if (pdist[process][page][i]>maxP){
			maxP = pdist[process][page][i];
			nextlikelypage = i;
		}
	}
	if(nextlikelypage == -1){
		//printf("Couldn't find Next Page");
	}
	return nextlikelypage;
}
				

void pageit(Pentry q[MAXPROCESSES]) {

    /* This file contains the stub for a predictive pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static struct ptrackinfo pageinfo [MAXPROCESSES][MAXPROCPAGES];
    static int recordingno[MAXPROCESSES];
    static float pdist[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES];
    static int predictive = 0;

    /* Local vars */
    int proctmp;
    int pagetmp;
    int nlp;
    //bool printline = false;

    //FILE* write_file;
    //write_file = fopen("sequences.txt","a");

    /* initialize static vars on first run */
    if(!initialized){
	/* Init complex static vars here */
	for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++){
		for(pagetmp=0;pagetmp<MAXPROCPAGES;pagetmp++){
			pageinfo[proctmp][pagetmp].firstchance = 0;
			pageinfo[proctmp][pagetmp].timestamp = 0;
			for(int i=0;i<MAXPROCPAGES;i++){
				pdist[proctmp][pagetmp][i] = 0.0;
			}
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
		
		/*
		if(!(pagetmp == recordingno[proctmp])){
			fprintf(write_file,"%d %d,",proctmp,pagetmp);
			recordingno[proctmp] = pagetmp;
			//printline = true;
		}
		*/

	//Check if page is already swapped in and attempt to swap it in
		if(!q[proctmp].pages[pagetmp]){
			if(!pagein(proctmp,pagetmp)){
				if (!LRUevict(tick,pageinfo,q)){
						break;
				}
			}
			else{	
				if(!(pagetmp == recordingno[proctmp]) && !(predictive)){
					pintegrate(proctmp,pagetmp,pdist);
					recordingno[proctmp] = pagetmp;
				}
				pageinfo[proctmp][pagetmp].timestamp = tick;
			}
			
		}
		else{
			if(!(pagetmp == recordingno[proctmp]) && !(predictive)){
				pintegrate(proctmp,pagetmp,pdist);
				recordingno[proctmp] = pagetmp;
			}
			pageinfo[proctmp][pagetmp].timestamp = tick;
			pageinfo[proctmp][pagetmp].firstchance = 0;
		}
		
		if(predictive){
			nlp = pagepredict(proctmp,pagetmp,pdist);
			if(nlp == -1){
				break;
			}
		}
		else{
			if(pagetmp == 19){
				nlp = 0;
			}
			else{
				nlp = pagetmp+1;
			}
		}
			
		//nlp = pagetmp+1;
		if(!q[proctmp].pages[nlp]){
			if(!pagein(proctmp,(nlp))){
				if(!LRUevict(tick,pageinfo,q)){
					break;
				}
			}
		}
		else{
			pageinfo[proctmp][nlp].timestamp = tick;
			pageinfo[proctmp][nlp].firstchance = 0;
		}
	}
    }
    /*if(printline){
	    fprintf(write_file,"\n");
	    printline = false;
    }*/

    //fclose(write_file);
    /* advance time for next pageit iteration */
    tick++;
    if(!predictive){
	    if(tick >= 75000) predictive = 1;
    }
    /*
    if(tick == 200000){
	    for(int i=0;i<MAXPROCESSES;i++){
		    printf("Process %d\n",i);
		    for(int j=0;j<MAXPROCPAGES;j++){
			    printf("\nPage %d\n",j);
			    for(int k=0;k<MAXPROCPAGES;k++){
				    printf("%d: %f   ",k,pdist[i][j][k]);
			    }
		    }
	    }
    }
    */
    //printf("%d\n",tick);
}
