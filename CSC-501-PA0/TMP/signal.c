/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
extern int ctr1000;
SYSCALL signal(int sem)
{
	proctab[currpid].sys_summary[16][0]++;
        proctab[currpid].sys_summary[16][1]=ctr1000;
	STATWORD ps;    
	register struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);
	restore(ps);
	proctab[currpid].sys_summary[16][1]=(ctr1000-proctab[currpid].sys_summary[16][1]);
	return(OK);
}
