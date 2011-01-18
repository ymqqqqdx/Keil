#include <stdio.h>
#include <reg52.h>
#include <intrins.h>
#define U8 unsigned char
#define U16 unsigned int
sbit scl=P1^1;
sbit sclk=P1^2;
sbit sda=P1^4;

void hc595_write(U8 dat)
{
	U8 i,save_P1=P1;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		sda=CY;
		scl=0;
		scl=1;
	}
	sclk=0;
	sclk=1;
	P1=save_P1;
}

void leds(U16 dat)
{
	U8 temp=0;
	temp|=(dat&(0xf<<9))>>5;
	temp|=(U8)(bit)(dat&(1<<0))<<1;
	temp|=(U8)(bit)(dat&(1<<1))<<0;
	temp|=(U8)(bit)(dat&(1<<2))<<3;
	temp|=(U8)(bit)(dat&(1<<3))<<2;
	hc595_write(temp);
	P1=(dat&(0x1f<<4))>>4;
}

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}

void main()
{
	U8 i;
	U16 temp=~1;
	while(1)
	{
		for(i=0;i<13;i++)
		{
			leds(temp);
			temp=_irol_(temp,1);
			delay_1ms(500);
		}
		temp=~2;
		for(i=0;i<7;i++)
		{
			leds(temp);
			temp=_irol_(temp,2);
			delay_1ms(500);
		}
	}
}