#include <reg52.h>
#include <stdio.h>
sbit sda=P1^1;
sbit scl=P1^0;
sbit wp=P1^2;
unsigned char read;
void delay(int);
void comm_init();
void delay0()
{;;}
void start()//开始
{
	sda=1;
	delay0();
	scl=1;
	delay0();
	sda=0;
	delay0();
}
void stopp()//停止
{
	sda=0;
	delay0();
	scl=1;
	delay0();
	sda=1;
	delay0();
}
void respons()//应答
{
	unsigned char i=0;
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
/************写入时序**********/
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
/***********读出时序**********/
char read_byte()
{
	char i,k;
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
/***********向24C02写一个char型，第一个参数是地址，第二个参数是数据**********/
void write_add(unsigned char page,unsigned char address,char date)
{
	start();
	page<<=1;
	page|=0xa0;
	write_byte(page);
	respons();
	write_byte(address);
	respons();
	write_byte(date);
	respons();
	stopp();
	delay(60);
}
/********从24c02读一个char型，参数为地址*************/
unsigned char read_add(unsigned char page,unsigned char address)
{
	unsigned char date;
	start();
	page<<=1;
	write_byte(page|0xa0);
	respons();
	write_byte(address);
	respons();
	start();
	write_byte(page|0xa1);
	respons();
	date=read_byte();
	stopp();
	return date;
}

