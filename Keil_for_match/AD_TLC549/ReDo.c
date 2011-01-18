#include <reg52.h>
#define U8 unsigned char
#define U16 unsigned int
#define CMD 0
#define DATA 1
sbit ad_clk=P3^2;
sbit ad_dat=P3^3;
sbit ad_cs=P3^4;

sbit da_dat=P3^5;
sbit da_clk=P3^6;
sbit da_cs=P3^7;

sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_dat=P0^2;

void da_write(U16 dat)
{
	U8 i;
	da_cs=0;
	dat<<=2;
	dat|=0x02;
	for(i=0;i<16;i++)
	{
		dat<<=1;
		da_clk=1;
		da_dat=CY;
		da_clk=0;
	}
	da_cs=1;
}

U8 ad_read()
{
	U8 i,temp;
	ad_cs=0;
	for(i=0;i<8;i++)
	{
		temp|=((U8)ad_dat)<<(7-i);
		ad_clk=1;
		ad_clk=0;
	}
	ad_cs=1;
	return temp;
}

void lcd_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		lcd_sid=CY;
		lcd_clk=0;
		lcd_clk=1;
	}
}