#include "heater_fan.h"
#include "user_eeprom.h"
#include "eeprom.h"
Fan_t defaultFan={
	.adjustFanHum=DEFAULT_ADJUST_FAN_HUM,
	.adjustFanTemp=DEFAULT_ADJUST_FAN_TEMP,
	.state=FanOff
};
Heater_t defaultHeater={
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
		HAL_GPIO_WritePin(FanExhaustOut_GPIO_Port,FanExhaustOut_Pin,GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(FanExhaustOut_GPIO_Port,FanExhaustOut_Pin,GPIO_PIN_SET);
}
void HeaterSave(Heater_t heater)
{
	EEWriteByte((uint8_t *)&heater.adjustHeaterTemp,2,EE_ADD_ADJUST_HEATER_TEMP);
	EEWriteByte((uint8_t *)&heater.adjustHeaterPwmp,2,EE_ADD_ADJUST_HEATER_PWMP);	
	EEWriteByte((uint8_t *)&heater.percent,2,EE_ADD_HEATER_PWM);	
}
void HeaterInit(Heater_t *heater)
{
	EEReadByte((uint8_t *)&(heater->adjustHeaterTemp),2,EE_ADD_ADJUST_HEATER_TEMP);
	EEReadByte((uint8_t *)&(heater->adjustHeaterPwmp),2,EE_ADD_ADJUST_HEATER_PWMP);	
	EEReadByte((uint8_t *)&(heater->percent),2,EE_ADD_HEATER_PWM);		
	HAL_TIM_PWM_Start(&HEATERTIMER, HEATERCHANNEL);//heater start

}
void HeaterSetPercent(uint16_t percent)
{
	__HAL_TIM_SetCompare(&HEATERTIMER,HEATERCHANNEL,percent);
}
