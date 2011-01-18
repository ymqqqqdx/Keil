#include <reg52.h>
#include <declare.h>
sbit DA_DAT=P3^5;
sbit DA_CLK=P3^6;
sbit DA_CS=P3^7;
void da_write(U16 dat)
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
void da_process(U8 dat)
{
	dat=dat%10;
	da_write(700-dat*70);
}