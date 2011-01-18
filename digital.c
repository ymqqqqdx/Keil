#include<reg52.h>
#include<intrins.h>
sbit dula=P3^0;
sbit wela=P3^1;
sbit h1=P3^2;
sbit h2=P3^3;
sbit h3=P3^4;
sbit h4=P3^5;
#define uint unsigned int;
#define uchar unsigned char;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
unsigned char code point[]={0xBF,0x86,0xDB, 0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF};
unsigned char code table1[]={0xff,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0x00};
unsigned char i,aa,one=0,two=0,shi=0,ge=0,time=0;
unsigned int num;
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void init()        
{
	TMOD=0x01;
	TH0=(65536-10000)/256; 
	TL0=(65536-10000)%256;
	EA=1; //开总中断
	ET0=1; //计时器0开启
	TR0=1; //计时器开始运行
}
void duan(unsigned char z)
{
	P2=table[z];
	delay(1);
	P2=0x00;
}
void wei(unsigned char z)
{
	P1=table1[z];
}
void duanpoint(unsigned char z)
{
	P2=point[z];
	delay(1);
}
void display()
{
	wei(3);
	duan(shi);
	wei(4);
	duanpoint(ge);
	wei(5);
	duan(one);
	wei(6);
	duan(two);
}
void display2()
{
	wei(3);
	duan(time/10);
	wei(4);
	duan(time%10);
}
void keyscan()
{
	if(h1==0)
	{
		delay(5);
		if(h1==0)
		{
			TR0=0;
			while(h1==0)
    	 	display();
		}
	}
	if(h2==0)
	{
		delay(5);
		if(h2==0)
		{
			TR0=1;
			while(h2==0)
        		display();
		}
	}
	if(h3==0)
	{
		delay(5);
		if(h3==0)
		{
		aa=0;num=0;
		while(h3==0)
     		{display();
			 shi=ge=one=two=0;
			}
		}
	}
	if(h4==0)
	{
		delay(5);
		if(h4==0)
		{
			while(h4==0)
        		display2();
		}
	}
}
void main()
{
	init();
	while(1)
	{
		display();
		keyscan();
	}
}
void timer0() interrupt 1
{
		TH0=(65536-10000)/256;
	   TL0=(65536-10000)%256;
		aa++;
		if(aa==1)
		{   
			aa=0;
			num++;
			if(num==6000)
			{num=0;time++;}
		shi=num/1000;
        ge=num%1000/100;
        one=num%100/10;
		two=num%10;
		  }
}
