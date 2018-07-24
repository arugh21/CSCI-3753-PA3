/*Pgenerate
 * Function to break sequences.txt into usable information and generate probability 
 * distribution for each of 400 possible pages in use by the simulator.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXPROCESSES 20
#define MAXPROCPAGES 20

struct procsummary {
	int lastpage;
	int currentpage;
	int pagecount[MAXPROCPAGES];
	int nextpages[MAXPROCPAGES][MAXPROCPAGES];
};

int pintegrate(int process,int page,float pdist[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES]){
	//Variables
	static struct procsummary processes[MAXPROCESSES];
	static int initialized = 0;
	float divisor;
	float dividend;

	if(!initialized){
		for(int i=0;i<MAXPROCESSES;i++){
			processes[i].lastpage = -1;
			processes[i].currentpage = -1;
			for(int j=0;j<MAXPROCPAGES;j++){
				processes[i].pagecount[j] = 0;
				for(int k=0;k<MAXPROCPAGES;k++){
					processes[i].nextpages[j][k] = 0;
				}
			}
		}
		initialized = 1;
	}

	//printf("Current process: %d\n",process);
	processes[process].currentpage = page;
	//printf("Current page: %d\n",processes[process].currentpage);
	processes[process].pagecount[page]++;
	//printf("Current pagecount: %d\n",processes[process].pagecount[page]);
	if(processes[process].lastpage != -1){
		processes[process].nextpages[processes[process].lastpage][page]++;
	}
	processes[process].lastpage = processes[process].currentpage;

	divisor = processes[process].pagecount[page];
	if(divisor <=0){
		return 0;
	}
	//printf("Divisor: %f\n",divisor);
	//printf("Updating prob for process %d, page %d\n",process,page);
	for(int i=0;i<MAXPROCPAGES;i++){
		dividend = processes[process].nextpages[page][i];
		//printf("\nDividend: %f",dividend);
		pdist[process][page][i] = dividend/divisor;
		//printf(" %d: %f ",i,pdist[process][page][i]);
	}
	//printf("\n");

	return 0;

}
