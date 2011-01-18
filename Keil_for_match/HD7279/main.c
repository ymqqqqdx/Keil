#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
#define U8 unsigned char
#define U16 unsigned int
sbit HD_CS=P0^4;
sbit HD_CLK=P0^5;
sbit HD_DAT=P0^6;
sbit HD_KEY=P0^7;
void Delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void Long_Delay()
{
	U8 i;
	for (i=0;i<50;i++);
}
void Short_Delay()
{
	U8 i;
	for (i=0;i<8;i++);
}
void HD_Write_Byte(U8 cmd)
{
	U8 i;
	HD_CS=0;
	HD_DAT=0;
	Long_Delay();
	for(i=0;i<8;i++)
	{
		cmd<<=1;
		HD_DAT=CY;
		HD_CLK=1; Short_Delay();
		HD_CLK=0; Short_Delay();
	}
	HD_DAT=0;
}
U8 HD_Read_Byte()
{
	U8 i,temp=0;
	HD_DAT=1;
	Long_Delay();
	for(i=0;i<8;i++)
	{
		HD_CLK=1;	Short_Delay();
		temp|=((char)HD_DAT)<<(7-i);
		HD_CLK=0;	Short_Delay();
	}
	HD_DAT=0;
	return temp;
}
void HD_Write_CD(U8 cmd,U8 dat)
{
	HD_Write_Byte(cmd);
	HD_Write_Byte(dat);
}
U8 HD_Read_CD(U8 cmd)
{
	HD_Write_Byte(cmd);
	return(HD_Read_Byte());
}
U8 Key_Convert(U8 temp)
{
	if(temp<28)
	{
		switch(temp%8)
		{
			case 6 : return temp/8*3+1; break;
			case 5 : return temp/8*3+2; break;
			case 4 : return temp/8*3+3; break;
			case 3 : return temp/8+13; break;
		}
	}
	else
	{
		if(temp==29) return 0;
		if(temp==30) return 11;
		if(temp==28) return 12;
	}
}
void main()
{
	U8 i,temp;
	HD_Write_Byte(0xa4);
	while(1)
	{
		if(!HD_KEY)
		{
			temp=HD_Read_CD(0x15);
			temp=Key_Convert(temp);
			HD_Write_CD(0xc8+1,temp/10);
			HD_Write_CD(0xc8,temp%10);
		}while(!HD_KEY);
		Delay_1ms(5);
	}
}