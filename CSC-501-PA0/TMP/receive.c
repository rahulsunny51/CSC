/* receive.c - receive */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include<proc.h>
/*------------------------------------------------------------------------
 *  receive  -  wait for a message and return it
 *------------------------------------------------------------------------
 */
extern int ctr1000;
SYSCALL	receive()
{
	proctab[currpid].sys_summary[6][0]++;
        proctab[currpid].sys_summary[6][1]=ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;
	WORD	msg;

	disable(ps);
	pptr = &proctab[currpid];
	if ( !pptr->phasmsg ) {		/* if no message, wait for one	*/
		pptr->pstate = PRRECV;
		resched();
	}
	msg = pptr->pmsg;		/* retrieve message		*/
	pptr->phasmsg = FALSE;
	restore(ps);
	proctab[currpid].sys_summary[6][1]=(ctr1000-proctab[currpid].sys_summary[6][1]);
	return(msg);
}
