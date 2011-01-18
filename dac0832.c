#include <reg52.h>
sbit bi=P3^3;
void delay(unsigned int xms)
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
	{
		for(j=110;i>0;j--);
	}
}
void main()
{
	unsigned char flag=0,val;
	while(1)
	{
		if(flag==0)
		{
			val+=5;
			P0=val;
			if(val==255)
			{
				flag=1;
				bi=0;
				delay(1000);
				bi=1;
			}
			delay(50);
		}
		else
		{
			val-=5;
			P0=val;
			if(val==0)
			{
				flag=0;
				bi=0;
				delay(1000);
				bi=1;
			}
			delay(50);
		}
	}
}
