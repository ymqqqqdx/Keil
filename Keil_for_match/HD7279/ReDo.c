#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
#include <math.h>
#define U8 unsigned char
#define U16 unsigned int
sbit hd_cs=P0^4;
sbit hd_clk=P0^5;
sbit hd_dat=P0^6;
sbit hd_key=P0^7;

void hd_longdelay()
{
	U8 i;
	for(i=0;i<50;i++);
}

void hd_shortdelay()
{
	U8 i;
	for(i=0;i<8;i++);
}

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}

void hd_writeonechar(U8 dat)
{
	U8 i;
	hd_cs=0;
	hd_dat=0;
	hd_longdelay();
	for(i=0;i<8;i++)
	{
		dat<<=1;
		hd_dat=CY;
		hd_clk=1; hd_shortdelay();
		hd_clk=0; hd_shortdelay();
	}
	hd_dat=0;
}
U8 hd_keyconvert(U8 dat)
{
	switch(dat)
	{
		case 6 : dat=1; break;
		case 5 : dat=2; break;
		case 4 : dat=3; break;
		case 3 : dat=4; break;
		case 14 : dat=5; break;
		case 13 : dat=6; break;
		case 12 : dat=7; break;
		case 11 : dat=8; break;
		case 22 : dat=9; break;
		case 21 : dat=10; break;
		case 20 : dat=11; break;
		case 19 : dat=12; break;
		case 30 : dat=13; break;
		case 29 : dat=14; break;
		case 28 : dat=15; break;
		case 27 : dat=16; break;
	}
	return dat;
}
U8 hd_readonechar()
{
	U8 i,temp=0;
	hd_dat=1;
	hd_longdelay();
	for(i=0;i<8;i++)
	{
		hd_clk=1; hd_shortdelay();
		temp|=((U8)hd_dat)<<(7-i);
		hd_clk=0; hd_shortdelay();
	}
	hd_cs=0;
	return temp;
}

void hd_writecd(U8 cmd,U8 dat)
{
	hd_writeonechar(cmd);
	hd_writeonechar(dat);
}

U8 hd_readcd(U8 cmd)
{
	hd_writeonechar(cmd);
	return(hd_readonechar());
}

void main()
{
	U8 temp=0;
	hd_writeonechar(0xa4);
	hd_writecd(0x80,1);
	while(1)
	{
		if(hd_key==0)
		{
			temp=hd_keyconvert(hd_readcd(0x15));
			hd_writecd(0x81,temp/10);
			hd_writecd(0x80,temp%10);
		}
		delay_1ms(5);
	}
}