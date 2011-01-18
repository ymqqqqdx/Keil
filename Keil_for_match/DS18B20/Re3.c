#include <reg52.h>
#include <stdio.h>
#define U8 unsigned char
#define U16 unsigned int
sbit DQ=P1^0;
sbit clk=P3^6;
sbit OE=P2^4;

sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;
U8 code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
U8 code rom[2][8]={{0x28,0xd0,0xd3,0xb2,0x02,0x00,0x00,0x13},{0x28,0x5e,0xfa,0xb2,0x02,0x00,0x00,0xe1}};
void delayus(U16 z)
{
	while(z--);
}

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
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

U16 temp_read__()
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

U16 temp_read(U8 num)
{
	U8 i;
	U8 low,high;
	U16 t;
	float tt;
	temp_init();
	temp_writeonechar(0x55);	
	for(i=0;i<8;i++)
		temp_writeonechar(rom[num][i]);
	temp_writeonechar(0x44);
	temp_init();
	temp_writeonechar(0x55);	
	for(i=0;i<8;i++)
		temp_writeonechar(rom[num][i]);
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

void digit(U16 add,U16 dat)
{
	P2=~(1<<(add-1));
	OE=0;
	P0=dat;
	clk=0;
	clk=1;
	delay_1ms(5);
}

void uart_init()
{
	TMOD=0x21;
	SCON=0x52;
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	EA=1;
}

void temp_rom()
{
	U8 i;
	puts("ROM:");
	temp_init();
	temp_writeonechar(0x33);
	for(i=0;i<8;i++)
		printf(" 0x%hhx ",temp_readonechar());
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
}

void main()
{
	U16 temp1,temp1_old,temp2,temp2_old;
	lcd_writecd(0,0x0c);
	lcd_string(0x80,"²âÊÔµã1:");
	lcd_string(0x90,"²âÊÔµã2:");
	uart_init();
	while(1)
	{
		temp1=temp_read(0);
		temp2=temp_read(1);
		if(temp1!=temp1_old || temp2!=temp2_old)
		{
			lcd_temp(0x84,temp1);
			lcd_temp(0x94,temp2);
			temp1_old=temp1;
			temp2_old=temp2;
		}
		delay_1ms(5);
	}
}