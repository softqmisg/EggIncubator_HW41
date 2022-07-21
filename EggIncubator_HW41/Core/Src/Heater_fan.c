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
		HAL_GPIO_WritePin(FanExhaustOut_GPIO_Port,FanExhaustOut_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LedExhaust_GPIO_Port,LedExhaust_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(FanExhaustOut_GPIO_Port,FanExhaustOut_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LedExhaust_GPIO_Port,LedExhaust_Pin,GPIO_PIN_RESET);		
	}
}
uint8_t FanStateTemp(Fan_t fan,int16_t setTemp,int16_t curTemp)
{
	static int8_t flag_temp=0;
	uint8_t r=0;
	int16_t u,l;
	if((int16_t)fan.adjustFanTemp>=0)
	{
		u=setTemp+fan.adjustFanTemp;
		l=setTemp;
	}
	else
	{
		u=setTemp;
		l=setTemp+fan.adjustFanTemp;
	}
	
	if(curTemp>=u)
	{
		r=1;
		flag_temp=1;
	}
	else if(curTemp<=l)
	{
		r=0;
		flag_temp=0;
	}
	else if(curTemp>l)
	{
		if(flag_temp==0)
		{
			r=0;
		}
		else if(flag_temp==1)
		{
			r=1;
		}
	}
	return r;

}
uint8_t  FanStateHum(Fan_t fan,int16_t setHum,int16_t curHum)
{
	static uint8_t flag_hum=0;
	uint8_t r=0;
	int16_t u,l;
	if((int16_t)fan.adjustFanHum>=0)
	{
		u=setHum+fan.adjustFanHum;
		l=setHum;
	}
	else
	{
		u=setHum;
		l=setHum+fan.adjustFanHum;
	}
	if(curHum>=u)
	{
		r=1;
		flag_hum=1;
	}
	else if(curHum<=l)
	{
		r=0;
		flag_hum=0;
	}
	else if(curHum>l)
	{
		if(flag_hum==0)
		{
			r=0;
		}
		else if(flag_hum==1)
		{
			r=1;
		}
	}
	return r;
}
void FanCheckTempHum(Fan_t fan,int16_t setTemp,int16_t curTemp,int16_t setHum,int16_t curHum)
{
	if(FanStateHum(fan,setHum,curHum) || FanStateTemp(fan,setTemp,curTemp) )
			FanSetState(&fan,FanOn);
	else
		FanSetState(&fan,FanOff);
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
	uint16_t prev_percent;
	prev_percent=1000-__HAL_TIM_GET_COMPARE(&HEATERTIMER,HEATERCHANNEL);
	if(prev_percent!=percent)
		__HAL_TIM_SET_COMPARE(&HEATERTIMER,HEATERCHANNEL,1000-percent);
	if(percent==0)
		HAL_GPIO_WritePin(LedHeater_GPIO_Port,LedHeater_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LedHeater_GPIO_Port,LedHeater_Pin,GPIO_PIN_RESET);
}

void HeaterCheck(Heater_t heater,int16_t setTemp,int16_t curTemp)
{
	int16_t u,l;

	if((int16_t)curTemp>(int16_t)heater.upperLimitTemp)
	{
		HAL_GPIO_WritePin(HeaterRelay_GPIO_Port,HeaterRelay_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(HeaterRelay_GPIO_Port,HeaterRelay_Pin,GPIO_PIN_RESET);
		if(curTemp>setTemp)
		{
			HeaterSetPercent(0);
		}
		else if((int16_t)curTemp>((int16_t)setTemp-heater.adjustHeaterTemp))
		{
			HeaterSetPercent(heater.adjustHeaterPwmp);
		}
		else
		{
			HeaterSetPercent(heater.percent);
		}
	}
}