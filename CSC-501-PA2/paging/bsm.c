/* bsm.c - manage the backing store mapping*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

/*-------------------------------------------------------------------------
 * init_bsm- initialize bsm_tab
 *-------------------------------------------------------------------------
 */
bs_map_t bsm_tab[];

SYSCALL init_bsm()
{
	STATWORD ps;
	disable(ps);
	int i;
	for(i=0; i<7;i++)
	{
		bsm_tab[i].bs_status= BSM_UNMAPPED;
		bsm_tab[i].bs_pid= -1;
		bsm_tab[i].bs_vpno=-1;
		bsm_tab[i].bs_npages=0;
		bsm_tab[i].bs_sem=0;
		bsm_tab[i].heap_flag=0;
	}	
	restore(ps);
	return OK;
}
	

/*-------------------------------------------------------------------------
 * get_bsm - get a free entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL get_bsm(int* avail)
{
	STATWORD ps;
        disable(ps);
	int i=0;
	for(i=0;i<7;i++)
	{
		if(bsm_tab[i].bs_status == BSM_UNMAPPED)
		{
			*avail=i;
			restore(ps);
		        return OK;

		}
	}
	kprintf("SYS ERROR Backing store unavailable");
	restore(ps);
	return SYSERR;
}



/*-------------------------------------------------------------------------
 * free_bsm - free an entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL free_bsm(int i)
{
	STATWORD ps;
        disable(ps);
	if(i>=0 && i<=7)
	{
		bsm_tab[i].bs_status= BSM_UNMAPPED;
                bsm_tab[i].bs_pid= -1;
                bsm_tab[i].bs_vpno=-1;
                bsm_tab[i].bs_npages=0;
                bsm_tab[i].bs_sem=0;
                bsm_tab[i].heap_flag=0;
		restore(ps);
                return OK;
	}
	else
	{
		restore(ps);
		return SYSERR;
	}

}


/*-------------------------------------------------------------------------
 * bsm_lookup - lookup bsm_tab and find the corresponding entry
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_lookup(int pid, long vaddr, int* store, int* pageth)
{
	STATWORD ps;
        disable(ps);
	int i=0;
 	for(i=0;i<7;i++)
	{
		if(bsm_tab[i].bs_pid==pid)
		{
			*store=i;
			int a=bsm_tab[i].bs_vpno*NBPG;
			a=(vaddr-a)/NBPG;
			*pageth=a;
			restore(ps);
        	        return OK;
		}
	}
	restore(ps);
        return SYSERR;
}


/*-------------------------------------------------------------------------
 * bsm_map - add an mapping into bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_map(int pid, int vpno, int source, int npages)
{
	STATWORD ps;
        disable(ps);
	if(!isbadpid(pid) && vpno>0 && source>=0 && source <=7 && npages>0)
	{
		bsm_tab[source].bs_status= BSM_MAPPED;
       	 	bsm_tab[source].bs_pid= pid;
	        bsm_tab[source].bs_vpno=vpno;
        	bsm_tab[source].bs_npages=npages;
       		bsm_tab[source].bs_sem=1;
        	bsm_tab[source].heap_flag=0;
		proctab[currpid].vhpno=vpno;
		proctab[currpid].store=source;
        	restore(ps);
        	return OK;
	}
	else
	{
		kprintf("Error Enountered\n");
		restore(ps);
                return OK;
	}

}



/*-------------------------------------------------------------------------
 * bsm_unmap - delete an mapping from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_unmap(int pid, int vpno, int flag)
{
	STATWORD ps;
        disable(ps);
	int source,pageth;
	if(bsm_lookup(pid, vpno*NBPG, &source, &pageth))
	{
		bsm_tab[source].bs_status= BSM_UNMAPPED;
                bsm_tab[source].bs_pid= -1;
                bsm_tab[source].bs_vpno=-1;
                bsm_tab[source].bs_npages=0;
                bsm_tab[source].bs_sem=0;
                bsm_tab[source].heap_flag=0;
                proctab[currpid].vhpno=0;
                proctab[currpid].store=-1;
                restore(ps);
                return OK;

	}
	else
	{
		kprintf("Error Enountered\n");
		restore(ps);
		return SYSERR;
	}




}



