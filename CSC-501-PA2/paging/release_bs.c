#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

SYSCALL release_bs(bsd_t bs_id) {

  /* release the backing store with ID bs_id */
	STATWORD ps;
	disable(ps);
	if(bs_id<0 || bs_id>7)
	{
		kprintf("Invalid BS ID\n");
		restore(ps);
		return SYSERR;
	}
	else
	{
		free_bsm(bs_id);
		restore(ps);
                return OK;
	}
    //kprintf("To be implemented!\n");
   return OK;

}

