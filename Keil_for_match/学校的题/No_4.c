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

sbit DQ=P1^0;
void rtc_gettime();
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
void delayus(U16 z)
{
	while(z--);
}
void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
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

void temp_init()
{
	bit s;
	DQ=1;
	delayus(8);
	DQ=0;
	delayus(80);
	DQ=1;
	delayus(14);
	s=DQ;
	delayus(20);
}

void temp_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat>>=1;
		DQ=0;
		DQ=CY;
		delayus(5);
		DQ=1;
	}
	delayus(4);
}

U8 temp_readonechar()
{
	U8 i,temp=0;
	for(i=0;i<8;i++)
	{
		DQ=0;
		DQ=1;
		temp|=((U8)DQ)<<i;
		delayus(4);
	}
	return temp;
}

U16 temp_read()
{
	U8 low,high;
	U16 t;
	float tt;
	temp_init();
	temp_writeonechar(0xcc);
	temp_writeonechar(0x44);
	temp_init();
	temp_writeonechar(0xcc);
	temp_writeonechar(0xbe);
	low=temp_readonechar();
	high=temp_readonechar();
	t=high;
	t<<=8;
	t|=low;
	tt=t*0.0625;
	t=tt*10+0.5;
	return t;
}



void lcd_normal_init()
{
	lcd_writecd(0,0x01);
	delay_1ms(5);
	lcd_string(0x83,":   : ");
	lcd_string(0x90,"20");
	lcd_string(0x92,"-   - ");
	lcd_string(0x96,"ÖÜ");
	rtc_gettime();
}
void lcd_temp_init()
{
	lcd_writecd(0,0x01);
	delay_1ms(5);
	lcd_string(0x81,"µ±Ç°ÊÒÎÂ£º");
}
void lcd_displaytwo(U8 add,U8 dat)
{
	lcd_writecd(0,add);
	lcd_writecd(1,dat/10+'0');
	lcd_writecd(1,dat%10+'0');
}
void rtc_gettime()
{
	U8 i,*pt=&time.second;
	for(i=0;i<7;i++)
		*pt++=rtc_read(0x80+(i*2));
	lcd_displaytwo(0x82,time.hour);
	lcd_displaytwo(0x84,time.minute);
	lcd_displaytwo(0x86,time.second);
	lcd_displaytwo(0x91,time.year);
	lcd_displaytwo(0x93,time.month);
	lcd_displaytwo(0x95,time.day);
	lcd_writecd(0,0x97);
	lcd_writecd(1,time.week+'0');
}
void lcd_normal()
{
	time.second=rtc_read(0x80);
	if(time.second!=second_old)
	{
		second_old=time.second;
		lcd_writecd(0,0x86);
		lcd_writecd(1,time.second/10+'0');
		lcd_writecd(1,time.second%10+'0');
		if(time.second==0)
			rtc_gettime();	
	}
}
void lcd_temp()
{
	U16 dat=temp_read();
	if(dat!=temp_old)
	{
		temp_old=dat;
		lcd_writecd(0,0x92);
		lcd_writecd(1,dat/100+'0');
		lcd_writecd(1,dat%100/10+'0');
		lcd_writecd(1,'.');
		lcd_writecd(1,dat%10+'0');
		lcd_string(0x95,"¡æ");
	}
}
void main()
{
	U16 i,j;
	lcd_writecd(0,0x0c);
	rtc_write(0x8e,0);
	lcd_normal_init();
	while(1)
	{
		lcd_normal_init();
		for(i=0;i<150;i++)
		{
			for(j=0;j<110;j++)
				lcd_normal();
		}
		lcd_temp_init();
		for(i=0;i<10;i++)
		{
			for(j=0;j<110;j++)
				lcd_temp();
		}
	}
}