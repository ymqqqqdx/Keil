#include <stdio.h>
#include <series.h>
bit write=0;
sbit sda=P1^2;
sbit scl=P1^1;
sbit wp=P1^0;
unsigned char read,i,t,temp;
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void delay0()
{;;}
void start()
{
	sda=1;
	delay0();
	scl=1;
	delay0();
	sda=0;
	delay0();
}
void stop()
{
	sda=0;
	delay0();
	scl=1;
	delay0();
	sda=1;
	delay0();
}
void respons()
{
	unsigned char i;
	scl=1;
	delay0();
	while((sda==1)&&(i<250))
		i++;
	scl=0;
	delay0();
}
void init_24c02()
{
	sda=1;
	delay0();
	scl=1;
	delay0();
	wp=0;
}
void write_byte(unsigned char date)
{
	unsigned char i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		scl=0;
		delay0();
		sda=CY;
		delay0();
		scl=1;
		delay0();
	}
	scl=0;
	delay0();
	sda=1;
	delay0();
}
unsigned char read_byte()
{
	unsigned char i,k;
	scl=0;
	delay0();
	sda=1;
	delay0();
	for(i=0;i<8;i++)
	{
		scl=1;
		delay0();
		k=(k<<1)|sda;
		scl=0;
		delay0();
	}
	return k;
}
void write_add(unsigned char address,unsigned char date)
{
	start();
	write_byte(0xa0);
	respons();
	write_byte(address);
	respons();
	write_byte(date);
	respons();
	stop();
	delay(1);
}
char read_add(unsigned char address)
{
	unsigned char date;
	start();
	write_byte(0xa0);
	respons();
	write_byte(address);
	respons();
	start();
	write_byte(0xa1);
	respons();
	date=read_byte();
	stop();
	return date;
	delay(1);
}
void commscan()
{
	temp=_getkey();
	if(temp=='&')
	{
		while(temp!='$')
		{
			temp=_getkey();
			write_add(i++,temp);
		}
		i=0;
	}
	if(temp=='^')
	{
		while(read_add(t++)!='$')
		{
			printf("%c",read_add(t-1));
		}
		t=0;
	}
}
void main()
{
	comm_init();
	init_24c02();
	while(1)
	{
		commscan();
	}
}
