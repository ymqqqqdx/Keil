#include <reg52.h>
#include <stdio.h>

sbit DQ=P3^7;

void delayus(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<24;i++);
}

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}

bit temp_init()
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
	return s;
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
	delayus(7);
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

void uart_init()
{
	TMOD=0x21;
	SCON=0x52;
	TH1=0xfa;
	TL1=0xfa;
	TR1=1;
	EA=1;
}

void main()
{
	U16 temp,temp_old;
	uart_init();
	puts("Tempreture");
	while(1)
	{
		temp=temp_read();
//		if(temp!=temp_old)
//		{
			printf("Tempreture: %.1f  C\n",(float)temp/10);
			temp_old=temp;
//		}
		delay_1ms(30000);
	}
}