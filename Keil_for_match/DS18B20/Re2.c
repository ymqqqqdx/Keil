#include <reg52.h>
#include <stdio.h>

sbit DQ=P1^0;
sbit clk=P3^6;
sbit OE=P2^4;

U8 code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void delayus(U16 z)
{
	while(z--);
}

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}

void temp_init()
{
	bit s;
	DQ=1;
	delayus(8);
	DQ=0;
	delayus(80);
	DQ=1;
	delayus(14);
	s=DQ;
	delayus(20);
}

void temp_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat>>=1;
		DQ=0;
		DQ=CY;
		delayus(5);
		DQ=1;
	}
	delayus(4);
}

U8 temp_readonechar()
{
	U8 i,temp=0;
	for(i=0;i<8;i++)
	{
		DQ=0;
		DQ=1;
		temp|=((U8)DQ)<<i;
		delayus(4);
	}
	return temp;
}

U16 temp_read()
{
	U8 low,high;
	U16 t;
	float tt;
	temp_init();
	temp_writeonechar(0xcc);
	temp_writeonechar(0x44);
	temp_init();
	temp_writeonechar(0xcc);
	temp_writeonechar(0xbe);
	low=temp_readonechar();
	high=temp_readonechar();
	t=high;
	t<<=8;
	t|=low;
	tt=t*0.0625;
	t=tt*10+0.5;
	return t;
}

void digit(U8 add,U8 dat)
{
	P2=~(1<<(add-1));
	OE=0;
	P0=dat;
	clk=0;
	clk=1;
	delay_1ms(5);
}

void main()
{
	U16 temp;
	while(1)
	{
		temp=temp_read();
		digit(2,table[temp/100]);
		digit(3,table[temp%100/10]&0x7f);
		digit(4,table[temp%10]);
	}
}