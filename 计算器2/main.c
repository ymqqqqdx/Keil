#include <reg52.h>
#include "declare.h"
void init_1602();
void display(unsigned char *);
void keyscan();
unsigned char i,buff=0,sign=0,point=0,symbol=0;
char first[16],second[16];
char len;
float aa,bb,mm;
void main()
{
	init_1602();
	display("0.");
	while(1)
	{
		keyscan();
	}
}

