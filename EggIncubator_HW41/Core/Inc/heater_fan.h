#ifndef INC_HEATER_FAN_H
#define INC_HEATER_FAN_H
#include "main.h"
#include "tim.h"
typedef enum{FanOn,FanOff} FanState_t;
typedef struct{
	uint16_t adjustFanTemp;
	uint16_t adjustFanHum;
	FanState_t state;
} Fan_t;

#define HEATERTIMER htim1
#define HEATERCHANNEL TIM_CHANNEL_2

typedef struct{
	uint16_t adjustHeaterTemp;
	uint16_t adjustHeaterPwmp;
	uint16_t percent;
} Heater_t;

void FanInit(Fan_t *fan);
void FanSetState(Fan_t *fan,FanState_t state);
void FanSave(Fan_t fan);

void HeaterInit(Heater_t *heater);
void HeaterSetPercent(uint16_t percent);
void HeaterSave(Heater_t heater);


extern Heater_t defaultHeater;
extern Fan_t defaultFan;

#endif