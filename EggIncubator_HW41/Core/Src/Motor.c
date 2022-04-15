#include "Motor.h"
#include <string.h>
#include "user_eeprom.h"
#include "eeprom.h"
#include "user_time.h"
Motor_t defaultMotor={
	.mode=MotorAuto,
	.state=MotorOn,
	.OnTime={
		.sec=DEFAULT_MOTORON_SEC,
		.min=DEFAULT_MOTORON_MIN,
		.hr=DEFAULT_MOTORON_HR,
		.day=DEFAULT_MOTORON_DAY,
	},
	.OffTime={
		.sec=DEFAULT_MOTOROFF_SEC,
		.min=DEFAULT_MOTOROFF_MIN,
		.hr=DEFAULT_MOTOROFF_HR,
		.day=DEFAULT_MOTOROFF_DAY,
	},
	.curTime={
		.sec=0,
		.min=0,
		.hr=0,
		.day=0,
	}
};
void MotorInit(Motor_t *motor)
{
	memset(&(motor->curTime),0,sizeof(Time_t));
	motor->state=MotorOn;
	motor->mode=MotorAuto;
	EEReadByte((uint8_t *)&(motor->OnTime),sizeof(Time_t),EE_ADD_MOTOR_ON);
	EEReadByte((uint8_t *)&(motor->OffTime),sizeof(Time_t),EE_ADD_MOTOR_OFF);
}
void MotorSetState(Motor_t *motor,MotorState_t state)
{
	if(motor->mode==MotorAuto)
	{
		motor->state=state;
		if(motor->state==MotorOff)
				HAL_GPIO_WritePin(Motor23Out_GPIO_Port,Motor23Out_Pin,GPIO_PIN_RESET);
		else if(motor->state==MotorOn)
				HAL_GPIO_WritePin(Motor23Out_GPIO_Port,Motor23Out_Pin,GPIO_PIN_SET);
	}
	
}
void MotorCheck(Motor_t *motor)
{
	if(motor->mode==MotorAuto)
	{
		switch(motor->state)
		{
			case MotorOn:
				if(compareTime(motor->curTime,motor->OnTime)>=0)
				{
					HAL_GPIO_WritePin(Motor23Out_GPIO_Port,Motor12Out_Pin,GPIO_PIN_RESET);
					motor->state=MotorOff;
					motor->curTime.hr=0;motor->curTime.min=0;motor->curTime.sec=0;
				}
				else
				{
					HAL_GPIO_WritePin(Motor23Out_GPIO_Port,Motor12Out_Pin,GPIO_PIN_SET);
				}
			break;
			case MotorOff:
				if(compareTime(motor->curTime,motor->OffTime)>=0)
				{
					HAL_GPIO_WritePin(Motor23Out_GPIO_Port,Motor12Out_Pin,GPIO_PIN_SET);
					motor->state=MotorOn;
					motor->curTime.hr=0;motor->curTime.min=0;motor->curTime.sec=0;
				}
				else
				{
					
					HAL_GPIO_WritePin(Motor23Out_GPIO_Port,Motor12Out_Pin,GPIO_PIN_RESET);
				}
			break;
		}
	}
}