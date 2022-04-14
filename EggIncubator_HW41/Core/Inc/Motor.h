#ifndef INC_MOTOR_H
#define INC_MOTOR_H
#include "main.h"
#include "user_time.h"
typedef enum {MotorOn=0,MotorOff} MotorState_t;
typedef enum {MotorAuto=0,MotorManual} MotorMode_t;

typedef struct{
	Time_t OnTime;
	Time_t OffTime;
	Time_t curTime;
	MotorState_t state; 
	MotorMode_t mode;
} Motor_t;

void MotorInit(Motor_t *motor);
void MotorCheck(Motor_t *motor);
void MotorSetState(Motor_t *motor,MotorState_t state);
extern Motor_t defaultMotor;
#endif