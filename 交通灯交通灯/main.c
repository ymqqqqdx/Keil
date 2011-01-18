#include <reg52.h>
#include <stdarg.h>
#define uchar unsigned char
#define uint unsigned int
sbit clk=P2^0;
sbit sclk=P2^2;
sbit ser=P2^1;
sbit OE=P2^3;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
void delay(int z)
{
	uint i,j;
	for(i=0;i<z;i++)
		for(j=0;j<110;j++);
}
void writenchar(uchar dat,...)
{
	va_list arg_ptr; 
	uchar i;
	uchar nargvalue=dat;
	va_start(arg_ptr,dat);
	do
	{
		clk=0;//???
		sclk=0;
		OE=1;
		for(i=0;i<8;i++)
		{
			ser=nargvalue&0x01;
			clk=1;
			clk=0;
			nargvalue>>=1;
		}
		sclk=1;
		sclk=0;		
		nargvalue=va_arg(arg_ptr,uchar);
	}
	while(nargvalue!=-1);
	OE=0;
}
void writetime(uchar dat)
{
	writenchar(table[dat%10],table[dat%100/10],table[dat/100],-1);
}
void main()
{
	uchar t=0;
	while(1)
	{
		writetime(t);
		t++;
		delay(500);
	}
}
