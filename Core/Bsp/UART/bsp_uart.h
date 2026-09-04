#ifndef BSP_UART_H
#define BSP_UART_H

#include "main.h"



void BSP_UART_Init(void);
void BSP_UART_SendString(const char *str);
void BSP_UART_SendFloat(float value);



#endif