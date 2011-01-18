#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define uchar unsigned char
#define uint unsigned int
#define r1 0xfe
#define r2 0xfd
#define r3 0xfb
#define r4 0xf7
#define r5 0xef
sbit ln=P2^1;
sbit ls=P2^0;
unsigned char i,key,buff=0,sign=0,point=0,symbol=0,cai=0;
char first[16],second[16];
char len;
float mm,a,b;
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void write_com(unsigned char com)
{
	ls=0;
    P0=com;
    delay(5);
    ln=1;
    delay(5);
    ln=0;
}
void write_data(unsigned char date)
{
	ls=1;
    P0=date;
    delay(5);
    ln=1;
    delay(5);
    ln=0;
}
void init_1602()
{
   write_com(0x38);
   write_com(0x0c);
   write_com(0x06);
   write_com(0x01);
}
void moveto(unsigned char row,unsigned char line)
{
	if(row==1)
		write_com(0x80+line);
	if(row==2)
		write_com(0x80+0x40+line);
}
void clear(unsigned char t)
{
	if(t==1)
	{
		write_com(0x80);
	}
	if(t==2)
	{
		write_com(0x80+0x40);
	}
	for(i=0;i<15;i++)
	{
		write_data(' ');
	}
}
cleardata(unsigned char t)
{
	if(t==1)
	{
		for(i=0;i<15;i++)
		first[i]='\0';
	}
	if(t==2)
	{
		for(i=0;i<15;i++)
		second[i]='\0';
	}
}
void display(unsigned char *temp)
{
	char n;
//	sprintf(buf,"%.3f\0",date);
	n=strlen(temp);
	if(n<=0)
	{
		clear(2);
		write_com(0x01);
		write_com(0x80+0x40+14);
		write_data('0');
		write_data('.');
	}
	else
	{
	if(n>16) n=16;
	clear(2);
	write_com(0x80+0x40+16-n);
	for(i=0;i<16;i++)
	write_data(temp[i]);
	}
}
void reset()
{
	len=0; sign=0; point=0; write_com(0x01); display("0.");
}
unsigned char keyscan()
{
	unsigned char temp;
	P1=r1;
	temp=P3; temp&=0xff;
	if(P3!=0xff)
	{
		temp=P3;
		switch(temp)
		{
			case r1 : key=31; break;//MC
			case r2 : key=32; break;//MR
			case r3 : key=33; break;//MS
			case r4 : key=34; break;//M+
			case r5 : key=35; break;//M-
		}
		while(P3!=0xff)
		{
			temp=P3;
			temp&=0xff;
		}
	}
	P1=r2;
	temp=P3; temp&=0xff;
	if(P3!=0xff)
	{
		temp=P3;
		switch(temp)
		{
			case r1 : key='B'; break;//backspace
			case r2 : key='C'; break;//clear
			case r3 : key='R'; break;//reset
			case r4 : key='F'; break;//plus or minus
			case r5 : key='S'; break;//sqrt
		}
	}
		while(P3!=0xff)
		{
			temp=P3;
			temp&=0xff;
		}
	P1=r3;
	temp=P3; temp&=0xff;
	if(P3!=0xff)
	{
		temp=P3;
		switch(temp)
		{
			case r1 : key='7'; break;//7
			case r2 : key='8'; break;//8
			case r3 : key='9'; break;//9
			case r4 : key='/'; break;//divide
			case r5 : key='%'; break;//%
		}
		while(P3!=0xff)
		{
			temp=P3;
			temp&=0xff;
		}
	}
	P1=r4;
	temp=P3; temp&=0xff;
	if(P3!=0xff)
	{
		temp=P3;
		switch(temp)
		{
			case r1 : key='4'; break;//4
			case r2 : key='5'; break;//5
			case r3 : key='6'; break;//6
			case r4 : key='*'; break;//multiply
			case r5 : key='D'; break;//1/x
		}
		while(P3!=0xff)
		{
			temp=P3;
			temp&=0xff;
		}
	}
	P1=r5;
	temp=P3; temp&=0xff;
	if(P3!=0xff)
	{
		temp=P3;
		switch(temp)
		{
			case r1 : key='1'; break;//1
			case r2 : key='2'; break;//2
			case r3 : key='3'; break;//3
			case r4 : key='-'; break;//minus
			case r5 : key='='; break;//equal
		}
		while(P3!=0xff)
		{
			temp=P3;
			temp&=0xff;
		}
	}
	P1=0xdf;
	temp=P3; temp&=0xff;
	if(P3!=0xff)
	{
		temp=P3;
		switch(temp)
		{
			case r1 : key='0'; break;//0
			case r2 : key='.'; break;//.
			case r3 : key='U'; break;//empty
			case r4 : key='+'; break;//add
			case r5 : key='='; break;//equal
		}
		while(P3!=0xff)
		{
			temp=P3;
			temp&=0xff;
		}
	}
	P1=0xff;
	P3=0xff;
	return(key);
}
void keypress()
{
	key=keyscan();
	if(key>='0'&&key<='9')
	{
		second[len++]=key;
	}
	if(key=='.'&&point==0)
	{
		second[len++]=key;
		point=1;
	}
	if(key=='B')
	{
		first[--len]='\0'; if(len<0) len=0;
	}
	if(key=='+'||key=='-'||key=='*'||key=='/')
	{
		if(buff==0)
		{
			for(i=0;i<15;i++)
			{
				first[i]=second[i];
				second[i]='\0';
			}
			reset();
			buff=1;
		}
		else if(buff==1)
		{
//			sscanf(first,"%f",&a);
			sscanf(second,"%f",&b);
			if(key=='+')
			{
				a=a+b;
			}
			if(key=='-')
			{
				a=a-b;
			}
			if(key=='*')
			{
				a=a*b;
			}
			if(key=='/')
			{
				a=a/b;
			}
		/*	if(a<0.000001||a>1000000)
			{
				sprintf(first,"%e",a);
			}
			else
			{
				sprintf(first,"%f",a);
			}*/
		}
	}
	if(key=='=')
	{
//		sprintf(first,"%f",a);
	}
}
