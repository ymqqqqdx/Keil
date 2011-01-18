#include <reg52.h>
#include "declare.h"
unsigned char code table6[]="温度与时间间隔五秒轮流显示";
char year,week,month,day,hour,minute,second,i,mode,buff,week;
char alarmhour,alarmminute;
unsigned int val;
void display1302();
void display18b20();
void init_ds1302();
void init_12232();
void displaytwo(unsigned char,unsigned char);
void delay(int);
void displaytemp(int);
void init_24c02();
int readtemp();
void write_com(unsigned char);
void write_data(unsigned char);
void adjust1302();
void keyscan();
void timer555();
void alarmsetting();
/*******主函数，初始化，分屏显示********/
void main()
{
	init_24c02();
	init_ds1302();
	init_12232();
	mode=1;
	write_com(0x80);
	for(i=0;i<14;i++)
	{
			write_data(table6[i]);
	}
	write_com(0x90);
	for(i=0;i<12;i++)
	{
			write_data(table6[i+14]);
	}
	delay(1000);
	readtemp();
	while(1)
	{
		keyscan();
		if(mode==1)
		{
			display1302();
		}
		if(mode==2)
		{
			display18b20();
		}
		if(mode==4)
		{
			adjust1302();
		}
		if(mode==3)
		{
			timer555();
		}
		if(mode==5)
		{
			alarmsetting();
		}
	}
}
