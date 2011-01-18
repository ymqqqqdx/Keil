#include <reg52.h>
sbit key1=P3^4;
sbit key2=P3^5;
sbit key3=P3^2;
sbit key4=P3^3;

sbit clk=P3^6;
sbit OE=P2^4;
sbit beep=P2^7;
U8 code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
char minute=0,second=0,counter=0,counter1=0,flag=0;
bit stat=0;

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
	P0=dat;
	clk=0;
	clk=1;
	beep=bee;
	delay_1ms(5);
}

void keyscan()
{
	if(!key1)
	{
		delay_1ms(5);
		if(!key1)
		{
			stat=~stat;
		}
	}while(!key1);
	if(!key2)
	{
		delay_1ms(5);
		if(!key2)
		{
			stat==0?minute++:second++;
			if(minute>99)
				minute=99;
			if(second>59)
				second=59;
		}
	}while(!key2);
	if(!key3)
	{
		delay_1ms(5);
		if(!key3)
		{
			stat==0?minute--:second--;
			if(minute<0)
				minute=0;
			if(second<0)
				second=0;
		}
	}while(!key3);
	if(!key4)
	{
		delay_1ms(5);
		if(!key4)
		{
			TR0=1;
		}
	}while(!key4);
}

void main()
{
	TMOD=0x11;
	TH0=(65536-45678)/256;
	TL0=(65536-45678)%256;
	TH1=(65536-45678)/256;
	TL1=(65536-45678)%256;
	ET0=1;
	ET1=1;
	TR1=0;
	TR0=0;
	EA=1;
	while(1)
	{
		digit(1,table[minute/10]);
		digit(2,table[minute%10]&0x7f);
		digit(3,table[second/10]);
		digit(4,table[second%10]);
		keyscan();
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
		second--;
		if(second==0)
		{
			beep=0;
			TR1=1;
		}
		if(second==-1)
		{
			second=59;
			minute--;
		}
	}
}

void timer1() interrupt 3
{
	TH0=(65536-45678)/256;
	TL0=(65536-45678)%256;
	counter1++;
	if(counter1==10)
	{
		counter1=0;
		if(flag==0)
			flag=1;
		else if(flag==1)
		{
			flag=2;
			beep=1;
		}
		else if(flag==2)
		{
			beep=0;
			flag=3;
		}
		else if(flag==3)
		{
			beep=1;
			TR1=0;
		}
	}
}