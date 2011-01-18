#include <reg52.h>
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=22;j>0;j--);
}