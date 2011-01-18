#include <reg52.h>
#include <declare.h>
sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;
extern RTC time;
extern U8 rtc_secondold;
void lcd_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		lcd_sid=CY;
		lcd_clk=0;
		lcd_clk=1;
	}
}
void lcd_writecd(bit cd,U8 dat)
{
	lcd_cs=1;
	lcd_writeonechar(cd==0?0xf8:0xfa);
	lcd_writeonechar(dat&0xf0);
	lcd_writeonechar(dat<<4);
	lcd_cs=0;
}
void lcd_writestring(U8 add,U8 *pt)
{
	lcd_writecd(CMD,add<10?0x80+add:0x90+add-10);
	while(*pt!='\0')
		lcd_writecd(DATA,*pt++);
}
void lcd_displaytwo(U8 add,U8 dat)
{
	lcd_writecd(CMD,add<10?0x80+add:0x90+add-10);
	lcd_writecd(DATA,dat/10+'0');
	lcd_writecd(DATA,dat%10+'0');
}
void lcd_normal_init()
{
	lcd_writecd(CMD,0x0c);
	lcd_writestring(2,"Äê");
	lcd_writestring(4,"ÔÂ");
	lcd_writestring(6,"ÈÕ");
	lcd_writestring(12,":");
	lcd_writestring(14,":");
	lcd_displaytwo(0,20);
	rtc_gettime();
}
void lcd_adjust_init()
{
	lcd_normal_init();
	lcd_writecd(CMD,0x0f);
	lcd_writecd(CMD,0x95);
}
void lcd_showtime()
{
	lcd_displaytwo(1,time.year);
	lcd_displaytwo(3,time.month);
	lcd_displaytwo(5,time.day);
	lcd_displaytwo(11,time.hour);
	lcd_displaytwo(13,time.minute);
	lcd_displaytwo(15,time.second);
}
U8 lcd_position(U8 dat)
{
	switch(dat)
	{
		case 0 : dat=0x95; break;
		case 1 : dat=0x93; break;
		case 2 : dat=0x91; break;
		case 3 : dat=0x85; break;
		case 4 : dat=0x83; break;
		case 6 : dat=0x81; break;
	}
	lcd_writecd(CMD,dat);
	return dat;
}
void lcd_normal()
{
	time.second=rtc_read(0x80);
	if(rtc_secondold!=time.second)
	{
		lcd_displaytwo(15,time.second);
		hd_displaytwo(1,time.second);
		da_process(time.second);
		LED_process(time.second);
		if(time.second==0)
			rtc_gettime();
		rtc_secondold=time.second;
	}	
}