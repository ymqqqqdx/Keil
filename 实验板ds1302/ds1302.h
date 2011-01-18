#include <reg52.h>
#define readyear read_clock(0x8d)
#define readweek read_clock(0x8b)
#define readmonth read_clock(0x89)
#define readday read_clock(0x87)
#define readhour read_clock(0x85)
#define readminute read_clock(0x83)
#define readsecond read_clock(0x81)
#define writeyear write_clock(0x8c,year)
#define writeweek write_clock(0x8a,week)
#define writemonth write_clock(0x88,month)
#define writeday write_clock(0x86,day)
#define writehour write_clock(0x84,hour)
#define writeminute write_clock(0x82,minute)
#define writesecond write_clock(0x80,second)
sbit  DS1302_CLK = P3^7;              //实时时钟时钟线引脚 
sbit  DS1302_IO  = P3^6;              //实时时钟数据线引脚 
sbit  DS1302_RST = P2^5;              //实时时钟复位线引脚
sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;
static xdata unsigned char year;
static xdata unsigned char week;
static xdata unsigned char month;
static xdata unsigned char day;
static xdata unsigned char hour;
static xdata unsigned char minute;
static xdata unsigned char second;
unsigned char bcdconvert(unsigned char bcd)
{
	unsigned char target;
	target=bcd>>4;
	target=target*10+(bcd&=0x0f);
	return(target);
}
unsigned char convertbcd(unsigned char ten)
{
	unsigned char bcd,a,b;
	a=ten/10;
	b=ten%10;
	a=a<<4;
	bcd=(a&=0xf0)+(b&=0x0f);
	return(bcd);
}
void DS1302InputByte(unsigned char d) 	//实时时钟写入一字节(内部函数)
{ 
    unsigned char i;
    ACC = d;
    for(i=8; i>0; i--)
    {
        DS1302_IO = ACC0;           	//相当于汇编中的 RRC
        DS1302_CLK = 1;
        DS1302_CLK = 0;
        ACC = ACC >> 1; 
    } 
}
unsigned char DS1302OutputByte(void) 	//实时时钟读取一字节(内部函数)
{ 
    unsigned char i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         			//相当于汇编中的 RRC 
        ACC7 = DS1302_IO;
        DS1302_CLK = 1;
        DS1302_CLK = 0;
    } 
    return(ACC); 
}
void write_clock(unsigned char ucAddr, unsigned char ucDa)	//ucAddr: DS1302地址, ucData: 要写的数据
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr);       	// 地址，命令 
    DS1302InputByte(convertbcd(ucDa));       	// 写1Byte数据
    DS1302_CLK = 1;
    DS1302_RST = 0;
} 
unsigned char read_clock(unsigned char ucAddr)	//读取DS1302某地址的数据
{
    unsigned char ucData;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr|0x01);        // 地址，命令 
    ucData = DS1302OutputByte();         // 读1Byte数据
    DS1302_CLK = 1;
    DS1302_RST = 0;
    return(bcdconvert(ucData));
}
void init_ds1302()
{
	year=10; writeyear;
	month=3; writemonth;
	day=15; writeday;
	hour=17; writehour;
	minute=41; writeminute;
	second=14; writesecond;
}
