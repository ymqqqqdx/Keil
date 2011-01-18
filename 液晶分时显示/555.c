#include <reg52.h>
#include "declare.h"
unsigned char flag;
unsigned int count;
unsigned char code table3[]="555 振荡器频率 Hz";
void displayfrequency(unsigned int);
void write_com(unsigned char);
void write_data(unsigned char);
void keyscan();
/*****************读取频率********************/
void timer555()
{
	TMOD=0x15;
	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	TH0=0;
	TL0=0;
	EA=1;
	ET1=1;
	TR1=1;
	TR0=1;
	count=0;
	write_com(0x01);
	write_com(0x80);
	for(i=0;i<14;i++)
	{
		write_data(table3[i]);
	}
	write_com(0x94);
	write_data(table3[15]);
	write_data(table3[16]);	
	displayfrequency(val);
   while(mode==3)
   {
		keyscan();
	}
	TR1=0;
}
/***************用定时器1定时一秒钟，读取TH0和TL0的值****************/
void timer1() interrupt 3
{
		TR1=0;
		TH1=(65536-50000)/256;
	   TL1=(65536-50000)%256;
		count++;
		if(count==20)
		{
			TR0=0;	
			count=0;
			val=TH0*256+TL0;
			displayfrequency(val);
			TH0=TL0=0;
			TR1=TR0=1;
		}
		else TR1=1;
}