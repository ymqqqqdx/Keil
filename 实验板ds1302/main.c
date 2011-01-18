#include <reg52.h>
#include "declare.h"
unsigned char code table1[]="ƒÍ‘¬»’:";
unsigned char secondold;
void delay(int);
void init_12232();
void write_com(unsigned char);
void write_data(unsigned char);
void displaytwo(unsigned char,unsigned char);
void init_ds1302();
unsigned char read_clock(unsigned char);
unsigned char year,month,day,hour,minute,second;
void gettime()
{
		displaytwo(0,readyear);
		delay(5);
		displaytwo(2,readmonth);
		delay(5);
		displaytwo(4,readday);
		delay(5);
		displaytwo(11,readhour);
		delay(5);
		displaytwo(13,readminute);
		delay(5);
		displaytwo(15,readsecond);
		delay(5);	
}
void main()
{
	init_ds1302();
	init_12232();
	write_com(0x81);
	write_data(table1[0]);
	write_data(table1[1]);
	write_com(0x83);
	write_data(table1[2]);
	write_data(table1[3]);
	write_com(0x85);
	write_data(table1[4]);
	write_data(table1[5]);
	write_com(0x92);
	write_data(table1[6]);
	write_com(0x94);
	write_data(table1[6]);
	gettime();
	secondold=readsecond; 
	while(1)
	{
		second=readsecond;
		delay(100);
		if(second!=secondold&&second<60) 
		{
			displaytwo(15,second); secondold=second;
			if(second==0)
			{
				gettime();
			}
		}	  
	}
}
