#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
sbit sda=P2^6;
sbit scl=P2^5;

sbit OE=P2^4;
sbit clk=P3^6;

U8 dat=0;
U8 counter=0;
U8 code table[]={0xC0,/*0*/0xF9,/*1*/0xA4,/*2*/0xB0,/*3*/0x99,/*4*/0x92,/*5*/0x82,/*6*/0xF8,/*7*/0x80,/*8*/0x90,/*9*/};

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}

void start()
{
	sda=1;
	scl=1;
	sda=0;
}

void stop()
{
	sda=0;
	scl=1;
	sda=1;
}

void response()
{
	U8 i=0;
	scl=1;
	while(sda==1&&i<250)
		i++;
	scl=0;
}

void iic_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		scl=0;
		sda=CY;
		scl=1;
	}
	scl=0;
	sda=1;
	response();
}

U8 iic_readonechar()
{
	U8 i,temp=0;
	scl=0;
	sda=1;
	for(i=0;i<8;i++)
	{
		scl=1;
		temp|=((U8)sda)<<(7-i);
		scl=0;
	}
	return temp;
}

void iic_write(U8 add,U8 dat)
{
	start();
	iic_writeonechar(0xa0);
	iic_writeonechar(add);
	iic_writeonechar(dat);
	stop();
}

U8 iic_read(U8 add)
{
	U8 temp=0;
	start();
	iic_writeonechar(0xa0);
	iic_writeonechar(add);
	start();
	iic_writeonechar(0xa1);
	temp=iic_readonechar();
	stop();
	return temp;
}

void iic_init()
{
	scl=1;
	sda=1;
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
	TH0=(65536-45678)/256;
	TL0=(65536-45678)%256;
	ET0=1;
	TR0=1;
	EA=1;
	dat=iic_read(1);
	while(1)
	{
		digit(2,dat/100);
		digit(3,dat%100/10);
		digit(4,dat%10);
	}
}

void timer0() interrupt 1
{
	iic_init();
	TH0=(65536-45678)/256;
	TL0=(65536-45678)%256;
	counter++;
	if(counter==20)
	{
		counter=0;
		dat++;
		if(dat==200)
			dat=0;
		iic_write(1,dat);
	}	
}