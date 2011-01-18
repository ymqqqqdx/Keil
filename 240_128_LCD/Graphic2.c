#include <reg52.h>
#include <math.h>
#include <stdlib.h>
#define uchar unsigned char
#define uint unsigned int
#define heng 1
#define shu 2
#define fill 1
#define clear 2
extern uint address;
extern uchar data1,data2,cmd;
extern uchar headhang,headlie,endhang,endlie;
extern uchar foodx,foody;
extern uchar snakelength;
extern uchar position[20][2];
extern bit addflag;
void writecommand(uchar);
void writedata(uchar);
void writedc(uchar,uchar);
void writeddc(uchar,uchar,uchar);
void writeadd()
{
	data1=address;
	data2=address>>8;
	writeddc(data1,data2,0x24);
}
void drawline(uchar hang,uchar lie,uchar length,uchar way,uchar foc)
{
	uchar i;
	writecommand(0xb2); //退出自动写模式
	address=hang*30+lie/8;
	writeadd();
	if(way==1)
	{
		if(length<(9-lie%8))
		{
			for(i=lie%8;i<length+lie%8;i++)
				writecommand(foc==1?0xf8-i+7:0xf0-i+7);
		}
		else
		{
			for(i=lie%8+1;i<=8;i++)
				writecommand(foc==1?0xf8-i+8:0xf0-i+7);
			address++; writeadd();
			writecommand(0xb0);
			for(i=0;i<(length+lie%8-8)/8;i++)
				writedata(foc==1?0xff:0x00);
			writecommand(0xb2);
			for(i=1;i<=(length+lie%8-8)%8;i++)
				writecommand(foc==1?0xf8-i+8:0xf0-i+8);
		}
	}
	else if(way==2)
	{
		for(i=0;i<length;i++)
		{
			writecommand(foc==1?0xff-lie%8:0xf8-lie%8);
			address+=30;
			writeadd();
		}
	}
}
void drawonedot(uchar hang,uchar lie)
{
	drawline(hang*4,lie*4,4,heng,fill);
	drawline(hang*4+1,lie*4,4,heng,fill);
	drawline(hang*4+2,lie*4,4,heng,fill);
	drawline(hang*4+3,lie*4,4,heng,fill);
}
void clearonedot(uchar hang,uchar lie)
{
	drawline(hang*4,lie*4,4,heng,clear);
	drawline(hang*4+1,lie*4,4,heng,clear);
	drawline(hang*4+2,lie*4,4,heng,clear);
	drawline(hang*4+3,lie*4,4,heng,clear);	
}
void drawsnake(uchar hang,uchar lie)
{
	uchar i;
	for(i=0;i<snakelength;i++)
	{
		drawonedot(hang,lie+i);
	}
}
void movearray()
{
	uchar i;
	for(i=0;i<snakelength-1;i++)
	{
		position[i][0]=position[i+1][0];
		position[i][1]=position[i+1][1];
	}
}
void writetoarray()
{
	position[snakelength-1][0]=headhang;
	position[snakelength-1][1]=headlie;
}
void move(uchar way)
{
	switch(way)
	{
		case 1: --headhang; break;//up
		case 2: ++headhang; break;//down
		case 3: --headlie; break;//left
		case 4: ++headlie; break;//right
	}
	if(headhang!=31&&headhang!=0&&headlie!=0&&headlie!=49)
	drawonedot(headhang,headlie);
	if(!addflag)
	{
		clearonedot(position[0][0],position[0][1]);	
		movearray();
	}
	writetoarray();
	addflag=0;
}
void food()
{
	uchar i=0,j=1;
	while(1)
	{
		foodx=rand()%32-2;
		foody=rand()%50-2;
		for(i=0;i<snakelength;i++)
		{
			if(foodx==position[i][0]&&foody==position[i][1])
			{
				j=1;
				break;
			}
			else j=0;
		}
		if(j==0) break;
	}
	drawonedot(foodx,foody);
}