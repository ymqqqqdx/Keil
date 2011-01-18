#include <reg52.h>
void comm_init()
{
	REN=1; //串行允许
	SCON=0x52;
	TMOD=0X21;//设置定时器1工作方式
	TH1=0XF3;//装初值
	TL1=0XF3;//装初值
	TR1=1;//打开定时器1
	SM0=0; //设置方式1
	SM1=1;
	EA=1;
}