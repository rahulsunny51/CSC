/* vcreate.c - vcreate */
    
#include <conf.h>
#include <i386.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <paging.h>

/*
static unsigned long esp;
*/

LOCAL	newpid();
/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
SYSCALL vcreate(procaddr,ssize,hsize,priority,name,nargs,args)
	int	*procaddr;		/* procedure address		*/
	int	ssize;			/* stack size in words		*/
	int	hsize;			/* virtual heap size in pages	*/
	int	priority;		/* process priority > 0		*/
	char	*name;			/* name (for debugging)		*/
	int	nargs;			/* number of args that follow	*/
	long	args;			/* arguments (treated like an	*/
					/* array in the code)		*/
{
	//kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);
	int pid=create(procaddr,ssize,priority,name,nargs,args);
	kprintf("create in vcreate successful\n");
	int store=1;
	if(get_bsm(&store)==SYSERR)
	{
		restore(ps);
		return SYSERR;
	}
	else
	{
		
		if(bsm_map(pid,4096,store,hsize)==SYSERR)
		{
			restore(ps);
                	return SYSERR;
		}
		if(hsize>255)
		{
			kprintf("Invalid hsize requested\n");
			restore(ps);
                        return SYSERR;
		}
		//kprintf("Backing store %d\n", store);
		bsm_tab[store].bs_status=BSM_MAPPED;
		bsm_tab[store].bs_pid=pid;
		bsm_tab[store].bs_npages=hsize;
		bsm_tab[store].bs_vpno=4096;
		bsm_tab[store].heap_flag=1;
		proctab[pid].store=store;
		proctab[pid].vhpno=4096;
		proctab[pid].vhpnpages=hsize;
		proctab[pid].vmemlist->mnext=4096*NBPG;
		struct mblock *new_mblock;
		struct mblock  *temp;
		 new_mblock=4096*NBPG;
		temp=BACKING_STORE_BASE+ (store*BACKING_STORE_UNIT_SIZE);
		new_mblock->mlen=hsize*NBPG;
		new_mblock->mnext=NULL;
		temp->mlen=hsize*NBPG;
		temp->mnext=NULL;
		restore(ps);
		return pid;	
	}
	
}

/*------------------------------------------------------------------------
 * newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */
LOCAL	newpid()
{
	int	pid;			/* process id to return		*/
	int	i;

	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ( (pid=nextproc--) <= 0)
			nextproc = NPROC-1;
		if (proctab[pid].pstate == PRFREE)
			return(pid);
	}
	return(SYSERR);
}
