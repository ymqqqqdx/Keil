#include "reg52.h"
#include "1602.h"
#include "ds18b20p.h"
//#include "mscomm.h"
unsigned int tt;
void main()
{
		init_1602();
	//	init_comm();
	while(1)
	{   

		displayfour(0x40+3,readtemp());
	} 
}
