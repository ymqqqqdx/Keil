#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
#define U8 unsigned char
#define U16 unsigned int
sbit DA_DAT=P3^5;
sbit DA_CLK=P3^6;
sbit DA_CS=P3^7;
void Write_DA(U16 dat)
{
	U8 i;
	DA_CS=0;
	dat<<=2;
	dat|=2;
	for(i=0;i<16;i++)
	{
		DA_CLK=1;
		dat<<=1;
		DA_DAT=CY;
		DA_CLK=0;
	}
	DA_CS=1;
}
void delay(int z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void main()
{
	U16 i,j;
	while(1)
	{
		j=0;
		for(i=0;i<5;i++)
		{
			j+=120;
			Write_DA(j);
			delay(1000);
		}
	}
}