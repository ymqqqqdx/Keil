#ifndef _TYPE_DEFINE_
#define _TYPE_DEFINE_
#define U8 unsigned char
#define U16 unsigned int
#define S8 singed char
#define S16 singed int
#define U32 unsigned long
#define S32 singed long
#endif

#ifndef _STRUCT_
#define _STRUCT_
typedef struct
{
	U8 second;
	U8 minute;
	U8 hour;
	U8 day;
	U8 month;
	U8 week;
	U8 year;
} RTC;
#endif

#ifndef _DS1302_H_
#define _DS1302_H_
void delay_1ms(U16);
U8 rtc_read(U8);
U8 rtc_readonechar();
void rtc_write(U8,U8);
void rtc_writeonechar(U8);
void rtc_gettime();
bit rtc_range(U8,U8);
void rtc_writertc();
void rtc_settime(U8,U8);
#endif

#ifndef _LCD_H_
#define _LCD_H_
#define CMD 0
#define DATA 1
void lcd_writecd(bit,U8);
void lcd_writeonechar(U8);
void lcd_writestring(U8,U8 *);
void lcd_normal_init();
void lcd_adjust_init();
void lcd_displaytwo(U8,U8);
void lcd_showtime();
U8 lcd_position(U8);
void lcd_normal();
#endif

#ifndef _HD7279_H_
#define _HD7279_H_
sbit hd_cs=P0^4;
sbit hd_clk=P0^5;
sbit hd_dat=P0^6;
sbit hd_key=P0^7;
void hd_writecd(U8,U8);
U8 hd_readcd(U8);
void hd_showtime();
void hd_displaytwo(U8,U8);
U8 hd_convert(U8);
void hd_writebyte(U8);
#endif

#ifndef _DA_TLC5619_H_
#define _DA_TLC5619_H_
void da_write(U16);
void da_process(U8);
#endif

#ifndef _LED_H_
#define _LED_H_
void LED_process(U8);
#endif

#ifndef _LS164_H_
#define _LS164_H_
void ls164_write(U8);
#endif

#ifndef _KEY_PROCESS_
#define _KEY_PROCESS_
void key_process(U8);
extern U8 code range[7][2];
#endif

#ifndef _BEEP_H_
#define _BEEP_H_
sbit beep=P2^7;
#endif
