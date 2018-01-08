/* recvclr.c - recvclr */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  recvclr  --  clear messages, returning waiting message (if any)
 *------------------------------------------------------------------------
 */
extern int ctr1000;
SYSCALL	recvclr()
{
	proctab[currpid].sys_summary[7][0]++;
        proctab[currpid].sys_summary[7][1]=ctr1000;
	STATWORD ps;    
	WORD	msg;

	disable(ps);
	if (proctab[currpid].phasmsg) {
		proctab[currpid].phasmsg = 0;
		msg = proctab[currpid].pmsg;
	} else
		msg = OK;
	restore(ps);
	proctab[currpid].sys_summary[7][1]=(ctr1000-proctab[getpid()].sys_summary[7][1]);
	return(msg);
}
