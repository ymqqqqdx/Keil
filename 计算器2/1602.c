#include <reg52.h>
#include "declare.h"
#include <string.h>
sbit ln=P2^1;
sbit ls=P2^0;
void delay(int);
void write_com(unsigned char com)
{
	ls=0;
    P0=com;
    delay(5);
    ln=1;
    delay(5);
    ln=0;
}
void write_data(unsigned char date)
{
	ls=1;
    P0=date;
    delay(5);
    ln=1;
    delay(5);
    ln=0;
}
void init_1602()
{
   write_com(0x38);
   write_com(0x0c);
   write_com(0x06);
   write_com(0x01);
}
void moveto(unsigned char row,unsigned char line)
{
	if(row==1)
		write_com(0x80+line);
	if(row==2)
		write_com(0x80+0x40+line);
}
void clear(unsigned char t)
{
	if(t==1)
	{
		write_com(0x80);
	}
	if(t==2)
	{
		write_com(0x80+0x40);
	}
	for(i=0;i<15;i++)
	{
		write_data(' ');
	}
}
cleardata(unsigned char t)
{
	if(t==1)
	{
		for(i=0;i<15;i++)
		first[i]='\0';
	}
	if(t==2)
	{
		for(i=0;i<15;i++)
		second[i]='\0';
	}
}
void display(unsigned char *temp)
{
	char n;
//	sprintf(buf,"%.3f\0",date);
	n=strlen(temp);
	i=n;
	if(n<=0)
	{
		clear(2);
		write_com(0x01);
		write_com(0x80+0x40+14);
		write_data('0');
		write_data('.');
	}
	else
	{
/*		while(temp[i-2]!='0')
		{
			temp[i-1]='\0';
			i--;
		}*/
		if(n>16) n=16;
		clear(2);
		write_com(0x80+0x40+16-n);
		for(i=0;i<16;i++)
		write_data(temp[i]);
	}
}
void displayup(unsigned char *temp)
{
	unsigned char n;
	n=strlen(temp);
	for(i=0;i<n;i++)
	write_data(temp[i]);
}