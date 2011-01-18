#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
sbit sda=P2^6;
sbit scl=P2^5;

sbit OE=P2^4;
sbit clk=P3^6;

sbit key0=P3^2;
U8 code table[]={0xC0,/*0*/0xF9,/*1*/0xA4,/*2*/0xB0,/*3*/0x99,/*4*/0x92,/*5*/0x82,/*6*/0xF8,/*7*/0x80,/*8*/0x90,/*9*/};
U8 key=0;

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
	U8 i;
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
	U8 i,temp;
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
	U8 temp;
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
void display()
{
		digit(2,key/100);
		digit(3,key%100/10);
		digit(4,key%10);
}
void main()
{
	EA=1;
	EX0=1;
	iic_init();
	key=iic_read(1);
	while(1)
	{
		display();
	}
}

void int0() interrupt 0
{
	key++;
	iic_write(1,key);
	if(key==200)
		key=0;
	while(key0==0)
		display();
}