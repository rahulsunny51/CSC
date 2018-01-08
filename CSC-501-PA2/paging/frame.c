/* frame.c - manage physical frames */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

/*-------------------------------------------------------------------------
 * init_frm - initialize frm_tab
 *-------------------------------------------------------------------------
*/
extern int frame_map[NFRAMES];
extern int page_replace_policy;
int fault_frame=0;
fr_map_t frm_tab[];
SYSCALL init_frm()
{
  //kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);
	int i=0;
	for(i=0;i<NFRAMES;i++)
	{
		frm_tab[i].fr_status=FRM_UNMAPPED;
		frm_tab[i].fr_pid=-1;
		frm_tab[i].fr_vpno= -1;
		frm_tab[i].fr_refcnt=0;
		frm_tab[i].fr_type=FR_INIT;
		frm_tab[i].fr_dirty=0;
		frame_map[i]=0;
	}
	restore(ps);
	return OK;
}

/*-------------------------------------------------------------------------
 * get_frm - get a free frame according page replacement policy
 *-------------------------------------------------------------------------
 */
SYSCALL get_frm(int* avail)
{
  //kprintf("To be implemented!\n");
  	STATWORD ps;
        disable(ps);
	int i=0;
        for(i=0;i<NFRAMES;i++)
        {
		if(frm_tab[i].fr_status==FRM_UNMAPPED)
		{
			*avail=i;
			frm_tab[i].fr_status=FRM_MAPPED;
		//	kprintf("Frame %d\n", *avail);
			restore(ps);
		        return OK;
		}
	}
	if(page_replace_policy == SC)
	{
		//kprintf("IN sc loop\n");
		int j=0;
		int temp;
		int flag=0;
		int flag1=0;
		for (j=fault_frame+1;j<NFRAMES;j++)
		{
			if(frm_tab[j].fr_type==FR_PAGE)
			{
			int fault_vpno=frm_tab[j].fr_vpno;
        		unsigned long fault_vaddr=fault_vpno*NBPG;
       			virt_addr_t *new_virt;
        		pt_t *page_table;
        		pd_t *page_dir;
			int pid=frm_tab[j].fr_pid;
        		new_virt=(virt_addr_t*)&fault_vaddr;
			page_dir=(proctab[pid].pdbr) + (new_virt->pd_offset*sizeof(pd_t));
			page_table= (pt_t *)((page_dir->pd_base*NBPG) + (new_virt->pt_offset*sizeof(pt_t)));
			if(page_table->pt_acc==1)
			{
				if(flag==0)
				{
					temp=j;		
					flag=1;
				}
				page_table->pt_acc=0;
			}
			else if(page_table->pt_acc==0)
			{
				fault_frame=j;
				kprintf("Replaced Frame number  %d\n", fault_frame);
				free_frm(fault_frame);
				restore(ps);
				return OK;
			}
			else
			{
				krpintf("Invalid pt_acc defined\n");
			}
			}
			if(j==NFRAMES-1 && flag==1)
			{
				j=0;
			}
		}
	}
	if(page_replace_policy==AGING)
	{
		//kprintf("In aging\n");
		int j=0;
		for(j=0;j<NFRAMES;j++)
		{
			 if(frm_tab[j].fr_type==FR_PAGE)
                        {
                        int fault_vpno=frm_tab[j].fr_vpno;
                        unsigned long fault_vaddr=fault_vpno*NBPG;
                        virt_addr_t *new_virt;
                        pt_t *page_table;
                        pd_t *page_dir;
                        int pid=frm_tab[j].fr_pid;
                        new_virt=(virt_addr_t*)&fault_vaddr;
                        page_dir=(proctab[pid].pdbr) + (new_virt->pd_offset*sizeof(pd_t));
                        page_table= (pt_t *)((page_dir->pd_base*NBPG) + (new_virt->pt_offset*sizeof(pt_t)));
			frm_tab[j].fr_refcnt=frm_tab[j].fr_refcnt>>1;
			if(page_table->pt_acc==1)
			{
				 frm_tab[j].fr_refcnt=frm_tab[j].fr_refcnt+128;
				if(frm_tab[j].fr_refcnt>255)
				{
					frm_tab[j].fr_refcnt=255;
				}
			}
			page_table->pt_acc=0;
			write_cr3(read_cr3());
			}
		}
		int min_age=255;
		int fault;
		for(j=0;j<NFRAMES;j++)
		{
			if(frm_tab[j].fr_type==FR_PAGE)
			{
				if(frm_tab[j].fr_refcnt <=min_age)
					{
						fault=j;
						min_age=frm_tab[j].fr_refcnt;
					}
			}			
		}
		kprintf("Replaced Frame number  %d\n", fault);
		free_frm(fault);	
		*avail=fault;
		restore(ps);
		return OK;
		}
	
}

/*-------------------------------------------------------------------------
 * free_frm - free a frame 
 *-------------------------------------------------------------------------
 */
                                   
SYSCALL free_frm(int i)
{

//  kprintf("To be implemented free frm!\n");
  	STATWORD ps;
        disable(ps);
	int fault_vpno=frm_tab[i].fr_vpno;
	unsigned long fault_vaddr=fault_vpno*NBPG;
//	kprintf("Fault_vpno= %d, addr =%d\n", fault_vpno,fault_vaddr);
	virt_addr_t *new_virt;
	pt_t *page_table;
	pd_t *page_dir;
	new_virt=(virt_addr_t*)&fault_vaddr;
//	kprintf("PD_offet= %d pt_offset %d\n", new_virt->pd_offset,new_virt->pt_offset);
	int pid=frm_tab[i].fr_pid;
	page_dir=(pd_t*)((proctab[pid].pdbr) + (new_virt->pd_offset*sizeof(pd_t)));
	page_table= (pt_t *)((page_dir->pd_base*NBPG) + (new_virt->pt_offset*sizeof(pt_t)));
//	kprintf("page_dir 0x%08x, page_table 0x%08x\n", page_dir, page_table);
	page_table->pt_pres=0;
	if(pid==currpid)
		asm volatile("invlpg (%0)" ::"r"(page_table) : "memory");
	frm_tab[i].fr_refcnt--;
	if(frm_tab[i].fr_refcnt==0)
	{
		page_dir->pd_pres=0;
		frm_tab[i].fr_pid=-1;
		frm_tab[i].fr_vpno=0;
		frm_tab[i].fr_status=FRM_UNMAPPED;
		frm_tab[i].fr_type=FR_INIT;
	}
	int store, pageth;
	if(bsm_lookup(pid,fault_vaddr,&store,&pageth)== OK)
		{
			write_bs(((i+FRAME0)*NBPG),store,pageth);
			//kprintf("Retrunning from free frm\n");
			restore(ps);
			return OK;
		}
	else
	{
		kill(pid);
		restore (ps);
		return SYSERR;
	}
	restore(ps);
        return OK;
}



