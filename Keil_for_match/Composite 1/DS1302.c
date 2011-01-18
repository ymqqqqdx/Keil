#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "declare.h"
#define U8 unsigned char
#define U16 unsigned int

sbit rtc_clk=P1^6;
sbit rtc_io=P1^7;
sbit rtc_rst=P3^0;

extern RTC time;
extern U8 stat;
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
	return (temp>>4)*10+temp%16;
}
void rtc_write(U8 add,U8 dat)
{
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	rtc_writeonechar(add);
	rtc_writeonechar((dat/10<<4)|(dat%10));
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
void rtc_gettime()
{
	U8 *pt=&time.second,i;
	for(i=0;i<7;i++)
		*pt++=rtc_read(0x80+2*i);
	lcd_showtime();
	hd_showtime();
}
void rtc_settime(U8 off,U8 dat)
{
	U8 *pt=&time.second;
	*(pt+off)=dat;
}
void rtc_writertc()
{
	U8 *pt=&time.second,i;
	for(i=0;i<7;i++)
		rtc_write(0x80+2*i,*pt++);
	stat=0;
}
bit rtc_range(U8 off,U8 dat)
{
	U8 i=~0;
	if( dat<range[off][0] || dat>range[off][1])
	{
		beep=0;
		while(i--);
		beep=1;
		return 1;
	}
	else return 0;
}