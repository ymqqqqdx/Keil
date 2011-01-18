#include <stdio.h>
#include <reg52.h>
#define U8 unsigned char
#define U16 unsigned int
sbit beep=P2^7;
U8 code Table_H[]={0,226,229,232,233,236,238,240,241,242,244,245,246,247,248};
U8 code Table_L[]={0,4,13,10,20,3,8,6,2,23,5,26,1,4,3};
U8 sid;
void delay(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void main()
{
	U8 i;
	EA=1;
	ET0=1;
	TMOD=0x00;
	while(1)
	{
		for(i=0;i<15;i++)
		{
			sid=i;
			TR0=1;
			delay(500);
		}
	}
}
void Timer0() interrupt 1
{
	beep=~beep;
	TH0=Table_H[sid];
	TL0=Table_L[sid];
}