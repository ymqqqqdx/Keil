#include <reg52.h>
#define U8 unsigned char
#define U16 unsigned int
sbit key1=P3^4;
sbit key2=P3^5;
sbit key3=P3^2;
sbit key4=P3^3;
sbit OE=P2^4;
sbit clk=P3^6;
sbit beep=P2^7;
U8 code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};
U8 counter=0,stat=0,count=0;
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
U8 keyscan()
{
	U8 i=0;
	i++;
	if(!key1)
		goto end;
	i++;
	if(!key2)
		goto end;
	i++;
	if(!key3)
		goto end;
	return 0;
end:
	count=i;
	stat=1;
	TR0=1;
	beep=0;
}
void main()
{
	TMOD=0x01;
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	ET0=1;
	TR0=0;
	EA=1;
	while(1)
	{
		if(stat)
		{
			digit(4,count);
			if(!key4)
			{
				digit(4,10);
				stat=0;
			}	
		}
		if(!stat)
		{
			keyscan();
			delay_1ms(5);
		}
	}
}

void timer0() interrupt 1
{
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	counter++;
	if(counter==20)
	{
		counter=0;
		beep=1;
		TR0=0;
	}
}