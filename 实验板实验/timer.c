#include <reg52.h>
#include "declare.h"
#include <intrins.h>
#include <stdio.h>
sbit clock=P3^1;
sbit dat=P3^0;
int count,max,buff,tt;
unsigned char way;
int ssscanf();
void timer()
{
	TH0=(65536-1000)/256; 
	TL0=(65536-1000)%256;
	while(way!='@')
	{
		do 
		{
			way=_getkey();
		} while(way=='#');
		if(way=='@') break;
		if(way=='1')
		{
			P2=0xff;
			clock=0;
			for(i=0;i<32;i++)
			{
				clock=~clock;
				clock=~clock;
				dat=1;
			}
		}
		if(way=='2')
		{
			temp=0xfe;
			P2=temp;
		}
		max=ssscanf();
		ET0=1;
		TR0=1;
		while(stop!='#')
		{
			stop=_getkey();
		}
		stop=0; 	TR0=0; ET0=0; way=0;
	}
}
void timer0() interrupt 1
{
	TH0=(65536-1000)/256; 
	TL0=(65536-1000)%256;
	count++;
	if(count==max)
	{
		count=0;
		if(way=='1')
		{
			if(buff==0)
			{
				tt++;
				clock=~clock;
				clock=~clock;
				dat=0;
				if(tt==32)
				{
					buff=1;
					tt=0;
				}
			}
			else if(buff==1)
			{
				tt++;
				clock=~clock;
				clock=~clock;
				dat=1;
				if(tt==32)
				{
					buff=0;
					tt=0;
				}
			}
		}
		if(way=='2')
		{
			temp=_crol_(temp,1);
			P2=temp;
		}
	}
}
