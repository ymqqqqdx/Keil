#include <reg52.h>
sbit key1=P3^4;
sbit key2=P3^5;
sbit key3=P3^2;
sbit key4=P3^3;
sbit rtc_clk=P1^6;
sbit rtc_io=P1^7;
sbit rtc_rst=P3^0;

sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;
struct RTC
{
	U8 second;
	U8 minute;
	U8 hour;
	U8 day;
	U8 month;
	U8 week;
	U8 year;
} time;
U8 second_old;
U16 temp_old;
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
	rtc_writeonechar((dat/10)<<4 | dat%10);
	rtc_rst=0;
}

U8 rtc_read(U8 add)
{
	U8 temp=0;
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	rtc_writeonechar(add|1);
	temp=rtc_readonechar();
	rtc_rst=0;
	return temp;
}