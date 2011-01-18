#include <reg52.h>
sbit a=P3^0;
sbit key1=P1^0;
sbit key2=P1^1;
sbit key3=P1^2;
unsigned char high=1,low=5;
unsigned time=0,mode=1;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,0x7f,0x6f};
void delay(int z)
{
	unsigned int i,j;
	for(i=0;i<z;i++)
		for(j=0;j<110;j++);
}
void display()
{
	P2=0xef;
	P0=table[mode];
	delay(5);
	
	P2=0xdf;
	if(time/100)
		P0=table[time/100];
	else
		P0=0x00;
	delay(5);
	
	P2=0xbf;
	P0=table[time%100/10];
	delay(5);
	
	P2=0x7f;
	P0=table[time%10];
	delay(5);
}
void PWM(unsigned char h,unsigned char l)
{
	unsigned char i;
	for(i=0;i<h;i++)
	{
		a=0; display();
	}
	for(i=0;i<l;i++)
	{
		a=1; display();
	}
}
void drive(unsigned char dat)
{
	switch(dat)
	{
		case 1: PWM(1,3); break;
		case 2: PWM(3,1); break;
		case 3: PWM(1,5); break;
	}
}
void keyscan()
{
	if(key1==0)
	{
		delay(5);
		if(key1==0)
		{
			mode++;
			if(mode==4)
				mode=1;
		}
		while(key1==0) display();
	}
}
void main()
{
	while(1)
	{
		keyscan();
		drive(mode);
	}
}