#include <reg52.h>
#include <stdio.h>
#include <string.h>
sbit hd_cs=P0^4;
sbit hd_clk=P0^5;
sbit hd_dat=P0^6;
sbit hd_key=P0^7;
sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;
U8 stat=0,symbol=0,offset=0,counter=0;
bit buff=0;
int a=0,b=0;
float result;
char buf[8]=0;
void hd_longdelay()
{
	U8 i;
	for(i=0;i<50;i++);
}

void hd_shortdelay()
{
	U8 i;
	for(i=0;i<8;i++);
}
void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void hd_writeonechar(U8 dat)
{
	U8 i;
	hd_cs=0;
	hd_dat=0;
	hd_longdelay();
	for(i=0;i<8;i++)
	{
		dat<<=1;
		hd_dat=CY;
		hd_clk=1; hd_shortdelay();
		hd_clk=0; hd_shortdelay();
	}
	hd_dat=0;
}

U8 hd_readonechar()
{
	U8 i,temp=0;
	hd_dat=1;
	hd_longdelay();
	for(i=0;i<8;i++)
	{
		hd_clk=1; hd_shortdelay();
		temp|=((U8)hd_dat)<<(7-i);
		hd_clk=0; hd_shortdelay();
	}
	hd_dat=0;
	return temp;
}

void hd_write(U8 add,U8 dat)
{
	hd_writeonechar(add);
	hd_writeonechar(dat);
}

U8 hd_read(U8 add)
{
	hd_writeonechar(add);
	return(hd_readonechar());
}

U8 hd_keyconvert(U8 dat)
{
	switch(dat)
	{
		case 6 : dat=1; break;
		case 5 : dat=2; break;
		case 4 : dat=3; break;
		case 3 : dat=13; break;
		case 14 : dat=4; break;
		case 13 : dat=5; break;
		case 12 : dat=6; break;
		case 11 : dat=14; break;
		case 22 : dat=7; break;
		case 21 : dat=8; break;
		case 20 : dat=9; break;
		case 19 : dat=15; break;
		case 30 : dat=11; break;
		case 29 : dat=0; break;
		case 28 : dat=12; break;
		case 27 : dat=16; break;
	}
	return dat;
}
void lcd_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		lcd_sid=CY;
		lcd_clk=0;
		lcd_clk=1;
	}
}

void lcd_writecd(bit cd,U8 dat)
{
	lcd_cs=1;
	lcd_writeonechar(cd==0?0xf8:0xfa);
	lcd_writeonechar(dat&0xf0);
	lcd_writeonechar(dat<<4);
	lcd_cs=0;
}

void lcd_string(U8 add,U8 *pt)
{
	lcd_writecd(0,add);
	while(*pt!='\0')
		lcd_writecd(1,*pt++);
}

void key_process(U8 key)
{
	U8 i;
	if(key>=0&&key<10)
	{
		buf[counter]=key+'0';
		counter++;
		lcd_string(0x90,"                 ");
		lcd_string(0x96-(strlen(buf)-1)/2,buf);
		if(buff==0)
			offset=strlen(buf)/2;
	}
	else if(key>=13&&key<=16&stat==0)
	{
		lcd_string(0x80,"                ");
		lcd_string(0x80,buf);
		sscanf(buf,"%d",&a);
		for(i=0;i<8;i++)
			buf[i]=0;
		switch(key-12)
		{
			case 1: lcd_writecd(1,'+'); break;
			case 2: lcd_writecd(1,'-'); break;
			case 3: lcd_writecd(1,'*'); break;
			case 4: lcd_writecd(1,'/'); break;
		}
		symbol=key-12;
		stat=1;
		counter=0;
		buff=1;
	}
	else if(key==12&&stat==1)
	{
		lcd_string(0x80+offset+1,buf);
		lcd_writecd(1,'=');
		sscanf(buf,"%d",&b);
		for(i=0;i<8;i++)
			buf[i]=0;
		lcd_string(0x90,"                   ");
		switch(symbol)
		{
			case 1: result=a+b; break;
			case 2: result=a-b; break;
			case 3: result=a*b; break;
			case 4: result=a/b; break;
		}
		sprintf(buf,"%.0f",result);
		lcd_string((0x96-(strlen(buf)-1)/2),buf);
		a=0;b=0;
		for(i=0;i<8;i++)
			buf[i]=0;
		stat=0;
		offset=0;
		counter=0;
		buff=0;
	}
}

void main()
{
	hd_writeonechar(0xa4);
	lcd_writecd(0,0x0c);
	lcd_writecd(0,0x01);
	delay_1ms(5);
	lcd_string(0x96,"0.");
	while(1)
	{
		if(!hd_key)
		{
			key_process(hd_keyconvert(hd_read(0x15)));
		}while(!hd_key);
		delay_1ms(5);
	}	
}