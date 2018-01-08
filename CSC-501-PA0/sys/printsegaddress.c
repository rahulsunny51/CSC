#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>

extern  etext,edata, *end;

void printsegaddress()
{
	printf("\n void printsegaddress() \n");
	int *etext_temp =&etext;
	int *edata_temp=&edata;
	int *end_temp=&end;
	printf("Current    : etext[0x%08x]= 0x%08x, edata[0x%08x]= 0x%08x, ebss[0x%08x]= 0x%08x \n", &etext,etext,&edata,edata,&end,*end);
	etext_temp=&etext-1;
	edata_temp=&edata-1;
	end_temp=&end-1;
	printf("Preceeding : etext[0x%08x]= 0x%08x, edata[0x%08x]= 0x%08x, ebbs[0x%08x]= 0x%08x \n", &etext-1,*etext_temp, &edata-1, *edata_temp, &end-1, *end_temp);
	etext_temp=&etext+1;
        edata_temp=&edata+1;
        end_temp=&end+1;
	printf("After      : etext[0x%08x]= 0x%08x, edata[0x%08x]= 0x%08x, ebbs[0x%08x]= 0x%08x \n", &etext+1,*etext_temp, &edata+1, *edata_temp, &end+1, *end_temp);
	return 0;
}
