#include <reg52.h>
#include "declare.h"
sbit k1=P1^7;
sbit k2=P1^6;
sbit k3=P1^5;
int count;
void keyscan();
void timer0() interrupt 1
{
	TH0=(65536-50000)/256; 
	TL0=(65536-50000)%256;
	count++;
	keyscan();
/*	if(count==100&&buff==1)
	{
		count=0;
		if(mode==1) mode=2;
		else if(mode==2) mode=1;
	}*/
}