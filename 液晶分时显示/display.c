#include <reg52.h>
#include "declare.h"
sbit beep=P3^3;
int tempold,tempnew;
unsigned char secondold,minuteold,hourold,dayold,monthold,yearold,weekold,alarmhourold,alarmminuteold;
unsigned char code table[]="0123456789";
unsigned char code table1[]="年月日: 周";
unsigned char code table2[]="板上18B20 温度:摄氏度 ";
unsigned char code table4[]=" 7123456";
unsigned char code table11[]="闹钟设置";
void write_com(unsigned char);
void write_data(unsigned char);
void delay(int);
void displaytwo(unsigned char,unsigned char);
unsigned char keyscan();
int readtemp();
unsigned char read_clock(unsigned char);
void displaytemp(int);
void write_com(unsigned char);
void write_add(unsigned char,unsigned char);
/*************获取显示当前时间***************/
void gettime()
{
		displaytwo(1,readyear);
		write_add(1,readyear);
		delay(5);
		displaytwo(3,readmonth);
		write_add(2,readmonth);
		delay(5);
		displaytwo(5,readday);
		write_add(3,readday);
		delay(5);
		displaytwo(10,readhour);
		write_add(4,readhour);
		delay(5);
		displaytwo(12,readminute);
		write_add(5,readminute);
		delay(5);
		displaytwo(14,readsecond);
		write_add(6,readsecond);
		week=readweek;
		write_com(0x97);
			write_data(table4[week]);
		write_add(7,readweek);
		delay(5);	
}
/*************初始化1302显示界面**************/
void init1302display()//初始化1302显示
{
	write_com(0x01);
	write_com(0x0c);
	write_com(0x82);
	write_data(table1[0]);
	write_data(table1[1]);
	write_com(0x84);
	write_data(table1[2]);
	write_data(table1[3]);
	write_com(0x86);
	write_data(table1[4]);
	write_data(table1[5]);
	write_com(0x91);
	write_data(table1[6]);
	write_com(0x93);
	write_data(table1[6]);
	write_com(0x80);
	write_data('2');
	write_data('0');
	write_com(0x96);
	write_data(table1[8]);
	write_data(table1[9]);
	gettime();
	write_com(0x81);
}
/*****************正常显示界面***************/
void display1302()
{
	init1302display();
	secondold=readsecond; 
	while(mode==1)
	{
		second=readsecond;
		delay(100);
		if(second!=secondold&&second<60) 
		{
			displaytwo(14,second);
		   write_add(6,second); secondold=second;
			if(second==0)
			{
				gettime();
				if(readminute==alarmminute&&readhour==alarmhour)
				{
					beep=0;
				}
			}
		}
		keyscan();
	}	
}
/***************18B20显示界面*******************/
void display18b20()
{
	write_com(0x01);
	write_com(0x0c);
	write_com(0x80);
	for(i=0;i<15;i++)
	{
		write_data(table2[i]);
	}
	write_com(0x93);
	for(i=0;i<6;i++)
	{
		write_data(table2[i+15]);
	}
	displaytemp(tempold);
	while(mode==2)
	{
		tempnew=readtemp();
		delay(100);
		if(tempnew!=tempold)
		{
			displaytemp(tempnew);
			tempold=tempnew;
		}
		keyscan();
	}
}
/*****************调整时间显示界面******************/
void adjust1302()
{
	init1302display();
	write_com(0x0f);//有光标闪
	delay(100);
	yearold=year; weekold=week; monthold=month; dayold=day; hourold=hour; minuteold=minute; secondold=second;
	while(mode==4)
	{
		if(year!=yearold)
		{
			displaytwo(1,year);
			yearold=year;
			write_com(0x81);
		}
		if(month!=monthold)
		{
			displaytwo(3,month);
			monthold=month;
			write_com(0x83);
		}
		if(day!=dayold)
		{
			displaytwo(5,day);
			dayold=day;
			write_com(0x85);
		}
		if(hour!=hourold)
		{
			displaytwo(10,hour);
			hourold=hour;
			write_com(0x90);
		}
		if(minute!=minuteold)
		{
			displaytwo(12,minute);
			minuteold=minute;
			write_com(0x92);
		}
		if(second!=secondold)
		{
			displaytwo(14,second);
			secondold=second;
			write_com(0x94);
		}
		if(week!=weekold)
		{
			write_com(0x97);
				write_data(table4[week]);
			write_com(0x97);
		}
		keyscan();
	}
}
/****************闹钟设置界面*****************/
void alarmsetting()
{
	write_com(0x01);
	write_com(0x82);
	for(i=0;i<8;i++)
	{
		write_data(table11[i]);
	}
	displaytwo(12,alarmhour);
	write_data(':');
	write_data(' ');
	displaytwo(14,alarmminute);
	alarmminuteold=alarmminute;
	alarmhourold=alarmhour;
	write_com(0x92);
	while(mode==5)
	{
		if(alarmminute!=alarmminuteold)
		{
			displaytwo(14,alarmminute);
			write_com(0x94);
			alarmminuteold=alarmminute;
		}
		if(alarmhour!=alarmhourold)
		{
			displaytwo(12,alarmhour);
			write_com(0x92);
			alarmhourold=alarmhour;
		}
		keyscan();
	}
}
