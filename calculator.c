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
unsigned char i,buff=0,sign=0,point=0,symbol=0,cai=0;
char first[16],second[16];
char len;
float aa,bb,mm;
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
void displayup(unsigned char *temp)
{
	unsigned char n;
	n=strlen(temp);
	for(i=0;i<n;i++)
	write_data(temp[i]);
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
			case r2 : key=2; if(buff==0) {aa=mm; } if(buff==1) {bb=mm; } break;//MR
			case r3 : key=3; if(buff==0) mm=aa; if(buff==1) mm=bb; break;//MS
			case r4 : key=4; if(buff==0) {aa=aa+mm; } if(buff==1) {bb=bb+mm; } break;//M+
			case r5 : key=5; if(buff==0) {aa=aa-mm; } if(buff==1) {bb=bb-mm; } break;//M-
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
			case r1 : key=6; if(buff==0) {first[--len]='\0'; display(first); if(len<0) len=0;}
		 						  if(buff==1) {second[--len]='\0'; display(second); if(len<0) len=0;} break;//back
			case r2 : key=7; if(buff==0) {aa=0; for(i=0;i<15;i++) first[i]='\0';display(first);} 
									if(buff==1) {bb=0; for(i=0;i<15;i++) second[i]='\0';display(second);} len=0; break;//ce
			case r3 : key=8; for(i=0;i<15;i++){first[i]='\0';second[i]='\0';} buff=0;len=0;sign=0;display(first); break;//reset
			case r4 : key=9; if(buff==0)
		 							{
										if(len>0)
										{
											if(sign==0&&len<15)
											{
												len++;
												for(i=len;i>0;i--)
												{
												 first[i]=first[i-1];
												}
												first[0]='-';
												sign=1;												
											}
											else if(sign==1)
											{
												for(i=0;i<len;i++)
												{
												 	first[i]=first[i+1];
												}
												sign=0;
												len--;
											}
										}	display(first);
									} 
									if(buff==1)
		 							{
										if(len>0)
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
										}	display(second);
									} break;//pORm
		case r5 : key=10; clear(1); moveto(1,0); displayup("sqrt()="); 
		 	if(buff==0){sscanf(first,"%f",&aa);aa=sqrt(aa);sprintf(first,"%f\0",aa); display(first);}
			if(buff==1){sscanf(second,"%f",&bb);bb=sqrt(bb);sprintf(second,"%f\0",bb); display(second);}
		break;//genhao
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
			case r1 : key='7'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//7
			case r2 : key='8'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//8
			case r3 : key='9'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//9
			case r4 : key='/'; buff=1; symbol=4; len=0; point=0; clear(1); moveto(1,0); displayup(first); write_data('/'); break;//divide
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
			case r1 : key='4'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//4
			case r2 : key='5'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//5
			case r3 : key='6'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//6
			case r4 : key=19; buff=1; symbol=3; len=0; point=0; clear(1); moveto(1,0); displayup(first); write_data('*');break;//muiltiply
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
			case r1 : key='1'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//1
			case r2 : key='2'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//2
			case r3 : key='3'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//3
			case r4 : key=24; buff=1; symbol=2; len=0; point=0; clear(1); moveto(1,0); displayup(first); write_data('-');break;//minus
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
				if(bb>=100000||bb<=0.000001)
				sprintf(second,"%e\0",bb);
				else sprintf(second,"%f",bb);
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
			case r1 : key='0'; if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } break;//0
			case r2 : key='.'; if(point==0) { if(buff==0) {first[len++]=key; display(first); } if(buff==1) {second[len++]=key; display(second); } point=1; }break;//point
		 	case r3 : cai++; if(cai==3)
									{
										write_com(0x01);
										moveto(1,0); displayup("Why did you pres");
										moveto(2,0); displayup("s me 3 time?");
									}
									if(cai==4)
									{
										write_com(0x01);
										moveto(1,0); displayup("This is a");
										moveto(2,0); displayup("Calculator!");
									}
									if(cai==5)
									{
										write_com(0x01);
										moveto(1,0); displayup("But I found");
										moveto(2,0); displayup("The 51MCU's weak");
									}
									if(cai==6)
									{
										write_com(0x01);
										moveto(1,0); displayup("It is unable");
										moveto(2,0); displayup("flexible to 7.");
									}
									if(cai==7)
									{
										write_com(0x01);
										moveto(1,0); displayup("I'm thinking to");
										moveto(2,0); displayup("virtual hand");
									}
									if(cai==8)
									{
										write_com(0x01);
										moveto(1,0); displayup("to advance it!");
										moveto(2,0); displayup("   BYE!BYE!");
									}
									if(cai==9)
									{
										write_com(0x01);
										display("0.");
									}
			break;//empty
			case r4 : key=29; buff=1; symbol=1; len=0; point=0;clear(1); moveto(1,0); displayup(first); write_data('+'); break;//add
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
void main()
{
	init_1602();
	display("0.");
	while(1)
	{
		keyscan();
	}
}

