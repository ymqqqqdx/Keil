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
extern char year,week,month,day,hour,minute,second,mode,buff,i,week;
extern char alarmhour,alarmminute;
extern unsigned int val;
