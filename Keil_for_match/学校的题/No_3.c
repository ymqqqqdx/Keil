#include <reg52.h>
#define U8 unsigned char
#define U16 unsigned int
#define CMD 0
#define DATA 1
sbit ad_clk=P3^2;
sbit ad_dat=P3^3;
sbit ad_cs=P3^4;
sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;

U8 ad_read()
{
	U8 i=0,temp=0;
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

void main()
{
	U8 voltage,vol_old;
	U16 converted;
	lcd_writecd(0,0x0c);
	lcd_string(0x80,"µ±Ç°µçÑ¹£º");
	while(1)
	{
		voltage=ad_read();
		if(voltage!=vol_old)
		{
			converted=(U16)((float)voltage/255*5*100);
			lcd_writecd(0,0x92);
			lcd_writecd(1,converted/100+'0');
			lcd_writecd(1,'.');
			lcd_writecd(1,converted%100/10+'0');
			lcd_writecd(1,converted%10+'0');
			lcd_writecd(1,'V');
			vol_old=voltage;
		}
	}
}