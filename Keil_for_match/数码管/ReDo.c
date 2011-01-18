#include <reg52.h>
sbit clk=P3^6;
sbit OE=P2^4;

U8 code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

U8 sec=0,counter=0;

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}

void digit(U8 add,U8 dat)
{
	P2=~(1<<(add-1));
	OE=0;
	P0=table[dat];
	clk=0;
	clk=1;
	delay_1ms(5);
}

void main()
{
	TMOD=0x01;
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	EA=1;
	ET0=1;
	TR0=1;
	while(1)
	{
		digit(3,sec/10);
		digit(4,sec%10);
	}
}

void timer0() interrupt 1
{
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;	
	counter++;
	if(counter==20)
	{
		sec++;
		if(sec==60)
			sec=0;
		counter=0;
	}
}