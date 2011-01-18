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
#define U8 unsigned char
#define U16 unsigned int
#define CMD 0
#define DATA 1
sbit LCD_CS=P0^3;
sbit LCD_SCLK=P1^2;
sbit LCD_SID=P0^2;
sbit  DS1302_CLK = P1^6;              //????????? 
sbit  DS1302_IO  = P1^7;              //????????? 
sbit  DS1302_RST = P3^0;              //?????????
sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;
static xdata unsigned char year;
static xdata unsigned char week;
static xdata unsigned char month;
static xdata unsigned char day;
static xdata unsigned char hour;
static xdata unsigned char minute;
static xdata unsigned char second;
void Delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}
void LCD_Send_Byte(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		LCD_SID=(bit)(dat&(1<<(7-i)));
		LCD_SCLK=0;
		LCD_SCLK=1;
	}
}
void LCD_Send_CD(bit CD,U8 dat)
{
	U8 temp;
	if(!CD) temp=0xf8;
		else temp=0xfa;
	LCD_CS=1;
	LCD_SCLK=0;
	LCD_Send_Byte(temp);
	temp=dat;
	temp&=0xf0;
	LCD_Send_Byte(temp);
	temp=dat;
	temp<<=4;
	LCD_Send_Byte(temp);
	LCD_CS=0;
	Delay_1ms(10);
}
void LCD_Init()
{
	Delay_1ms(100);
	LCD_Send_CD(CMD,0x30);
	LCD_Send_CD(CMD,0x02);
	LCD_Send_CD(CMD,0x06);
	LCD_Send_CD(CMD,0x0c);
	LCD_Send_CD(CMD,0x01);
	LCD_Send_CD(CMD,0x80);
}

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
void DS1302InputByte(unsigned char d) 	//?????????(????)
{ 
    unsigned char i;
    ACC = d;
    for(i=8; i>0; i--)
    {
        DS1302_IO = ACC0;           	//??????? RRC
        DS1302_CLK = 1;
        DS1302_CLK = 0;
        ACC = ACC >> 1; 
    } 
}
unsigned char DS1302OutputByte(void) 	//?????????(????)
{ 
    unsigned char i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         			//??????? RRC 
        ACC7 = DS1302_IO;
        DS1302_CLK = 1;
        DS1302_CLK = 0;
    } 
    return(ACC); 
}
void write_clock(unsigned char ucAddr, unsigned char ucDa)	//ucAddr: DS1302??, ucData: ?????
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr);       	// ??,?? 
    DS1302InputByte(convertbcd(ucDa));       	// ?1Byte??
    DS1302_CLK = 1;
    DS1302_RST = 0;
} 
unsigned char read_clock(unsigned char ucAddr)	//??DS1302??????
{
    unsigned char ucData;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr|0x01);        // ??,?? 
    ucData = DS1302OutputByte();         // ?1Byte??
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
void main()
{
	unsigned char second,secondold;
	init_ds1302();
	LCD_Init();
	while(1)
	{
		second=readsecond;
		LCD_Send_CD(CMD,0x80);
		LCD_Send_CD(DATA,second/10+'0');
		LCD_Send_CD(DATA,second%10+'0');
		Delay_1ms(1000);
	}
}