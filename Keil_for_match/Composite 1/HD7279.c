#include <reg52.h>
#include <intrins.h>
#include <declare.h>
extern RTC time;
void hd_longdelay()
{
	U8 i;
	for (i=0;i<50;i++);
}
void hd_shortdelay()
{
	U8 i;
	for (i=0;i<8;i++);
}
void hd_writebyte(U8 cmd)
{
	U8 i;
	hd_cs=0;
	hd_dat=0;
	hd_longdelay();
	for(i=0;i<8;i++)
	{
		cmd<<=1;
		hd_dat=CY;
		hd_clk=1; hd_shortdelay();
		hd_clk=0; hd_shortdelay();
	}
	hd_dat=0;
}
U8 hd_readbyte()
{
	U8 i,temp=0;
	hd_dat=1;
	hd_longdelay();
	for(i=0;i<8;i++)
	{
		hd_clk=1;	hd_shortdelay();
		temp|=((char)hd_dat)<<(7-i);
		hd_clk=0;	hd_shortdelay();
	}
	hd_dat=0;
	return temp;
}
void hd_writecd(U8 cmd,U8 dat)
{
	hd_writebyte(cmd);
	hd_writebyte(dat);
}
U8 hd_readcd(U8 cmd)
{
	hd_writebyte(cmd);
	return(hd_readbyte());
}
U8 hd_convert(U8 temp)
{
	if(temp<28)
	{
		switch(temp%8)
		{
			case 6 : return temp/8*3+1; break;
			case 5 : return temp/8*3+2; break;
			case 4 : return temp/8*3+3; break;
			case 3 : return temp/8+13; break;
		}
	}
	else
	{
		if(temp==29) return 0;
		if(temp==30) return 11;
		if(temp==28) return 12;
	}
}
void hd_displaytwo(U8 add,U8 dat)
{
	hd_writecd(0x80+add,dat/10);
	hd_writecd(0x80+add-1,dat%10);
}
void hd_showtime()
{
	hd_writecd(0x80+5,10);
	hd_writecd(0x80+5,10);
	hd_writecd(0x80+2,10);
	hd_displaytwo(7,time.hour);
	hd_displaytwo(4,time.minute);
	hd_displaytwo(1,time.second);
}