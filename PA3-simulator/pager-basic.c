/*
 * File: pager-basic.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a simple pageit
 *      implmentation at performs very poorly, but
 *      provided a basic idea of how to user the
 *      simulator interface. Your job is to improve
 *      upon this implmentation.
 */
#include <stdio.h>
#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

    /* Local vars */
    int proc;
    int pc;
    int page;
    int oldpage;
    int pagetrack = -1;
    int proctrack = 0;

    FILE* write_file;
    write_file = fopen("basicsequences.txt","a");

    /* Trivial paging strategy */
    /* Select first active process */
    for(proc=0; proc<MAXPROCESSES; proc++) {
	/* Is process active? */
	if(q[proc].active) {
	    if(!(proc == proctrack)){
		    fprintf(write_file,"\n");
		    proctrack = proc;
	    }
	    /* Dedicate all work to first active process*/
	    pc = q[proc].pc; 		        // program counter for process
	    page = pc/PAGESIZE;	    // page the program counter needs
	    if(!(page == pagetrack)){
		    fprintf(write_file,"%d ",page);
		    pagetrack = page;
	    }
	    /* Is page swaped-out? */
	    if(!q[proc].pages[page]) {
		/* Try to swap in */
		if(!pagein(proc,page)) {
		    /* If swapping fails, swap out another page */
		    for(oldpage=0; oldpage < q[proc].npages; oldpage++) {
	 		/* Make sure page isn't one I want */
			if(oldpage != page) {
			    /* Try to swap-out */
			    if(pageout(proc,oldpage)) {
				/* Break loop once swap-out starts*/
				break;
			    }
			}
		    }
		}
	    }
	    fclose(write_file);
	    /* Break loop after finding first active process */
	    break;
	}
    }
}
