#include <reg52.h>
#include <stdio.h>
#define U8 unsigned char
#define U16 unsigned int

sbit scl=P1^1;
sbit sclk=P1^2;
sbit sda=P1^4;

sbit clk=P3^6;
sbit OE=P2^4;

U8 code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
U8 counter=0,buf=0;
char date=0;

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}

void hc595_write(U8 dat)
{
	U8 i,save_P1=P1,temp=0;
	temp|=(dat&0xf0);
	temp|=(U8)(bit)(dat&(1<<1))<<0;
	temp|=(U8)(bit)(dat&(1<<0))<<1;
	temp|=(U8)(bit)(dat&(1<<2))<<3;
	temp|=(U8)(bit)(dat&(1<<3))<<2;
	for(i=0;i<8;i++)
	{
		temp<<=1;
		sda=CY;
		scl=0;
		scl=1;
	}
	sclk=0;
	sclk=1;
	P1=save_P1;
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
	hc595_write(~1);
	TMOD=0x01;
	TH0=(65536-45678)/256;
	TL0=(65536-45678)%256;
	ET0=1;
	TR0=1;
	EA=1;
	while(1)
	{
		digit(3,date/10);
		digit(4,date%10);
	}
}

void timer0() interrupt 1
{
	TH0=(65536-45678)/256;
	TL0=(65536-45678)%256;
	counter++;
	if(counter==20)
	{
		counter=0;
		if(buf==0)
			date++;
		else
			date--;
		if(date==7)
			buf=1;
		if(date==0)
			buf=0;
		hc595_write(~(1<<date));
	}
}