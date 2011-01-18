#include <reg52.h>
#include <declare.h>
#include <intrins.h>
sbit clk=P1^1;
sbit sclk=P1^2;
sbit sda=P1^4;
void Write2595(U8 dat)
{
	U8 i,save_P1;
	save_P1=P1;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		sda=CY;
		clk=0;
		clk=1;
	}
	sclk=0;
	sclk=1;
	P1=save_P1;
}
void LED(U16 dat)
{
	U8 temp=0,temp1=0;
	temp|=(dat&(0x0f<<9))>>5;
	temp|=(char)(bit)(dat&(1<<0))<<1;
	temp|=(char)(bit)(dat&(1<<1))<<0;
	temp|=(char)(bit)(dat&(1<<2))<<3;
	temp|=(char)(bit)(dat&(1<<3))<<2;
	temp1|=(dat&(0x1f<<4))>>4;
	Write2595(temp);
	P1=temp1;
}
void LED_process(U8 dat)
{
	U16 i=~(0x0f<<9);
	dat=dat%10;
	i=_iror_(i,dat);
	LED(i);
}