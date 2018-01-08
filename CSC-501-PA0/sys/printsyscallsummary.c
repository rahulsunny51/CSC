#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>

extern struct pentry proctab[]; 
extern int ctr1000;
void startsyscallsummary();
void stopsyscallsummary();

void printsyscallsummary()
{
	printf("\n void printsyscallsummary() \n");
	char proc_names[27][10]={"freemem","chprio","getpid","getprio","gettime","kill","receive","recvclr","recvtim","resume","scount","sdelete","send", "setdev","setnok","screate","signal","signaln","sleep","sleep10","sleep100","sleep1000","sreset","stacktrace","suspend","unsleep","wait"};
		int i=0,j=0,flag=0;
		for (i=0;i<NPROC;i++)
		{
			if(isbadpid(i) || proctab[i].pname == "")
			{
				continue;
			}
			else
			{
				for (j=0;j<27;j++)
				{
					if(proctab[i].sys_summary[j][0] !=0)
					{	
						if (flag==0)
						{
						printf("Process [pid:%d] \n",i);
						flag=1;
						}
						else
						{
							printf("	Syscall: %s , count: %d , average execution time : %d (ms) \n", proc_names[j], proctab[i].sys_summary[j][0], proctab[i].sys_summary[j][1]/proctab[i].sys_summary[j][0]);
						}
					}

				}
			flag=0;
			}
		}

	return 0;
}

void startsyscallsummary()
{
	
	int i=0,j=0,k=0;
	for(i=0;i<NPROC;i++)
	{
		for(j=0;j<27;j++)
		{
			{
				for(k=0;k<2;k++)
				{
					proctab[i].sys_summary[j][k]=0;
				}
			}	
		}
	}

}
void stopsyscallsummary()
{
	int i=0,j=0,k=0;
        for(i=0;i<NPROC;i++)
        {
                for(j=0;j<27;j++)
                {
                        {
                                for(k=0;k<2;k++)
                                {
                                        proctab[i].sys_summary[j][k]=0;
                                }
                        }
                }
        }

		
}	
