#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>

unsigned long int  *esp;
unsigned long int  *ebp;
void printtos()
{
	printf(" \n void printtos()\n");
	asm("movl %esp, esp");
	asm("movl %ebp, ebp");
	printf("Before  [0x%08x] = 0x%08x\n", ebp, *ebp+2);
	printf("After   [0x%08x] = 0x%08x\n", ebp, *ebp);
	int *new_esp=&esp;
	new_esp=esp-1;
	printf("	element[0x%08x] : 0x%08x\n", new_esp,*new_esp);
	new_esp=esp-2;
        printf("        element[0x%08x] : 0x%08x\n", new_esp,*new_esp);
	new_esp=esp-3;
        printf("        element[0x%08x] : 0x%08x\n", new_esp,*new_esp);
	new_esp=esp-4;
        printf("        element[0x%08x] : 0x%08x\n", new_esp,*new_esp);
}


