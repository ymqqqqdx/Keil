#include <reg52.h>
void delay(int z)
{
	int i=0,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}