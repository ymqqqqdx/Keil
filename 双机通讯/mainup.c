#include <reg52.h>
#include <stdio.h>
void delay(int);
void comm_init();
void main()
{
	unsigned char i=0;
	comm_init();
	while(1)
	{
//		printf("%d",i++);
		SBUF=i++;
		delay(1000);
		if(i==10)
		i=0;
	}
}
