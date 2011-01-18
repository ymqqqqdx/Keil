//2010/1/2 18:38
#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
#include "24c02.h"
sbit ln=P2^1;
sbit ls=P2^0;
sbit k1=P1^0;
sbit k2=P1^1;
sbit k3=P1^2;
sbit k4=P1^3;
sbit mode=P1^4;
uchar code table[]="0123456789";
uchar code table1[]=":.-";
uchar code table2[]="Number";
uchar code table3[]="MONTUEWEDTHUFRISATSUN";
uchar t=0,i,aa=0,bb=0,flag=0,month,day,week,hour,min,sec,min2=0,sec2=0,secp=0,num=0,max,flag2=0;
uchar mins[20]=0,secs[20]=0,secps[20]=0;
int year;
#include "1602.h"
#include "display.h"
#include "init.h"
#include "keyscan.h"   
void main()
{
	init();
	init_24c02();
	ET0=1; //计时器0开启
	TR0=1; //计时器打开
	while(1)
	{
		keyscan();
		if(flag==1)
		{
			display2();
		}
	}
}
void timer0() interrupt 1
{
	TH0=(65536-5000)/256;
   TL0=(65536-5000)%256;
	aa++;
	if(aa==200)
	{   
		aa=0;
		sec++;
		if(sec==60)
		{
			min++;
			sec=0;
			if(min==60)
			{
				hour++;
				min=0;
				if(hour==24)
				{
					day++;
					week++;
					if(week==8)
					week=1;
					write_add(7,week);
					if(flag!=1)
					displayweek(week);
					hour=0;
					if(day==32)
					{
						month++;
						day=1;
						if(month==13)
						{
						year++;
						write_add(4,year-2010);
						if(flag!=1)
						displayyear(year);
						month=1;
						}
						write_add(5,month);
						if(flag!=1)
						displaydate(6,month);
					}
					write_add(6,day);
					if(flag!=1)
					displaydate(9,day);
				}
				write_add(1,hour);
				if(flag!=1)
				displaytime(4,hour);
			}
			write_add(2,min);
			if(flag!=1)
			displaytime(7,min);
		}
		write_add(3,sec);
		if(flag!=1)
		displaytime(10,sec);
	}
}
void timer1() interrupt 3
{
	TH1=(65536-10000)/256;
   TL1=(65536-10000)%256;
	bb++;
	if(bb==1)
	{   
		bb=0;
		secp++;
		if(secp==100)
		{
			sec2++;
			secp=0;
			if(sec2==60)
			{
				min2++;
				sec2=0;
			}
		}
	}		
}

