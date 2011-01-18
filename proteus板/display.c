#include <reg52.h>
#include "declare.h"
sbit dula=P2^0;
sbit datese=P2^1;
sbit timese=P2^2;
sbit tempse=P2^3;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71,0x00};
unsigned char code table2[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xff};
unsigned char code point[]={0xBF,0x86,0xDB, 0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF};
unsigned char read_clock(unsigned char);
int tempnew,aa;
void delay(int z);
int readtemp();
void displayonechar(unsigned char sel,unsigned char add,unsigned char dat)
{
	if(sel==1)
	{
		datese=1;
		P3=table2[add-1];
		datese=0;
		P3=0xff;
		dula=1;
		P3=table[dat];
		dula=0;
		P3=0x00;
		delay(1);
	}
	if(sel==2)
	{
		timese=1;
		P3=table2[add-1];
		timese=0;
		P3=0xff;
		dula=1;
		P3=table[dat];
		dula=0;
		P3=0x00;
		delay(1);
	}
	if(sel==3)
	{
		tempse=1;
		P3=table2[add-1];
		tempse=0;
		P3=0xff;
		dula=1;
		P3=table[dat];
		dula=0;
		P3=0x00;
		delay(1);
	}
}
void displaydate(unsigned char dyear,unsigned char dmonth,unsigned char dday)
{
	displayonechar(1,1,2);
	displayonechar(1,2,0);
	displayonechar(1,3,dyear/10);
	displayonechar(1,4,dyear%10);
	displayonechar(1,5,dmonth/10);
	displayonechar(1,6,dmonth%10);
	displayonechar(1,7,dday/10);
	displayonechar(1,8,dday%10);
	displayonechar(1,9,2);//不加此行显示不正常，原因不明
}
void displaytime(unsigned char dhour,unsigned char dminute,unsigned char dsecond,unsigned char dweek)
{
	displayonechar(2,1,dhour/10);
	displayonechar(2,2,dhour%10);
	displayonechar(2,3,dminute/10);
	displayonechar(2,4,dminute%10);
	displayonechar(2,5,dsecond/10);
	displayonechar(2,6,dsecond%10);
	displayonechar(2,7,dweek);
	displayonechar(2,8,3);//同上
}
void gettime()
{
	hour=readhour; minute=readminute; second=readsecond; year=readyear; month=readmonth; day=readday; week=readweek;
}
void display1302()
{
	second=readsecond;
	if(second!=secondold)
	{
		secondold=second;
		if(second==0)
		{
			gettime();
		}
	}
	displaydate(year,month,day);
	delay(5);
	displaytime(hour,minute,second,week);
	delay(5);
}
void display18b20()
{
	unsigned char qian,bai,shi,ge;
//	tempnew=readtemp();
	qian=tempnew/1000;
	bai=tempnew%1000/100;
	shi=tempnew%100/10;
	ge=tempnew%10;

	displayonechar(3,1,bai);
		tempse=1;
		P3=table2[1];
		tempse=0;
		P3=0xff;
		dula=1;
		P3=point[shi];
		dula=0;
		P3=0x00;
		delay(1);
	displayonechar(3,3,ge);
	displayonechar(3,9,1);
}
void timer0() interrupt 1
{
	TH0=(65536-10000)/256; 
	TL0=(65536-10000)%256;
	aa++;
	if(aa==50)
	{
		tempnew=readtemp();
	}
}
