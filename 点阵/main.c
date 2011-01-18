#include <reg52.h>
unsigned char code table[]={0x10,0xFE,0x92,0x92,0xFE,0x92,0x10,0x10};
unsigned char code table2[]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
unsigned char i;
void write7219one(unsigned char,unsigned char);
void init7219();
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void main()
{
	init7219();
	while(1)
	{
		for(i=1;i<9;i++)
		{
			write7219one(i,table[i-1]);
		}
		while(1);
	}
}
