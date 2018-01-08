/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
extern int ctr1000;
SYSCALL	setdev(int pid, int dev1, int dev2)
{
	proctab[currpid].sys_summary[13][0]++;
        proctab[currpid].sys_summary[13][1]=ctr1000;
	short	*nxtdev;

	if (isbadpid(pid))
		return(SYSERR);
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;
	proctab[currpid].sys_summary[13][1]=(ctr1000-proctab[currpid].sys_summary[13][1]);
	return(OK);
}
