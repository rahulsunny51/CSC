#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lab1.h>

double pow(double x, int y)
{
	double  i=1;
	while(y!=0)
	{
		i=i*x,
		y--;	
	}
	return i;
}
