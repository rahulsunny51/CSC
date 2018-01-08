/* send.c - send */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  send  --  send a message to another process
 *------------------------------------------------------------------------
 *
  */
 extern int ctr1000;
SYSCALL	send(int pid, WORD msg)
{
	proctab[currpid].sys_summary[12][0]++;
        proctab[currpid].sys_summary[12][1]=ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || ( (pptr= &proctab[pid])->pstate == PRFREE)
	   || pptr->phasmsg != 0) {
		restore(ps);
		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg = TRUE;
	if (pptr->pstate == PRRECV)	/* if receiver waits, start it	*/
		ready(pid, RESCHYES);
	else if (pptr->pstate == PRTRECV) {
		unsleep(pid);
		ready(pid, RESCHYES);
	}
	restore(ps);
	proctab[currpid].sys_summary[12][1]=(ctr1000-proctab[currpid].sys_summary[12][1]);
	return(OK);
}
