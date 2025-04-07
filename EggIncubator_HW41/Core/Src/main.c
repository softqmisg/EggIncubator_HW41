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
#include <stdlib.h>
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
typedef struct {
    const char *date;
    const char *time;
    const char *version;
} FirmwareInfo;

const FirmwareInfo firmware = {
    .date = __DATE__,
    .time = __TIME__,
    .version = "4.1.2"
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DELAY_GOMAINMENU	20//s
//#define DELAY_SWITCHMAINDISPLAY		3//s
uint8_t DELAY_SWITCHMAINDISPLAY[]={3,9,6};  //Bird,TempHum,TimeDay,,
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
volatile uint8_t tik_3s=0;
volatile uint8_t cnt_tik3s=0;
volatile uint8_t TimerGoMainMenu=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//call every 10ms
	if(htim->Instance==TIM16)
	{
		//Buzzer evaluate//
		if(Buzzer.active)
		{
			Buzzer.counterTime++;
			if(Buzzer.stateoffon) //buzzer ON
			{
				if(Buzzer.counterTime>Buzzer.rhythmProg[Buzzer.indexrhythm].onduration)
				{
					Buzzer.stateoffon=0;
					Buzzer.counterTime=0;
				}
				else
				{
					if(!Buzzer.mute)
						BUZZER_ON();
				}
			}
			else
			{
				if(Buzzer.counterTime>Buzzer.rhythmProg[Buzzer.indexrhythm].offduration)
				{
					Buzzer.indexrhythm++;
					if(Buzzer.indexrhythm>=Buzzer.numofrhythms)
					{
						Buzzer.indexrhythm=0;
						if(!Buzzer.repeatactive)
						{
							Buzzer.active=0;
						}
					}
					Buzzer.stateoffon=1;
					Buzzer.counterTime=0;
				}
				else
				{
					BUZZER_OFF();
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
		cnt_tik3s++;
		if(cnt_tik3s==3)
		{
			cnt_tik3s=0;
			tik_3s=1;
		}
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
char negstr[16];
char *negativStr(int16_t x)
{
	if(x>0)
		sprintf(negstr,"+%2d.%1d",x/10,x%10);
	else if(x==0)
		sprintf(negstr," %2d.%1d",x/10,x%10);
	else
		sprintf(negstr,"-%2d.%1d",(-x)/10,(-x)%10);
	return negstr;
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
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim16); //timer 10ms
	HAL_TIM_Base_Start_IT(&htim17);	//timer 1s
	
  HAL_GPIO_WritePin(LedHeater_GPIO_Port, LedHeater_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LedHumidity_GPIO_Port, LedHumidity_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LedExhaust_GPIO_Port, LedExhaust_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LedHatcher_GPIO_Port, LedHatcher_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LedCntlOut_GPIO_Port,LedCntlOut_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor12Out_GPIO_Port,Motor12Out_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor23Out_GPIO_Port,Motor23Out_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(HeaterRelay_GPIO_Port,HeaterRelay_Pin,GPIO_PIN_SET);
BirdLoadDefault();
	LCD_init();
	BuzzerInit();
	KeysInit();
	BuzzerSetRhythm(Rhythm1,0);
//	HAL_Delay(10000);
//	BuzzerSetRhythm(SilentRhythm,1);
//	HAL_Delay(5000);
//	BuzzerSetRhythm(Rhythm3,1);
//	HAL_Delay(10000);
//	BuzzerSetRhythm(SilentRhythm,1);

	char lcd_str[2][16];
	uint8_t tmp_uint8;
	uint16_t tmp_uint16;
	Time_t curTime={.sec=0,.min=0,.hr=0,.day=0};
	LCD_putstralign("Ghoghnoos Plus+",0,0,AlignCenter);
	LCD_putstralign((char *)firmware.version,0,1,AlignCenter);
	uint8_t firsttime=0;
	EEReadByte(&firsttime,1,EE_ADD_FIRST_TIME);
	HAL_Delay(200);
	EEReadByte(&firsttime,1,EE_ADD_FIRST_TIME);
	if((Keys[KEYUP].RawState==Press && Keys[KEYUP].RawState==Press)|| (firsttime!=0xA1))
	{
		HAL_Delay(800);
		if((Keys[KEYUP].RawState==Press && Keys[KEYUP].RawState==Press)|| (firsttime!=0xA1))
		{
			LCD_putstralign("Save Default...",0,1,AlignCenter);
			firsttime=0xA1;
			EEWriteByte(&firsttime,1,EE_ADD_FIRST_TIME);
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
	}
	KeysInit();
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
	HAL_Delay(3000);
	LCD_clear_home();
	///////////////////////////////////////////////////////////////
	HAL_IWDG_Init(&hiwdg);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	enum{MainMenu=0,
			SettingBirdMenu,
			SettingBirdDayMenu,
			SettingManualTemperatureMenu,SettingManualhumidityMenu,SettingManualDaysMenu,SettingManualHatchersMenu,
			PasswordMenu,
			AdvancedMenu,
			AdvanceMotorOnTimeMenu,AdvanceMotorOffTimeMenu,
			AdvanceAdjustFanTempMenu,AdvanceAdjustFanHumMenu,
			AdvanceAdjustHeaterTempMenu,AdvanceAdjustHeaterPwmMenu,
			AdvanceCalibShtTempMenu,AdvanceCalibShtHumMenu,
			} curMenu=MainMenu;
	uint8_t counter_switchmaindispaly=0,tik_switchmaindispaly=1,MainPageNumber=0;
	uint8_t tmpType,tmpProg;
	Time_t tmpTime;
	int16_t tmpTemp,tmpHumid,tmpDay;
	int16_t tmpInt16;
	char tmpIsHatch;
	uint8_t indexpos;
	char tmppass[5];
	uint8_t sensor_error=0;
	BuzzerSetRhythm(SilentRhythm,1);

  while (1)
  {
		HAL_IWDG_Refresh(&hiwdg);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		switch(curMenu)
		{
			//Main Menu//
			case MainMenu:
				if(tik_1s)
				{
					tik_1s=0;

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

					if(tik_3s)
					{
						tik_3s=0;
						ShtReadSensor(&sht20);
					}
					if(!sht20.error)
					{
						FanCheckTempHum(fan,curBird.pProgs[curProg].temperature,sht20.temperature,curBird.pProgs[curProg].humidity,sht20.humidity);
						HeaterCheck(heater,curBird.pProgs[curProg].temperature,sht20.temperature);
						if(sht20.humidity<curBird.pProgs[curProg].humidity)
						{
							HAL_GPIO_WritePin(HumidityOut_GPIO_Port,HumidityOut_Pin,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(LedHumidity_GPIO_Port,LedHumidity_Pin,GPIO_PIN_RESET);
						}
						else
						{
							HAL_GPIO_WritePin(HumidityOut_GPIO_Port,HumidityOut_Pin,GPIO_PIN_SET);
							HAL_GPIO_WritePin(LedHumidity_GPIO_Port,LedHumidity_Pin,GPIO_PIN_SET);							
						}
						///check for alarms///
						if(sht20.temperature<(curBird.pProgs[curProg].temperature-DEFAULT_BUZZER_TEMP))
							BuzzerSetRhythm(Rhythm1,1);
						else if(sht20.temperature>(curBird.pProgs[curProg].temperature+DEFAULT_BUZZER_TEMP))
							BuzzerSetRhythm(Rhythm3,1);
						else if(sht20.humidity<(curBird.pProgs[curProg].humidity-DEFAULT_BUZZER_HUM))
							BuzzerSetRhythm(Rhythm1,1);
						else if(sht20.humidity>(curBird.pProgs[curProg].humidity+DEFAULT_BUZZER_HUM))
							BuzzerSetRhythm(Rhythm3,1);
						else
							BuzzerSetRhythm(SilentRhythm,1);
						//////////////////////////
						if(sensor_error)
						{
							sensor_error=0;
							tik_switchmaindispaly=1;
							MainPageNumber=0;
						}
					}
					else
					{
						HeaterSetPercent(0);
						FanSetState(&fan,FanOff);						
						LCD_clear_home();
						LCD_putstralign("SENSOR ERROR",0,0,AlignCenter);
						sensor_error=1;
						MainPageNumber=3;
						BuzzerSetRhythm(Rhythm2,1);
					}
					//main menu tik//
					counter_switchmaindispaly++;
					if(counter_switchmaindispaly>DELAY_SWITCHMAINDISPLAY[MainPageNumber])
					{
						counter_switchmaindispaly=0;
						tik_switchmaindispaly=1;
					}	
					//MainPageNumber=2;
					switch(MainPageNumber)
					{
						case 0:
							LCD_putstralign("Bird",0,0,AlignCenter);
							LCD_putpersian((uint8_t)curBird.Type,0,1,AlignCenter);
							if(tik_switchmaindispaly)
							{
								tik_switchmaindispaly=0;
								MainPageNumber=1;
								LCD_clear_home();
								
							}
							break;
						case 1:
							if(sht20.error)
								{
								}
								else
								{
									sprintf(lcd_str[0],"=%3d.%1d %cC",sht20.temperature/10,sht20.temperature%10,DEGREE_CH_CODE);
									sprintf(lcd_str[1],"=%3d.%1d %%",sht20.humidity/10,sht20.humidity%10);
									//sprintf(lcd_str[1],"=%3d.%1d %cC",curBird.pProgs[curProg].temperature/10,curBird.pProgs[curProg].temperature%10,DEGREE_CH_CODE);
									LCD_putpersian(DAMA_STR,3,0,AlignNone);
									LCD_putstralign(lcd_str[0],7,0,AlignNone);
									LCD_putpersian(ROTOBAT_STR,1,1,AlignNone);
									LCD_putstralign(lcd_str[1],7,1,AlignNone);
									if(tik_switchmaindispaly)
									{
										tik_switchmaindispaly=0;
										MainPageNumber=2;
										LCD_clear_home();
										
									}
								}
							break;
						case 2:
							LCD_putpersian(ZAMAN_STR,2,0,AlignNone);
							sprintf(lcd_str[0],"= %02d:%02d  ",curTime.hr,curTime.min);
						//sprintf(lcd_str[0],"=%02d:%02d:%02d",curTime.hr,curTime.min,curTime.sec);
							LCD_putstralign(lcd_str[0],7,0,AlignNone);

							LCD_putpersian(ROOZ_STR,2,1,AlignNone);
							sprintf(lcd_str[0],"= %3d",curTime.day);
							LCD_putstralign(lcd_str[0],7,1,AlignNone);
							if(tik_switchmaindispaly)
							{
								tik_switchmaindispaly=0;
								MainPageNumber=0;
								LCD_clear_home();
								
							}						
							break;
						case 3:

							break;
					}
				}
				//Check Keys
				//Up key:Motor rotation
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
					if(sht20.error)
						sprintf(lcd_str[1],"T=----  H=----");
					else
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
				//LEDBUZZER key:Lamp On Off
				if(Keys[KEYLEDBUZZER].ShortPress)
				{
					Keys[KEYLEDBUZZER].ShortPress=0;
					HAL_GPIO_TogglePin(LedCntlOut_GPIO_Port,LedCntlOut_Pin);
				}
				//LEDBUZZER key long:Buzzer On Off
				if(Keys[KEYLEDBUZZER].LongPress)
				{
					Keys[KEYLEDBUZZER].LongPress=0;
					LCD_clearrow(0);
					if(Buzzer.mute)
					{
						Buzzer.mute=0;
						LCD_putstralign("BUZZER ON",0,0,AlignCenter);
					}
					else
					{
						Buzzer.mute=1;
						LCD_putstralign("BUZZER OFF",0,0,AlignCenter);						
					}
					HAL_Delay(1000);
					LCD_clear_home();
				}				
				// both Key Setting and Down:Password menu
				if(Keys[KEYDOWN].LongPress && Keys[KEYUP].LongPress)
				{
					Keys[KEYUP].LongPress=Keys[KEYUP].LongLongPress=0;
					Keys[KEYDOWN].LongPress=Keys[KEYDOWN].LongLongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;

					HeaterSetPercent(0);
					FanSetState(&fan,FanOff);
					motor.mode=MotorManual;	MotorSetState(&motor,MotorOff);
					HAL_GPIO_WritePin(HumidityOut_GPIO_Port,HumidityOut_Pin,GPIO_PIN_SET);
					HAL_GPIO_WritePin(LedHumidity_GPIO_Port,LedHumidity_Pin,GPIO_PIN_SET);		
					HAL_GPIO_WritePin(LedHatcher_GPIO_Port,LedHatcher_Pin,GPIO_PIN_SET);

					LCD_clear_home();
					LCD_putstralign("PASSWORD",0,0,AlignCenter);
					sprintf(tmppass,"0000");
					LCD_putstralign(tmppass,0,1,AlignCenter);
					indexpos=0;
					LCD_cursor_on();
					LCD_gotoxy(indexpos+6,1);
					Keys[KEYSETTING].LongPress=Keys[KEYSETTING].LongLongPress=0;
					Keys[KEYDOWN].LongPress=Keys[KEYDOWN].LongLongPress=0;
					curMenu=PasswordMenu;
				}
				//
				if(Keys[KEYSETTING].ShortPress )
				{
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					
					LCD_clear_home();
					LCD_putstrpos("Bird Select",1,0);
					LCD_putstrpos("=>",1,1);
					tmpType=(uint8_t)curBird.Type;
					LCD_putpersian((uint8_t)tmpType,0,1,AlignCenter);
					
					HeaterSetPercent(0);
					FanSetState(&fan,FanOff);
					motor.mode=MotorManual;	MotorSetState(&motor,MotorOff);
					HAL_GPIO_WritePin(HumidityOut_GPIO_Port,HumidityOut_Pin,GPIO_PIN_SET);
					HAL_GPIO_WritePin(LedHumidity_GPIO_Port,LedHumidity_Pin,GPIO_PIN_SET);		
					HAL_GPIO_WritePin(LedHatcher_GPIO_Port,LedHatcher_Pin,GPIO_PIN_SET);
					
					curMenu=SettingBirdMenu;
				}
				break;
			//Password Menu//
			case PasswordMenu:
				if(Keys[KEYUP].ShortPress||Keys[KEYUP].LongLongPress)
				{
					Keys[KEYUP].ShortPress=0;Keys[KEYUP].LongLongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					tmppass[indexpos]++;
					if(tmppass[indexpos]>'9') tmppass[indexpos]='0';
					LCD_putstralign(tmppass,0,1,AlignCenter);
					LCD_gotoxy(indexpos+6,1);					

				}
				if(Keys[KEYDOWN].ShortPress||Keys[KEYDOWN].LongLongPress)
				{
					Keys[KEYDOWN].ShortPress=0;Keys[KEYDOWN].LongLongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					if(tmppass[indexpos]=='0') tmppass[indexpos]='9'+1;
					tmppass[indexpos]--;
					LCD_putstralign(tmppass,0,1,AlignCenter);
					LCD_gotoxy(indexpos+6,1);					
				}				
				if(Keys[KEYSETTING].ShortPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					indexpos++;
//					LCD_putstralign(tmppass,0,1,AlignCenter);

					if(indexpos>3)
					{
						LCD_cursor_off();
						if(!strcmp(tmppass,DEFAULT_PASSWORD))
						{
							LCD_clear_home();
							LCD_putstralign("Motor On",0,0,AlignCenter);
							sprintf(lcd_str[1],"%02d:%02d:%02d",motor.OnTime.hr,motor.OnTime.min,motor.OnTime.sec);
							LCD_putstralign(lcd_str[1],0,1,AlignCenter);
							indexpos=0;
							curMenu=AdvancedMenu;
						}
						else
						{
							LCD_putstralign("Wrong",0,1,AlignCenter);
							HAL_Delay(2000);
							TimerGoMainMenu=0;
						}
					}
					else
					{
							LCD_gotoxy(indexpos+6,1);								
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
				//Advance Menu:selectItem
			case AdvancedMenu: 
				if(Keys[KEYUP].ShortPress || Keys[KEYDOWN].ShortPress)
				{
					TimerGoMainMenu=DELAY_GOMAINMENU;
					if(Keys[KEYUP].ShortPress)
					{
						Keys[KEYUP].ShortPress=0;
						indexpos++;
						if(indexpos>8)
							indexpos=0;
					}
					else
					{
						Keys[KEYDOWN].ShortPress=0;
						if(indexpos==0)
							indexpos=9;
						indexpos--;
					}
					LCD_clear_home();
					switch(indexpos)
					{
						case 0:
							LCD_putstralign("Motor On",0,0,AlignCenter);
							sprintf(lcd_str[1],"%02d:%02d:%02d",motor.OnTime.hr,motor.OnTime.min,motor.OnTime.sec);
						break;
						case 1:
							LCD_putstralign("Motor Off",0,0,AlignCenter);
							sprintf(lcd_str[1],"%02d:%02d:%02d",motor.OffTime.hr,motor.OffTime.min,motor.OffTime.sec);
						break;
						case 2:
							LCD_putstralign("Adj Fan Temp",0,0,AlignCenter);
							sprintf(lcd_str[1],"%s %cC",negativStr(fan.adjustFanTemp),DEGREE_CH_CODE);//							sprintf(lcd_str[1],"%+3d.%1d %cC",fan.adjustFanTemp/10,abs(fan.adjustFanTemp%10),DEGREE_CH_CODE);
						break;
						case 3:
							LCD_putstralign("Adj Fan HUM",0,0,AlignCenter);
							sprintf(lcd_str[1],"%s %%",negativStr(fan.adjustFanHum));//sprintf(lcd_str[1],"%+3d.%1d %%",fan.adjustFanHum/10,abs(fan.adjustFanHum%10));
						break;
						case 4:
							LCD_putstralign("Adj Heat Temp",0,0,AlignCenter);
							sprintf(lcd_str[1],"%s %cC",negativStr( heater.adjustHeaterTemp),DEGREE_CH_CODE);//sprintf(lcd_str[1],"%+3d.%1d %cC",heater.adjustHeaterTemp/10,abs(heater.adjustHeaterTemp%10),DEGREE_CH_CODE);
						break;
						case 5:
							LCD_putstralign("Adj Heat PWM",0,0,AlignCenter);
							sprintf(lcd_str[1],"%3d.0 %%",heater.adjustHeaterPwmp/10);
						break;
						case 6:
							LCD_putstralign("Calib SHT Temp",0,0,AlignCenter);
							sprintf(lcd_str[1],"%s %cC",negativStr(sht20.calibTemp),DEGREE_CH_CODE);//sprintf(lcd_str[1],"%+3d.%1d %cC",sht20.calibTemp/10,abs(sht20.calibTemp),DEGREE_CH_CODE);
						break;
						case 7:
							LCD_putstralign("Calib SHT HUM",0,0,AlignCenter);
							sprintf(lcd_str[1],"%s %%",negativStr(sht20.calibHum));//sprintf(lcd_str[1],"%+3d.%1d %%",sht20.calibHum/10,abs(sht20.calibHum%10));
						break;
						case 8:
							LCD_putstralign("EXIT",0,0,AlignCenter);
							LCD_clearrow(1);
							sprintf(lcd_str[1]," ");//sprintf(lcd_str[1],"%+3d.%1d %%",sht20.calibHum/10,abs(sht20.calibHum%10));

						break;
					}
					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
				}
				if(Keys[KEYSETTING].ShortPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					switch(indexpos)
					{
						case 0://Motor OnTime
							tmpTime=motor.OnTime;
							indexpos=0;
							LCD_cursor_on();
							LCD_gotoxy(5+indexpos*3,1);
							curMenu=AdvanceMotorOnTimeMenu;
						break;
						case 1://Motor Off Time
							tmpTime=motor.OffTime;
							indexpos=0;
							LCD_cursor_on();
							LCD_gotoxy(5+indexpos*3,1);
							curMenu=AdvanceMotorOffTimeMenu;						
							break;
						case 2://Adjust Fan Temp
							tmpInt16=fan.adjustFanTemp;
							LCD_cursor_on();
							LCD_gotoxy(8,1);
							curMenu=AdvanceAdjustFanTempMenu;
							break;
						case 3://Adjust Fan Hum
							tmpInt16=fan.adjustFanHum;
							LCD_cursor_on();
							LCD_gotoxy(8,1);
							curMenu=AdvanceAdjustFanHumMenu;							
							break;
						case 4://Adjust Heater Temp
							tmpInt16=heater.adjustHeaterTemp;
							LCD_cursor_on();
							LCD_gotoxy(8,1);
							curMenu=AdvanceAdjustHeaterTempMenu;							
						break;
						case 5://Adjust Heater PWM
							tmpInt16=heater.adjustHeaterPwmp;
							LCD_cursor_on();
							LCD_gotoxy(8,1);
							curMenu=AdvanceAdjustHeaterPwmMenu;	
						break;
						case 6://Calib sht Temp
							tmpInt16=sht20.calibTemp;
							LCD_cursor_on();
							LCD_gotoxy(8,1);
							curMenu=AdvanceCalibShtTempMenu;	
						break;
						case 7://Calib sht hum
							tmpInt16=sht20.calibHum;
							LCD_cursor_on();
							LCD_gotoxy(8,1);
							curMenu=AdvanceCalibShtHumMenu;	
						break;
						case 8://exit
							TimerGoMainMenu=0;
						break;
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
				//Advance Menu:Adjust Temp && Humid for Fan on/off
				case AdvanceAdjustFanTempMenu:
				case AdvanceAdjustFanHumMenu:
				case AdvanceAdjustHeaterTempMenu:
				case AdvanceAdjustHeaterPwmMenu:
				case AdvanceCalibShtTempMenu:
				case AdvanceCalibShtHumMenu:
				if(Keys[KEYUP].ShortPress||Keys[KEYUP].LongLongPress)
				{
					if(Keys[KEYUP].ShortPress)
					{
						Keys[KEYUP].ShortPress=0;
						if(curMenu==AdvanceAdjustHeaterPwmMenu)
							tmpInt16+=10;
						else
							tmpInt16++;
					}
					else
					{
						Keys[KEYUP].LongLongPress=0;
						tmpInt16+=10;
					}
					TimerGoMainMenu=DELAY_GOMAINMENU;
					
					switch(curMenu)
					{
						case AdvanceAdjustFanTempMenu:
						case AdvanceAdjustHeaterTempMenu:
						case AdvanceCalibShtTempMenu:
							if(tmpInt16>TEMP_ADJUST_UPPER_LIMIT) tmpInt16=TEMP_ADJUST_UPPER_LIMIT;
							sprintf(lcd_str[1],"%s %cC",negativStr(tmpInt16),DEGREE_CH_CODE);
						break;
						case AdvanceCalibShtHumMenu:
						case AdvanceAdjustFanHumMenu:
							if(tmpInt16>HUM_ADJUST_UPPER_LIMIT) tmpInt16=HUM_ADJUST_UPPER_LIMIT;
							sprintf(lcd_str[1],"%s %%",negativStr(tmpInt16));
						break;
						case AdvanceAdjustHeaterPwmMenu:
							if(tmpInt16>1000) tmpInt16=1000;
							sprintf(lcd_str[1],"%3d.0 %%",tmpInt16/10);
						break;
					}
					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
					LCD_gotoxy(8,1);
				}

				if(Keys[KEYDOWN].ShortPress||Keys[KEYDOWN].LongLongPress)
				{
					if(Keys[KEYDOWN].ShortPress)
					{
						Keys[KEYDOWN].ShortPress=0;
						if(curMenu==AdvanceAdjustHeaterPwmMenu)
							tmpInt16-=10;
						else
							tmpInt16--;
					}
					else
					{
						Keys[KEYDOWN].LongLongPress=0;
						tmpInt16-=10;
					}
					TimerGoMainMenu=DELAY_GOMAINMENU;
					switch(curMenu)
					{
						case AdvanceAdjustFanTempMenu:
						case AdvanceAdjustHeaterTempMenu:
						case AdvanceCalibShtTempMenu:
							if(tmpInt16<TEMP_ADJUST_LOWER_LIMIT) tmpInt16=TEMP_ADJUST_LOWER_LIMIT;
							sprintf(lcd_str[1],"%s %cC",negativStr(tmpInt16),DEGREE_CH_CODE);
						break;
						case AdvanceCalibShtHumMenu:
						case AdvanceAdjustFanHumMenu:
							if(tmpInt16<HUM_ADJUST_LOWER_LIMIT) tmpInt16=HUM_ADJUST_LOWER_LIMIT;
							sprintf(lcd_str[1],"%s %%",negativStr(tmpInt16));
						break;
						case AdvanceAdjustHeaterPwmMenu:
							if(tmpInt16<0) tmpInt16=0;
							sprintf(lcd_str[1],"%3d.0 %%",tmpInt16/10);
						break;
					}
					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
					LCD_gotoxy(8,1);
				}
				if(Keys[KEYSETTING].ShortPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=0;
					LCD_cursor_off();
					switch(curMenu)
					{
						case AdvanceAdjustFanTempMenu:
								fan.adjustFanTemp=tmpInt16;
								FanSave(fan);
							break;
						case AdvanceAdjustFanHumMenu:
								fan.adjustFanHum=tmpInt16;
								FanSave(fan);
							break;
						case AdvanceAdjustHeaterTempMenu:
								heater.adjustHeaterTemp=tmpInt16;
								HeaterSave(heater);
						break;
						case AdvanceAdjustHeaterPwmMenu:
								heater.adjustHeaterPwmp =tmpInt16;
								HeaterSave(heater);							
							break;
						case AdvanceCalibShtTempMenu:
							sht20.calibTemp=tmpInt16;
							ShtSave(sht20);
							break;
						case AdvanceCalibShtHumMenu:
							sht20.calibHum=tmpInt16;
							ShtSave(sht20);
						break;
					}
				}				
				
				if(TimerGoMainMenu==0)
				{
						LCD_cursor_off();

					switch(curMenu)
					{
						case AdvanceAdjustFanTempMenu:
							sprintf(lcd_str[1],"%s %cC",negativStr(fan.adjustFanTemp),DEGREE_CH_CODE);//							sprintf(lcd_str[1],"%+3d.%1d %cC",fan.adjustFanTemp/10,abs(fan.adjustFanTemp%10),DEGREE_CH_CODE);
						indexpos=2;
						break;
						case AdvanceAdjustFanHumMenu:
							sprintf(lcd_str[1],"%s %%",negativStr(fan.adjustFanHum));//sprintf(lcd_str[1],"%+3d.%1d %%",fan.adjustFanHum/10,abs(fan.adjustFanHum%10));

						indexpos=3;
						break;
						case AdvanceAdjustHeaterTempMenu:
							sprintf(lcd_str[1],"%s %cC",negativStr( heater.adjustHeaterTemp),DEGREE_CH_CODE);//sprintf(lcd_str[1],"%+3d.%1d %cC",heater.adjustHeaterTemp/10,abs(heater.adjustHeaterTemp%10),DEGREE_CH_CODE);

						indexpos=4;
						break;
						case AdvanceAdjustHeaterPwmMenu:
							sprintf(lcd_str[1],"%3d.0 %%",heater.adjustHeaterPwmp/10);
						indexpos=5;
						break;
						case AdvanceCalibShtTempMenu:
							sprintf(lcd_str[1],"%s %cC",negativStr(sht20.calibTemp),DEGREE_CH_CODE);//sprintf(lcd_str[1],"%+3d.%1d %cC",sht20.calibTemp/10,abs(sht20.calibTemp),DEGREE_CH_CODE);
						indexpos=6;
						break;
						case AdvanceCalibShtHumMenu:
							sprintf(lcd_str[1],"%s %%",negativStr(sht20.calibHum));//sprintf(lcd_str[1],"%+3d.%1d %%",sht20.calibHum/10,abs(sht20.calibHum%10));
						indexpos=7;
						break;
					}
					LCD_putstralign(lcd_str[1],0,1,AlignCenter);

					TimerGoMainMenu=DELAY_GOMAINMENU;
					curMenu=AdvancedMenu;
				}					
					break;
				//Advance Menu:Set Motor On Off Time 
			case AdvanceMotorOnTimeMenu:
			case AdvanceMotorOffTimeMenu:
				if(Keys[KEYUP].ShortPress||Keys[KEYUP].LongLongPress)
				{
					Keys[KEYUP].ShortPress=0;Keys[KEYUP].LongLongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					switch(indexpos)
					{
						case 0:
							tmpTime.hr++;
							if(tmpTime.hr>99) tmpTime.hr=99;
							break;
						case 1:
							tmpTime.min++;
							if(tmpTime.min>59) tmpTime.min=59;
						break;
						case 2:
							tmpTime.sec++;
							if(tmpTime.sec>59) tmpTime.sec=59;
						break;
					}
					sprintf(lcd_str[1],"%02d:%02d:%02d",tmpTime.hr,tmpTime.min,tmpTime.sec);
					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
					LCD_gotoxy(5+indexpos*3,1);
					
				}
				if(Keys[KEYDOWN].ShortPress||Keys[KEYDOWN].LongLongPress)
				{
					Keys[KEYDOWN].ShortPress=0;Keys[KEYDOWN].LongLongPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					switch(indexpos)
					{
						case 0:
							if(tmpTime.hr==0) tmpTime.hr=1;
							tmpTime.hr--;
							break;
						case 1:
							if(tmpTime.min==0) tmpTime.min=1;
							tmpTime.min--;
						break;
						case 2:
							if(tmpTime.sec==0) tmpTime.sec=1;
							tmpTime.sec--;
						break;
					}
					sprintf(lcd_str[1],"%02d:%02d:%02d",tmpTime.hr,tmpTime.min,tmpTime.sec);
					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
					LCD_gotoxy(5+indexpos*3,1);
				}
				if(Keys[KEYSETTING].ShortPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					indexpos++;
					if(indexpos>2)
					{
						TimerGoMainMenu=0;
						if(curMenu==AdvanceMotorOnTimeMenu)
						{
							memcpy(&motor.OnTime,&tmpTime,sizeof(Time_t));
							TimeSave(motor.OnTime,EE_ADD_MOTOR_ON);
						}
						else
						{
							memcpy(&motor.OffTime,&tmpTime,sizeof(Time_t));
							TimeSave(motor.OffTime,EE_ADD_MOTOR_OFF);
						}
						LCD_cursor_off();
					}
					else
					{
						LCD_gotoxy(5+indexpos*3,1);
					}
				}				

				if(TimerGoMainMenu==0)
				{
						LCD_cursor_off();
					if(curMenu==AdvanceMotorOnTimeMenu)
					{
						sprintf(lcd_str[1],"%02d:%02d:%02d",motor.OnTime.hr,motor.OnTime.min,motor.OnTime.sec);
						indexpos=0;
					}
					else
					{
						sprintf(lcd_str[1],"%02d:%02d:%02d",motor.OffTime.hr,motor.OffTime.min,motor.OffTime.sec);
						indexpos=1;
					}
					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
				
					TimerGoMainMenu=DELAY_GOMAINMENU;
					curMenu=AdvancedMenu;
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
					if(tmpType==(uint8_t)Manual)
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
					tmpHumid=defaultBirds[Manual].pProgs[tmpProg].humidity;
					LCD_clear_home();
					LCD_putpersian(ROTOBAT_STR,0,0,AlignCenter);						
					LCD_putstrpos("=>",1,1);
					sprintf(lcd_str[1],"%2d.%1d %%",tmpHumid/10,tmpHumid%10);
					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
					curMenu=SettingManualhumidityMenu;
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
			case SettingManualhumidityMenu:
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
//					Keys[KEYSETTING].ShortPress=0;
//					TimerGoMainMenu=DELAY_GOMAINMENU;
//					tmpDay=defaultBirds[Manual].pProgs[tmpProg].durationDays;
//					LCD_clear_home();
//					LCD_putpersian(ROOZ_STR,0,0,AlignCenter);						
//					LCD_putstrpos("=>",1,1);
//					sprintf(lcd_str[1],"%3d",tmpDay);
//					LCD_putstralign(lcd_str[1],0,1,AlignCenter);
//					curMenu=SettingManualDaysMenu;
					
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=DELAY_GOMAINMENU;
					tmpIsHatch=(tmpProg==0)?'S':'H';
					LCD_clear_home();
					LCD_putpersian(DOOREH_STR,0,0,AlignCenter);						
					LCD_putstrpos("=>",1,1);
					LCD_putpersian((tmpIsHatch=='S')?SETTER_STR:HATCHER_STR,0,1,AlignCenter);
					curMenu=SettingManualHatchersMenu;					
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
				//Setting Manual:Hatcher/Setter
			case SettingManualHatchersMenu:
				if(Keys[KEYUP].ShortPress || Keys[KEYUP].LongLongPress)
				{
						Keys[KEYUP].ShortPress=0;
						Keys[KEYUP].LongLongPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
						tmpIsHatch=(tmpIsHatch=='H')?'S':'H';
						LCD_putpersian((tmpIsHatch=='S')?SETTER_STR:HATCHER_STR,0,1,AlignCenter);
				}
				if(Keys[KEYDOWN].ShortPress ||Keys[KEYDOWN].LongLongPress)
				{
						Keys[KEYDOWN].ShortPress=0;
						Keys[KEYDOWN].LongLongPress=0;
						TimerGoMainMenu=DELAY_GOMAINMENU;
						tmpIsHatch=(tmpIsHatch=='H')?'S':'H';
						LCD_putpersian((tmpIsHatch=='S')?SETTER_STR:HATCHER_STR,0,1,AlignCenter);
				}
				if(Keys[KEYSETTING].ShortPress)
				{
					Keys[KEYSETTING].ShortPress=0;
					TimerGoMainMenu=0;
					LCD_clear_home();
					LCD_putstralign("Saving...",0,0,AlignCenter);
					curTime.day=0;curTime.hr=0;curTime.min=0;curTime.sec=0;
					TimeSave(curTime,EE_ADD_CURTIME);
					TimeInit(&curTime);
					curBird.Type=Manual;
					curBird.TotalDurationDays=DAY_UPPER_VALUE;
					if(tmpIsHatch=='S')
					{
						curBird.HatchTotalDays=0;
						curBird.pProgs[0].durationDays=DAY_UPPER_VALUE;
						curBird.pProgs[0].temperature=tmpTemp;
						curBird.pProgs[0].humidity=tmpHumid;
						curBird.pProgs[1].durationDays=0;
					}
					else
					{
						curBird.HatchTotalDays=DAY_UPPER_VALUE;
						curBird.pProgs[1].durationDays=DAY_UPPER_VALUE;
						curBird.pProgs[1].temperature=tmpTemp;
						curBird.pProgs[1].humidity=tmpHumid;	
						curBird.pProgs[0].durationDays=0;							
					}
					
					EEWriteByte((uint8_t *)&curBird.Type,1,EE_ADD_CURBIRDTYPE);
					BirdSaveManual(curBird);
					BirdInit(&curBird);
					///checking type
//					sprintf(lcd_str[0],"%d,%d ",curBird.Type,tmpType);
//					LCD_putstrpos(lcd_str[0],0,0);
//					HAL_Delay(3500);
					///
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
					curBird.Type=(BirdType_t)tmpType;
					MotorInit(&motor); 
					EEWriteByte((uint8_t *)&curBird.Type,1,EE_ADD_CURBIRDTYPE);HAL_Delay(50);
					BirdInit(&curBird);
					////checking type
//					sprintf(lcd_str[0],"%d,%d ",curBird.Type,tmpType);
//					LCD_putstrpos(lcd_str[0],0,0);
//					sprintf(lcd_str[1],"%d ",curBird.pProgs[0].temperature);
//					LCD_putstrpos(lcd_str[1],0,1);
//					
//					HAL_Delay(3500);
					////

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
