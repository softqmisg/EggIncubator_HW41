/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "iwdg.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "lcd_ch.h"
#include "string_num.h"
#include "sht2x_BB.h"
#include "Buzzer.h"
#include  "Keys.h"
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
uint8_t tik_1s=0;
GPIO_PinState st;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//call every 10ms
	if(htim->Instance==TIM16)
	{
		//Buzzer evaluate//
		if(Buzzer.active)
		{
			if(!Buzzer.mute)
				BUZZER_ON();
			Buzzer.counter_on++;
			if(Buzzer.counter_on>Buzzer.duration)
			{
				Buzzer.counter_on=0;
				BUZZER_OFF();
				Buzzer.active=0;
			}
		}
		else
		{
			if(Buzzer.repeatstate)
			{
				Buzzer.counter_repeat++;
				if(Buzzer.counter_repeat>Buzzer.delaybetweenrepeat)
				{
					Buzzer.counter_repeat=0;
					Buzzer.active=1;
				}
			}
		}
		//Read Keys//
		for(uint8_t i=0;i<KEYSNUM;i++)
		{
			st=HAL_GPIO_ReadPin(Keys[i].port,Keys[i].pin);
			Keys[i].RawState=(KeyState_t)st;
			if(Keys[i].Prev_RawState==Release && Keys[i].RawState==Press) //press
			{
				Keys[i].counter=0;
				Keys[i].LongPress=0;
				Keys[i].ShortPress=0;
				Keys[i].LongLongPress=0;
			}
			else if(Keys[i].Prev_RawState==Press && Keys[i].RawState==Press) //continue press
			{
				Keys[i].counter++;
				if(Keys[i].counter>LONGPRESS_MIN)
				{
					Keys[i].LongPress=1;
					Keys[i].counter=0;
				}
				else if(Keys[i].LongPress && Keys[i].counter>LONGLONGPRESS_MIN)
				{
					Keys[i].LongLongPress=1;
					Keys[i].counter=0;
				}
			}
			else if(Keys[i].Prev_RawState==Press && Keys[i].RawState==Release) //release
			{
				if(Keys[i].counter>SHORTPRESS_MIN && Keys[i].counter<SHORTPRESS_MAX)
				{
					if(!Keys[i].LongPress && !Keys[i].LongLongPress)
						Keys[i].ShortPress=1;
				}
				Keys[i].counter=0;
			}
			Keys[i].Prev_RawState=Keys[i].RawState;
		}
	}
	////////////////////////////////////////////////////////////////////
	//call every 1s
	if(htim->Instance==TIM17)
	{
		tik_1s=1;
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void ftoa(float a ,uint8_t *integer_part,uint8_t *fractional_part)
//{
//	   *integer_part=(uint8_t)a;
//	   *fractional_part=(int)((a-(double)*integer_part)*10.0);

//}
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
  MX_TIM1_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start_IT(&htim17);
	
  HAL_GPIO_WritePin(LedHeater_GPIO_Port, LedHeater_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LedHumidity_GPIO_Port, LedHumidity_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LedExhaust_GPIO_Port, LedExhaust_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LedHatcher_GPIO_Port, LedHatcher_Pin, GPIO_PIN_SET);

  LCD_init();
	BuzzerInit();
	KeysInit();
  char lcd_str[16];
	//char lcd_float[16];
  //sprintf(lcd_str,"Hello Mehdi%d",12);
  //LCD_putstrpos(lcd_str, 0, 0);
	LCD_putpersian(ROTOBAT_STR,9,0);

LCD_putpersian(DAMA_STR,5,0);

double a=30.2;
//  uint8_t integer_part,fractional_part;
//  ftoa(a, &integer_part,&fractional_part);
//  sprintf(lcd_str,"%d.%d",integer_part,fractional_part);
//  LCD_putstrpos(lcd_str, 0, 1);
  
	sprintf(lcd_str,"Mehdi:%s",ftoa(a));
  LCD_putstrpos(lcd_str, 0, 1);
	HAL_Delay(1000);
	a=42.6;
	sprintf(lcd_str,"Mehdi:%s",ftoa(a));
  LCD_putstrpos(lcd_str, 0, 1);
	SHT2x_SoftReset();
	float temp,hum;
	BuzzerOn(100);
	LCD_clearrow(0);
	a=50.0;
	HAL_IWDG_Init(&hiwdg);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_IWDG_Refresh(&hiwdg);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(tik_1s)
		{
			tik_1s=0;
			if(!SHT2x_GetValue(&temp,&hum))
			{
				BuzzerRepeatStop();
				sprintf(lcd_str,"%s,",ftoa(temp));
				sprintf(lcd_str,"%s%s",lcd_str,ftoa(hum));
			}
			else
			{
				BuzzerRepeatStart(100,4000);
				sprintf(lcd_str,"----,----");
			}
			LCD_putstrpos(lcd_str,0,0);
		}
		if(Keys[KEYUP].ShortPress)
		{
			Keys[KEYUP].ShortPress=0;
			a+=0.1;
			sprintf(lcd_str,"Mehdi:%s",ftoa(a));
			LCD_putstrpos(lcd_str, 0, 1);			
		}
		if(Keys[KEYUP].LongLongPress)
		{
			Keys[KEYUP].LongLongPress=0;
			a+=1.0;
			sprintf(lcd_str,"Mehdi:%s",ftoa(a));
			LCD_putstrpos(lcd_str, 0, 1);			
		}		
		if(Keys[KEYLEDBUZZER].LongPress)
		{
			Keys[KEYLEDBUZZER].LongPress=0;
			if(Buzzer.mute)
			{
				BuzzerMute(0);
				sprintf(lcd_str,"Alarm on");
			}
			else
			{
				BuzzerMute(1);
				sprintf(lcd_str,"Alarm off");				
			}
			LCD_putstrpos(lcd_str, 0, 0);	
		}			
		if(Keys[KEYDOWN].RawState==Press && Keys[KEYUP].RawState==Press)
		{
				sprintf(lcd_str,"Both");				
				LCD_putstrpos(lcd_str, 0, 0);	
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
