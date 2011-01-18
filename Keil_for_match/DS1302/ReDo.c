#include <reg52.h>
#include <stdio.h>
#include <string.h>
#define U8 unsigned char
#define U16 unsigned int
#define CMD 0
#define DATA 1
sbit rtc_clk=P1^6;
sbit rtc_io=P1^7;
sbit rtc_rst=P3^0;

sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}

void rtc_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat>>=1;
		rtc_clk=0;
		rtc_io=CY;
		rtc_clk=1;
	}
}

U8 rtc_readonechar()
{
	U8 i,temp=0;
	for(i=0;i<8;i++)
	{
		rtc_clk=0;
		temp|=((U8)rtc_io)<<i;
		rtc_clk=1;
	}
	return temp/16*10+temp%16;
}

void rtc_write(U8 add,U8 dat)
{	
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	rtc_writeonechar(add);
	rtc_writeonechar((dat/10)<<4|(dat%10));
	rtc_rst=0;
}

U8 rtc_read(U8 add)
{
	U8 temp;
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	rtc_writeonechar(add|1);
	temp=rtc_readonechar();
	rtc_rst=0;
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

void lcd_writecd(bit cd,U8 dat)
{
	lcd_cs=1;
	lcd_writeonechar(cd==0?0xf8:0xfa);
	lcd_writeonechar(dat&0xf0);
	lcd_writeonechar(dat<<4);
	lcd_cs=0;
}

void lcd_string(U8 add,U8 *pt)
{
	lcd_writecd(0,add);
	while(*pt!='\0')
		lcd_writecd(1,*pt++);
}

void main()
{
	U8 second,second_old;
	lcd_writecd(0,0x0c);
	lcd_string(0x80,"¡æ");
	while(1)
	{
		second=rtc_read(0x80);
		if(second!=second_old)
		{
			second_old=second;
			lcd_writecd(0,0x90);
			lcd_writecd(1,second/10+'0');
			lcd_writecd(1,second%10+'0');
		}
		delay_1ms(5);
	}
}