#include <reg52.h>
#include "MyFun.h"
#include <stdio.h>

#define U8 unsigned char
#define U16 unsigned int
#define U32 unsigned long

U8 BUF[504]=0; //写入缓冲区
U8 HBUF[480]=0;//手写缓冲区
U8 data DBUF[3]=0;//显示缓冲区
U8 CBUF[3]=0;//命令缓冲区
U16 count=0;//输入计数
U8 DataSize=0,mode=0;//汉字量，运行状态
U8 Page=0;//手写页数
U16 Total=0;//总数据量
U16 DUTotal=0;
U16 DUENDP=0;
U16 DUENDL=0;
U8 Speed=11;//流动速度
U8 DisplayMode=0;
U16 PublicTemp=0;
U32 Ltemp,L2temp;
sbit clk=P3^3;
sbit dat=P3^2;

void delay(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<20;i++);
}

void Uart_init()
{
	SCON=0x52;
	TMOD=0x21;
//	PCON=0x80;//波特率加倍
	TH1=TL1=0xfa;
	TR1=1;
	TI=0;
	RI=0;
	ES=1;
	EA=1;
	IP=0X10;
	REN=1;
}

void Write2Flash(U16 add)
{
	U16 i,j=0;
	ISP_sectorerase(add);
	for(i=0;i<504;i++)
		ISP_write(add+i,BUF[i]);
}
void Hand_Write_Display()
{
	U8 i;
	for(i=0;i<160;i++)
	{

		if(i)
			dat=0;
		else
			dat=1;
		clk=0;
		clk=1;
		P0=HBUF[3*i];
		P1=HBUF[3*i+1];
		P2=HBUF[3*i+2];
		delay(3);
		P0=P1=P2=0x00;
		if(mode!=3 && mode!=4)
			break;
	}
}
void RIGHT_TO_LEFT_Display()
{
	U16 j;
	U8 k,l;
	U16 temp;
	for(j=0;Total<=480?j<Total:j<Total-160;j++)
	{
		for(k=0;k<Speed;k++)
		{
			for(l=0;l<160;l++)
			{
				temp=((3*(j+l))/504)*512 + (3*(j+l))%504 + 0x0200;
				DBUF[0]=ISP_read(temp);
				DBUF[1]=ISP_read(temp+1);
				DBUF[2]=ISP_read(temp+2);
				if(l)
					dat=0;
				else
					dat=1;
				clk=0;
				clk=1;
				P0=DBUF[0];
				P1=DBUF[1];
				P2=DBUF[2];
				delay(3);
				P0=P1=P2=0x00;
				if(mode || DisplayMode)	break;
			}
			if(mode || DisplayMode)	break;
		}
		if(mode || DisplayMode)	break;
	}
}
void DOWN_TO_UP_Display()
{
	U8 k,l;
	U16 j,temp;
	for(j=0;j<DUTotal+24;j++)
	{
		for(k=0;((j+1)%24==0)?k<Speed*10:k<Speed;k++)
		{
			for(l=0;l<160;l++)
			{
				if(j<24)
					Ltemp=0;
				else if(j>=DUENDP+24 && l>DUENDL)
					L2temp=0;
				else
				{
					temp=0x0200 + l*3 + (j/24-1)*512;
					DBUF[0]=ISP_read(temp);
					DBUF[1]=ISP_read(temp+1);
					DBUF[2]=ISP_read(temp+2);
					Ltemp=DBUF[0];
					Ltemp<<=8;
					Ltemp|=DBUF[1];
					Ltemp<<=8;
					Ltemp|=DBUF[2];
				}
				if(j>DUTotal)
					L2temp=0;
				else if(j>DUENDP && l>DUENDL)
					L2temp=0;
				else
				{
					temp=0x0200 + l*3 + (j/24)*512;
					DBUF[0]=ISP_read(temp);
					DBUF[1]=ISP_read(temp+1);
					DBUF[2]=ISP_read(temp+2);
					L2temp=DBUF[0];
					L2temp<<=8;
					L2temp|=DBUF[1];
					L2temp<<=8;
					L2temp|=DBUF[2];
				}
				if(l)
					dat=0;
				else
					dat=1;
				clk=0;
				clk=1;
				P0=P1=P2=0x00;	
				Ltemp<<=j%24;
				L2temp>>=(23-j%24);
				Ltemp|=L2temp;
				DBUF[2]=(U8)(Ltemp&0xff);
				Ltemp>>=8;
				DBUF[1]=(U8)(Ltemp&0xff);
				Ltemp>>=8;
				DBUF[0]=(U8)(Ltemp&0xff);
				P0=DBUF[0];
				P1=DBUF[1];
				P2=DBUF[2];
				if(j<24)
					delay(1);
				if(mode || DisplayMode!=1)	break;
			}
			if(mode || DisplayMode!=1)	break;
		}
		if(mode || DisplayMode!=1)	break;
	}
}


