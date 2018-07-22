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

    /*Output file for writing page sequence*/
    //FILE* write_file;

    //write_file = fopen("output.txt","a");

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
		//printf("Paging for process %d\n",proctmp);
		pagetmp = q[proctmp].pc/PAGESIZE;
		if(pagetmp > MAXPROCPAGES){
			printf("Page Fault");
			exit(EXIT_FAILURE);
		}
		//fprintf(write_file,"%d %d,",proctmp,pagetmp);
		if(!q[proctmp].pages[pagetmp]){
			//fprintf(write_file,"%d %d,",proctmp,pagetmp);
			if(!pagein(proctmp,pagetmp)){
				/*Find the process which is both active and has the lowest timestamp, and swap it out*/
				//fprintf(write_file,"%d %d,",proctmp,pagetmp);
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

    //fprintf(stderr, "pager-lru not yet implemented. Exiting...\n");
    //exit(EXIT_FAILURE);

    /* advance time for next pageit iteration */
    tick++;
    //fclose(write_file);
    //printf("pageit() called %d times",tick);
}
