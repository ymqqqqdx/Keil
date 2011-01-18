#define readyear read_clock(0x8d)
#define readweek read_clock(0x8b)
#define readmonth read_clock(0x89)
#define readday read_clock(0x87)
#define readhour read_clock(0x85)
#define readminute read_clock(0x83)
#define readsecond read_clock(0x81)
#define writeyear write_clock(0x8c,year)
#define writeweek write_clock(0x8a,week)
#define writemonth write_clock(0x88,month)
#define writeday write_clock(0x86,day)
#define writehour write_clock(0x84,hour)
#define writeminute write_clock(0x82,minute)
#define writesecond write_clock(0x80,second)
#include <stdio.h>
#include <reg52.h>
#include "declare.h"
sbit  DS1302_CLK = P3^7;              //实时时钟时钟线引脚 
sbit  DS1302_IO  = P3^6;              //实时时钟数据线引脚 
sbit  DS1302_RST = P2^5;              //实时时钟复位线引脚
sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;
static unsigned char year,week,month,day,hour,minute,second,secondold;
void refresh();
unsigned char bcdconvert(unsigned char bcd)
{
	unsigned char target;
	target=bcd>>4;
	target=target*10+(bcd&=0x0f);
	return(target);
}
unsigned char convertbcd(unsigned char ten)
{
	unsigned char bcd,a,b;
	a=ten/10;
	b=ten%10;
	a=a<<4;
	bcd=(a&=0xf0)+(b&=0x0f);
	return(bcd);
}
void DS1302InputByte(unsigned char d) 	//实时时钟写入一字节(内部函数)
{ 
    unsigned char i;
    ACC = d;
    for(i=8; i>0; i--)
    {
        DS1302_IO = ACC0;           	//相当于汇编中的 RRC
        DS1302_CLK = 1;
        DS1302_CLK = 0;
        ACC = ACC >> 1; 
    } 
}
unsigned char DS1302OutputByte(void) 	//实时时钟读取一字节(内部函数)
{ 
    unsigned char i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         			//相当于汇编中的 RRC 
        ACC7 = DS1302_IO;
        DS1302_CLK = 1;
        DS1302_CLK = 0;
    } 
    return(ACC); 
}
void write_clock(unsigned char ucAddr, unsigned char ucDa)	//ucAddr: DS1302地址, ucData: 要写的数据
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr);       	// 地址，命令 
    DS1302InputByte(convertbcd(ucDa));       	// 写1Byte数据
    DS1302_CLK = 1;
    DS1302_RST = 0;
} 
unsigned char read_clock(unsigned char ucAddr)	//读取DS1302某地址的数据
{
    unsigned char ucData;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr|0x01);        // 地址，命令 
    ucData = DS1302OutputByte();         // 读1Byte数据
    DS1302_CLK = 1;
    DS1302_RST = 0;
    return(bcdconvert(ucData));
}
void init_ds1302()
{
	year=10; writeyear;
	month=3; writemonth;
	day=15; writeday;
	hour=17; writehour;
	minute=41; writeminute;
	second=14; writesecond;
}
unsigned char tm;
unsigned int aa;
int forpr;
void gettime();
void delay(int);
void comm_init();
void ds1302commscan()
{
	mode=_getkey();
	switch(mode)
	{
		case '1' : temp=readyear; forpr=temp; tm='1'; printf("%c#%2d",mode,forpr); break;
		case '2' : temp=readmonth; forpr=temp; tm='2'; printf("%c#%2d",mode,forpr); break;
		case '3' : temp=readday; forpr=temp; tm='3'; printf("%c#%2d",mode,forpr); break;
		case '4' : temp=readhour; forpr=temp; tm='4'; printf("%c#%2d",mode,forpr); break;
		case '5' : temp=readminute; forpr=temp; tm='5'; printf("%c#%2d",mode,forpr); break;
		case '6' : temp=readsecond; forpr=temp; tm='6'; printf("%c#%2d",mode,forpr); break;
		case 'g' : gettime(); break;
		case 'i' : init_ds1302(); break;
		case '0' : ES=1;
					  do
					  {
					  		secondold=readsecond; 
					  }
					  while(secondold>=60);
					  while(mode=='0')
					  {
							refresh();
					  }
					  ES=0;
	}
	switch(mode)
	{
		case '+' : temp++;
			switch(tm)
			{
				case '1' : year=temp; writeyear; break;
				case '2' : month=temp; writemonth; break;
				case '3' : day=temp; writeday; break;
				case '4' : hour=temp; writehour; break;
				case '5' : minute=temp; writeminute; break;
				case '6' : second=temp; writesecond; break;
			}
			forpr=temp; printf("%c#%2d",tm,forpr);
	      break;
		case '-' : temp--;
			switch(tm)
			{
				case '1' : year=temp; writeyear; break;
				case '2' : month=temp; writemonth; break;
				case '3' : day=temp; writeday; break;
				case '4' : hour=temp; writehour; break;
				case '5' : minute=temp; writeminute; break;
				case '6' : second=temp; writesecond; break;
			}
			forpr=temp; printf("%c#%2d",tm,forpr);
	 		break;
	}
}
void refresh()
{
	second=readsecond;
	delay(20);
	if(second!=secondold&&second<60) 
	{
		forpr=second; printf("6#%2d",forpr); secondold=second;
		if(second==0)
		{
			temp=readyear; forpr=temp; printf("1#%2d",forpr);
			temp=readmonth; forpr=temp; printf("2#%2d",forpr);
			temp=readday; forpr=temp; printf("3#%2d",forpr);
			temp=readhour; forpr=temp; printf("4#%2d",forpr);
			temp=readminute; forpr=temp; printf("5#%2d",forpr);
		}
	}
}
void gettime()
{
			temp=readyear; forpr=temp; printf("1#%2d",forpr);
			temp=readmonth; forpr=temp; printf("2#%2d",forpr);
			temp=readday; forpr=temp; printf("3#%2d",forpr);
			temp=readhour; forpr=temp; printf("4#%2d",forpr);
			temp=readminute; forpr=temp; printf("5#%2d",forpr);
			temp=readsecond; forpr=temp; printf("6#%2d",forpr);
}
void ds1302()
{  
//	unsigned char buf[20];
   comm_init();
   SCON=0x52;
	write_clock(0x90,0xab);  //允许涓流冲电
//	if(readsecond>60)
//	{
//		second=12; writesecond;
//	}
	init_ds1302();
	delay(50);
	while(mode!='#')
	{
		ds1302commscan();
   }
}
