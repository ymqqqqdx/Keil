#include <reg52.h>
#include <stdio.h>
sbit hd_cs=P0^4;
sbit hd_clk=P0^5;
sbit hd_dat=P0^6;
sbit hd_key=P0^7;
sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;
sbit beep=P2^7;
U8 stat=0,counter=0;
U8 password_old[5]={0,1,2,3,4};
U8 password_new[5]=0;
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

bit password_check()
{
	U8 i;
	for(i=0;i<5;i++)
	{
		if(password_new[i]!=password_old[i])
			return 0;
	}
	return 1;
}
void error_handle()
{
	lcd_writecd(0,0x01);
	delay_1ms(5);
	lcd_string(0x80,"´íÎó£¡");	
}
void start()
{
	lcd_writecd(0,0x01);
	delay_1ms(5);
	lcd_string(0x80,"ÇëÊäÈëÃÜÂë£º");
}
void right_handle()
{
	lcd_writecd(0,0x01);
	delay_1ms(5);
	lcd_string(0x80,"welcome!");
}
void keyprocess(U8 key)
{
	U8 i;
	if(key>=0&&key<10)
	{
		if(counter<5)
		{
			password_new[counter]=key;
			counter++;
		}
		lcd_writecd(0,0x90);
		for(i=0;i<counter;i++)
			lcd_writecd(1,'*');
		if(counter==5)
			counter=0;
	}
	else if(key==16)
	{
		if(stat==0)
		{
			if(password_check())
			{
				right_handle();
				stat=1;
			}
			else
			{
				error_handle();
				counter=0;
				beep=0;
				delay_1ms(1000);
				beep=1;
				start();
				stat=0;
			}
		}
		else
		{
			for(i=0;i<5;i++)
				password_old[i]=password_new[i];
			lcd_writecd(0,0x01);
			delay_1ms(5);
			lcd_string(0x80,"ÃÜÂëÐÞ¸Ä³É¹¦£¡");
			delay_1ms(1000);
			start();
			stat=0;
		}
	}
	else if(key==15&&stat==1)
	{
		lcd_writecd(0,0x01);
		delay_1ms(5);
		lcd_string(0x80,"ÊäÈëÐÂÃÜÂë");
	}
	
}
void main()
{
	hd_writeonechar(0xa4);
	lcd_writecd(0,0x0c);
	start();
	while(1)
	{
		if(!hd_key)
		{
			keyprocess(hd_keyconvert(hd_read(0x15)));
		}while(!hd_key);
		delay_1ms(5);
	}
}