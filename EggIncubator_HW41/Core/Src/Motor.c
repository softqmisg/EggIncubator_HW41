#include "Motor.h"
#include <string.h>
#include "user_eeprom.h"
#include "eeprom.h"
Motor_t defaultMotor={
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
	EEReadByte((uint8_t *)&(motor->OnTime),sizeof(Time_t),EE_ADD_MOTOR_ON);
	EEReadByte((uint8_t *)&(motor->OffTime),sizeof(Time_t),EE_ADD_MOTOR_OFF);
}
void MotorSave(Time_t t,uint16_t add)
{
		EEWriteByte((uint8_t *) &t,sizeof(Time_t),add);
}