//Header file for probability integraion function//

#define MAXPROCESSES 20
#define MAXPROCPAGES 20

struct procinfo {
	int lastpage;
	int currentpage;
	int pagecount[MAXPROCPAGES];
	int nextpages[MAXPROCPAGES][MAXPROCPAGES];
};

int pintegration(int process, int page, float pdist[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES]);
