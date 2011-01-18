#include <reg52.h>
#include <codesnake.h>
#define uchar unsigned char
#define uint unsigned int
#define heng 1
#define shu 2
#define fill 1
#define clear 2
sbit key1=P1^0;
sbit key2=P1^1;
sbit key3=P1^2;
sbit key4=P1^3;
uchar headhang,headlie,endhang,endlie;
uchar foodx,foody;
uchar snakelength=6;
uchar counter=0;
uchar direct=4;
bit addflag;
uchar position[30][2]={{3,4},{3,5},{3,6},{3,7},{3,8},{3,9}};
void LCD_Init();
void clearlcd();
void delay(int);
void drawline(uchar,uchar,uchar,uchar,uchar);
void writeddc(uchar,uchar,uchar);
void writecommand(uchar);
void drawsnake(uchar,uchar);
void drawonedot(uchar,uchar);
void clearonedot(uchar,uchar);
void move(uchar);
void food();
void writedata(uchar);
void init()
{
	EA=1;
	TMOD=0x01;
	TH0=(65536-5000)/256;
	TL0=(65536-5000)%256;
	ET0=1;
	TR0=1;
}
void keyscan()
{
	if(key1==0&&direct!=2)
	{
		move(1); direct=1; while(key1==0);
	}
	else if(key2==0&&direct!=1)
	{
		move(2); direct=2; while(key2==0);
	}
	else if(key3==0&&direct!=4)
	{
		move(3); direct=3; while(key3==0);
	}
	else if(key4==0&&direct!=3)
	{
		move(4); direct=4; while(key4==0);
	}
}
void refreshscreen()
{
	uchar i;
	for(i=0;i<64;i++)
	{
		drawline(i,0,240,heng,fill);
		drawline(127-i,0,240,heng,fill);
		delay(10);
	}
	for(i=0;i<64;i++)
	{
		drawline(64-i,0,239,heng,clear);
		drawline(63+i,0,239,heng,clear);
		delay(10);
	}
}
void main()
{
	
	LCD_Init();
	clearlcd();
	drawline(0,0,200,heng,fill);
	drawline(2,2,196,heng,fill);
	drawline(127,0,200,heng,fill);
	drawline(125,2,196,heng,fill);
	drawline(0,0,128,shu,fill);
	drawline(2,2,124,shu,fill);
	drawline(0,199,128,shu,fill);
	drawline(2,197,124,shu,fill);
	drawsnake(3,4);
	endhang=3; endlie=4; headhang=3; headlie=9;
	food();
	init();
	while(1)
	{
		keyscan();
		delay(4);
	}
}
void gameover()
{
	uchar i,j;
	if(headhang==31||headhang==0||headlie==0||headlie==49)
	{
		for(j=0;j<2;j++)
		{
			for(i=0;i<snakelength-1;i++)
				clearonedot(position[i][0],position[i][1]);
			delay(500);
			for(i=0;i<snakelength-1;i++)
				drawonedot(position[i][0],position[i][1]);
			delay(500);
			TR0=0;				
		}
	}
	clearlcd();
	writeddc(0x00,0x00,0x24);
	writecommand(0xb0);
	for(i=0;i<128;i++)
	{
		for(j=0;j<30;j++)
		{
			writedata(over[i*30+j]);
		}
	}
	writecommand(0xb2);
//	refreshscreen();
}
void timer0() interrupt 1
{
	TH0=(65536-5000)/256;
	TL0=(65536-5000)%256;
	counter++;

	if(counter==20)
	{
		counter=0;
			move(direct);
				addflag=0;
		if(foodx==headhang&&foody==headlie)
		{
			snakelength++;
			addflag=1;
			food();
		}
		if(headhang==31||headhang==0||headlie==0||headlie==49)
			gameover();
	}
}