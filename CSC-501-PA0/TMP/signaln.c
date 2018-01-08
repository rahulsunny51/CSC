/* signaln.c - signaln */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  signaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
extern int ctr1000;
SYSCALL signaln(int sem, int count)
{
	proctab[currpid].sys_summary[17][0]++;
        proctab[currpid].sys_summary[17][1]=ctr1000;
	STATWORD ps;    
	struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		restore(ps);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore(ps);
	proctab[currpid].sys_summary[17][1]=(ctr1000-proctab[currpid].sys_summary[17][1]);
	return(OK);
}
