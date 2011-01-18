#include <reg52.h>
#include <declare.h>
RTC time;
U8 rtc_secondold,stat=0,offset=0,count=0,key_record;
U8 code range[7][2]={{0,59},{0,59},{0,23},{1,31},{1,12},{1,7},{0,99}};
void main()
{
	U8 key_delay;
	lcd_normal_init();
	ls164_write(0xff);
	while(1)
	{
		key_delay=0xff;
		switch(stat)
		{
			case 0:	lcd_normal(); break;
			case 1: break;
		}
		while(key_delay--)
		{
			if(!hd_key)
				key_process(hd_convert(hd_readcd(0x15)));
			while(!hd_key);
		}
	}
}