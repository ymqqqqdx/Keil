#include <string.h>
#include <intrins.h>
#include <stdio.h>
#include <reg52.h>
sbit led1=P2^0;
sbit led2=P2^1;
sbit led3=P2^2;
sbit led4=P2^3;
sbit led5=P2^4;
sbit led6=P2^5;
sbit led7=P2^6;
sbit led8=P2^7;
void delay(int);
int ssscanf()
{
	unsigned char buf[7],i,t=0;
	int s;
	while(i!='%')
	{
		i=_getkey();
		buf[t++]=i;
	}
	buf[strlen(buf)-1]='\0';
	sscanf(buf,"%d",&s);
	for(i=0;i<7;i++)
	{
		buf[i]='\0';
	}
	return(s);
}
void flow(int t,unsigned char way)
{
	unsigned char temp,i;
	if(way==1)
	{
		temp=0xfe;
		for(i=0;i<8;i++)
		{
			P2=temp;
			temp=_crol_(temp,1);
			delay(t);
		}
	}
	if(way==2)
	{
		temp=0x7f;
		for(i=0;i<8;i++)
		{
			P2=temp;
			temp=_cror_(temp,1);
			delay(t);
		}
	}
}
void led()
{
	unsigned char i;
	int s;
	while(i!='!')
	{
		i=_getkey();
		if(i=='d')
		{
			P2=0xff;
			while(i!='^')
			{
				i=_getkey();
				switch(i)
				{
					case '1' : led1=~led1; break;
					case '2' : led2=~led2; break;
					case '3' : led3=~led3; break;
					case '4' : led4=~led4; break;
					case '5' : led5=~led5; break;
					case '6' : led6=~led6; break;
					case '7' : led7=~led7; break;
					case '8' : led8=~led8; break;
					default : break;
				}
			}
		}
		if(i=='f')
		{
			P2=0xff;
			while(i!='#')
			{
				i=_getkey();
				if(i!='#')
				{
					s=ssscanf();
				}
				if(i=='u')
				{
					flow(s,1);
				}
				if(i=='b')
				{
					flow(s,2);
				}
			}
		}
	}
}