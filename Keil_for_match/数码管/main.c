#include <reg52.h>
#define U8 unsigned char
#define U16 unsigned int
sbit clk=P3^6;
sbit OE=P2^4;
U8 code table[]={0xC0,/*0*/0xF9,/*1*/0xA4,/*2*/0xB0,/*3*/0x99,/*4*/0x92,/*5*/0x82,/*6*/0xF8,/*7*/0x80,/*8*/0x90,/*9*/};
void delay(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void main()
{
	U8 i;
	while(1)
	{
		for(i=0;i<4;i++)
		{
			P2=~(1<<i);
			OE=0;
			P0=table[i+1];
			clk=0;
			clk=1;
		}
	}
}