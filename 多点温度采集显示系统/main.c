#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
bit symbol;//负号标志位
void init7219();
void write7219(uchar,uchar);
void init_ds18b20();
void writeonechar(uchar);
uchar readonechar();
uint readtemp(uchar);
//延时函数
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
//显示函数
void display(uchar num,uint temp)
{
	uchar shi,ge,one;
	shi=temp/100;
	ge=temp%100/10;
	one=temp%10;
	write7219(1,0x4e);
	write7219(2,num);
	write7219(3,0x78);
	if(symbol)	//写负号
		write7219(4,10);
	else
		write7219(4,15);
	if(shi)		//十位为零不显示
		write7219(5,shi);
	else
		write7219(5,15);
	write7219(6,ge|0x80);
	write7219(7,one);
	write7219(8,0x63);
}
void main()
{
	uchar i;
	init7219();
	for(i=1;i<9;i++)//显示之前读18b20一遍，防止第一次读出错
		readtemp(i);
	while(1)
	{
		for(i=1;i<9;i++)
		{
			display(i,readtemp(i-1));
			delay(1000);
		}
	}
}