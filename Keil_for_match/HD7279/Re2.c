#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
#define U8 unsigned char
#define U16 unsigned int
sbit hd_cs=P0^4;
sbit hd_clk=P0^5;
sbit hd_dat=P0^6;
sbit hd_key=P0^7;

void hd_shortdelay()
{
	U8 i;
	for(i=0;i<8;i++);
}

void hd_longdelay()
{
	U8 i;
	for(i=0;i<50;i++);
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
	hd_dat=0;
	return temp;
}

void hd_write(U8 add,U8 dat)
{
	hd_writeonechar(add);
	hd_writeonechar(dat);
}

U8 hd_read(U8 add)
{
	hd_writeonechar(add);
	return(hd_readonechar());
}

void main()
{
	U8 temp;
	hd_writeonechar(0xa4);
	hd_write(0x80,1);
	while(1)
	{
		if(!hd_key)
		{
			temp=hd_read(0x15);
			hd_write(0x81,temp/10);
			hd_write(0x80,temp%10);	
		}while(!hd_key);
		delay_1ms(5);
	}
}