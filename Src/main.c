/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "lcd.h"
#include <string.h>
#include "stm32f1xx_hal_flash.h"
#include "flash_stm32f103_hal_sm.h"

// 		GPIO to data bus
// D0 -> PA9
// D1 -> PA7		//	PC7 BAZHEN CHANGE TO PA7	#define NEW_BIT1
// D2 -> PA10
// D3 -> PB3
// D4 -> PB5
// D5 -> PB4
// D6 -> PB10
// D7 -> PA8

// 		GPIO to control bus
// RST	->	PB1  	// PC1 BAZHEN CHANGE TO PB1	#define NEW_RST
// CS	->	PB0		on board RED-LED
// RS	->	PA4		(CD)
// WR	->	PA1
// RD	->	PA0

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
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  uint32_t pointer_u32 = 0;

#define DEBUG_STRING_SIZE 300
  //	char DebugString[DEBUG_STRING_SIZE];
  //
  //	sprintf(DebugString,"VRGC-056th \r\n for_debug USART2 on PA2 115200/8-N-1\r\n");
  //	HAL_UART_Transmit(DebugH.uart, (uint8_t *)DebugString, strlen(DebugString), 100);

  LCD_Init();
  LCD_SetRotation(1);
  LCD_FillScreen(ILI92_WHITE);
  LCD_SetTextColor(ILI92_GREEN, ILI92_WHITE);
  LCD_Printf("\n START 'VRGC-056th'\n ");
  LCD_Printf("for_debug USART2 on PA2 115200/8-N-1 \n");

  LCD_Printf("Flash read.. \n");
  uint32_t flash_word_u32 = Flash_Read(MY_FLASH_PAGE_ADDR);
  LCD_Printf(" word_u32 = 0x%x; \n", flash_word_u32);
  LCD_Printf(" Rotation: '%s'; \n ", (char *)&flash_word_u32);

#define STRING_LEFT ((uint32_t)0x7466654C)
#define STRING_RIGHT ((uint32_t)0x74676952)

#define WRITE_TO_FLASH 0
#if WRITE_TO_FLASH == 1
  flash_word_u32 = STRING_RIGHT;
  if (flash_word_u32 == STRING_RIGHT)
  {
    LCD_Printf("FLASH_Unlock\n ");
    HAL_FLASH_Unlock();

    LCD_Printf("FLASH Erase Page\n ");
    Flash_Erase_Page(MY_FLASH_PAGE_ADDR);

    // uint32_t flash_string_u32 = STRING_LEFT;
    uint32_t flash_string_u32 = STRING_RIGHT;
    LCD_Printf("Write to flash: %s\n ", (char *)&flash_string_u32);
    Flash_Write(MY_FLASH_PAGE_ADDR, flash_string_u32);

    HAL_FLASH_Lock();
    LCD_Printf("HAL FLASH Lock\n ");
  }
  else
  {
    LCD_Printf("NO write to FLASH.\n ");
  }
#endif

  switch (flash_word_u32)
  {
  case STRING_LEFT:
    LCD_SetRotation(3);
    break;
  case STRING_RIGHT:
    LCD_SetRotation(1);
    break;
  default:
    LCD_SetRotation(1);
    break;
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    for (int i = 0; i < 8; i++)
    {
      // HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);	on board RED-LED on PB0 (CS_pin)
      char DebugStr[DEBUG_STRING_SIZE];
      sprintf(DebugStr, " pointer %04u\r\n", (int)pointer_u32++);
      HAL_UART_Transmit(&huart2, (uint8_t *)DebugStr, strlen(DebugStr), 100);
      LCD_SetCursor(100, 100);
      LCD_Printf("%d) ", i);
      switch (i)
      {
      case 7:
        LCD_Printf("Sunday____;");
        break;
      case 1:
        LCD_Printf("Monday____;");
        break;
      case 2:
        LCD_Printf("Tuesday___;");
        break;
      case 3:
        LCD_Printf("Wednesday_;");
        break;
      case 4:
        LCD_Printf("Thursday__;");
        break;
      case 5:
        LCD_Printf("Friday____;");
        break;
      case 6:
        LCD_Printf("Saturday__;");
        break;
      default:
        LCD_Printf("Out_of_day;");
        break;
      } // end switch
      HAL_Delay(1000);
    } // end for i=0
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
