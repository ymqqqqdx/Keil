#include <reg52.h>
#include "declare.h"
sbit k1=P1^3;
sbit k2=P1^4;
sbit k3=P1^5;
char count,daymax,timetemp;
void delay(int);
void write_clock(unsigned char,unsigned char);
void displayalarm();
void refreshtime();
unsigned char read_clock(unsigned char);
void write7219one(unsigned char,unsigned char);
void calcdaymax()
{
	if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
		daymax=31;
	else if(month==4||month==6||month==9||month==11)
		daymax=30;
	else if(month==2)
	{
		if((year+2000)%4!=0)
			daymax=28;
		else
			daymax=29;
	}
}
void keyscan()
{
	if(k1==0)
	{
		delay(50);
		if(k1==0&&mode==0)
		{
			mode++;
		}
		if(k1==0&&mode!=0)
		{
			count++;
			timetemp=0;
			switch(count)
			{
				case 1 : timetemp=readyear; P0=0x01; break;
				case 2 : timetemp=readmonth; P0=0x02; break;
				case 3 : timetemp=readday; P0=0x04; calcdaymax(); break;
				case 4 : timetemp=readweek; P0=0x08; break;
				case 5 : timetemp=readhour; P0=0x10; break;
				case 6 : timetemp=readminute; P0=0x20; break;
				case 7 : timetemp=readsecond; P0=0x40; break;
				case 8 : mode=2; timetemp=alarmhour; P0=0x10; break;
				case 9 : timetemp=alarmminute; P0=0x20; break;
				case 10 : count=0; mode=0; P0=0x00; break;
			}
			delay(5);
		}
		if(k1==1) delay(30);
		while(k1==0);
	}
	if(k2==0)
	{
		delay(50);
		if(k2==0&&mode==0)
		{
			displayalarm();
			delay(3000);
			write7219one(0x09,0x0ff);
			refreshtime();
		}
		if(k2==0&&mode!=0)
		{
			switch(count)
			{
				case 1 : timetemp++; if(timetemp>=100) timetemp=0; if(timetemp<0) timetemp=99; year=timetemp; writeyear; break;
				case 2 : timetemp++; if(timetemp>=13) timetemp=1; if(timetemp<1) timetemp=12; month=timetemp; writemonth; break;
				case 3 : timetemp++; if(timetemp>daymax) timetemp=1; if(timetemp<1) timetemp=daymax; day=timetemp; writeday; break;
				case 5 : timetemp++; if(timetemp>=24) timetemp=0; if(timetemp<0) timetemp=23; hour=timetemp; writehour; break;
				case 6 : timetemp++; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; minute=timetemp; writeminute; break;
				case 7 : timetemp++; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; second=timetemp; writesecond; break;
				case 4 : timetemp++; if(timetemp>=8) timetemp=1; if(timetemp<=0) timetemp=7; week=timetemp; writeweek; break;
				case 8 : timetemp++; if(timetemp>=24) timetemp=0; if(timetemp<0) timetemp=23; alarmhour=timetemp; break;
				case 9 : timetemp++; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; alarmminute=timetemp; break;
			}
			if(k2==1) delay(30);
			while(k2==0);
		}
	}
	if(k3==0)
	{
		delay(50);
		if(k3==0&&mode!=0)
		{
			switch(count)
			{
				case 1 : timetemp--; if(timetemp>=100) timetemp=0; if(timetemp<0) timetemp=99; year=timetemp; writeyear; break;
				case 2 : timetemp--; if(timetemp>=13) timetemp=1; if(timetemp<1) timetemp=12; month=timetemp; writemonth; break;
				case 3 : timetemp--; if(timetemp>daymax) timetemp=1; if(timetemp<1) timetemp=daymax; day=timetemp; writeday; break;
				case 5 : timetemp--; if(timetemp>=24) timetemp=0; if(timetemp<0) timetemp=23; hour=timetemp; writehour; break;
				case 6 : timetemp--; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; minute=timetemp; writeminute; break;
				case 7 : timetemp--; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; second=timetemp; writesecond; break;
				case 4 : timetemp--; if(timetemp>=8) timetemp=1; if(timetemp<=0) timetemp=7; week=timetemp; writeweek; break;
				case 8 : timetemp--; if(timetemp>=24) timetemp=0; if(timetemp<0) timetemp=23; alarmhour=timetemp; break;
				case 9 : timetemp--; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; alarmminute=timetemp; break;
			}
			if(k3==1) delay(30);
			while(k3==0);			
		}
	}
}
