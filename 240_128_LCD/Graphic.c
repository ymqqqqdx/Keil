#include <reg52.h>
#include <math.h>
#define uchar unsigned char
#define uint unsigned int
extern uint address;
extern uchar data1,data2,cmd;
void writecommand(uchar);
void writedata(uchar);
void writedc(uchar,uchar);
void writeddc(uchar,uchar,uchar);
void DrawLine(uchar hori,uchar veri,uchar length,uchar width,uchar way)
{
	uchar i,j,l,m,n=0;
	uint addbak;
	writecommand(0xb2);
	hori-=1;veri-=1;
	address=hori*30+veri/8;
	addbak=address;
	data1=address;
	data2=address>>8;
	writeddc(data1,data2,0x24);
	if(way==1)
	{
		for(j=0;j<width;j++)
		{
			n=0;
			writecommand(0xb0);
			for(i=0;i<length/8+1;i++)
			{
				l=0;
				if(i==0&&veri%8!=0)
				{
					for(m=0;m<veri%8;m++)
					{
						n+=pow(2,7-m);
					}
					writedata(255-n);
				}
				else if(i==length/8)
				{
					for(m=0;m<length%8;m++)
					{
						l+=pow(2,7-m);
					}
					veri%8!=0?writedata(l|n>>(length%8)):
					writedata(l);
				}
				else writedata(0xff);
			}
			m=0;	
			if(veri%8>=(9-length%8))
			{
				for(i=0;i<(length%8+veri%8)%8;i++)
				{
					m+=pow(2,7-i);
				}
				writedata(m);
			}
			writecommand(0xb2);
			address+=30;
			data1=address;
			data2=address>>8;
			writeddc(data1,data2,0x24);
		}
	}
	else if(way==2)
	{
		
		for(j=0;j<width/8+1;j++)
		{
			for(i=0;i<length;i++)
			{
				n=0;
				if(j==width/8)
				{
					for(m=0;m<width%8;m++)
					{
						n+=pow(2,7-m);
					}
				}
				else n=0xff;
				writecommand(0xb0);
				writedata(n);
				writecommand(0xb2);
				address-=30;
				data1=address;
				data2=address>>8;
				writeddc(data1,data2,0x24);
			}
			addbak+=1;
			address=addbak;
			data1=address;
			data2=address>>8;
			writeddc(data1,data2,0x24);
		}
	}
}