#ifndef BSP_UART_H
#define BSP_UART_H

#include "main.h"

typedef enum {
    LOG_LEVEL_DBG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERR,
    LOG_LEVEL_NUM
} LogLevel_t;

void BSP_UART_Init(void);
void BSP_UART_SendString(const char *str);
void BSP_UART_SendFloat(float value);

void Log_Printf(LogLevel_t level, const char *fmt, ...);

#define LOG_DBG(fmt, ...)   Log_Printf(LOG_LEVEL_DBG,  fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  Log_Printf(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  Log_Printf(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...)   Log_Printf(LOG_LEVEL_ERR,  fmt, ##__VA_ARGS__)


#endif