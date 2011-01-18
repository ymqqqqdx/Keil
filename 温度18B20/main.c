#include <stdio.h>
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
unsigned char code point[]={
        0xBF,/*0p*/
		0x86,/*1p*/
        0xDB,/*2p*/
        0xCF,/*3p*/
        0xE6,/*4p*/
        0xED,/*5p*/
        0xFD,/*6p*/
        0x87,/*7p*/
        0xFF,/*8p*/
        0xEF,/*9p*/
};
int temp,tempold;
unsigned char shi,ge,one;
void write_onechar(unsigned char);
void delay(int);
int readtemp();
void main()
{
	while(1)
	{
		temp=readtemp();
		delay(100);
		if(temp!=tempold)
		{
			shi=temp%1000/100;
			ge=temp%100/10;
			one=temp%10;
			write_onechar(table[one]);
			write_onechar(point[ge]);
			write_onechar(table[shi]);
			write_onechar(0x00);
			tempold=temp;
		}
	}
}