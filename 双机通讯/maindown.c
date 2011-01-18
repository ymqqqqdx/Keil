#include <reg52.h>
#include <stdio.h>
unsigned char i,t;
void comm_init();
void init7219();
void write7219one(unsigned char,unsigned char);
void delay(int);
void main()
{
	comm_init();
	init7219();
	ES=1;
	while(1);
/*	{
		t=_getkey();
		P2=t;
		for(i=1;i<9;i++)
		{
			write7219one(i,t-48);
		}
		delay(2000);
	}*/
}
void ser() interrupt 4
{
	t=SBUF;
	for(i=1;i<9;i++)
	{
		write7219one(i,t-48);
	}
	RI=0;
	delay(1000);
}
