#include <reg52.h>
#include <intrins.h>
#include "declare.h"
#define uchar unsigned char
#define uint unsigned int
sbit bee=P3^3;
unsigned char year,week,month,day,hour,minute,second,i,week,mode;
unsigned char code tablecc[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
uchar code tableca[]={0xC0,/*0*/0xF9,/*1*/0xA4,/*2*/0xB0,/*3*/
0x99,/*4*/0x92,/*5*/0x82,/*6*/0xF8,/*7*/0x80,/*8*/0x90,/*9*/0x88,/*A*/0x83,/*b*/
0xC6,/*C*/0xA7,/*c*/0xA1,/*d*/0x86,/*E*/0x8E,/*F*/};
unsigned char code pointcc[]={0xBF,/*0p*/0x86,/*1p*/0xDB,/*2p*/0xCF,/*3p*/
0xE6,/*4p*/0xED,/*5p*/0xFD,/*6p*/0x87,/*7p*/0xFF,/*8p*/0xEF,/*9p*/};
unsigned char code pointca[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
uint temp,tempold;
uint readtemp();
void write_onechar(uchar);
void delay(int);
void init7219();
void write7219one(uchar,uchar);
void refreshtime();
void init_ds1302();
uchar read_clock(uchar);
void beep()
{
	bee=0;
	delay(20);
	bee=1;
}
void ls164()
{
	uchar i;
	beep();	
	i=15;
	while(i--)
	{
		write_onechar(tablecc[15-i]);
		delay(1000);
	}	
}
void led()
{
	uchar i;
	beep();	
	P2=0xfe;
	for(i=0;i<4;i++)
	{
		if(i)
		P2=_crol_(P2,2);
		delay(1000);
	}
	P2=0xfd;
	for(i=0;i<4;i++)
	{
		if(i)
		P2=_crol_(P2,2);
		delay(1000);
	}
	i=3;
	while(i--)
	{
		P2=0x00; delay(1000);
		P2=0xff; delay(1000);
	}
}
void traffic()
{
	uchar i=2;
	beep();	
	while(i--)
	{
		P0=0x21; delay(1000);
		P0=0x12; delay(1000);
		P0=0x0c; delay(1000);
	}
}
void segscan()
{
	uchar i;
	uint t=200;
	beep();	
	while(t--)
	{
		i=5;
		P1=0xef;
		while(i--)
		{
		P0=tableca[i];
		if(i-4!=0)
		P1=_crol_(P1,1);
		delay(5);
		P0=0xff;
		}
	}
}
void test7219()
{
	uchar i;
	beep();
	init7219();
	for(i=0;i<16;i++)
		write7219one(i+1,15);
	for(i=0;i<16;i++)
	{
		write7219one(i+1,i%8+1);
		delay(500);
	}
	delay(1000);
	
}
void ds1302()
{
	uchar secondold;
	uchar shi,ge,one;
	beep();
	init_ds1302();
	refreshtime();
	write7219one(2,10); write7219one(7,10); write7219one(10,10); write7219one(15,10); delay(2000);
	while(1)
	{
		second=readsecond;
		temp=readtemp();
		delay(5);
		if(second!=secondold)
		{
			write7219one(11,second/10);write7219one(12,second%10);
			if(second==0)
			refreshtime();
			secondold=second;
		}
		if(temp!=tempold)
		{
			shi=temp%1000/100;
			ge=temp%100/10;
			one=temp%10;
			write_onechar(0x00);
			write_onechar(tablecc[shi]);
			write_onechar(pointcc[ge]);
			write_onechar(tablecc[one]);
			tempold=temp;
		}
	}
}
void main()
{
//	led();
//	traffic();
//	ls164();
//	segscan();
//	test7219();
	init7219();
	ds1302();
	while(1);
}