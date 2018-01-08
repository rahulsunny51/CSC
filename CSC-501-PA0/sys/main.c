/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
void create_process1();
int main()
{
	kprintf("\n\nHello World, Xinu lives\n\n"); 
	printsegaddress();
	printtos();
	printf("0x%08x\n", zfunction(0xaabbccdd));	
//	printprocstks(3);
	startsyscallsummary();
	int proc_counter[2];
	 resume(proc_counter[0]= create(create_process1, 100, 2, "Process A", 0,0));
               // printf("Num procs %d\n", numproc);
                resume(proc_counter[1]= create(create_process1, 180, 10, "Process B", 0,0));
               // printf("Num procs %d %d\n", numproc,getpid());
	//printf("Before start syscall sumamry\n");
//	startsyscallsummary();
	sleep(2);
	printprocstks(3);
	//printf("Before print syscall sumamry\n");
	printsyscallsummary();
	stopsyscallsummary();
	//printf("Before stop syscall sumamry\n");
	//printf("End of main\n");
	//printf("0x%08x\n", zfunction(0xaabbccdd));
	return 0;

}
void create_process1()
{
	//printf("Process [pid:%d] \n", currpid);
        //chprio();
	getprio(49);
        //printf("Process completed\n");
        //printf("        Syscall: %s , count: %d , average execution time : %d (ms) \n", proctab[currpid].pname, proctab[currpid].sys_summary                                                                 [18][0], proctab[currpid].sys_summary[18][1]);
}

