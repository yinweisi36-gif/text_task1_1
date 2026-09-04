#include "bsp_uart.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1;  // CubeMX生成的串口句柄

void BSP_UART_Init(void)
{
    
}


void BSP_UART_SendString(const char *str)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}


void BSP_UART_SendFloat(float value)
{
	char buffer[32];
	int len = snprintf(buffer, sizeof(buffer), "%.2f\r\n", value);
	if(len>0 && len < sizeof(buffer))
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, HAL_MAX_DELAY);
	}
	else
	{
		buffer[sizeof(buffer)-2] = '\r';
		buffer[sizeof(buffer)-1] = '\n';
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sizeof(buffer) - 1, HAL_MAX_DELAY);
	}
	
	
}