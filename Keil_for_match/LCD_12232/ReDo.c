#include <reg52.h>
#include <stdio.h>
#define U8 unsigned char
#define U16 unsigned int
#define CMD 0
#define DATA 1
sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;
void delay_1ms(int z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);	
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
	U8 i;
	lcd_writecd(CMD,0x0c);
	lcd_string(0x87,"Welcome");
	lcd_string(0x97,"Good Luck!");
	while(1)
	{
		lcd_writecd(CMD,0x18);
		delay_1ms(1000);
	}
}