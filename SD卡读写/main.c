#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
#define chipselect() cs=0
#define chipdeselect() cs=1
sbit cs=P1^0;
sbit din=P1^1;
sbit dout=P1^2;
sbit clk=P1^3;
void delay()
{
	uchar i;
	for(i=0;i<1;i++);
}
void writeonebit(bit buff)
{
	clk=0;
	delay();
	if(buff) din=1;
	else	din=0;
	delay();
	clk=1;
	delay();
}
void writeonebyte(uchar dat)
{
	uchar i;
	bit t;
	for(i=0;i<8;i++)
	{
		t=dat&0x80;
		writeonebit(t);
		dat<<=1;
	}
}
void busclock()
{
	clk=0;
	delay();
	clk=1;
	delay();
}
void init()
{
	uint i=100;
	din=1;
	while(i--)
		busclock();
	chipselect();
	writeonebyte(0x40);
	for(i=0;i<33;i++)
		writeonebit(0);
	writeonebyte(0x95);
	din=1;
	for(i=0;i<33;i++)
		busclock();
	chipdeselect();
}
void main()
{
	uint i;
	init();
//	for(i=0;i<1233;i++)
//		writeonebyte(i%256);
	while(1);
}