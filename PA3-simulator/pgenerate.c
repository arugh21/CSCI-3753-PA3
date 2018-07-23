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

int main(){

	//Variables
	FILE* sequence;
	char buffer[1025];
	char* pair;
	char pagestr[2] = " ";
	char procstr[2] = " ";
	struct procsummary processes[MAXPROCESSES];
	float pdist[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES];
	int proctmp;
	int pagetmp;
	float divisor;
	float dividend;

	sequence = fopen("testsequence.txt","r");
	if(sequence == NULL) fputs("File error",stderr);

	for(int i=0;i<MAXPROCESSES;i++){
		processes[i].lastpage = -1;
		processes[i].currentpage = -1;
		for(int j=0;j<MAXPROCPAGES;j++){
			processes[i].pagecount[j] = 0;
			for(int k=0;k<MAXPROCPAGES;k++){
				processes[i].nextpages[j][k] = 0;
				//printf("%d\n",processes[i].nextpages[j][k]);
				pdist[i][j][k] = 0.0;
			}
		}
	}

	while(fgets(buffer,5,sequence)!=NULL){
		pair = strtok(buffer, ",\n");
		while(pair!=NULL){
			//printf("%s\n",pair);
			procstr[0] = pair[0];
			pagestr[0] = pair[2];
			//printf("%s ",procstr);
			//printf("%s\n",pagestr);
			proctmp = atoi(procstr);
			pagetmp = atoi(pagestr);
			if((0<=proctmp && proctmp<=20) && (0<=pagetmp && proctmp<=20)){
				//printf("%d ",proctmp);
				//printf("%d\n",pagetmp);
				processes[proctmp].currentpage = pagetmp;
				processes[proctmp].pagecount[pagetmp]++;
				if(processes[proctmp].lastpage != -1){
					processes[proctmp].nextpages[processes[proctmp].lastpage][pagetmp]++;
				}
				processes[proctmp].lastpage = processes[proctmp].currentpage;
				printf("Current page: %d, page count: %d\n",processes[proctmp].currentpage,processes[proctmp].pagecount[pagetmp]);
			}
			//printf("%c %c\n",proctmp,pagetmp);
			pair = strtok(NULL,", \n");
		}
	}
	printf("Last page used: %d\n",processes[proctmp].currentpage);	
	processes[proctmp].pagecount[pagetmp]--;

	
	for(int i=0;i<2;i++){
		for(int j=0;j<MAXPROCPAGES;j++){
			for(int k=0;k<MAXPROCPAGES;k++){
				if(processes[i].pagecount[j]>0){
					dividend = processes[i].nextpages[j][k];
					divisor = processes[i].pagecount[j];
					//printf("%f divided by %f\n",dividend,divisor);
					pdist[i][j][k] = dividend/divisor ;
					//printf("%f\n",pdist[i][j][k]);
				}
			}
		}
	}
	
		
	for(int i=0;i<2;i++){
		printf("Process %d Summary: \n\n",i);
		for(int j=0;j<MAXPROCPAGES;j++){
			printf("Page %d:\n",j);
			printf("Times used: %d\n",processes[i].pagecount[j]);
			for(int k=0;k<MAXPROCPAGES;k++){
				printf("%d: %d,%f ",k,processes[i].nextpages[j][k],pdist[i][j][k]);
			}
			printf("\n");
		}
		printf("\n\n");
	}
	
	fclose(sequence);
	return 0;

}
