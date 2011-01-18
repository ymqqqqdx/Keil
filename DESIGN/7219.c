#include <reg52.h>
#define decode_mode 0X09 //解码模式，0xff全部解码，0x0f解前四个
#define intensity 0x0a
#define scan_limit 0x0b//扫描限度，0x00-0x07
#define shut_down 0x0c//关闭模式，0x00为关机,0x01为正常
#define display_test 0x0f//显示模式

sbit load=P2^1;//第一片
sbit din=P2^0;//data in
sbit clk=P2^2;//clock

sbit load1=P2^4;//第二片
sbit din1=P2^3;
sbit clk1=P2^5;

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

void write7219onechar1(unsigned char temp)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		clk1=0;
		din1=(bit)(temp&0x80);
		temp<<=1;
		clk1=1;
	}
}


void write7219one(unsigned char address,unsigned char dat)
{
	load=0;
	write7219onechar0(address);
	write7219onechar0(dat);
	load=1;
}

void write7219two(unsigned char address,unsigned char dat)
{
	load1=0;
	write7219onechar1(address);
	write7219onechar1(dat);
	load1=1;
}

void init7219()
{
	write7219one(shut_down,0x01);
	write7219one(display_test,0x00);
	write7219one(decode_mode,0xff);
	write7219one(scan_limit,0x07);
	write7219one(intensity,0x04);

	write7219two(shut_down,0x01);
	write7219two(display_test,0x00);
	write7219two(decode_mode,0xff);
	write7219two(scan_limit,0x07);
	write7219two(intensity,0x04);
}
