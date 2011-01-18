#include <reg52.h>
#include <stdio.h>
#include "declare.h"
void led();
void shumaguan();
void i2c();
void ds1302();
void ds18b20();
void timer();
void commscan()
{
	comm=_getkey();
	switch(comm)
	{
		case '1' : P2=0x00; led(); break;
		case '2' : SCON=0x50; shumaguan();  break;
		case '4' : timer(); break;
		case '6' :  P2=0x00; i2c(); break;
		case '7' : ds18b20(); break;
		case '8' : ds1302(); break;
	}
}