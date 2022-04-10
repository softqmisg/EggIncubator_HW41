#ifndef INC_MOTOR_H
#define INC_MOTOR_H
#include "main.h"
#include "user_time.h"
typedef enum {MotorOn,MotorOff} MotorState_t;
typedef struct{
	Time_t OnTime;
	Time_t OffTime;
	Time_t curTime;
	MotorState_t state;
} Motor_t;

void MotorInit(Motor_t *motor);
void MotorSave(Time_t t,uint16_t add);
extern Motor_t defaultMotor;
#endif