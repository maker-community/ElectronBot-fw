#ifndef CPROJECT_MYPRINTF_H
#define CPROJECT_MYPRINTF_H

#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"
#include "usart.h"

void myPrintf(const char* format, ...);
void myDebug(const char* format, ...);


#endif //CPROJECT_MYPRINTF_H
