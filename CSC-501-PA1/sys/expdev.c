#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lab1.h>


double expdev(double lambda) {
    double dummy;
    do
        dummy= (double) rand() / 32767;
    while (dummy == 0.0);
    return -log(dummy) / lambda;
}
