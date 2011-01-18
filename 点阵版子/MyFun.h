#include <reg52.h>

#ifndef _ISP_IAP_H_
#define _ISP_IAP_H_
void ISP_sectorerase(U16);
void ISP_write(U16,U8);
U8 ISP_read(U16);
#endif