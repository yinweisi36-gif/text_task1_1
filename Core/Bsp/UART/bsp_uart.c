#include "bsp_uart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define LOG_BUF_SIZE 128

static const char *s_level_prefix[LOG_LEVEL_NUM] = 
{
	"[DBG]",
	"[INFO]",
	"[WARN]",
	"[ERR]"
};

extern UART_HandleTypeDef huart1;  

void Log_Printf(LogLevel_t level, const char *fmt, ...)
{
	char buf[LOG_BUF_SIZE];
	int prefix_len, body_len, total_len;
	va_list ap;
	if (level >= LOG_LEVEL_NUM) 
	{
    return;
	}
	uint32_t tick_ms = HAL_GetTick();
	float timestamp = tick_ms / 1000.0f;
	prefix_len = snprintf(buf, LOG_BUF_SIZE, "%s[%.1f]",s_level_prefix[level], timestamp);
	if (prefix_len < 0) 
	{
    return;
	}
	if (prefix_len >= LOG_BUF_SIZE) 
	{
    prefix_len = LOG_BUF_SIZE - 1;
    buf[prefix_len] = '\0';
	}
	
	
	va_start(ap, fmt);
	body_len = vsnprintf(buf + prefix_len, LOG_BUF_SIZE - prefix_len, fmt, ap);
	va_end(ap);
	if (body_len < 0) 
	{
    return;
	}
	total_len = prefix_len + body_len;
	if (total_len >= LOG_BUF_SIZE) 
	{
    total_len = LOG_BUF_SIZE - 1;
    buf[total_len] = '\0';
	}
	int remain = LOG_BUF_SIZE - 1 - total_len;
	if (remain >= 2) 
	{
    buf[total_len++] = '\r';
    buf[total_len++] = '\n';
    buf[total_len] = '\0';
	}
	else if (remain == 1) 
	{
    buf[total_len++] = '\n';
    buf[total_len] = '\0';
	}
	HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)total_len, 100);
}


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