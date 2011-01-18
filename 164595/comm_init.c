#include <reg52.h>
void comm_init()
{	//REN=1; //串行允许
	SCON=0x50;
	TMOD=0X20;//设置定时器1工作方式
	PCON=0x00;
	TH1=0XF3;//装初值
	TL1=0XF3;//装初值
	EA=1;
	EX0=1; IT0=1;
	ES=0;
	IP=0X10;
	TR1=1;//打开定时器1
	SM0=0;
	SM1=1;
	REN=1;
}