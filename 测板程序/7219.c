#include <reg52.h>
#define decode_mode 0X09 //解码模式，0xff全部解码，0x0f解前四个
#define intensity 0x0a
#define scan_limit 0x0b//扫描限度，0x00-0x07
#define shut_down 0x0c//关闭模式，0x00为关机,0x01为正常
#define display_test 0x0f//显示模式

sbit load=P2^5;
sbit din=P2^4;//data in
sbit clk=P2^6;//clock

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
	if(address>=1&&address<=8)
	{
		write7219onechar0(address);
		write7219onechar0(dat);
		write7219onechar0(0);
		write7219onechar0(15);
	}
	else if(address>=9&&address<=16)
	{
		write7219onechar0(0);
		write7219onechar0(15);
		write7219onechar0(address-8);
		write7219onechar0(dat);
	}
	else
	{
		write7219onechar0(address);
		write7219onechar0(dat);
	}
	load=1;
}
void write7219com(unsigned char add1,unsigned char dat1,unsigned char add2,unsigned char dat2)
{
	load=0;
	write7219onechar0(add1);
	write7219onechar0(dat1);
	write7219onechar0(add2);
	write7219onechar0(dat2);
	load=1;
}

void init7219()
{
	write7219com(shut_down,0x01,shut_down,0x01);
	write7219com(display_test,0x00,display_test,0x00);
	write7219com(decode_mode,0xff,decode_mode,0xff);
	write7219com(scan_limit,0x07,scan_limit,0x07);
	write7219com(intensity,0x04,intensity,0x04);

}
