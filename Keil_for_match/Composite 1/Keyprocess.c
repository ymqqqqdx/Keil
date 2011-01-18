#include <reg52.h>
#include <declare.h>
extern U8 stat,offset,count,key_record;
extern RTC time;
void key_process(U8 key)
{
	if(key>10)
	{
		switch(key)
		{
			case 11 : if(offset==4)	offset++; if(offset<6) lcd_position(++offset); break;
			case 12 : if(offset==6) offset--; if(offset>0) lcd_position(--offset); break;
			case 13 : stat=0; rtc_writertc(); lcd_normal_init(); break;
			case 14 : stat=1; lcd_adjust_init(); break;
		}
	}
	else if(stat==1)
	{
		if(count==0)
		{
			lcd_position(offset);
			lcd_writecd(DATA,' ');
			lcd_writecd(DATA,' ');
			lcd_position(offset);
			lcd_writecd(DATA,key+'0');
			count++;
			key_record=10*key;
		}
		else
		{
			lcd_writecd(DATA,key+'0');
			lcd_position(offset);
			count=0;
			key_record+=key;
			if(!rtc_range(offset,key_record))
				rtc_settime(offset,key_record);
			else
			{
				lcd_position(offset);
				lcd_writecd(DATA,range[offset][0]/10+'0');
				lcd_writecd(DATA,range[offset][0]%10+'0');
				lcd_position(offset);
			}
		}
	}
}