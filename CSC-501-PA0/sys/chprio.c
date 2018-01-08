/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
extern int ctr1000;

SYSCALL chprio(int pid, int newprio)
{
	proctab[currpid].sys_summary[1][0]++;
        proctab[currpid].sys_summary[1][1]=ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);
	proctab[currpid].sys_summary[1][1]=(ctr1000-proctab[currpid].sys_summary[1][1]);
	return(newprio);
}
