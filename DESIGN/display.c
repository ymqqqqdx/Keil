#include <reg52.h>
#include "declare.h"
#define decode_mode 0X09
unsigned char secondold,minuteold,hourold,dayold,monthold,yearold,weekold;
unsigned char alarmhourold,alarmminuteold;
unsigned char code table[]={0x77,0x0e,0x77,0x77,0x76,0x76};
void write7219one(unsigned char,unsigned char);
void write7219two(unsigned char,unsigned char);
void write7219three(unsigned char,unsigned char);
void delay(int);
void keyscan();
unsigned char read_clock(unsigned char);
void write7219one(unsigned char,unsigned char);
void display18b20();
void refreshtime()
{
	hour=readhour;
	write7219one(0x07,hour/10);
	write7219one(0x08,hour%10);

	minute=readminute;
	write7219two(0x01,minute/10);
	write7219two(0x02,minute%10);

	second=readsecond;
	write7219two(0x03,second/10);
	write7219two(0x04,second%10);
	
	week=readweek;
	if(week==1) week=7;
	else week-=1;
	write7219two(0x05,week);

	year=readyear;
	write7219one(0x01,year/10);
	write7219one(0x02,year%10);

	month=readmonth;
	write7219one(0x03,month/10);
	write7219one(0x04,month%10);

	day=readday;
	write7219one(0x05,day/10);
	write7219one(0x06,day%10);
}
void refreshsecond()
{
	second=readsecond;
	write7219two(0x03,second/10);
	write7219two(0x04,second%10);
}
void displaytime()
{
	write7219one(decode_mode,0xff);
	refreshtime();
	secondold=readsecond;
	while(mode==0)
	{
		display18b20();
		second=readsecond;
		delay(50);
		if(second!=secondold)
		{
			refreshsecond();
			secondold=second;
			if(second==0)
			{
				refreshtime();
			}
		}
		keyscan();
	}
}
void adjusttime()
{
	write7219one(decode_mode,0xff);
	refreshtime();
	yearold=year;weekold=week;monthold=month;
	dayold=day;hourold=hour;minuteold=minute;secondold=second;
	while(mode==1)
	{
		display18b20();
		if(hour!=hourold)
		{
			write7219one(0x07,hour/10);
			write7219one(0x08,hour%10);
			hourold=hour;
		}
		if(minute!=minuteold)
		{
			write7219two(0x01,minute/10);
			write7219two(0x02,minute%10);
			minuteold=minute;
		}
		if(second!=secondold)
		{
			write7219two(0x03,second/10);
			write7219two(0x04,second%10);
			secondold=second;
		}
		if(week!=weekold)
		{
			unsigned char showweek;
			if(week==1) showweek=7;
			else showweek=week-1;
			write7219two(0x05,showweek);
			weekold=week;
		}
		if(year!=yearold)
		{
			write7219one(0x01,year/10);
			write7219one(0x02,year%10);
			yearold=year;
		}
		if(month!=monthold)
		{
			write7219one(0x03,month/10);
			write7219one(0x04,month%10);
			monthold=month;
		}
		if(day!=dayold)
		{
			write7219one(0x05,day/10);
			write7219one(0x06,day%10);
			dayold=day;
		}
		keyscan();
	}
}
void displayalarm()
{
	write7219one(decode_mode,0xc0);
	for(i=1;i<7;i++)
	{
		write7219one(i,table[i-1]);
	}
	write7219one(0x07,alarmhour/10);
	write7219one(0x08,alarmhour%10);
	write7219two(0x01,alarmminute/10);
	write7219two(0x02,alarmminute%10);	
	write7219two(0x03,0);
	write7219two(0x04,0);
	alarmhourold=alarmhour;
	alarmminuteold=alarmminute;
	while(mode==2)
	{
		display18b20();
		if(alarmhour!=alarmhourold)
		{
			write7219one(0x07,alarmhour/10);
			write7219one(0x08,alarmhour%10);
			alarmhourold=alarmhour;
		}
		if(alarmminute!=alarmminuteold)
		{
			write7219two(0x01,alarmminute/10);
			write7219two(0x02,alarmminute%10);
			alarmminuteold=alarmminute;
		}
		keyscan();
	}
}
