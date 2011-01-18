#include <reg52.h>
unsigned char i;
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void main()
{
	P3=0x0f;
	while(1)
	{
		P3=0x9f;
		delay(100);
		P3=0x3f;
		delay(100);
		P3=0x6f;
		delay(100);
		P3=0xcf;
		delay(100);
	}
}
