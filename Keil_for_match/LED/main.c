#include <reg52.h>
#include <intrins.h>
#define U16 unsigned int
#define U8 unsigned char
sbit clk=P1^1;
sbit sclk=P1^2;
sbit ser=P1^4;
void delay(int z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void Write2595(U8 dat)
{
	U8 i,save_P1;
	save_P1=P1;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		ser=CY;
		clk=0;
		clk=1;
	}
	sclk=0;
	sclk=1;
	P1=save_P1;	
}
void LEDS(U16 dat)
{
	U8 temp=0,temp1=0;
	temp|=(dat&(0xf<<9))>>5;
	temp|=(char)(bit)(dat&(1<<1))<<0;
	temp|=(char)(bit)(dat&(1<<0))<<1;
	temp|=(char)(bit)(dat&(1<<3))<<2;
	temp|=(char)(bit)(dat&(1<<2))<<3;
	temp1|=(dat&(0x1f<<4))>>4;
	P1=temp1;
	Write2595(temp);
}
void main()
{
	U8 i;
	U16 j,k;
	while(1)
	{
		for(i=0,j=~1;i<7;i++)
		{
			LEDS(j);
			j=_irol_(j,2);
			delay(300);
		}
		for(i=0,j=~0x02;i<6;i++)
		{
			LEDS(j);
			j=_irol_(j,2);
			delay(300);
		}
		for(i=0,j=k=~(1<<6);i<7;i++)
		{
			LEDS(j&k);
			j=_irol_(j,1);
			k=_iror_(k,1);
			delay(300);
		}
		for(i=0,j=~1,k=~(1<<12);i<7;i++)
		{
			LEDS(j&k);
			j=_irol_(j,1);
			k=_iror_(k,1);
			delay(300);
		}
	}
}