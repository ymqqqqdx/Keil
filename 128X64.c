#include <reg52.h>
//#include <absacc.h>
#define uchar unsigned char
#define uint unsigned int
sbit lcden=P2^0;//lcd_enable高电平有效
sbit rw=P2^1;//H:read    L:write
sbit rs=P2^2; //H:display  L:command
sbit cs1=P2^3;
sbit cs2=P2^4;
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void write_com(uchar dat)
{
	lcden=0;
	rw=0;
	cs1=0;
	rs=1;
	lcden=1;
	P0=dat;
	lcden=0;
	rw=1;
	cs1=1;
	rs=0;
}
void write_data(uchar dat)
{
	lcden=0;
	rw=0;
	cs1=0;
	rs=0;
	lcden=1;
	P0=dat;
	lcden=0;
	rw=1;
	cs1=1;
	rs=0;
}
void main()
{
	write_com(0x1f);
	write_com(0x61);
	write_data('a');
	delay(1);
	while(1);
}