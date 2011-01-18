#include <reg52.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "declare.h"
#define r1 0xfe
#define r2 0xfd
#define r3 0xfb
#define r4 0xf7
#define r5 0xef
void display(unsigned char *);
void clear(unsigned char);
void moveto(unsigned char,unsigned char);
void cleardata(unsigned char);
void write_com(unsigned char);
void displayup(unsigned char *);
void write_data(unsigned char);
void copytofirst()
{
	for(i=0;i<16;i++)
	{
		first[i]=second[i];
		second[i]='\0';
	}
}
void convert()
{
	sscanf(second,"%f",&bb);
}
void keyscan()
{
	unsigned char temp,key=0;
	P1=r1;
	temp=P3; temp&=0xff;
	if(P3!=0xff)
	{
		temp=P3;
		switch(temp)
		{
			case r1 : key=1; mm=0; break;//MC
			case r2 : key=2; bb=mm; break;//MR
			case r3 : key=3; mm=bb; break;//MS
			case r4 : key=4; bb+=mm; break;//M+
			case r5 : key=5; bb-=mm; break;//M-
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
			case r1 : key=6; second[--len]='\0'; display(second); if(len<0) len=0; break;//back
			case r2 : key=7; bb=0; for(i=0;i<15;i++) second[i]='\0';display(second); len=0; break;//ce
			case r3 : key=8; for(i=0;i<15;i++){first[i]='\0';second[i]='\0';} buff=0;len=0;sign=0;display(first); break;//reset
			case r4 : key=9; if(len>0)
										{
											if(sign==0&&len<15)
											{
												len++;
												for(i=len;i>0;i--)
												{
												 second[i]=second[i-1];
												}
												second[0]='-';
												sign=1;												
											}
											else if(sign==1)
											{
												for(i=0;i<len;i++)
												{
												 	second[i]=second[i+1];
												}
												sign=0;
												len--;
											}
										}	
									display(second);  break;//正负
		case r5 : key=10; clear(1); moveto(1,0); displayup("sqrt()="); 
		convert();bb=sqrt(bb);sprintf(second,"%g\0",bb); display(second);
		break;//根号
	}
		while(P3!=0xff)
		{
			temp=P3;
			temp&=0xff;
		}
	}
	P1=r3;
	temp=P3; temp&=0xff;
	if(P3!=0xff)
	{
		temp=P3;
		switch(temp)
		{
			case r1 : key='7'; second[len++]=key; display(second);  break;//7
			case r2 : key='8'; second[len++]=key; display(second);  break;//8
			case r3 : key='9'; second[len++]=key; display(second);  break;//9
			case r4 : key='/'; symbol=4; len=0; point=0; clear(1); copytofirst(); moveto(1,0); displayup(first); write_data('/'); break;//divide
			case r5 : key=15; break;//%
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
			case r1 : key='4'; second[len++]=key; display(second);  break;//4
			case r2 : key='5'; second[len++]=key; display(second);  break;//5
			case r3 : key='6'; second[len++]=key; display(second);  break;//6
			case r4 : key=19;  symbol=3; len=0; point=0; clear(1); copytofirst(); moveto(1,0); displayup(first); write_data('*');break;//muiltiply
			case r5 : key=20; buff=1; break;//1/x
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
			case r1 : key='1'; second[len++]=key; display(second);  break;//1
			case r2 : key='2'; second[len++]=key; display(second);  break;//2
			case r3 : key='3'; second[len++]=key; display(second);  break;//3
			case r4 : key=24; symbol=2; len=0; point=0; clear(1); copytofirst(); moveto(1,0); displayup(first); write_data('-');break;//minus
			case r5 : key=25; 
			write_com(0x80+strlen(first)+1);
			displayup(second);
			write_data('=');
 		   	sscanf(first,"%f",&aa);
				sscanf(second,"%f",&bb);
				switch(symbol)
				{
					case 1 : bb=aa+bb; break;
					case 2 : bb=aa-bb; break;
					case 3 : bb=aa*bb; break;
					case 4 : bb=aa/bb; break;
				}
				if(bb>=100000||(bb<=0.000001&&bb>=0.000001)||bb<=-100000)
				sprintf(second,"%e\0",bb);
				else sprintf(second,"%g\0",bb);//不显示后边的0
				display(second);
		   buff=0;sign=0;aa=0;bb=0;len=0;point=0;cleardata(1);cleardata(2);
			break;//equal
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
			case r1 : key='0'; second[len++]=key; display(second);  break;//0
			case r2 : key='.'; if(point==0) {second[len++]=key; display(second);  point=1;} break;//point
		 	case r3 : break;//empty
			case r4 : key=29; symbol=1; len=0; point=0;clear(1); copytofirst(); moveto(1,0); displayup(first); write_data('+'); break;//add
			case r5 : key=30; buff=1;
			break;//equal
		}
		while(P3!=0xff)
		{
			temp=P3;
			temp&=0xff;
		}
	}
	P1=0xff;
	P3=0xff;
}