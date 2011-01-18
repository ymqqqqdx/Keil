#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit start=P1^5;
sbit clk=P1^4;
sbit eoc=P1^6;
sbit oe=P1^7;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
uint count;
uchar way=0;
void delay(int z)
{
	int i,j;
	for(i=0;i<z;i++)
		for(j=0;j<110;j++);
}
void display(uint dat)
{
	P2=0xfb;
	P0=table[dat%10];
	delay(5);
	
	P2=0xfd;
	P0=table[dat%100/10];
	delay(5);
	
	P2=0xfe;
	P0=table[dat/100]|0x80;
	delay(5);
}
void main()
{
	float volt;
	TMOD=0x02;
	TH0=0x14;
	TL0=0x00;
	IE=0x82;
	TR0=1;	
	while(1)
	{
		P1=way|0xf0;
		start=0;start=1;start=0;
		while(!eoc);
		oe=1;
		volt=P3*1.953;
		display((int)volt);
		oe=0;
	}
}
void timer0() interrupt 1
{
	clk=!clk;
	count++;
	if(count==5000)
	{
		count=0;
		way++;
		if(way==7)
			way=0;
	}
}