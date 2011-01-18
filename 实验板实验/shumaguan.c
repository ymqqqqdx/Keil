#include <reg52.h>
#include <stdio.h>
#include "declare.h"
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
void delay(int);
void shumaguan()
{
	unsigned char a,b,c,d,buff;
	while(buff!='#')
	{
		buff=_getkey();
		a=_getkey();
		b=_getkey();
		c=_getkey();
		d=_getkey();
		stop=0;
		if(stop!='!')
		{
			ES=1;
			while(stop!='!')
			{
				P2=0xf8;
				if(a>=48&&a<=58) P0=table[a-48];
				else if(a>=65) P0=table[a-55];
				delay(1);
				P2=0xf4;
				if(b>=48&&b<=58) P0=table[b-48];
				else if(b>=65) P0=table[b-55];
				delay(1);
				P2=0xf2;
				if(c>=48&&c<=58) P0=table[c-48];
				else if(c>=65) P0=table[c-55];
				delay(1);
				P2=0xf1;
				if(d>=48&&d<=58) P0=table[d-48];
				else if(d>=65) P0=table[d-55];
				delay(1);
			}
			ES=0;
		}
	}
}