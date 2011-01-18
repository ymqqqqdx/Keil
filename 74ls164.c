#include <reg52.h>
sbit clock=P3^1;
sbit dat=P3^0;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
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
		delay(1);
}
void main()
{
	int i;
	delay(1000);
	while(1)
	{
		for(i=0;i<16;i++)
		{
			write_onechar(table[i]);
			delay(1000);
		}
	}
}
