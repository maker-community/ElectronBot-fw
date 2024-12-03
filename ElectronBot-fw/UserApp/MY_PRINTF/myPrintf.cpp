#include "myPrintf.h"


/*int fputc(int ch,FILE *f)
{
    uint8_t temp[1] = {(uint8_t)ch};
    HAL_UART_Transmit(&huart1,temp, 1,50);
    return ch;
}*/


#ifdef __GNUC__									//串口重定向
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

void myPrintf(const char* format, ...)
{
    printf(format);
    //HAL_Delay(20);
    HAL_Delay(10);
}

void myDebug(const char* format, ...)
{
#if  0
    printf(format);
    HAL_Delay(20);
#endif
}

