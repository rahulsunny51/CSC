/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
extern int ctr1000;
SYSCALL	sleep(int n)
{
	proctab[currpid].sys_summary[18][0]++;
	
	proctab[currpid].sys_summary[18][1]=ctr1000;

	STATWORD ps;    
	if (n<0 || clkruns==0)
		return(SYSERR);
	if (n == 0) {
	        disable(ps);
		resched();
		restore(ps);
		return(OK);
	}
	while (n >= 1000) {
		sleep10(10000);
		n -= 1000;
	}
	if (n > 0)
		sleep10(10*n);
	proctab[currpid].sys_summary[18][1]=(ctr1000-proctab[currpid].sys_summary[18][1]);
	return(OK);
}
