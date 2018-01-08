#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>

extern struct pentry proctab[]; 
extern int numproc;
extern int currpid;



void printprocstks(int priority)
{
	printf(" \n void printprocstks(int priority) \n");
	
	int i=0;
	for( i=0;i<NPROC;i++)
	{
		if(proctab[i].pprio>priority && proctab[i].pname !="")
		{
			
			printf("Process [%s]\n", proctab[i].pname);
			printf("	pid: %d\n", i);
			printf("	priority: %d\n", proctab[i].pprio);
			printf("	base: 0x%08x\n", proctab[i].pbase);
			printf("	limit: 0x%08x\n", proctab[i].plimit);
			printf("	len: 0x%08x\n", proctab[i].pstklen);
			printf("	pointer: 0x%08x\n", proctab[i].pesp);
		}
	}
	return 0;
}


