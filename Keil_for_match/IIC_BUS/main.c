#include <stdio.h>
#include <reg52.h>
#include <intrins.h>
#define U8 unsigned char
#define U16 unsigned int
sbit sda=P2^6;
sbit scl=P2^5;
void Uart_Init()
{
	REN=1;
	SCON=0x52;
	TMOD=0X20;
	TH1=0XFD;
	TL1=0XFD;
	TR1=1;
	ES=1;
	SM0=0;
	SM1=1;
	EA=1;
}
void Start()
{
	sda=1;	_nop_();
	scl=1;	_nop_();
	sda=0;	_nop_();
}
void Stop()
{
	sda=0;	_nop_();
	scl=1;	_nop_();
	sda=1;	_nop_();
}
void Respons()
{
	U8 i;
	scl=1;	_nop_();
	while((sda==1)&&(i<250))
		i++;
	scl=0;	_nop_();	
}
void AT24C02_Init()
{
	sda=1;	_nop_();
	scl=1;	_nop_();
}
void Write_Byte(U8 temp)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		scl=0;	_nop_();
		sda=(bit)(temp&(1<<(7-i)));
		scl=1;	_nop_();
	}
	scl=0;	_nop_();
	sda=1;	_nop_();
}
U8 Read_Byte()
{
	U8 i,k;
	scl=0;	_nop_();
	sda=1;	_nop_();
	for(i=0;i<8;i++)
	{
		scl=1;		_nop_();
		k=(k<<1)|sda;
		scl=0;		_nop_();
	}
	return k;
}
void Write_Add_Data(U8 add,U8 dat)
{
	Start();
	Write_Byte(0xa0);
	Respons();
	Write_Byte(add);
	Respons();
	Write_Byte(dat);
	Respons();
	Stop();
}
U8 Read_Add(U8 add)
{
	U8 date;
	Start();
	Write_Byte(0xa0);
	Respons();
	Write_Byte(add);
	Respons();
	Start();
	Write_Byte(0xa1);
	Respons();
	date=Read_Byte();
	Stop();
	return date;
}
void Write()
{
	U8 i;
	puts("########### To Write to AT24c02 ##########");
	for(i=0;i<128;i++)
	{
		printf(" %4d ",(int)i);
		if((i+1)%10==0)	printf("\n");
		Write_Add_Data(i,i+1);
	}
	puts("\n################# Write Complete! ##############");
}
void Read()
{
	U8 i;
	puts("################# Now Read it! #################");
	for(i=0;i<128;i++)
	{
		printf(" %4d ",(int)Read_Add(i));
		if((i+1)%10==0)	printf("\n");
	}
	puts("\n############### Done! #######################");
}
void main()
{
	U8 c;
	AT24C02_Init();
	Uart_Init();
start:
	puts("\n############## IICBUS TEST ###############");
	puts("1. Write 1-127 to AT24c01");
	puts("2. Read from AT24c01");
	c=_getkey();
	switch(c)
	{
		case '1': Write(); goto start; break;
		case '2': Read(); goto start; break;
	}
	while(1);
}
void ser() interrupt 4
{
	RI=0;
}