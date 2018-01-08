/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include<lab1.h>
unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
void create_epoch()
{
	int i=0;
	for( i=0;i<NPROC;i++)
	{
		if(proctab[i].pstate == PRFREE)
			continue;
		if(proctab[i].counter>0)
		{
			proctab[i].counter=(proctab[i].counter >>1)+ proctab[i].pprio;
		}
		else
		{
			proctab[i].counter=proctab[i].pprio;
			proctab[i].goodness=proctab[i].counter+proctab[i].pprio;
		}
	}
}

int setEpochFlag(a)
{
	int i=0;
	for(i=0;i<NPROC;i++)
                {
                        if((proctab[i].pstate==PRREADY) && (proctab[i].counter>0))
                        {
                                a=0;
                                break;
                        }
                }
	return a;
			
}
int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */
	int sched_type=getschedclass();
	/* no switch needed if current process priority higher than next*/
	if(sched_type==0)
	{
	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) {
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
	}
	else if(sched_type==2)
	{	
		int i=0;
		int flag_epoch=1,maxgoodness=0;
		optr=&proctab[currpid];
		if(preempt<=0)
		{
			preempt=0;
			proctab[currpid].counter=0;
			proctab[currpid].goodness=0;
		}
		proctab[currpid].goodness = proctab[currpid].goodness - proctab[currpid].counter + preempt;
		optr->counter=preempt;
		flag_epoch=setEpochFlag(flag_epoch);
		if(flag_epoch==1)
		{
			if((optr->pstate== PRCURR) && (optr->counter>0))
			{
				preempt=optr->counter;
			}
			else
			{
				create_epoch();
			}
		}
		int next;
		next=currpid;
		for(i=0;i<NPROC;i++)
		{
			if((proctab[i].pstate== PRREADY) && (proctab[i].goodness > maxgoodness))
			{
				maxgoodness=proctab[i].goodness;
				next=i;
			}
		}
		if((maxgoodness < optr->goodness)&&(optr->pstate == PRCURR))
			{
				preempt = proctab[currpid].counter;
				return OK;
			}
		else if(proctab[next].counter>0)
			{
				if (optr->pstate == PRCURR)
                        	{
                                	optr->pstate = PRREADY;
                                	insert(currpid,rdyhead,optr->pprio);
                        	}
                        	currpid = next;
                        	dequeue(next);
                        	nptr = &proctab[next];
                        	nptr->pstate = PRCURR;
                        	preempt = nptr->counter;
                        	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
                        	return OK;
			}
		else
		{	
			return -1;	
																							}		
	}
	else if(sched_type==1)
	{
		optr=&proctab[currpid];
		double random =expdev(0.1);
		if (optr->pstate == PRCURR)
		{
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);

		}
		int next_pid;
		int head,tail;
		head=q[rdyhead].qnext;
		tail=q[rdytail].qprev;
		if((int)random <proctab[head].pprio)
		{
			next_pid=q[rdyhead].qnext;
		}
		else if((int)random >proctab[tail].pprio)
		{
			next_pid=q[rdytail].qprev;
		}
		else if((int) random > proctab[head].pprio && (int) random < proctab[tail].pprio)
		{
			next_pid = q[rdyhead].qnext;
			while (q[next_pid].qkey <(int) random)
			{
				next_pid = q[next_pid].qnext;
			}
		}
		currpid = next_pid;
                dequeue(next_pid);
                nptr = &proctab[next_pid];
                nptr->pstate = PRCURR;
                preempt= QUANTUM;
                ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);

	}
	else
	{
		printf("\n Invalid scheduler specified \n");
		
	}
	
}




































		
		
