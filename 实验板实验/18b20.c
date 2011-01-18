#include <stdio.h>
#include <reg52.h>
#include "declare.h"
int tempold,tempnew;
int readtemp();
void delay(int);
void ds18b20()
{
	while(stop!='#')
	{
		tempnew=readtemp();
		delay(100);
		ES=1;
		if(tempnew!=tempold)
		{
			printf("%d",tempnew);
			tempold=tempnew;
		}
		ES=0;
	}
	stop=0;
}
