#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
sbit k1=P2^0;
sbit k2=P2^1;
sbit k3=P2^2;
sbit k4=P2^3;
uchar temp,i,a;
void plan1();
void plan2();
void plan3();
void keyscan();
void delay1(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
		  {
		  	keyscan();
         for(j=110;j>0;j--);
		  }
}
void keyscan()
{
	if(k1==0)
	{
		delay1(5);
		if(k1==0)
		plan1();
	}
	if(k2==0)
	{
		delay1(5);
		if(k2==0)
		plan2();
	}
	if(k3==0)
	{
		delay1(5);
		if(k3==0)
		plan3();
	}
	if(k4==0)
	{
		delay1(5);
		if(k4==0)
		{
		plan1();
		plan2();
		plan3();
		}
	}
}
void main()
{
	P1=0xff;
	while(1)
	{
		keyscan();
	}
}
void plan1()
{
			temp=0xfe;
			P1=temp;
			delay1(200);
			for(a=0;a<2;a++)
			{
				for(i=0;i<7;i++)
				{
					temp=_crol_(temp,1);
					delay(200);
					P1=temp;
				}
				temp=0x7f;
				for(i=0;i<7;i++)
				{
					temp=_cror_(temp,1);
					delay(200);
					P1=temp;
				}
			}
			delay(200);
			P1=0xff;
			delay(200);
			for(a=0;a<5;a++)
			{
				P1=0x00;
				delay(200);
				P1=0xff;
				delay(200);
			}
}
void plan3()
{
	for(a=0;a<3;a++)
	{
		P1=0xaa;
		delay(200);
		P1=0x55;
		delay(200);
	}
	for(a=0;a<3;a++)
	{
				temp=0xfe;
				for(i=0;i<7;i++)
				{
					temp=_crol_(temp,1);
					delay(1);
					P1=temp;
				}
				temp=0x7f;
				for(i=0;i<7;i++)
				{
					temp=_cror_(temp,1);
					delay(1);
					P1=temp;
				}
	}
	for(a=0;a<3;a++)
	{
		P1=0x3c;
		delay(200);
		P1=0x99;
		delay(200);
		P1=0xc3;
		delay(200);
		P1=0xe7;
		delay(200);
	}
	for(a=0;a<3;a++)
	{
		P1=0xe7;
		delay(200);
		P1=0xc3;
		delay(200);
		P1=0x99;
		delay(200);
		P1=0x3c;
		delay(200);
	}
	for(a=0;a<3;a++)
	{
		P1=0x00;
		delay(200);
		P1=0xff;
		delay(200);
	}
}
void plan2()
{
		for(a=0;a<8;a++)
		{
		switch(a)
			{
			case 0 :	P1=0xfe;	temp=0xfe;	delay(200);	break;
			case 1 : P1=0xfc;	temp=0xfc;  delay(200);	break;
			case 2 : P1=0xf8;	temp=0xf8;  delay(200);	break;
			case 3 : P1=0xf0;	temp=0xf0;  delay(200);	break;
			case 4 : P1=0xe0;	temp=0xe0;  delay(200);	break;
			case 5 : P1=0xc0;	temp=0xc0;  delay(200);	break;
			case 6 : P1=0x80;	temp=0x80;  delay(200);	break;
			case 7 : P1=0x00;	temp=0x00;  delay(200);	break;
			}	
			for(i=0;i<8-a;i++)
				{
					temp=_crol_(temp,1);
					delay(200);
					P1=temp;
				}
			}
		for(a=0;a<8;a++)
		{
		switch(a)
			{
			case 0 :	P1=0x80;	temp=0x80;	delay(200);	break;
			case 1 : P1=0xc0;	temp=0xc0;  delay(200);	break;
			case 2 : P1=0xe0;	temp=0xe0;  delay(200);	break;
			case 3 : P1=0xf0;	temp=0xf0;  delay(200);	break;
			case 4 : P1=0xf8;	temp=0xf8;  delay(200);	break;
			case 5 : P1=0xfc;	temp=0xfc;  delay(200);	break;
			case 6 : P1=0xfe;	temp=0xfe;  delay(200);	break;
			case 7 : P1=0xff;	temp=0xff;  delay(200);	break;
			}	
			for(i=0;i<8-a;i++)
				{
					temp=_cror_(temp,1);
					delay(200);
					P1=temp;
				}
			}
		for(a=0;a<3;a++)
		{
			P1=0xff;
			delay(500);
			P1=0x00;
			delay(500);
		}
		P1=0xff;
		while(1) delay(1);
}
