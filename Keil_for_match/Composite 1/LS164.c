#include <reg52.h>
#include <declare.h>
sbit LS164_CLK=P0^1;
sbit LS164_DAT=P0^0;
void ls164_write(U8 temp)
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