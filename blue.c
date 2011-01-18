#include <stdio.h>
#include <reg52.h>
void comm_init()
{
	REN=1; //串行允许
	SCON=0x52;
	TMOD=0X21;//设置定时器1工作方式
	TH1=0XFF;//装初值
	TL1=0XFF;//装初值
	TR1=1;//打开定时器1
	SM0=0; //设置方式1
	SM1=1;
	EA=1;
}
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void main()
{
	unsigned char i;
	i='x';
	comm_init();
	while(1)
	{
		puts("23332");
		delay(1000);
	}
}