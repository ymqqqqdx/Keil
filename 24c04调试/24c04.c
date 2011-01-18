#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
void write_add(unsigned char,unsigned char);
void init_24c02();
unsigned char read_add(unsigned char);
void delay(int);
void main()
{
	uchar i,temp;
	init_24c02();
	while(1)
	{
		for(i=0;i<=0xfe;i++)
		{
			write_add(i,i);
			delay(10);
		}
		for(i=0;i<=0xfe;i++)
		{
			temp=read_add(i);
		}
		while(1);
	}
}