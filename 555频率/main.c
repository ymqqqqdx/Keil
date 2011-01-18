#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned char
//unsigned char code table6[]="555¶àÐ³Õñµ´Æ÷ Êä³ö :Hz ";
//unsigned char code table[]="0123456789";
unsigned int aa,flag=0;
int val;
void initlcd();
void write_data(unsigned char);
void write_com(unsigned char);
void init_12232();
void displayfour(int);

void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}

main()
{
	char t1,th1,th2;
	init_12232();
	aa=0;
	TMOD=0x15;
	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	TH0=0;
	TL0=0;
	EA=1;
	ET1=1;
	TR1=1;
	TR0=1;
   while(1)
   {
		if(flag==1)
		{  
		   flag=0;
			th1=TH0;
			t1=TL0;
			th2=TH0;
			if(th1==th2)
			{
				val=th1*256+t1;
			}
			write_com(0x80);
			displayfour(val);
			TH0=0;
			TL0=0;
		}
	}
}
void timer1() interrupt 3
{
		TH1=(65536-50000)/256;
	   TL1=(65536-50000)%256;
		aa++;
		if(aa==20)
		{
			  aa=0;
           flag=1;
		}
}