#include <reg52.h>
#include <intrins.h>
#define U8 unsigned char
#define U16 unsigned int
sbit DQ=P1^0;

sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;

sbit key1=P3^4;
sbit key2=P3^5;
sbit key3=P3^2;
sbit key4=P3^3;

sbit beep=P2^7;

sbit scl=P2^5;
sbit sda=P2^6;
U8 counter=0;
U16 temp_up=0;
bit start=0,warned=0;
void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void start1()
{
	sda=1; _nop_();
	scl=1; _nop_();
	sda=0; _nop_();
}

void stop()
{
	sda=0; _nop_();
	scl=1; _nop_();
	sda=1; _nop_();
}

void response()
{
	U8 i=0;
	scl=1; _nop_();
	while(sda==1&&i<250)
		i++;
	scl=0; _nop_();
}
void iic_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		scl=0; _nop_();
		sda=CY;
		scl=1; _nop_();
	}
	scl=0; _nop_();
	sda=1; _nop_();
	response();
}

U8 iic_readonechar()
{
	U8 i,temp=0;
	scl=0; _nop_();
	sda=1; _nop_();
	for(i=0;i<8;i++)
	{
		scl=1; _nop_();
		temp|=((U8)sda)<<(7-i);
		scl=0; _nop_();
	}
	return temp;
}
void iic_write(U8 add,U8 dat)
{
	start1();
	iic_writeonechar(0xa0);
	iic_writeonechar(add);
	iic_writeonechar(dat);
	stop();
}
U8 iic_read(U8 add)
{
	U8 temp=0;
	start1();
	iic_writeonechar(0xa0);
	iic_writeonechar(add);
	start1();
	iic_writeonechar(0xa1);
	temp=iic_readonechar();
	stop();
	return temp;
}
void iic_init()
{
	scl=1; _nop_();
	sda=1; _nop_();
}
void delayus(U16 z)
{
	while(z--);
}

void temp_init()
{
	bit s;
	DQ=1;
	delayus(8);
	DQ=0;
	delayus(80);
	DQ=1;
	delayus(14);
	s=DQ;
	delayus(20);
}

void temp_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat>>=1;
		DQ=0;
		DQ=CY;
		delayus(5);
		DQ=1;
	}
	delayus(4);
}

U8 temp_readonechar()
{
	U8 i,temp=0;
	for(i=0;i<8;i++)
	{
		DQ=0;
		DQ=1;
		temp|=((U8)DQ)<<i;
		delayus(4);
	}
	return temp;
}

U16 temp_read()
{
	U8 low,high;
	U16 t;
	float tt;
	temp_init();
	temp_writeonechar(0xcc);
	temp_writeonechar(0x44);
	temp_init();
	temp_writeonechar(0xcc);
	temp_writeonechar(0xbe);
	low=temp_readonechar();
	high=temp_readonechar();
	t=high;
	t<<=8;
	t|=low;
	tt=t*0.0625;
	t=tt*10+0.5;
	return t;
}

void lcd_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		lcd_sid=CY;
		lcd_clk=0;
		lcd_clk=1;
	}
}

void lcd_writecd(bit cd,U8 dat)
{
	lcd_cs=1;
	lcd_writeonechar(cd==0?0xf8:0xfa);
	lcd_writeonechar(dat&0xf0);
	lcd_writeonechar(dat<<4);
	lcd_cs=0;
}

void lcd_string(U8 add,U8 *pt)
{
	lcd_writecd(0,add);
	while(*pt!='\0')
		lcd_writecd(1,*pt++);
}

void lcd_temp(U8 add,U16 dat)
{
	lcd_writecd(0,add);
	lcd_writecd(1,dat/100+'0');
	lcd_writecd(1,dat%100/10+'0');
	lcd_writecd(1,'.');
	lcd_writecd(1,dat%10+'0');
	lcd_string(add+2,"℃");
}

void keyscan()
{
	if(!key1)
	{
		delay_1ms(5);
		if(!key1)
		{
			temp_up+=10;
			lcd_temp(0x84,temp_up);
			iic_write(0,(U8)(temp_up/10));
		}while(!key1);
	}
	else if(!key2)
	{
		delay_1ms(5);
		if(!key2)
		{
			temp_up-=10;
			lcd_temp(0x84,temp_up);
			iic_write(0,(U8)(temp_up/10));
		}while(!key2);
	}
	else if(!key4)
	{
		delay_1ms(5);
		if(!key4)
		{
			start=1;
			lcd_string(0x94,"         ");
		}while(!key4);
	}
	delay_1ms(5);
}

void main()
{
	U16 temp,temp_old;
	iic_init();
	temp_up=iic_read(0);
	temp_up*=10;
	TMOD=0x01;
	TH0=(65536-45278)/256;
	TL0=(65536-45278)%256;
	ET0=1;
	TR0=1;
	EA=1;
	lcd_writecd(0,0x0c);
	lcd_string(0x80,"上限值：");
	lcd_string(0x90,"采集值：按S1开始");
	lcd_temp(0x84,temp_up);
	while(1)
	{
		if(!start)
			keyscan();
		else
		{
			temp=temp_read();
			if(temp!=temp_old)
			{
				temp_old=temp;
				lcd_temp(0x94,temp);
				if(temp>=temp_up&&warned==0)
				{
					beep=0;
					TR0=1;
					warned=1;
				}
				if(temp<temp_up)
					warned=0;
			}
		}
	}
}

void timer0() interrupt 1
{
	TH0=(65536-45278)/256;
	TL0=(65536-45278)%256;	
	counter++;
	if(counter==60)
	{
		counter=0;
		beep=1;
		TR0=0;
	}
}