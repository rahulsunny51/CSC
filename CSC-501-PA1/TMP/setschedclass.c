#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lab1.h>

int sched_type=0;

void setschedclass(int sched_class)
{
	sched_type=sched_class;

}
