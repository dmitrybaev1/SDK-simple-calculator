/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "kb.h"
#include "sdk_uart.h"
#include "pca9538.h"
#include "oled.h"
#include "fonts.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t KB_Read();
void OLED_KB( uint8_t OLED_Keys[]);
void oled_Reset( void );
void invite(char* str);
void invite_op(void);
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
  MX_I2C1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  oled_Init();
  uint8_t digitsTotal = 10;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE BEGIN 3 */
	  UART_Transmit( (uint8_t*)"KB test start\n" );
	  uint8_t Row[4] = {0xF7, 0x7B, 0x3D, 0x1E}, Key, OldKey, OLED_Keys[12] = {0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30}, new_key;
	  new_key = 0;

	  while(1) {
		  uint8_t enter = 0;
		  int first_num = 0;
		  int second_num = 0;
		  double result = 0;
		  oled_Reset();
		  invite("Enter first");
		  uint8_t digitCount = 0;
		  char first_num_str[50];
		  sprintf(first_num_str, "%d", first_num);
		  oled_WriteString(first_num_str,  Font_7x10, White);
		  oled_UpdateScreen();
		  while(!(enter == 1)) {
			  new_key = KB_Read();
			  if (new_key > 0 && new_key <= digitsTotal) {
				  if (new_key == 10) {
				  	new_key = 0;
				  }
				invite("Enter first");
				first_num *= 10;
				first_num += new_key;
			  	sprintf(first_num_str, "%d", first_num);
			  	oled_WriteString(first_num_str,  Font_7x10, White);
			  	oled_UpdateScreen();
			  	digitCount++;
				HAL_Delay(200);
			  } else if (new_key > 0 && new_key == 11) {
				  enter = 1;
			  }
			  HAL_Delay(50);
		  }
		  enter = 0;
		  digitCount = 0;
		  invite("Enter second");
		  char second_num_str[50];
		  sprintf(second_num_str, "%d", second_num);
		  oled_WriteString(second_num_str,  Font_7x10, White);
		  oled_UpdateScreen();
		  while(!(enter == 1)) {
		  			  new_key = KB_Read();
		  			  if (new_key > 0 && new_key <= digitsTotal) {
		  				  if (new_key == 10) {
		  					  new_key = 0;
		  				  }
		  				invite("Enter second");
		  				second_num *= 10;
		  				second_num += new_key;
		  			  	sprintf(second_num_str, "%d", second_num);
		  			  	oled_WriteString(second_num_str,  Font_7x10, White);
		  			  	oled_UpdateScreen();
		  			  	digitCount++;
		  				HAL_Delay(200);
		  			  } else if (new_key > 0 && new_key == 11) {
		  				  enter = 1;
		  			  }
		  			  HAL_Delay(50);
		  		  }
		  enter = 0;
		  digitCount = 0;
		  invite_op()

		  while(!(enter == 1)) {
		   			  new_key = KB_Read();
		   			  if (new_key > 0 && new_key <= digitsTotal) {
		   				  if (new_key == 1) {
		   					result = ((double) first_num / second_num);
		   				  }
		   				  if (new_key == 4) {
		   					result = ((double) first_num * second_num);
		   				  }
		   				  if (new_key == 7) {
		   					result = ((double) first_num - second_num);
		   				  }
		   				  if (new_key == 10) {
		   					result = ((double) first_num + second_num);
		   				  }
		   				enter = 1;
		   			  }
		   			HAL_Delay(50);
		   }
		  enter = 0;
		  oled_Fill(Black);
		  oled_SetCursor(0, 0);
		  oled_WriteString("Result", Font_7x10, White);
		  oled_SetCursor(0, 20);
		  char result_str[50];
		  sprintf(result_str, "%f", result);
		  oled_WriteString(result_str, Font_7x10, White);
		  oled_UpdateScreen();
		  while(!(enter == 1)) {
		  	new_key = KB_Read();
		  	if (new_key > 0 && new_key == 11) {
		  		enter = 1;
		  	}
		  	HAL_Delay(50);
		  }
		  enter = 0;
		  HAL_Delay(50);
	  }
  }
  /* USER CODE END 3 */
  /* USER CODE END WHILE */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void invite(char* str){
	oled_Fill(Black);
	oled_SetCursor(0,0);
	oled_WriteString(str, Font_7x10, White);
	oled_UpdateScreen();
	oled_SetCursor(0,20);
}

void invite_op(){
	oled_Fill(Black);
	oled_SetCursor(0, 0);
	oled_WriteString("Enter operation", Font_7x10, White);
	oled_SetCursor(0, 12);
	oled_WriteString("/", Font_7x10, White);
	oled_SetCursor(0, 24);
	oled_WriteString("*", Font_7x10, White);
	oled_SetCursor(0, 36);
	oled_WriteString("-", Font_7x10, White);
	oled_SetCursor(0, 48);
	oled_WriteString("+", Font_7x10, White);
	oled_UpdateScreen();	
}

uint8_t KB_Read() {
uint8_t Row[4] = {0xF7, 0x7B, 0x3D, 0x1E};
	switch (Check_Row(Row[0])) {
		case 0x01: return 12;
		case 0x02: return 11;
		case 0x04: return 10;
	}
	switch (Check_Row(Row[1])) {
		case 0x01: return 9;
		case 0x02: return 8;
		case 0x04: return 7;
	}
	switch (Check_Row(Row[2])) {
		case 0x01: return 6;
		case 0x02: return 5;
		case 0x04: return 4;
	}
	switch (Check_Row(Row[3])) {
		case 0x01: return 3;
		case 0x02: return 2;
		case 0x04: return 1;
	}
	return 0;
}

void OLED_KB( uint8_t OLED_Keys[12]) {
	for (int i = 3; i >= 0; i--) {
		oled_SetCursor(56, 5+(4-i)*10);
		for (int j = 0; j < 3; j++) {
			oled_WriteChar(OLED_Keys[j+3*i], Font_7x10, White);
		}
	}
	oled_UpdateScreen();
}
void oled_Reset( void ) {
	oled_Fill(Black);
	oled_SetCursor(0, 0);
	oled_UpdateScreen();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
