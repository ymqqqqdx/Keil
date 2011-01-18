#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
#define U8 unsigned char
#define U16 unsigned int
sbit AD_CLK=P3^2;
sbit AD_DAT=P3^3;
sbit AD_CS=P3^4;
sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;
sbit DA_DAT=P3^5;
sbit DA_CLK=P3^6;
sbit DA_CS=P3^7;
void Write_DA(U16 dat)
{
	U8 i;
	DA_CS=0;
	dat<<=2;
	dat|=0x02;
	for(i=0;i<16;i++)
	{
		DA_CLK=1;
		dat<<=1;
		DA_DAT=CY;
		DA_CLK=0;
	}
	DA_CS=1;
}
U8 Read_ADC()
{
	U8 temp=0,i;
	AD_CS=0;
	for(i=0;i<8;i++)
	{
		temp|=((U8)AD_DAT)<<(7-i);
		AD_CLK=1;
		AD_CLK=0;
	}
	AD_CS=1;
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
	lcd_string(0x81,"µ±Ç°µçÑ¹£º");
	while(1)
	{
		voltage=Read_ADC();
		if(voltage!=vol_old)
		{
			converted=(U16)((float)voltage/255*5*100);
			Write_DA(1024-(U16)((float)voltage/255*1024));
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