#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit ln=P2^1;
sbit ls=P2^0;
sbit k1=P1^0;
sbit k2=P1^1;
sbit k3=P1^2;
sbit k4=P1^3;
uchar code table[]="0123456789";
uchar code table1[]=":.";
uchar code table2[]="Number";
uchar m=0,t=0,i,aa=0,max,min1,min0,sec1,sec0,one,num0,num1,num=0,min[34]=0;
int add,sec[34]=0;
int s=0;
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
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
void convert0(uchar temp)
{
	min1=temp/10;
	min0=temp%10;
}
void convert1(int temp)
{
	sec1=temp/100;
	sec0=temp%100/10;
	one=temp%10;
}
void convert2(uchar temp)
{
	num1=(temp+0)/10;
	num0=(temp+0)%10;
}
void displayinit()
{
	write_com(0x80+0x40+0x03);
	for(i=0;i<6;i++)
	write_data(table2[i]);
}
void displaytime()
{
	write_com(0x80+0x04);
	write_data(table[min1]);
	write_data(table[min0]);
	write_data(table1[0]);
	write_data(table[sec1]);
	write_data(table[sec0]);
	write_data(table1[1]);
	write_data(table[one]);
}
displaynum()
{
	write_com(0x80+0x47+0x03);
	write_data(table[num1]);
	write_data(table[num0]);
}
void keyscan()
{
	if(k1==0&&ET0==1)
	{
		delay(5);
		if(k1==0&&ET0==1)
		{
			if(TR0==1)
			{
				num++;
				min[num]=m;
				sec[num]=s;
			}
				if(TR0==0)
				{TR0=1;}
		}
		while(k1==0)
		{
		convert0(m);
		convert1(s);
		convert2(num);
		displaytime();
		displaynum();
		}
	}
	if(k2==0&&TR0==1)
	{
		delay(5);
		if(k2==0&&TR0==1)
		{
			num++;
			min[num]=m;
			sec[num]=s;
			TR0=0;
			ET0=0;
			max=num;
		}
		while(k2==0)
		{
		convert0(m);
		convert1(s);
		convert2(num);
		displaytime();
		displaynum();
		}
	}
	if(k2==0&&TR0==0)
	{
		delay(5);
		if(k2==0&&TR0==0)
		{
			m=0;s=0;num=0;
			for(i=0;i<max;i++)
			{
				min[i]=0;sec[i]=0;
			}
			max=0;ET0=1;
		}
		while(k2==0);
	}
	if(k3==0&&ET0==0)
	{
		delay(5);
		if(k3==0&&ET0==0)
		{
			if(ET0==0)
				if(num>1)
				num--;
				m=min[num];
				s=sec[num];
		}
		while(k3==0)
		{
		convert0(m);
		convert1(s);
		convert2(num);
		displaytime();
		displaynum();
		}
	}
	if(k4==0&&ET0==0)
	{
		delay(5);
		if(k4==0)
		{
			if(ET0==0)
				if(num<max)
				num++;
				m=min[num];
				s=sec[num];
		}
		while(k4==0)
		{
		convert0(m);
		convert1(s);
		convert2(num);
		displaytime();
		displaynum();
		}
	}
}
void main()
{
	init();
	displayinit();
	while(1)
	{
		convert0(m);
		convert1(s);
		convert2(num);
		displaytime();
		displaynum();
		keyscan();
	}
}
void timer0() interrupt 1
{
		TH0=(65536-50000)/256;
	   TL0=(65536-50000)%256;
		aa++;
		if(aa==2)
		{   
			aa=0;
			s++;
			if(s==600)
			{
				m++;
				s=0;
			}
		}		
}

