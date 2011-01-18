#include <reg52.h>
sbit clock=P3^1;
sbit dat=P3^0;
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void write_onechar(unsigned char temp)
{
		int i;
		for(i=0;i<8;i++)
		{
			dat=temp&0x01;
			clock=1;
			temp>>=1;			
			clock=0;
		}
//		clock=1;
//		delay(1);
}

