#include <reg52.h>
#define decode_mode 0x09 //解码模式，0xff全部解码，0x0f解前四个
#define intensity 0x0a
#define scan_limit 0x0b//扫描限度，0x00-0x07
#define shut_down 0x0c//关闭模式，0x00为关机,0x01为正常
#define display_test 0x0f//显示模式
sbit load=P2^1;//load
sbit din=P2^0;//data in
sbit clk=P2^2;//clock
//向7219写一个字节
void write7219onechar(unsigned char temp)
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
//向7219写命令、数据
void write7219(unsigned char address,unsigned char dat)
{
	load=0;
	write7219onechar(address);
	write7219onechar(dat);
	load=1;
}
//7219初始化
void init7219()
{
	unsigned char i;
	write7219(shut_down,0x01);
	write7219(display_test,0x00);
	write7219(decode_mode,0x7a);//第1、3、8位不解码
	write7219(scan_limit,0x07);
	write7219(intensity,0x04);
	for(i=0;i<8;i++)
		write7219(i+1,15);
}
