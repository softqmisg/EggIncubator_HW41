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
#include <string.h>
#include "lcd_ch.h"
#include "string_num.h"
#include "sht2x_BB.h"
#include "Buzzer.h"
#include "Keys.h"
#include "user_eeprom.h"
#include "eeprom.h"
#include "user_time.h"
#include "bird.h"
#include "Motor.h"
#include "user_sht.h"
#include "heater_fan.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DELAY_GOMAINMENU	20//s
#define DELAY_SWITCHMAINDISPLAY		3//s
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
volatile  uint8_t tik_1s=0;
volatile uint8_t TimerGoMainMenu=0;
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
			Keys[i].RawState=(KeyState_t)HAL_GPIO_ReadPin(Keys[i].port,Keys[i].pin);
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
		if(TimerGoMainMenu>0) 
			TimerGoMainMenu--;
	}
}
////////////////////////////////////
uint8_t FindProgNum(Bird_t bird,Time_t time)
{
	uint8_t prognum=0;
	uint16_t sumdays=0;
	if(bird.Type==Manual)
	{
		if(bird.pProgs[0].durationDays)
			prognum=0;
		else
			prognum=1;
	}
	else
	{
	while(bird.TotalDurationDays>sumdays)
	{
		sumdays+=bird.pProgs[prognum].durationDays;
		if(sumdays>time.day) break;
		prognum++;
	}
	if(prognum>=bird.NumofProg) 
		prognum=bird.NumofProg-1;
	}
	return prognum;
}
uint8_t IsHatcher(Bird_t bird,Time_t time)
{
	if(time.day>=(bird.TotalDurationDays-bird.HatchTotalDays) && bird.pProgs[bird.NumofProg-1].durationDays)
		return 1;
	else 
		return	0;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//	Time_t st={.day=300,.hr=52,.min=32,.sec=0};
//	Time_t rt;
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
//  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim16); //timer 10ms
	HAL_TIM_Base_Start_IT(&htim17);	//timer 1s
	
  HAL_GPIO_WritePin(LedHeater_GPIO_Port, LedHeater_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LedHumidity_GPIO_Port, LedHumidity_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LedExhaust_GPIO_Port, LedExhaust_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LedHatcher_GPIO_Port, LedHatcher_Pin, GPIO_PIN_SET);
  LCD_init();
	BuzzerInit();
	KeysInit();
	BuzzerOn(100);
	
	char lcd_str[2][16];
	uint8_t tmp_uint8;
	uint16_t tmp_uint16;
	Time_t curTime={.sec=0,.min=0,.hr=0,.day=0};
	LCD_putstralign("Ghoghnoos",0,0,AlignCenter);
	if(Keys[KEYUP].RawState==Press && Keys[KEYDOWN].RawState==Press)
	{
		LCD_putstralign("Save Default...",0,1,AlignCenter);
		TimeSave(curTime,EE_ADD_CURTIME);
		tmp_uint8=(uint8_t)DEFAULT_CURBIRDTYPE;
		EEWriteByte(&tmp_uint8,1,EE_ADD_CURBIRDTYPE);
		BirdSaveManual(defaultBirds[Manual]);
		TimeSave(defaultMotor.OnTime,EE_ADD_MOTOR_ON);
		TimeSave(defaultMotor.OffTime,EE_ADD_MOTOR_OFF);
		FanSave(defaultFan);
		HeaterSave(defaultHeater);
		ShtSave(defaultSht);
	}
	//////////////////////Initialize after boot///////////////////////////////////////////
	LCD_clearrow(1);
	LCD_putstralign("Loading...",0,1,AlignCenter);
	TimeInit(&curTime);
	Bird_t curBird;
	BirdInit(&curBird);
	Motor_t motor;
	MotorInit(&motor);
	Sht_t sht20;
	ShtInit(&sht20);
	Fan_t fan;
	FanInit(&fan);
	Heater_t heater;
	HeaterInit(&heater);
	uint8_t curProg=FindProgNum(curBird,curTime);
	if(IsHatcher(curBird,curTime))
		HAL_GPIO_WritePin(LedHatcher_GPIO_Port,LedHatcher_Pin,GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(LedHatcher_GPIO_Port,LedHatcher_Pin,GPIO_PIN_SET);	
	HAL_Delay(3000);
	LCD_clear_home();
	///////////////////////////////////////////////////////////////
//	HAL_IWDG_Init(&hiwdg);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	enum{MainMenu=0,
			SettingBirdMenu,
			SettingBirdDayMenu,
			SettingManualTemperatureMenu,SettingManualHumidtyMenu,SettingManualDaysMenu,SettingManualHatchersMenu,
			AdvanceMenu
			} curMenu=MainMenu;
	uint8_t counter_switchmaindispaly=0,tik_switchmaindispaly=1,MainPageNumber=0;
	uint8_t tmpType,tmpProg;
	Time_t tmpTime;
	int16_t tmpTemp,tmpHumid,tmpDay;
	uint8_t tmpIsHatch;
  while (1)
  {
//		HAL_IWDG_Refresh(&hiwdg);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		switch(curMenu)
		{
			//Main Menu//
			case MainMenu:
				if(tik_1s)
				{
					tik_1s=0;
					//main menu tik//
					counter_switchmaindispaly++;
					if(counter_switchmaindispaly>DELAY_SWITCHMAINDISPLAY)
					{
						counter_switchmaindispaly=0;
						tik_switchmaindispaly=1;
					}
					//increment time and save//
					IncTimesec(&curTime);
					if(curTime.sec==0)
							TimeSave(curTime,EE_ADD_CURTIME);
					//find current program for current bird  and turn the hatcher LED on, if necessary//
					curProg=FindProgNum(curBird,curTime);
					//Check Motor
					//Setter Hatcher LED
					IncTimesec(&motor.curTime);
					if(IsHatcher(curBird,curTime))
					{
						HAL_GPIO_WritePin(LedHatcher_GPIO_Port,LedHatcher_Pin,GPIO_PIN_RESET);
						if(motor.mode==MotorAuto)
							MotorSetState(&motor,MotorOff);
					}
					else
					{
						HAL_GPIO_WritePin(LedHatcher_GPIO_Port,LedHatcher_Pin,GPIO_PIN_SET);							
						MotorCheck(&motor);
					}
					//Read sensor//
					ShtReadSensor(&sht20);
					if(!sht20.error)
					{
						FanCheckHum(fan,curBird.pProgs[curProg].humidty,sht20.humidity);
						FanCheckTemp(fan,curBird.pProgs[curProg].temperature,sht20.temperature);
						HeaterCheck(heater,curBird.pProgs[curProg].temperature,sht20.temperature);
						if(sht20.humidity<curBird.pProgs[curProg].humidty)
						{
							HAL_GPIO_WritePin(HumidityOut_GPIO_Port,HumidityOut_Pin,GPIO_PIN_SET);
							HAL_GPIO_WritePin(LedHumidity_GPIO_Port,LedHumidity_Pin,GPIO_PIN_RESET);
						}
						else
						{
							HAL_GPIO_WritePin(HumidityOut_GPIO_Port,HumidityOut_Pin,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(LedHumidity_GPIO_Port,LedHumidity_Pin,GPIO_PIN_SET);							
						}
					}
				}
				if(tik_switchmaindispaly)
				{
					tik_switchmaindispaly=0;
					switch(MainPageNumber)
					{
						case 0:
							LCD_clear_home();
							LCD_putstralign("Bird",0,0,AlignCenter);
							LCD_putpersian((uint8_t)curBird.Type,0,1,AlignCenter);
							MainPageNumber=1;
							break;
						case 1:
							LCD_clear_home();
							LCD_putpersian(DAMA_STR,3,0,AlignNone);
							sprintf(lcd_str[0],"=%3d.%1d %cC",sht20.temperature/10,sht20.temperature%10,DEGREE_CH_CODE);
							LCD_putstralign(lcd_str[0],7,0,AlignNone);

							LCD_putpersian(ROTOBAT_STR,1,1,AlignNone);
							sprintf(lcd_str[0],"=%3d.%1d %%",sht20.humidity/10,sht20.humidity%10);
							LCD_putstralign(lcd_str[0],7,1,AlignNone);
							MainPageNumber=2;
							break;
						case 2:
							LCD_clear_home();
							LCD_putpersian(ZAMAN_STR,2,0,AlignNone);
							sprintf(lcd_str[0],"= %02d:%02d  ",curTime.hr,curTime.min);
							LCD_putstralign(lcd_str[0],7,0,AlignNone);

							LCD_putpersian(ROOZ_STR,2,1,AlignNone);
							sprintf(lcd_str[0],"= %3d",curTime.day);
							LCD_putstralign(lcd_str[0],7,1,AlignNone);
						
							MainPageNumber=0;
							break;
					}
				}
				//Check Keys
				if(Keys[KEYUP].RawState==Press)
				{
					if(motor.mode==MotorAuto)
					{
						motor.mode=MotorManual;
						MotorSetState(&motor,MotorOn);
						MainPageNumber=3;
						LCD_clear_home();
						LCD_putpersian(CHARKHSH_STR,0,0,AlignCenter);
					}
					sprintf(lcd_str[1],"T=%2d.%1d  H=%2d.%1d",
																										sht20.temperature/10,sht20.temperature%10,
																										sht20.humidity/10,sht20.humidity%10
									);
					LCD_putstrpos(lcd_str[1],1,1);
				}
				else if(Keys[KEYUP].RawState==Release)
				{
					if(motor.mode==MotorManual)
					{
						motor.mode=MotorAuto;
						MotorSetState(&motor,MotorOff);						
						motor.curTime.hr=0;motor.curTime.min=0;motor.curTime.sec=0;
						MainPageNumber=0;
						tik_switchmaindispaly=1;
						LCD_clear_home();
					}
				}
				if(Keys[KEYLEDBUZZER].ShortPress)
				{
					Keys[KEYLEDBUZZER].ShortPress=0;
					HAL_GPIO_TogglePin(LedCntlOut_GPIO_Port,LedCntlOut_Pin);
				}
				if(Keys[KEYLEDBUZZER].LongPress)
				{
					Keys[KEYLEDBUZZER].LongPress=0;
					LCD_clearrow(0);
					if(Buzzer.mute)
					{
						Buzzer.mute=0;
						LCD_putstralign("BUZZER OFF",0,0,AlignCenter);
					}
					else
					{
						Buzzer.mute=1;
						LCD_putstralign("BUZZER ON",0,0,AlignCenter);						
					}
				}				
				if(Keys[KEYSETTING].ShortPress || Keys[KEYSETTING].LongPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					Keys[KEYSETTING].LongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					
					LCD_clear_home();
					LCD_putstrpos("Bird Select",1,0);
					LCD_putstrpos("=>",1,1);
					tmpType=curBird.Type;
					LCD_putpersian((uint8_t)tmpType,0,1,AlignCenter);
					
					HeaterSetPercent(0);
					FanSetState(&fan,FanOff);
					motor.mode=MotorManual;	MotorSetState(&motor,MotorOff);
					HAL_GPIO_WritePin(HumidityOut_GPIO_Port,HumidityOut_Pin,GPIO_PIN_RESET);
					HAL_GPIO_WritePin(LedHumidity_GPIO_Port,LedHumidity_Pin,GPIO_PIN_SET);		
					HAL_GPIO_WritePin(LedHatcher_GPIO_Port,LedHatcher_Pin,GPIO_PIN_SET);
					
					curMenu=SettingBirdMenu;
				}
				break;
			//Setting Bird Menu//				
			case SettingBirdMenu:
				if(Keys[KEYUP].ShortPress || Keys[KEYUP].LongLongPress)
				{
					Keys[KEYUP].ShortPress=0; Keys[KEYUP].LongLongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					tmpType++;
					if(tmpType>=NumOfBirds)
						tmpType=0;
//					LCD_putstrpos("        ",4,1);
					LCD_clear_size(4,1,8);
					LCD_putpersian((uint8_t)tmpType,0,1,AlignCenter);
				}
				if(Keys[KEYDOWN].ShortPress  || Keys[KEYDOWN].LongLongPress)
				{
					Keys[KEYDOWN].ShortPress=0; Keys[KEYDOWN].LongLongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					if(tmpType==0)
						tmpType=NumOfBirds;
					tmpType--;
					LCD_clear_size(4,1,8);
					LCD_putpersian((uint8_t)tmpType,0,1,AlignCenter);
				}
				if(Keys[KEYSETTING].ShortPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					LCD_clear_home();
					if(tmpType==Manual)
					{
						TimerGoMainMenu=DELAY_GOMAINMENU;
						if(defaultBirds[Manual].pProgs[0].durationDays)
						{
							tmpProg=0;
						}
						else
						{
							tmpProg=1;
						}
						tmpTemp=defaultBirds[Manual].pProgs[tmpProg].temperature;
						
						LCD_putpersian(DAMA_STR,0,0,AlignCenter);						
						LCD_putstrpos("=>",1,1);
						sprintf(lcd_str[1],"%2d.%1d %cC",tmpTemp/10,tmpTemp%10,DEGREE_CH_CODE);
						LCD_putstralign(lcd_str[1],0,1,AlignCenter);

						curMenu=SettingManualTemperatureMenu;
					}
					else
					{
						LCD_putstrpos("Day Select",1,0);
						LCD_putpersian(ROOZ_STR,1,1,AlignNone);
						tmpTime.day=0;tmpTime.hr=0;tmpTime.min=0;tmpTime.sec=0;
						if(IsHatcher(defaultBirds[tmpType],tmpTime))
							sprintf(lcd_str[1],"=%03d Hatcher",tmpTime.day);
						else
							sprintf(lcd_str[1],"=%03d Setter ",tmpTime.day);
						LCD_putstralign(lcd_str[1],0,1,AlignRight);
						curMenu=SettingBirdDayMenu;
					}
				}
				if(TimerGoMainMenu==0)
				{
					LCD_clear_home();
					tik_1s=1;
					tik_switchmaindispaly=1;
					MainPageNumber=0;
					motor.mode=MotorAuto;
					curMenu=MainMenu;
				}
				break;
				//Setting Manual:Temperature
			case SettingManualTemperatureMenu:
				if(Keys[KEYUP].ShortPress)
				{
						Keys[KEYUP].ShortPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
						tmpTemp++;
						if(tmpTemp>TEMP_UPPER_VALUE)
							tmpTemp=TEMP_UPPER_VALUE;
						sprintf(lcd_str[1],"%2d.%1d %cC",tmpTemp/10,tmpTemp%10,DEGREE_CH_CODE);
						LCD_putstralign(lcd_str[1],0,1,AlignCenter);					
				}
				if(Keys[KEYUP].LongLongPress)
				{
						Keys[KEYUP].LongLongPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
						tmpTemp+=10;
						if(tmpTemp>TEMP_UPPER_VALUE)
							tmpTemp=TEMP_UPPER_VALUE;
						sprintf(lcd_str[1],"%2d.%1d %cC",tmpTemp/10,tmpTemp%10,DEGREE_CH_CODE);
						LCD_putstralign(lcd_str[1],0,1,AlignCenter);					
				}
				if(Keys[KEYDOWN].ShortPress)
				{
						Keys[KEYDOWN].ShortPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
					
						if(tmpTemp<=TEMP_LOWER_VALUE) 
								tmpTemp=TEMP_LOWER_VALUE+1;
						tmpTemp--;
						sprintf(lcd_str[1],"%2d.%1d %cC",tmpTemp/10,tmpTemp%10,DEGREE_CH_CODE);
						LCD_putstralign(lcd_str[1],0,1,AlignCenter);					
				}
				if(Keys[KEYDOWN].LongLongPress)
				{
						Keys[KEYDOWN].LongLongPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
	
						if((tmpTemp-10)<TEMP_LOWER_VALUE) 
								tmpTemp=TEMP_LOWER_VALUE+10;
						tmpTemp-=10;
						sprintf(lcd_str[1],"%2d.%1d %cC",tmpTemp/10,tmpTemp%10,DEGREE_CH_CODE);
						LCD_putstralign(lcd_str[1],0,1,AlignCenter);					
				}
				if(Keys[KEYSETTING].ShortPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					tmpHumid=defaultBirds[Manual].pProgs[tmpProg].humidty;
					LCD_clear_home();
					LCD_putpersian(ROTOBAT_STR,0,0,AlignCenter);						
					LCD_putstrpos("=>",1,1);
					sprintf(lcd_str[1],"%2d.%1d %%",tmpHumid/10,tmpHumid%10);
					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
					curMenu=SettingManualHumidtyMenu;
				}
				if(TimerGoMainMenu==0)
				{
					LCD_clear_home();
					tik_1s=1;
					tik_switchmaindispaly=1;
					MainPageNumber=0;
					motor.mode=MotorAuto;
					curMenu=MainMenu;
				}				
			break;
				//Setting Manual:Humidity
			case SettingManualHumidtyMenu:
				if(Keys[KEYUP].ShortPress)
				{
						Keys[KEYUP].ShortPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
						tmpHumid++;
						if(tmpHumid>HUM_UPPER_VALUE)
							tmpHumid=HUM_UPPER_VALUE;
						sprintf(lcd_str[1],"%2d.%1d %%",tmpHumid/10,tmpHumid%10);
						LCD_putstralign(lcd_str[1],0,1,AlignCenter);					
				}
				if(Keys[KEYUP].LongLongPress)
				{
						Keys[KEYUP].LongLongPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
						tmpHumid+=10;
						if(tmpHumid>HUM_UPPER_VALUE)
							tmpHumid=HUM_UPPER_VALUE;
						sprintf(lcd_str[1],"%2d.%1d %%",tmpHumid/10,tmpHumid%10);
						LCD_putstralign(lcd_str[1],0,1,AlignCenter);					
				}
				if(Keys[KEYDOWN].ShortPress)
				{
						Keys[KEYDOWN].ShortPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
					
						if(tmpHumid<=HUM_LOWER_VALUE) 
								tmpHumid=HUM_LOWER_VALUE+1;
						tmpHumid--;
						sprintf(lcd_str[1],"%2d.%1d %%",tmpHumid/10,tmpHumid%10);
						LCD_putstralign(lcd_str[1],0,1,AlignCenter);					
				}
				if(Keys[KEYDOWN].LongLongPress)
				{
						Keys[KEYDOWN].LongLongPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
	
						if((tmpHumid-10)<HUM_LOWER_VALUE) 
								tmpHumid=HUM_LOWER_VALUE+10;
						tmpHumid-=10;
						sprintf(lcd_str[1],"%2d.%1d %%",tmpHumid/10,tmpHumid%10);
						LCD_putstralign(lcd_str[1],0,1,AlignCenter);					
				}
				if(Keys[KEYSETTING].ShortPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
//					tmpHumid=defaultBirds[Manual].pProgs[0].humidty;
//					LCD_clear_home();
//					LCD_putpersian(ROTOBAT_STR,0,0,AlignCenter);						
//					LCD_putstrpos("=>",1,1);
//					sprintf(lcd_str[1],"%2d.%2d %%",tmpHumid/10,tmpHumid%10);
//					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
//					curMenu=SettingManualHumidtyMenu;
				}
				if(TimerGoMainMenu==0)
				{
					LCD_clear_home();
					tik_1s=1;
					tik_switchmaindispaly=1;
					MainPageNumber=0;
					motor.mode=MotorAuto;
					curMenu=MainMenu;
				}				
			break;

				//SettingDay Menu
			case SettingBirdDayMenu:
				if(Keys[KEYUP].ShortPress || Keys[KEYUP].LongLongPress)
				{
					Keys[KEYUP].ShortPress=0;
					Keys[KEYUP].LongLongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					tmpTime.day++;
					if(tmpTime.day>=defaultBirds[tmpType].TotalDurationDays)
						tmpTime.day=0;
					if(IsHatcher(defaultBirds[tmpType],tmpTime))
						sprintf(lcd_str[1],"=%03d Hatcher",tmpTime.day);
					else
						sprintf(lcd_str[1],"=%03d Setter ",tmpTime.day);
					LCD_putstralign(lcd_str[1],0,1,AlignRight);
				}
				if(Keys[KEYDOWN].ShortPress || Keys[KEYDOWN].LongLongPress)
				{
					Keys[KEYDOWN].ShortPress=0;
					Keys[KEYDOWN].LongLongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					if(tmpTime.day==0)
						tmpTime.day=defaultBirds[tmpType].TotalDurationDays;						
					tmpTime.day--;
					if(IsHatcher(defaultBirds[tmpType],tmpTime))
						sprintf(lcd_str[1],"=%03d Hatcher",tmpTime.day);
					else
						sprintf(lcd_str[1],"=%03d Setter ",tmpTime.day);
					LCD_putstralign(lcd_str[1],0,1,AlignRight);
				}
				if(Keys[KEYSETTING].ShortPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=0;
					LCD_clear_home();
					LCD_putstralign("Saving...",0,0,AlignCenter);
					curTime.day=tmpTime.day;curTime.hr=0;curTime.min=0;curTime.sec=0;
					TimeSave(curTime,EE_ADD_CURTIME);
					TimeInit(&curTime);
					curBird.Type=tmpType;
					EEWriteByte((uint8_t *)&curBird.Type,1,EE_ADD_CURBIRDTYPE);
					BirdInit(&curBird);
				}
				if(TimerGoMainMenu==0)
				{
					LCD_clear_home();
					tik_1s=1;
					tik_switchmaindispaly=1;
					motor.mode=MotorAuto;
					curMenu=MainMenu;
					MainPageNumber=0;
				}				
				break;
			//Advance Menu//
			case AdvanceMenu:
				break;
		}
		#if 0
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
	#endif
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
