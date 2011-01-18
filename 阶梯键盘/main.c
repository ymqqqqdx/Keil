#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit i1=P0^0;
sbit i2=P0^1;
sbit i3=P0^2;
sbit i4=P0^3;
sbit wei1=P2^0;
sbit wei2=P2^1;
uchar keyvalue=0,keyvalueold=0;
uchar key;
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
void delay(int z)
{
	uchar i,j;
	for(i=z;i>0;i--)
		for(j=110;j>0;j--);
}
uchar keyscan()
{
	P0=0xfe;
	if(i2==0)
	{
		i1=1;
		if(i2==0) keyvalue=14;
		else keyvalue=1;
		while(i2==0);
		goto end;
	}
	if(i3==0)
	{
		i1=1;
		if(i3==0) keyvalue=15;
		else keyvalue=2;
		while(i3==0);
		goto end;
	}
	if(i4==0)
	{
		i1=1;
		if(i4==0) keyvalue=16;
		else keyvalue=3;
		while(i3==0);
		goto end;
	}
	P0=0xfd;
	if(i3==0)
	{
		i2=1;
		if(i3==0) keyvalue=15;
		else keyvalue=4;
		while(i3==0);
		goto end;
	}
	if(i4==0)
	{
		i2=1;
		if(i4==0) keyvalue=16;
		else keyvalue=5;
		while(i3==0);
		goto end;
	}
	P0=0xfb;
	if(i4==0)
	{
		i3=1;
		if(i4==0) keyvalue=16;
		else keyvalue=6;
		while(i4==0);
		goto end;
	}
	P0=0xf7;
	if(i1==0)
	{
		i4=1;
		if(i1==0) keyvalue=13;
		else keyvalue=7;
		while(i1==0);
		goto end;
	}
	if(i2==0)
	{
		i4=1;
		if(i2==0) keyvalue=14;
		else keyvalue=8;
		while(i2==0);
		goto end;
	}
	if(i3==0)
	{
		i4=1;
		if(i3==0) keyvalue=15;
		else keyvalue=9;
		while(i3==0);
		goto end;
	}
	P0=0xfb;
	if(i1==0)
	{
		i3=1;
		if(i1==0) keyvalue=13;
		else keyvalue=10;
		while(i1==0);
		goto end;
	}
	if(i2==0)
	{
		i3=1;
		if(i2==0) keyvalue=13;
		else keyvalue=11;
		while(i2==0);
		goto end;
	}
	P0=0xfd;
	if(i1==0)
	{
		i2=1;
		if(i1==0) keyvalue=13;
		else keyvalue=12;
		while(i1==0);
		goto end;
	}
	end: 
	if(keyvalue!=keyvalueold)
	{
		return keyvalue;
		keyvalueold=keyvalue;
	}
}
void main()
{
	while(1)
	{
		key=keyscan();
		wei1=0; wei2=1;
		P1=table[key/10];
		delay(5);
		P1=0x00;
		
		wei1=1; wei2=0;
		P1=table[key%10];
		delay(5);
		P1=0x00;
	}
}