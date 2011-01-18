#include <reg52.h>
#include "declare.h"
unsigned char stop,i,t,temp,mode,comm;
//unsigned char year,week,month,day,hour,minute,second;
void comm_init();
void commscan();
void delay(int);
void main()
{
	comm_init();
	while(1)
	{
		commscan();
	}
}