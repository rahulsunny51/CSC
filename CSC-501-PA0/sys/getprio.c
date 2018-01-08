/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
extern int ctr1000;

SYSCALL getprio(int pid)
{
	proctab[currpid].sys_summary[3][0]++;
        proctab[currpid].sys_summary[3][1]=ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	restore(ps);
	proctab[currpid].sys_summary[3][1]=(ctr1000-proctab[currpid].sys_summary[3][1]);
	return(pptr->pprio);
}
