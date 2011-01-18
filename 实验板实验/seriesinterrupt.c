#include <reg52.h>
#include "declare.h"
void ser() interrupt 4
{
	if(comm=='2'||comm=='7')
	stop=SBUF;
	if(comm=='8')
	mode=SBUF;
	RI=0;
}