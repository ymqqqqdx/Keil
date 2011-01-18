#include <reg52.h>
#include <stdio.h>
#define U8 unsigned char
#define U16 unsigned int
#define JUMP 0xcc
#define START 0x44
#define READ 0xbe
sbit DQ=P1^0;
sbit clk=P3^6;
sbit OE=P2^4;
U8 code table[]={0xC0,/*0*/0xF9,/*1*/0xA4,/*2*/0xB0,/*3*/0x99,/*4*/0x92,/*5*/0x82,/*6*/0xF8,/*7*/0x80,/*8*/0x90,/*9*/};
void delay(U16 z)
{	
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void delayus(U16 z)
{
	while(z--);
}
void Uart_Init()
{
	REN=1;
	SCON=0x52;
	TMOD=0X21;
	TH1=0XFD;
	TL1=0XFD;
	TR1=1;
	SM0=0;
	SM1=1;
	EA=1;
}
bit DS18B20_Init()
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
U8 Read_One_Char(void)
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
void Write_One_Char(U8 dat)
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
	delayus(4);
}
U16 Read_Tempreture()
{
	U8 low=0,high=0;
	U16 t=0;
	float tt=0;
	DS18B20_Init();
	Write_One_Char(JUMP);
	Write_One_Char(START);
	DS18B20_Init();
	Write_One_Char(JUMP);
	Write_One_Char(READ);
	low=Read_One_Char();
	high=Read_One_Char();
	t=high;
	t<<=8;
	t|=low;
	tt=t*0.0625;
	t=tt*10+0.5;
	return(t);
}
void Display(U16 dat)
{
	U8 shi,ge,one;
	shi=dat/100;
	ge=dat%100/10;
	one=dat%10;
	P2=~(1<<1);
	OE=0;
	P0=table[shi];
	clk=0;
	clk=1;
	delay(3);
	P2=~(1<<2);
	OE=0;
	P0=table[ge]&0x7f;
	clk=0;
	clk=1;
	delay(3);
	P2=~(1<<3);
	OE=0;
	P0=table[one];
	clk=0;
	clk=1;
}
void main()
{
	U16 temp,tempold;
	Uart_Init();
	puts("\n###################Tempreture###################\n");
	while(1)
	{
		temp=Read_Tempreture();
		Display(temp);
		if(temp!=tempold)
		{
			printf("Tempreture:%.1f\n",((float)temp)/10);
			tempold=temp;
		}
	}
}