/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>
#include <proc.h>
/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
extern int ctr1000;
SYSCALL scount(int sem)
{
	proctab[currpid].sys_summary[10][0]++;
  	proctab[currpid].sys_summary[10][1]=ctr1000;

extern	struct	sentry	semaph[];

	if (isbadsem(sem) || semaph[sem].sstate==SFREE)
		return(SYSERR);
	proctab[currpid].sys_summary[10][1]=(ctr1000-proctab[currpid].sys_summary[10][1]);
	return(semaph[sem].semcnt);
}
