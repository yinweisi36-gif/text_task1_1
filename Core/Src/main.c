/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include <string.h> 

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h> 
#include "bsp_uart.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define FRAME_HEAD 0xFA       
#define FUNC_CODE 0xA0
#define DATA_LEN 12
#define FRAME_LEN 15

float targ_pos = 0.0f;
float step = 0.1f;
float max_val = 6.0f;
float min_val = 0.0f;
static uint8_t flag = 0;


uint8_t rx_buffer[64];
uint8_t tx_buffer[FRAME_LEN];

volatile uint8_t rx_flag = 0;
volatile uint16_t rx_len = 0;

float rec_float[3];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void triangle_wave_generate(void)
{
	if(flag == 0)
	{
		targ_pos += step;
		if(targ_pos >= max_val )
		{
			flag = 1;
		}
	}
	else if(flag == 1)
	{
		targ_pos -= step;
		if(targ_pos <= min_val)
		{
			flag = 0;
		}
	}
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t Parse_Frame(uint8_t *buf, uint16_t len)
{
	if(len < 3)
	{
		return -1;
	}
	if(buf[0] != FRAME_HEAD)
	{
		return -2;
	}
	if(buf[1] != DATA_LEN)
	{
		return -3;
	}
	if(buf[2] != FUNC_CODE)
	{
		return -4;
	}
	if(len < FRAME_LEN)
	{
		return -1;
	}
	memcmp(&rec_float[0],&buf[3],4);
	memcmp(&rec_float[1],&buf[7],4);
	memcmp(&rec_float[2],&buf[11],4);
	return 0;
}

void Send_Frame(float f1, float f2, float f3)
{
	uint8_t *p;
	uint16_t idx = 0;
	
	tx_buffer[idx] = FRAME_HEAD;
	idx++;
	tx_buffer[idx] = 1 + DATA_LEN;
	idx++;
	tx_buffer[idx] = FUNC_CODE;
	idx++;
	
	p = (uint8_t *)&f1;
	tx_buffer[idx++] = p[0];
	tx_buffer[idx++] = p[1];
	tx_buffer[idx++] = p[2];
	tx_buffer[idx++] = p[3];

	p = (uint8_t *)&f2;
	tx_buffer[idx++] = p[0];
	tx_buffer[idx++] = p[1];
	tx_buffer[idx++] = p[2];
	tx_buffer[idx++] = p[3];

	p = (uint8_t *)&f3;
	tx_buffer[idx++] = p[0];
	tx_buffer[idx++] = p[1];
	tx_buffer[idx++] = p[2];
	tx_buffer[idx++] = p[3];
	
	HAL_UART_Transmit(&huart1, tx_buffer, FRAME_LEN, 100);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_DMA(&huart1, rx_buffer, sizeof(rx_buffer));
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	
	LOG_INFO("targ_pos = %.2f", 3.14f);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/*三角波*/
		triangle_wave_generate();
		BSP_UART_SendFloat(targ_pos );
		HAL_Delay(100);
		
		if(rx_flag == 1)
		{
			rx_flag = 0;
			HAL_UART_Transmit(&huart1, (uint8_t*)"OK\r\n", sizeof("OK\r\n")-1, HAL_MAX_DELAY);
		}
		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
