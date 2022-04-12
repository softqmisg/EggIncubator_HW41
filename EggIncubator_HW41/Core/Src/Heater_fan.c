#include "heater_fan.h"
#include "user_eeprom.h"
#include "eeprom.h"
Fan_t defaultFan={
	.adjustFanHum=DEFAULT_ADJUST_FAN_HUM,
	.adjustFanTemp=DEFAULT_ADJUST_FAN_TEMP,
	.state=FanOff
};
Heater_t defaultHeater={
	.upperLimitTemp=DEFAULT_UPPER_LIMIT_TEMP,
	.adjustHeaterPwmp=DEFAULT_ADJUST_HEATER_PWMP,
	.adjustHeaterTemp=DEFAULT_ADJUST_HEATER_TEMP,
	.percent=DEFAULT_HEATER_PWM
};

void FanSave(Fan_t fan)
{
	EEWriteByte((uint8_t *)&fan.adjustFanTemp,2,EE_ADD_ADJUST_FAN_TEMP);
	EEWriteByte((uint8_t *)&fan.adjustFanHum,2,EE_ADD_ADJUST_FAN_HUM);
}
void FanInit(Fan_t *fan)
{
	EEReadByte((uint8_t *)&(fan->adjustFanTemp),2,EE_ADD_ADJUST_FAN_TEMP);
	EEReadByte((uint8_t *)&(fan->adjustFanHum),2,EE_ADD_ADJUST_FAN_HUM);
	fan->state=FanOff;
}
void FanSetState(Fan_t *fan,FanState_t state)
{
	fan->state=state;
	if(state==FanOff)
	{
		HAL_GPIO_WritePin(FanExhaustOut_GPIO_Port,FanExhaustOut_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LedExhaust_GPIO_Port,LedExhaust_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(FanExhaustOut_GPIO_Port,FanExhaustOut_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LedExhaust_GPIO_Port,LedExhaust_Pin,GPIO_PIN_RESET);		
	}
}
void FanCheckTemp(Fan_t fan,int16_t setTemp,int16_t curTemp)
{
	static int16_t prev_temp=0;
	if(curTemp>=setTemp)
	{
		FanSetState(&fan,FanOn);
	}
	else if(setTemp-curTemp>=fan.adjustFanTemp)
	{
		FanSetState(&fan,FanOff);
	}
	else if(setTemp-curTemp<fan.adjustFanTemp)
	{
		if(curTemp-prev_temp>0)
		{
			FanSetState(&fan,FanOff);
		}
		else if(curTemp-prev_temp<0)
		{
			FanSetState(&fan,FanOn);
		}
	}
	prev_temp=curTemp;
}

void FanCheckHum(Fan_t fan,int16_t setHum,int16_t curHum)
{
	static int16_t prev_hum=0;
	if(curHum>=setHum)
	{
		FanSetState(&fan,FanOn);
	}
	else if(setHum-curHum>=fan.adjustFanHum)
	{
		FanSetState(&fan,FanOff);
	}
	else if(setHum-curHum<fan.adjustFanHum)
	{
		if(curHum-prev_hum>0)
		{
			FanSetState(&fan,FanOff);
		}
		else if(curHum-prev_hum<0)
		{
			FanSetState(&fan,FanOn);
		}
	}
	prev_hum=curHum;
}

void HeaterSave(Heater_t heater)
{
	EEWriteByte((uint8_t *)&heater.upperLimitTemp,2,EE_ADD_UPPER_LIMIT_TEMP);
	EEWriteByte((uint8_t *)&heater.adjustHeaterTemp,2,EE_ADD_ADJUST_HEATER_TEMP);
	EEWriteByte((uint8_t *)&heater.adjustHeaterPwmp,2,EE_ADD_ADJUST_HEATER_PWMP);	
	EEWriteByte((uint8_t *)&heater.percent,2,EE_ADD_HEATER_PWM);	
}
void HeaterInit(Heater_t *heater)
{
	EEReadByte((uint8_t *)&(heater->upperLimitTemp),2,EE_ADD_UPPER_LIMIT_TEMP);
	EEReadByte((uint8_t *)&(heater->adjustHeaterTemp),2,EE_ADD_ADJUST_HEATER_TEMP);
	EEReadByte((uint8_t *)&(heater->adjustHeaterPwmp),2,EE_ADD_ADJUST_HEATER_PWMP);	
	EEReadByte((uint8_t *)&(heater->percent),2,EE_ADD_HEATER_PWM);		
	HAL_TIM_PWM_Start(&HEATERTIMER, HEATERCHANNEL);//heater start

}
void HeaterSetPercent(uint16_t percent)
{
	__HAL_TIM_SetCompare(&HEATERTIMER,HEATERCHANNEL,percent);
	if(percent==0)
		HAL_GPIO_WritePin(LedHeater_GPIO_Port,LedHeater_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LedHeater_GPIO_Port,LedHeater_Pin,GPIO_PIN_RESET);
}

void HeaterCheck(Heater_t heater,int16_t setTemp,int16_t curTemp)
{
	if(curTemp>heater.upperLimitTemp)
	{
		HAL_GPIO_WritePin(HeaterRelay_GPIO_Port,HeaterRelay_Pin,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(HeaterRelay_GPIO_Port,HeaterRelay_Pin,GPIO_PIN_SET);
		if(curTemp>setTemp)
		{
			HeaterSetPercent(0);
		}
		else if(setTemp-curTemp<heater.adjustHeaterTemp)
		{
			HeaterSetPercent(heater.adjustHeaterPwmp);
		}
		else
		{
			HeaterSetPercent(heater.percent);
		}
	}
}