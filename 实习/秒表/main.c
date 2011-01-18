#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit clock=P3^1;
sbit dat=P3^0;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
void delay(int z)
{
	int x,y;
	for(x=0;x<z;x++)
		for(y=0;y<110;y++);
}
void writeonechar(uchar temp)
{
	int i;
	for(i=0;i<8;i++)
	{
		dat=temp&0x01;
		clock=0;
		temp>>=1;			
		clock=1;
	}
}
void main()
{
	uchar fen=0,miao=0;
	while(1)
	{
		miao++;
		if(miao==60)
		{
			fen++;
			miao=0;
			if(fen==60)
				fen=0;
		}
		writeonechar(table[miao%10]);
		writeonechar(table[miao/10]);
		writeonechar(table[fen%10]);
		writeonechar(table[fen/10]);
		delay(1000);
	}
}