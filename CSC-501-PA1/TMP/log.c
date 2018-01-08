#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lab1.h>

double log(double x)
{
        double temp=x-1;
        double i=1;
        double  log1=0;
        for(i=1;i<21;i++)
        {
                log1=log1+(pow((-1),(i+1))*pow(temp,i)/i);
	}
        return log1;
}

