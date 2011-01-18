#include <reg52.h>
#include "declare.h"
sbit dula=P2^0;
sbit datese=P2^1;
sbit timese=P2^2;
sbit tempse=P2^3;
unsigned char secondold;
unsigned char year,week,month,day,hour,minute,second,i,week;
void display1302();
void gettime();
void display18b20();
unsigned char read_clock(unsigned char);
void main()
{
	timese=0;
	tempse=0;
	gettime();
	TMOD=0x01;
	TH0=(65536-10000)/256; 
	TL0=(65536-10000)%256;
	EA=1; //开总中断
	ET0=1; //计时器0开启
	TR0=1; //计时器开始运行
	secondold=readsecond;
	while(1)
	{
		display1302();
		display18b20();
	}
}
