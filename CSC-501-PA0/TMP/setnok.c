/* setnok.c - setnok */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  setnok  -  set next-of-kin (notified at death) for a given process
 *------------------------------------------------------------------------
 */
extern int ctr1000;
SYSCALL	setnok(int nok, int pid)
{
	proctab[currpid].sys_summary[14][0]++;
        proctab[currpid].sys_summary[14][1]=ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);
		return(SYSERR);
	}
	pptr = &proctab[pid];
	pptr->pnxtkin = nok;
	restore(ps);
	proctab[currpid].sys_summary[14][1]=(ctr1000-proctab[currpid].sys_summary[14][1]);
	return(OK);
}
