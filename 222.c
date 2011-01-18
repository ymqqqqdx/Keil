#include<reg52.h>
#include<intrins.h>
#define uint unsigned int;
#define uchar unsigned char;
#define ulong	unsigned long int;
sbit k1=P1^0;
sbit k2=P1^1;
sbit k3=P1^2;
sbit k4=P1^3;
sbit ln=P2^0;
sbit ls=P2^1;
sbit d=P2^6;
sbit w=P2^7;
ulong  add,dat[20]=0;
uchar t,aa,max,min1,min0,sec1,sec0,one;
uchar code table[]="Number ";
uchar code table1[]="0123456789";
uchar code table2[]=":.";
void delay(int z)
{
	int i,j;
   for(i=z;i>0;i--)
   for(j=110;j>0;j--);
}
void write_com(uchar com)
{
	ls=0;
   P0=com;
   delay(5);
   ln=1;
   delay(5);
   ln=0;
}
void write_data(uchar date)
{
	ls=1;
   P0=date;
   delay(5);
   ln=1;
   delay(5);
   ln=0;
}