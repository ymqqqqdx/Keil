#include <reg52.h>
#include "declare.h"
unsigned char year,week,month,day,hour,minute,second,i,week,mode;
unsigned char alarmhour,alarmminute;
unsigned int val;
void init7219();
void write7219one(unsigned char,unsigned char);
void write7219two(unsigned char,unsigned char);
void write7219three(unsigned char,unsigned char);
void refreshtime();
void refreshsecond();
void delay(int);
void adjusttime();
void displaytime();
void keyscan();
void display18b20();
void displayalarm();
unsigned char read_clock(unsigned char);
void main()
{
	P0=0;
	init7219();
	while(1)
	{
		keyscan();
		if(mode==0)
			displaytime();
		if(mode==1)
			adjusttime();
		if(mode==2)
			displayalarm();
	}
}
