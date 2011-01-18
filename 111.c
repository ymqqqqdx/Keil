#include<reg52.h>
#include<intrins.h>
#define uint unsigned int;
#define uchar unsigned char;
#define ulong	unsigned long int;
sbit k1=P1^0;
sbit k2=P1^1;
sbit k3=P1^2;
sbit k4=P1^3;
sbit ln=P2^0;
sbit ls=P2^1;
ulong  add,dat[20]=0;
uchar t,aa,max,min1,min0,sec1,sec0,one;
uchar code table[]="Number ";
uchar code table1[]="0123456789";
uchar code table2[]=":.";
void delay(int);
void write_com(uchar com)
{
	ls=0;
   P0=com;
   delay(5);
   ln=1;
   delay(5);
   ln=0;
}
void write_data(uchar date)
{
	ls=1;
   P0=date;
   delay(5);
   ln=1;
   delay(5);
   ln=0;
}
void init();
void main()
{
	init();
	while(1)
	{
	if(k1==0)
	{
		delay(5);
		if(k1==0)
		{
			TR0=~TR0;
			if(TR0==1)
			dat[t++]=add;
		}
	}
	if(k2==0)
	{
		delay(5);
		if(k2==0)
		{
			TR0=0;
			ET0=0;
			max=t;
			if(k2==0)
			delay(5);
				if(k2==0)
				add=0;	
		}
	}
	if(k3==0)
	{
		delay(5);
		if(k3==0)
		{
			if(ET0==0)
				if(t>0)
				t--;
		}
	}
	if(k4==0)
	{
		delay(5);
		if(k4==0)
		{
			if(ET0==0)
				if(t<max)
				t++;
		}
	}
}
void init()        
{
	TMOD=0x01;
	TH0=(65536-10000)/256; 
	TL0=(65536-10000)%256;
	EA=1; //开总中断
	ET0=1; //计时器0开启
	TR0=0; //计时器先关闭
	ln=0;
   write_com(0x38);
   write_com(0x0c);
   write_com(0x06);
	write_com(0x01);
}
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void timer0() interrupt 1
{
		TH0=(65536-10000)/256;
	   TL0=(65536-10000)%256;
		aa++;
		if(aa==1)
		{   
			aa=0;
			add++;
		}
}
