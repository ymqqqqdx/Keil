#include <reg52.h>
#define decode_mode 0X09 //解码模式，0xff全部解码，0x0f解前四个
#define intensity 0x0a
#define scan_limit 0x0b//扫描限度，0x00-0x07
#define shut_down 0x0c//关闭模式，0x00为关机,0x01为正常
#define display_test 0x0f//显示模式

sbit load=P2^1;//第1-3片
sbit din=P2^0;//data in
sbit clk=P2^2;//clock

sbit load1=P2^5;//第4-6片
sbit din1=P2^4;
sbit clk1=P2^6;

sbit load2=P1^3;//第7片
sbit din2=P1^2;
sbit clk2=P1^4;
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

void write7219onechar2(unsigned char temp)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		clk2=0;
		din2=(bit)(temp&0x80);
		temp<<=1;
		clk2=1;
	}
}

void write7219one(unsigned char address1,unsigned char dat1,unsigned char address2,unsigned char dat2,unsigned char address3,unsigned char dat3)
{
	load=0;
	write7219onechar0(address1);
	write7219onechar0(dat1);
	write7219onechar0(address2);
	write7219onechar0(dat2);
	write7219onechar0(address3);
	write7219onechar0(dat3);
	load=1;
}

void write7219two(unsigned char address1,unsigned char dat1,unsigned char address2,unsigned char dat2,unsigned char address3,unsigned char dat3)
{
	load1=0;
	write7219onechar1(address1);
	write7219onechar1(dat1);
	write7219onechar1(address2);
	write7219onechar1(dat2);
	write7219onechar1(address3);
	write7219onechar1(dat3);
	load1=1;
}

void write7219three(unsigned char address,unsigned char dat)
{
	load2=0;
	write7219onechar2(address);
	write7219onechar2(dat);
	load2=1;
}

void init7219()
{
	write7219one(shut_down,0x01,shut_down,0x01,shut_down,0x01);
	write7219one(display_test,0x00,display_test,0x00,display_test,0x00);
	write7219one(decode_mode,0x00,decode_mode,0x00,decode_mode,0x00);
	write7219one(scan_limit,0x07,scan_limit,0x07,scan_limit,0x07);
	write7219one(intensity,0x04,intensity,0x04,intensity,0x04);

	write7219two(shut_down,0x01,shut_down,0x01,shut_down,0x01);
	write7219two(display_test,0x00,display_test,0x00,display_test,0x00);
	write7219two(decode_mode,0x00,decode_mode,0x00,decode_mode,0x00);
	write7219two(scan_limit,0x07,scan_limit,0x07,scan_limit,0x07);
	write7219two(intensity,0x04,intensity,0x04,intensity,0x04);

	write7219three(shut_down,0x01);
	write7219three(display_test,0x00);
	write7219three(decode_mode,0xff);
	write7219three(scan_limit,0x07);
	write7219three(intensity,0x04);
}
