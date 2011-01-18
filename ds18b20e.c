#include <reg52.h>
sbit DQ =P3^5;   //定义通信端口
unsigned char A1,A2,A3,A2t,dispno;
unsigned int temp1;
void delayus(unsigned int i)
{
	 while(i--);
}
Init_DS18B20(void)
{
	 unsigned char x=0;
	 DQ = 1;    //DQ复位
	 delayus(8);  //稍做延时
	 DQ = 0;    //单片机将DQ拉低
	 delayus(80); //精确延时 大于 480us
	 DQ = 1;    //拉高总线
	 delayus(14);
	 x=DQ;      //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	 delayus(20);
}
ReadOneChar(void)
{
unsigned char i=0;
unsigned char dat = 0;
for (i=8;i>0;i--)
 {
	  DQ = 0; // 给脉冲信号
	  dat>>=1;
	  DQ = 1; // 给脉冲信号
	  if(DQ)
	   dat|=0x80;
	  delayus(4);
 }
 return(dat);
}
//写一个字节
WriteOneChar(unsigned char dat)
{
 unsigned char i=0;
 for (i=8; i>0; i--)
 {
  DQ = 0;
  DQ = dat&0x01;
  delayus(5);
  DQ = 1;
  dat>>=1;
 }
delayus(4);
}
//读取温度
unsigned int readtemp(void)
{
	unsigned char a=0;
	unsigned char b=0;
	unsigned int t=0;
	float tt=0;
	Init_DS18B20();
	WriteOneChar(0xCC); // 跳过读序号列号的操作
	WriteOneChar(0x44); // 启动温度转换
	Init_DS18B20();
	WriteOneChar(0xCC); //跳过读序号列号的操作
	WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	a=ReadOneChar();
	b=ReadOneChar();
	t=b;
	t<<=8;
	t=t|a;
	tt=t*0.0625;
	t= tt*10+0.5; //放大10倍输出并四舍五入---此行没用
	return(t);
}