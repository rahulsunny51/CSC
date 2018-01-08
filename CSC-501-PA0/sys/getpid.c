/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
extern int ctr1000;

SYSCALL getpid()
{
	proctab[currpid].sys_summary[2][0]++;
        proctab[currpid].sys_summary[2][1]=ctr1000;
	proctab[currpid].sys_summary[2][1]=(ctr1000-proctab[currpid].sys_summary[2][1]);
	
	return(currpid);
}
