/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include<proc.h>
extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
extern int ctr1000;
SYSCALL	gettime(long *timvar)
{
    /* long	now; */

	/* FIXME -- no getutim */
	proctab[currpid].sys_summary[4][0]++;
        proctab[currpid].sys_summary[4][1]=ctr1000;
	proctab[currpid].sys_summary[4][1]=(ctr1000-proctab[currpid].sys_summary[4][1]);
    return OK;
}
