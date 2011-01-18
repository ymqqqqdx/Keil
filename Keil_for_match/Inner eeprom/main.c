#include <reg52.h>
#include <stdio.h>
void Uart_Init()
{
	REN=1;
	SCON=0x52;
	TMOD=0X20;
	TH1=0XFA;
	TL1=0XFA;
	TR1=1;
	ES=1;
	SM0=0;
	SM1=1;
	EA=1;
}
void ISP_enable()
{
	EA=0;
	ISP_CONTER&=0x18;
	ISP_CONTER|=WaitTime;
	ISP_CONTER|=0x80;
}
void ISP_disable()
{
	ISP_CONTER&=0x17;
	ISP_TRIG=0x00;
	EA=1;
}
void ISP_go()
{
	ISP_enable();
	ISP_TRIG=0x5A;
	ISP_TRIG=0xA5;
	_nop_();
}
U8 ISP_read(U16 add)
{
	ISP_ADDRH=(U8)(add>>8);
	ISP_ADDRL=(U8)add;
	ISP_CMD&=0xf8;
	ISP_CMD|=ReadCommand;
	ISP_go();
	ISP_disable();
	return(ISP_DATA);
}
void ISP_sectorerase(U16 sectoradd)
{
	ISP_ADDRH=(U8)((sectoradd&0xfe00)>>8);
	ISP_ADDRL=0;
	ISP_CMD&=0xf8;
	ISP_CMD|=EraseCommand;
	ISP_go();
	ISP_disable();
}
void ISP_write(U16 add,U8 dat)
{
	ISP_ADDRH=(U8)(add>>8);
	ISP_ADDRL=(U8)add;
	ISP_CMD&=0xf8;
	ISP_CMD|=ProgCommand;
	ISP_DATA=dat;
	ISP_go();
	ISP_disable();
}
void delay(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<200;i++);
}
void main()
{
	U8 i;
	U16 j=0x0000;
	Uart_Init();
	puts("OK!");
	delay(200);
//	ISP_sectorerase(0x0000);
//	for(i=0;i<255;i++)
//		ISP_write(0x0000+i,i);
	for(i=0;i<255;i++)
		printf(" %d ",(int)ISP_read(0x0000+i));
	while(1);
}