void main()
{
	U16 i;
	for(i=0;i<480;i++)
		HBUF[i]=0;
	for(i=0;i<504;i++)
		BUF[i]=0;
	Total=ISP_read(0x0000);
	DUENDP=(Total/7)*24;
	DUENDL=(Total%7)*24;
	DUTotal=(Total/7+1)*24;
	Total*=24;
	Uart_init();

	while(1)
	{
		switch(mode)
		{
			case 0 : 
					switch(DisplayMode)
					{
						case 0 : RIGHT_TO_LEFT_Display(); break;
						case 1 : DOWN_TO_UP_Display(); break; 
					}					
					break;
			case 3 : 
			case 4 : Hand_Write_Display();  break;
		}
	}
}
bit CMDCMP(U8 *pt)
{
	U8 i;
	for(i=0;i<3;i++)
	{
		if(*pt++!=CBUF[i])
			return 0;
	}
	return 1;
}
void Clear_CBUF()
{
	U8 i;
	for(i=0;i<3;i++)
		CBUF[i]=0;
}
void Ser_Handle() interrupt 4
{
	RI=0;
	if(mode==0)
	{
		if(SBUF>=1 && SBUF<=15)
		{
			Speed=SBUF;
			Speed=31-SBUF*2;
		}
		else
		{
			CBUF[0]=CBUF[1];
			CBUF[1]=CBUF[2];
			CBUF[2]=SBUF;
		}
		if(CMDCMP("STA"))
		{
			Clear_CBUF();
			mode=1;
			count=0;
		}
		else if(CMDCMP("HAN"))
		{
			Clear_CBUF();
			mode=3;
			count=0;
		}
		else if(CMDCMP("DUP"))
		{
			Clear_CBUF();
			DisplayMode=1;
		}
		else if(CMDCMP("RTL"))
		{
			Clear_CBUF();
			DisplayMode=0;			
		}
	}
	else if(mode==1)
	{
		DataSize=SBUF;
		if(DataSize!=0)
		{
			ISP_sectorerase(0x0000);
			ISP_write(0x0000,DataSize);
			Total=DataSize*24;
			DUENDP=(DataSize/7)*24;
			DUENDL=(DataSize%7)*24;
			DUTotal=(DataSize/7+1)*24;
			mode=2;
		}
	}
	else if(mode==2)
	{
		BUF[count%504]=SBUF;
		count++;
		if(count%72==0)
		{	
			if(count%504==0)
			{
				Write2Flash(0x0200+(count/504-1)*512);
			}
			if(count==(DataSize*72))
			{
				Write2Flash(0x0200+(count/504)*512);	
				count=0;
				mode=0;
			}
		}
	}
	else if(mode==3)
	{
		Page=SBUF;
		if(Page=='Q')
			mode=0;
		else if(Page=='C')
		{
			for(PublicTemp=0;PublicTemp<480;PublicTemp++)
				HBUF[PublicTemp]=0;
		}
		else if(Page>=0 && Page<=6)
			mode=4;
	}
	else if(mode==4)
	{
		if(Page==6)
		{
			if(count>=48)
			{
				count++;
				if(count==72)
				{
					count=0;
					mode=3;				
				}
			}
			else
				HBUF[432+ count++]=SBUF;
		}
		else
		{
			HBUF[Page*72+count++]=SBUF;
			if(count==72)
			{
				count=0;
				mode=3;
			}
		}
	}
}
