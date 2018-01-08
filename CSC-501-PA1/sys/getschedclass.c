#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lab1.h>

extern int sched_type;

int getschedclass()
{
	return sched_type;
}

