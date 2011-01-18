#include <reg52.h>
#define U8 unsigned char
#define U16 unsigned int
sbit OE=P2^4;
sbit clk=P3^6;
sbit beep=P2^7;
U8 code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void digit(U8 add,U8 dat)
{
	bit bee=beep;
	P2=~(1<<(add-1));
	OE=0;
	P0=table[dat];
	clk=0;
	clk=1;
	beep=bee;
	delay_1ms(5);	
}
void init()
{
	TMOD=0x11;
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	TH1=(65536-45872)/256;
	TL1=(65536-45872)%256;
	ET0=1;
	ET1=1;
	TR0=0;
	TR1=0;
	EA=1;
}
void main()
{
	init();
}