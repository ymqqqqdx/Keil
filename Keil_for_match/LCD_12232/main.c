#include <reg52.h>
#include <stdio.h>
#define U8 unsigned char
#define U16 unsigned int
#define CMD 0
#define DATA 1
sbit LCD_CS=P0^3;
sbit LCD_SCLK=P1^2;
sbit LCD_SID=P0^2;
U8 code table[]={"I LOVE MCU!"};
void Delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void LCD_Send_Byte(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		LCD_SID=(bit)(dat&(1<<(7-i)));
		LCD_SCLK=0;
		LCD_SCLK=1;
	}
}
void LCD_Send_CD(bit CD,U8 dat)
{
	U8 temp;
	if(!CD) temp=0xf8;
		else temp=0xfa;
	LCD_CS=1;
	LCD_SCLK=0;
	LCD_Send_Byte(temp);
	temp=dat;
	temp&=0xf0;
	LCD_Send_Byte(temp);
	temp=dat;
	temp<<=4;
	LCD_Send_Byte(temp);
	LCD_CS=0;
	Delay_1ms(10);
}
void LCD_Init()
{
//	Delay_1ms(100);
//	LCD_Send_CD(CMD,0x30);
//	LCD_Send_CD(CMD,0x02);
//	LCD_Send_CD(CMD,0x06);
	LCD_Send_CD(CMD,0x0c);
//	LCD_Send_CD(CMD,0x01);
}
void main()
{
	U8 i;
	LCD_Init();
	LCD_Send_CD(CMD,0x80);
	for(i=0;i<11;i++)
		LCD_Send_CD(DATA,table[i]);
	while(1);
}