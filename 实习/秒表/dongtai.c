#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
uchar fen=0,miao=0;
int count;
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void init()
{
	EA=1;
	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	ET0=1;
	TR0=1;
}
void main()
{
	init();
	while(1)
	{
		P2=0x08;
		P0=table[fen/10];
		delay(5);
		
		P2=0x04;
		P0=table[fen%10];
		delay(5);
		
		P2=0x02;
		P0=table[miao/10];
		delay(5);
		
		P2=0x01;
		P0=table[miao%10];
		delay(5);
	}
}
void int0() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	count++;
	if(count==20)
	{
		miao++;
		if(miao==60)
		{
			miao=0;
			fen++;
			if(fen==60)
				fen=0;
		}
	}
}