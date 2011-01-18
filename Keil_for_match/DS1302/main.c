#include <reg52.h>
#include <stdio.h>
#define SETTED
#define U8 unsigned char
#define U16 unsigned int
#define CMD 0
#define DATA 1
sbit RTC_CLK=P1^6;
sbit RTC_IO=P1^7;
sbit RTC_RST=P3^0;

sbit LCD_CS=P0^3;
sbit LCD_SCLK=P1^2;
sbit LCD_SID=P0^2;
struct RTC
{
	U8 Second;
	U8 Minute;
	U8 Hour;
	U8 Day;
	U8 Month;
	U8 Week;
	U8 Year;
}	Time={0,48,15,11,9,6,10};
U8 code Table[]={"年月日时分秒"};
void Display_Two(U8 Add,U8 Dat);
void Delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void Uart_Init()
{
	REN=1;
	SCON=0x52;
	TMOD=0X20;
	TH1=0XFD;
	TL1=0XFD;
	TR1=1;
	ES=1;
	SM0=0;
	SM1=1;
	EA=1;
}
void RTC_Input_Char(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat>>=1;
		RTC_IO=CY;
		RTC_CLK=1;
		RTC_CLK=0;
	}
}
U8 RTC_Output_Char()
{
	U8 i,temp=0;
	for(i=0;i<8;i++)
	{
		temp|=((char)RTC_IO)<<i;
		RTC_CLK=1;
		RTC_CLK=0;
	}
	return (temp>>4)*10+(temp%16);
}
void Write_RTC(U8 Add,U8 Dat)
{
    RTC_RST = 0;
    RTC_CLK = 0;
    RTC_RST = 1;
	RTC_Input_Char(Add);
	RTC_Input_Char((Dat/10<<4)|(Dat%10));
    RTC_CLK = 1;
    RTC_RST = 0;	
}
U8 Read_RTC(U8 Add)
{
	U8 temp;
    RTC_RST = 0;
    RTC_CLK = 0;
    RTC_RST = 1;
	RTC_Input_Char(Add|1);
	temp=RTC_Output_Char();
    RTC_CLK = 1;
    RTC_RST = 0;
	return temp;
}
void LCD_Send_Byte(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		LCD_SID=CY;
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
	Delay_1ms(5);
}
void LCD_Init()
{
	LCD_Send_CD(CMD,0x30);
	LCD_Send_CD(CMD,0x02);
	LCD_Send_CD(CMD,0x06);
	LCD_Send_CD(CMD,0x0c);
	LCD_Send_CD(CMD,0x01);
	LCD_Send_CD(CMD,0x80);
}
void Get_Time()
{
	U8 i,*Pt=&Time.Second;
	for(i=0;i<7;i++)
		*Pt++=Read_RTC(0x80+2*i);
	Display_Two(1,Time.Year);
	Display_Two(3,Time.Month);
	Display_Two(5,Time.Day);
	Display_Two(11,Time.Hour);
	Display_Two(13,Time.Minute);
	Display_Two(15,Time.Second);
}
void Display_Two(U8 Add,U8 Dat)
{
	if(Add<10)
	LCD_Send_CD(CMD,0x80+Add);
	else
	LCD_Send_CD(CMD,0x90+Add-10);
	LCD_Send_CD(DATA,Dat/10+'0');
	LCD_Send_CD(DATA,Dat%10+'0');
}
void RTC_Init()
{
	U8 i,*Pt=&Time.Second;
	for(i=0;i<7;i++)
		Write_RTC(0x80+2*i,*Pt++);
}
void Display_Init()
{
	U8 i;
	for(i=0;i<3;i++){
		LCD_Send_CD(CMD,0x80+2*i+2);
		LCD_Send_CD(DATA,Table[i*2]);
		LCD_Send_CD(DATA,Table[i*2+1]);
	}
	for(i=0;i<3;i++){
		LCD_Send_CD(CMD,0x90+2*i+2);
		LCD_Send_CD(DATA,Table[i*2+6]);
		LCD_Send_CD(DATA,Table[i*2+7]);
	}
}
void main()
{
	U8 Second_Old;
#ifndef SETTED
	RTC_Init();
#endif
	Uart_Init();
	LCD_Init();
	Display_Init();
	Get_Time();
	while(1)
	{
		Time.Second=Read_RTC(0x80);
		if(Second_Old!=Time.Second)
		{
			Display_Two(15,Time.Second);
			if(Time.Second==0)
				Get_Time();
			Second_Old=Time.Second;
		}
	}
}