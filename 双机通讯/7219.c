#include <reg52.h>
#define decode_mode 0X09 //解码模式，0xff全部解码，0x0f解前四个
#define intensity 0x0a
#define scan_limit 0x0b//扫描限度，0x00-0x07
#define shut_down 0x0c//关闭模式，0x00为关机,0x01为正常
#define display_test 0x0f//显示模式

sbit load=P1^1;//第一片
sbit din=P1^0;//data in
sbit clk=P1^2;//clock


void write7219onechar0(unsigned char temp)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		clk=0;
		din=(bit)(temp&0x80);
		temp<<=1;
		clk=1;
	}
}


void write7219one(unsigned char address,unsigned char dat)
{
	load=0;
	write7219onechar0(address);
	write7219onechar0(dat);
	load=1;
}

void init7219()
{
	write7219one(shut_down,0x01);
	write7219one(display_test,0x00);
	write7219one(decode_mode,0xff);
	write7219one(scan_limit,0x07);
	write7219one(intensity,0x04);
}
