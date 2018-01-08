/* pfint.c - pfint */

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include<proc.h>

/*-------------------------------------------------------------------------
 * pfint - paging fault ISR
 *-------------------------------------------------------------------------
 */
extern frame_map[NFRAMES];
SYSCALL pfint()
{

  //kprintf("PFINT To be implemented! fo\n");
 	STATWORD ps;
	disable(ps);
  	unsigned long var_addr;
	var_addr = read_cr2();
	//kprintf("\n\n\nvadd= 0x%08x ",var_addr);
 	virt_addr_t *virt_addr;
	virt_addr=(virt_addr_t *)&var_addr;
        unsigned int pg_offset= virt_addr->pg_offset;
	unsigned int pd_offset=virt_addr->pd_offset;
	unsigned int pt_offset=virt_addr->pt_offset;
	//kprintf("\n pg_offset =%d, pd__offset =%d, pt_offset= %d", pg_offset,pd_offset,pt_offset);
	pd_t *page_dir;
	pt_t *page_table;
	unsigned long pdbr=proctab[currpid].pdbr;
	//kprintf("pdbr= 0x%08x\n",pdbr);
	page_dir=pdbr+(pd_offset*sizeof(pd_t));
	//kprintf("new pdbr= 0x%08x \n", page_dir);
	//kprintf("pd_pres== %d\n", page_dir->pd_pres);
	if(page_dir->pd_pres==0)
	{
	//	kprintf("\nIN if of pd_pres==0\n");
		int frame=0;
		get_frm(&frame);
//		frame_map[frame]=1;
		frm_tab[frame].fr_status=FRM_MAPPED;
                frm_tab[frame].fr_type=FR_TBL;
                frm_tab[frame].fr_pid=currpid;
		unsigned int frame_addr=(FRAME0+ frame)*NBPG;
		page_table= (pt_t*)frame_addr;
	//	kprintf("beforeAfter for loop pagetable %d\n", page_table);
		int i=0;
		for(i=0;i<NFRAMES;i++)
		{
		//	kprintf("pi %d, in for loop \n",currpid);
			page_table->pt_pres=0;
			page_table->pt_write=0;
			page_table->pt_user=0;
			page_table->pt_pwt=0;	
			page_table->pt_pcd=0;
			page_table->pt_dirty=0;
			page_table->pt_acc=0;
			page_table->pt_mbz=0;
			page_table->pt_global=0;
			page_table->pt_avail=0;
			page_table->pt_base=0;
			page_table++;
		}
	//	kprintf("After for loop pagetable %d\n", page_table);
	//	frm_tab[frame].fr_status=FRM_MAPPED;
	//	frm_tab[frame].fr_type=FR_TBL;
	//	frm_tab[frame].fr_pid=currpid;
		page_dir=pdbr+(pd_offset*sizeof(pd_t));
		page_dir->pd_pres=1;
		page_dir->pd_write=1;
		page_dir->pd_user=0;
		page_dir->pd_pwt=0;

		page_dir->pd_pcd=0;
		page_dir->pd_acc=0;
		page_dir->pd_mbz=0;
		page_dir->pd_fmb=0;
		page_dir->pd_global=0;
		page_dir->pd_avail=0;
		page_dir->pd_base=FRAME0+frame;
	}
	pt_t *page_table1;	
	page_table1=(page_dir->pd_base*NBPG)+ (pt_offset*sizeof(pt_t));
	int new_frame;
	if(page_table1->pt_pres==0)
	{	
		get_frm(&new_frame);
	//	kprintf("Frame_number =%d\n", new_frame);
		frame_map[new_frame]=1;
		page_table1->pt_pres=1;
		page_table1->pt_write=1;
		page_table1->pt_base=FRAME0+new_frame;
		frm_tab[new_frame].fr_status=FRM_MAPPED;
                frm_tab[new_frame].fr_type=FR_PAGE;
                frm_tab[new_frame].fr_pid=currpid;
		frm_tab[new_frame].fr_vpno=var_addr/NBPG;
		int store;
		int pageth;
		bsm_lookup(currpid,virt_addr,&store, &pageth);
		read_bs((char *)((page_table1->pt_base)*NBPG), store,pageth);
	}
	write_cr3(pdbr);
	restore(ps);
	return OK;
}


