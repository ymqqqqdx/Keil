#include <reg52.h>
#include "declare.h"
unsigned char code table[]="0123456789";
sbit ln=P2^7;
sbit ls=P2^5;
sbit rw=P2^6;
void delay(int);
void write_com(unsigned char com)
{
	rw=0;
	delay(1);
	ls=0;
	delay(1);
    P0=com;
    delay(1);
    ln=1;
    delay(1);
    ln=0;
	 delay(1);
	 rw=1;
	 delay(1);
}
void write_data(unsigned char date)
{
	rw=0;
	delay(1);
	ls=1;
	delay(1);
    P0=date;
    delay(1);
    ln=1;
    delay(1);
    ln=0;
	 delay(1);
	 rw=1;
	 delay(1);
}
void init_12232()
{
	 rw=0;
    ln=0;
    write_com(0x30);
    write_com(0x02);
    write_com(0x0f);
	 write_com(0x01);	
	 write_com(0x0c);
	 delay(50);
}
void displaytwo(unsigned char add,unsigned char date)
{
	unsigned char shi,ge;
	shi=date/10;
	ge=date%10;
	if(add<10)
	write_com(0x80+add);
	else if(add>=10)
	write_com(0x90+add-10);
	write_data(table[shi]);
	delay(5);
	write_data(table[ge]);
	delay(5);
}
