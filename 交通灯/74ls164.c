#include <reg52.h>
sbit clock=P1^6;
sbit dat=P1^7;
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

