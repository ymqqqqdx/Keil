#include <reg52.h>
#include <stdio.h>
#define U8 unsigned char
#define U16 unsigned int
U8 code table[]={0x0c,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void hc595_write(U8 dat)
{
	U8 i,save_P1=P1;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		hc595_dat=CY;
		hc595_clk=0;
		hc595_clk=1;
	}
	hc595_sclk=0;
	hc595_sclk=1;
	P1=save_P1;
}

void leds(U16 dat)
{
	U8 temp=0;
	temp|=(dat&(0xf<<9))>>5;
	temp|=(U8)(bit)(dat&(1<<0))<<1;
	temp|=(U8)(bit)(dat&(1<<1))<<0;
	temp|=(U8)(bit)(dat&(1<<2))<<3;
	temp|=(U8)(bit)(dat&(1<<3))<<2;
	hc595_write(temp);
	P1=(dat&(0x1f<<4))>>4;
}

void digit(U8 add,U8 dat)
{
	P2=(~1<<(add-1));
	OE=0;
	P0=table[dat];
	clk=0;
	clk=1;
	delay_1ms(5);
}

void start()
{
	sda=1;
	scl=1;
	sda=0;
}

void stop()
{
	sda=0;
	scl=1;
	sda=1;
}

void response()
{
	U8 i=0;
	scl=1;
	while(sda==1&&i<250)
		i++;
	scl=0;
}

void iic_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		scl=0;
		sda=CY;
		scl=1;
	}
	scl=0;
	sda=1;
	response();
}

U8 iic_readonechar()
{
	U8 i,temp=0;
	scl=0;
	sda=1;
	for(i=0;i<8;i++)
	{
		scl=1;
		temp|=((U8)sda)<<(7-i);
		scl=0;
	}
	return temp;
}

void iic_write(U8 add,U8 dat)
{
	start();
	iic_writeonechar(0xa0);
	iic_writeonechar(add);
	iic_writeonechar(dat);
	stop();
}

U8 iic_read(U8 add)
{
	U8 temp;
	start();
	iic_writeonechar(0xa0);
	iic_writeonechar(add);
	start();
	iic_writeonechar(0xa1);
	temp=iic_readonechar();
	return temp;
}

void iic_init()
{
	scl=1;
	sda=1;
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
	U16 t;
	float tt;
	U8 low,high;
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

void temp_rom()
{
	U8 i;
	temp_init();
	temp_writeonechar(0x33);
	for(i=0;i<8;i++)
		printf(" %x ",temp_readonechar());
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

void rtc_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat>>=1;
		rtc_clk=0;
		rtc_dat=CY;
		rtc_clk=1;
	}
}

U8 rtc_readonechar()
{
	U8 i,temp=0;
	for(i=0;i<8;i++)
	{
		rtc_clk=0;
		temp|=((U8)rtc_dat)<<i;
		rtc_clk=1;
	}
	return temp/16*10+temp%16;
}

void rtc_write(U8 add,U8 dat)
{
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	rtc_writeonechar(add);
	rtc_writeonechar((dat/10)<<4|dat%10);
	rtc_rst=0;
}

U8 rtc_read(U8 add)
{
	U8 temp=0;
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	rtc_writeonechar(add|1);
	temp=rtc_readonechar();
	rtc_rst=0;
	return temp;
}

void hd_longdelay()
{
	U8 i;
	for(i=0;i<50;i++);
}

void hd_shortdelay()
{
	U8 i;
	for(i=0;i<8;i++);
}

void hd_writeonechar(U8 dat)
{
	U8 i;
	hd_cs=0;
	hd_dat=0;
	hd_longdelay();
	for(i=0;i<8;i++)
	{
		dat<<=1;
		hd_dat=CY;
		hd_clk=1; hd_shortdelay();
		hd_clk=0; hd_shortdelay();
	}
	hd_dat=0;
}

U8 hd_readonechar()
{
	U8 i,temp=0;
	hd_dat=1;
	hd_longdelay();
	for(i=0;i<8;i++)
	{
		hd_clk=1; hd_shortdelay();
		temp|=((U8)hd_dat)<<(7-i);
		hd_clk=0; hd_shortdelay();
	}
	hd_dat=0;
}

void hd_write(U8 cmd,U8 dat)
{
	hd_writeonechar(cmd);
	hd_writeonechar(dat);
}

U8 hd_read(U8 cmd)
{
	hd_writeonechar(cmd);
	return(hd_readonechar());
}

U8 ad_read()
{
	U8 i,temp=0;
	ad_cs=0;
	for(i=0;i<8;i++)
	{
		temp|=((U8)ad_dat)<<(7-i);
		ad_clk=1;
		ad_clk=0;
	}
	ad_cs=1;
	return temp;
}

void da_write(U16 dat)
{
	U8 i;
	dat<<=2;
	dat|=0x02;
	da_cs=0;
	for(i=0;i<16;i++)
	{
		dat<<=1;
		da_clk=1;
		da_dat=CY;
		da_clk=0;
	}
	da_cs=1;
}
