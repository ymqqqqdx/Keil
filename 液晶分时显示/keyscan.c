#include <reg52.h>
#include "declare.h"
sbit k1=P1^7;
sbit k2=P1^6;
sbit k3=P1^5;
sbit k4=P1^4;
sbit beep=P3^3;
char daymax;//每月最多的天数
char order,timetemp; //order为调时的次数
void delay(int);//buff=0为正常显示，=1为轮流显示 =2为调时模式
unsigned char read_clock(unsigned char);
void write_com(unsigned char);
void write_clock(unsigned char,unsigned char);
void write_add(unsigned char,char);
char read_add(unsigned char);
/******************月最大天数计算*********************/
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
/******************键盘扫描*****************/
void keyscan()
{
	if(k1==0)
	{
		beep=1;//关蜂鸣器
		delay(50);
		if(k1==0&&buff==0)
		{
			if(mode<=3)
			mode++;
			if(mode==4)
			{
				buff=2;
				delay(2);
			}
		}
		if(k1==0&&buff==2)
		{
			order++;
			timetemp=0;
			write_com(0x81);
			delay(200);
			switch(order)
			{
				case 1 : timetemp=read_add(1); write_com(0x81); break;
				case 2 : timetemp=read_add(2); write_com(0x83); break;
				case 3 : timetemp=read_add(3); calcdaymax(); write_com(0x85); break;
				case 4 : timetemp=read_add(4); write_com(0x90); break;
				case 5 : timetemp=read_add(5); write_com(0x92); break;
				case 6 : timetemp=read_add(6); write_com(0x94); break;
				case 7 : timetemp=read_add(7); write_com(0x97); break;
				case 8 : timetemp=alarmhour; mode=5; write_com(0x92); break;
				case 9 : timetemp=alarmminute; write_com(0x94); break;
				case 10 : order=0; buff=0; mode=1; break;
			}
			delay(5);
		}
		if(k1==1)
		delay(10);
		while(k1==0);
	}
	if(k2==0)
	{
		delay(50);
		beep=1;
		if(k2==0&&buff==2)
		{
			switch(order)
			{
				case 1 : timetemp++; if(timetemp>=100) timetemp=0; if(timetemp<0) timetemp=99; year=timetemp; writeyear; break;
				case 2 : timetemp++; if(timetemp>=13) timetemp=1; if(timetemp<1) timetemp=12; month=timetemp; writemonth; break;
				case 3 : timetemp++; if(timetemp>daymax) timetemp=1; if(timetemp<1) timetemp=daymax; day=timetemp; writeday; break;
				case 4 : timetemp++; if(timetemp>=24) timetemp=0; if(timetemp<0) timetemp=23; hour=timetemp; writehour; break;
				case 5 : timetemp++; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; minute=timetemp; writeminute; break;
				case 6 : timetemp++; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; second=timetemp; writesecond; break;
				case 7 : timetemp++; if(timetemp>=8) timetemp=1; if(timetemp<=0) timetemp=7; week=timetemp; writeweek; break;
				case 8 : timetemp++; if(timetemp>=24) timetemp=0; if(timetemp<0) timetemp=23; alarmhour=timetemp;break;			
				case 9 : timetemp++; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; alarmminute=timetemp;break;	
			}
			write_add(order,timetemp);
		}
		if(k2==1);
		delay(10);
		while(k2==0);
	}
	if(k3==0)
	{
		delay(50);
		beep=1;
		if(k3==0&&buff==2)
		{
			switch(order)
			{
				case 1 : timetemp--; if(timetemp>=100) timetemp=0; if(timetemp<0) timetemp=99; year=timetemp; writeyear; break;
				case 2 : timetemp--; if(timetemp>=13) timetemp=1; if(timetemp<1) timetemp=12; month=timetemp; writemonth; break;
				case 3 : timetemp--; if(timetemp>daymax) timetemp=1; if(timetemp<1) timetemp=daymax; day=timetemp; writeday; break;
				case 4 : timetemp--; if(timetemp>=24) timetemp=0; if(timetemp<0) timetemp=23; hour=timetemp; writehour; break;
				case 5 : timetemp--; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; minute=timetemp; writeminute; break;
				case 6 : timetemp--; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; second=timetemp; writesecond; break;
				case 7 : timetemp--; if(timetemp>=8) timetemp=1; if(timetemp<=0) timetemp=7; week=timetemp; writeweek; break;
				case 8 : timetemp--; if(timetemp>=24) timetemp=0; if(timetemp<0) timetemp=23; alarmhour=timetemp;break;			
				case 9 : timetemp--; if(timetemp>=60) timetemp=0; if(timetemp<0) timetemp=59; alarmminute=timetemp;break;	
			}
			write_add(order,timetemp);
		}
		if(k3==1)
		delay(10);
		while(k3==0);
	}	
}
