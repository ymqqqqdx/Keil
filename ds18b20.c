#include "reg52.h"
//#include "1602.h"
#include "stdio.h"
//#include "mscomm.h"
unsigned char code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
unsigned char code point[]={
        0xBF,/*0p*/
		0x86,/*1p*/
        0xDB,/*2p*/
        0xCF,/*3p*/
        0xE6,/*4p*/
        0xED,/*5p*/
        0xFD,/*6p*/
        0x87,/*7p*/
        0xFF,/*8p*/
        0xEF,/*9p*/
};
//unsigned char code table1[]="0123456789";
unsigned char bai,shi,ge,one;
unsigned int tt,aa;
float d;
unsigned int readtemp();
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void display()
{
		P2=0x08;
		P0=table[bai];
		delay(2);
		P2=0x04;
		P0=table[shi];
		delay(2);
		P2=0x02;
		P0=point[ge];
		delay(2);
		P2=0x01;
		P0=table[one];
		delay(2);
}
void main()
{
	//	init_1602();
//	init_comm();
	REN=1; //串行允许
	SCON=0x52;
	TMOD=0X21;//设置定时器1工作方式
	TH1=0XF3;//装初值
	TL1=0XF3;//装初值
	TR1=1;//打开定时器1
	SM0=0; //设置方式1
	SM1=1;
	TH0=(65536-50000)/256; 
	TL0=(65536-50000)%256;
	EA=1; //开总中断
	ET0=1; //计时器0开启
	TR0=1; //计时器开始运行
	while(1)
	{   
		display();
	} 
}
void timer0() interrupt 1 //每250ms获取一次温度
{
		TH0=(65536-50000)/256;
	   TL0=(65536-50000)%256;
		aa++;
		if(aa==30)
		{   
			aa=0;
		tt=readtemp();
		d=tt;
		printf("%.1f\n",d/10);
		bai=tt/1000;
        shi=tt%1000/100;
        ge=tt%100/10;
		one=tt%10;
		}
}
