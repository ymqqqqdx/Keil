#include <reg52.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int
sbit rst=P1^0;
sbit dat=P1^1;
sbit clk=P3^4;
sbit cs=P3^5;
void delay(uint z)
{
	uint i,j;
	for(i=0;i<z;i++)
		for(j=0;j<1100;j++);
}
void comm_init()
{
	REN=1; //串行允许
	SCON=0x52;
	TMOD=0X21;//设置定时器1工作方式
	TH1=0XFD;//装初值
	TL1=0XFD;//装初值
	TR1=1;//打开定时器1
	SM0=0; //设置方式1
	SM1=1;
	EA=1;
}
int getonechar()
{
	int temp=0,i;
	for(i=0;i<8;i++)
	{
		while(!clk);
		if(clk)
		{
			if(i)
				temp<<=1;
			if(dat)
				temp|=0x01;
			while(clk);	
		}	
	}
	return temp;
}

void readtemphumi()
{
	int a=0,b=0,c=0,d=0,e=0;
	int tempreture=0;
	int humidity=0;
	rst=1;
	
	cs=0;
	delay(8);
	
	a=getonechar();
	b=getonechar();
	c=getonechar();
	d=getonechar();
	e=getonechar();
	
//	delay(240);
	cs=1;
	
	
	tempreture=a;
	tempreture<<=8;
	tempreture+=b;
	tempreture-=400;
	
	humidity=c;
	humidity<<=8;
	humidity+=d;
	
	printf(" %d %d ",tempreture,humidity);
}
void main()
{
	comm_init();
	while(1)
	{
		readtemphumi();
		delay(2000);
	}
}