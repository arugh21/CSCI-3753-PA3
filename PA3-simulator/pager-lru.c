/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    /* Local vars */
    int proctmp;
    int pagetmp;
    int minproc = 0;
    int minpage = 0;
    int ticktmp;

    /* initialize static vars on first run */
    if(!initialized){
	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
	    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
		timestamps[proctmp][pagetmp] = 0;
	    }
	}
	initialized = 1;
    }

    /* DONE: Implement LRU Paging */
    for(proctmp=0;proctmp<MAXPROCESSES; proctmp++){
	if(q[proctmp].active){
		//Find current page for active process, check to make sure there isn't an actual page fault
		pagetmp = q[proctmp].pc/PAGESIZE;
		if(pagetmp > MAXPROCPAGES){
			printf("Page Fault");
			exit(EXIT_FAILURE);
		}
		//Check if the page is already in and attempt to page it in if not active
		if(!q[proctmp].pages[pagetmp]){
			if(!pagein(proctmp,pagetmp)){
				/*Find the process which is both active and has the lowest timestamp, and swap it out*/
				minproc = -1;
				minpage = -1;
				ticktmp = tick;
				for(int i=0;i<MAXPROCESSES;i++){
					for(int j=0;j<MAXPROCPAGES;j++){
						if((timestamps[i][j]<ticktmp) && (q[i].pages[j])){
							ticktmp = timestamps[i][j];
							minproc = i;
							minpage = j;
						}
					}
				}
				if(minproc == -1 || minpage == -1){
					break;
				}
				if(!pageout(minproc,minpage)){
					printf("Ruh roh, pageout did not work with process %d, page %d\n",minproc,minpage);
					break;
					//exit(EXIT_FAILURE);
				}
				else{
					//printf("Process %d, Page %d evicted\n",minproc,minpage);
				}
		/*Set the timestamp to the current tick to track when it is used*/
			}
			else{
			//printf("Process %d page %d swapped in\n",proctmp,pagetmp);
			timestamps[proctmp][pagetmp] = tick;
			}
		}
		else{
			timestamps[proctmp][pagetmp] = tick;
		}
		
	}

    }

    /* advance time for next pageit iteration */
    tick++;
    //printf("pageit() called %d times",tick);
}
