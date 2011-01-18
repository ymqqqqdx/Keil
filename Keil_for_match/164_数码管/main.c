#include <reg52.h>
#define U8 unsigned char
#define U16 unsigned int
sbit LS164_CLK=P0^1;
sbit LS164_DAT=P0^0;
sbit WEI1=P1^0;
sbit WEI2=P1^1;
U8 code Table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,};
void Delay_1ms(U8 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void Write_164(U8 temp)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		LS164_CLK=0;
		temp<<=1;
		LS164_DAT=CY;
		LS164_CLK=1;
	}
}
void main()
{
	while(1)
	{
		WEI1=0;
		Write_164(Table[1]);
		Delay_1ms(11);
		WEI1=1;
		
		WEI2=0;
		Write_164(Table[2]);
		Delay_1ms(11);
		WEI2=1;
	}
}