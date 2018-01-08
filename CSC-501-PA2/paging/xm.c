/* xm.c = xmmap xmunmap */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>


/*-------------------------------------------------------------------------
 * xmmap - xmmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmmap(int virtpage, bsd_t source, int npages)
{
 // kprintf("xmmap - to be implemented!\n");
	STATWORD ps;
	disable(ps);
	if(virtpage <4096)
	{
		kprintf("Error encounterd- Invalid virtual page\n");
                restore(ps);
                return SYSERR;
	}
	else if( source <0 && source >7)
	{
		kprintf("Error encounterd- Invalid BSD source\n");
		restore(ps);
		return SYSERR;
	}
	else if( npages <=0 && npages >4096)
        {
                kprintf("Error encounterd- Invalid page count \n");
                restore(ps);
                return SYSERR;
        }
	else		
	{
		bsm_map(currpid,virtpage, source,npages);
		restore (ps);
		return OK;
	}
	

  return SYSERR;
}



/*-------------------------------------------------------------------------
 * xmunmap - xmunmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmunmap(int virtpage)
{
 // kprintf("To be implemented!");
	STATWORD ps;
        disable(ps);
        if(virtpage <4096)
        {
                kprintf("Error encounterd- Invalid virtual page\n");
                restore(ps);
                return SYSERR;
        }
	else
	{
		bsm_unmap(currpid,virtpage);
		restore (ps);
		return OK;
	}
  return SYSERR;
}
