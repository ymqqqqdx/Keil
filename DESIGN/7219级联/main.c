#include <reg52.h>
void init7219();
void write7219one(unsigned char,unsigned char);
void delay(int);
void main()
{
	unsigned char i;
	init7219();
	for(i=1;i<17;i++)
	{
		write7219one(i,6);
		delay(1000);
	}
	write7219one(14,2);
	while(1);
}
