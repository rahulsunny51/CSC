#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

int get_bs(bsd_t bs_id, unsigned int npages) {

  /* requests a new mapping of npages with ID map_id */
   	STATWORD ps;
        disable(ps);
	if(npages<=0 && npages>256)
	{
		kprintf("Error Encountered\n");
        	restore(ps);
		return SYSERR;
	}   
	if(bs_id<0 && bs_id>7)
	{
		kprintf("Error Encountered\n");
		restore(ps);
                return SYSERR;
	}
	if(bsm_tab[bs_id].heap_flag==1)
	{
		kprintf("Error Encountered\n");
                restore(ps);
		return SYSERR;
	}
	else
	{
		if(bsm_tab[bs_id].bs_status==BSM_UNMAPPED &&  bsm_tab[bs_id].bs_npages==0)
		{
			//bsm_tab[bs_id].bs_status=MAPPED;
			bsm_tab[bs_id].bs_pid=currpid;
			bsm_tab[bs_id].bs_npages=npages;
			restore(ps);
			return npages;
		}
		else
		{
			restore(ps);
			return bsm_tab[bs_id].bs_npages;
		}
	}
}


