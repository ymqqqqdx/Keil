#include <reg52.h>
#include <stdio.h>
void comm_init();
void delay(int);
void main()
{
	unsigned char i;
	unsigned char buffer[32];
	unsigned int forpr;
	comm_init();
	while(1)
	{
/*		for(i=0;i<32;i++)
		{
			buffer[i]=_getkey();
		}
		for(i=0;i<32;i++)
		{
			forpr=buffer[i];
			printf("%d",forpr);	
			delay(10);
		}*/
		printf("%s","123456");
		delay(1000);
	}
}