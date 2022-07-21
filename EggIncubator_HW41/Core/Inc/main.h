/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BuzzerOut_Pin GPIO_PIN_13
#define BuzzerOut_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_0
#define LCD_RS_GPIO_Port GPIOA
#define LCD_RW_Pin GPIO_PIN_1
#define LCD_RW_GPIO_Port GPIOA
#define LCD_E_Pin GPIO_PIN_2
#define LCD_E_GPIO_Port GPIOA
#define LCD_D4_Pin GPIO_PIN_3
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_4
#define LCD_D5_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_5
#define LCD_D6_GPIO_Port GPIOA
#define LCD_D7_Pin GPIO_PIN_6
#define LCD_D7_GPIO_Port GPIOA
#define KeyUP_Pin GPIO_PIN_7
#define KeyUP_GPIO_Port GPIOA
#define KeySetting_Pin GPIO_PIN_0
#define KeySetting_GPIO_Port GPIOB
#define KeyDown_Pin GPIO_PIN_1
#define KeyDown_GPIO_Port GPIOB
#define KeyLedBuzzer_Pin GPIO_PIN_2
#define KeyLedBuzzer_GPIO_Port GPIOB
#define EE_SCL_Pin GPIO_PIN_10
#define EE_SCL_GPIO_Port GPIOB
#define EE_SDA_Pin GPIO_PIN_11
#define EE_SDA_GPIO_Port GPIOB
#define LedCntlOut_Pin GPIO_PIN_12
#define LedCntlOut_GPIO_Port GPIOB
#define FanExhaustOut_Pin GPIO_PIN_13
#define FanExhaustOut_GPIO_Port GPIOB
#define Motor12Out_Pin GPIO_PIN_14
#define Motor12Out_GPIO_Port GPIOB
#define Motor23Out_Pin GPIO_PIN_15
#define Motor23Out_GPIO_Port GPIOB
#define HumidityOut_Pin GPIO_PIN_8
#define HumidityOut_GPIO_Port GPIOA
#define HeaterPwm_Pin GPIO_PIN_9
#define HeaterPwm_GPIO_Port GPIOA
#define EE_WP_Pin GPIO_PIN_11
#define EE_WP_GPIO_Port GPIOA
#define HeaterRelay_Pin GPIO_PIN_7
#define HeaterRelay_GPIO_Port GPIOF
#define LedHatcher_Pin GPIO_PIN_3
#define LedHatcher_GPIO_Port GPIOB
#define LedExhaust_Pin GPIO_PIN_4
#define LedExhaust_GPIO_Port GPIOB
#define SHT_SCL_Pin GPIO_PIN_6
#define SHT_SCL_GPIO_Port GPIOB
#define SHT_SDA_Pin GPIO_PIN_7
#define SHT_SDA_GPIO_Port GPIOB
#define LedHumidity_Pin GPIO_PIN_8
#define LedHumidity_GPIO_Port GPIOB
#define LedHeater_Pin GPIO_PIN_9
#define LedHeater_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define ENABLE_HUMIFIER	0
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
