#include <stdio.h>
#include <series.h>
#include <string.h>
#include <intrins.h>
sbit led1=P2^0;
sbit led2=P2^1;
sbit led3=P2^2;
sbit led4=P2^3;
sbit led5=P2^4;
sbit led6=P2^5;
sbit led7=P2^6;
sbit led8=P2^7;
unsigned int aa;
unsigned char comm,stop;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
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
void i2c()
{
	
}
void commscan()
{
	comm=_getkey();
	switch(comm)
	{
		case '1' : P2=0xff; led(); break;
		case '2' : shumaguan();  break;
		case '3' : led3=0; break;
		case '6' : i2c(); break;
	}
}
void main()
{
	comm_init();
	while(1)
	{
		commscan();
	}
}
void ser() interrupt 4
{
	stop=SBUF;
	RI=0;
}
