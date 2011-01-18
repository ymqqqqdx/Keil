#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
//sbit ds=P3^0;
//sbit clk=P3^1; 
void main()
{
	P2=0xfe;
	P0=0x00;
	P1=0x00;
	while(1);
}
