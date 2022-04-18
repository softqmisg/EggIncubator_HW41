#ifndef INC_HEATER_FAN_H
#define INC_HEATER_FAN_H
#include "main.h"
#include "tim.h"
typedef enum{FanOn,FanOff} FanState_t;
typedef struct{
	int16_t adjustFanTemp;
	int16_t adjustFanHum;
	FanState_t state;
} Fan_t;

#define HEATERTIMER htim1
#define HEATERCHANNEL TIM_CHANNEL_2

typedef struct{
	int16_t upperLimitTemp;
	int16_t adjustHeaterTemp;
	int16_t adjustHeaterPwmp;
	uint16_t percent;
} Heater_t;

void FanInit(Fan_t *fan);
void FanSetState(Fan_t *fan,FanState_t state);
void FanSave(Fan_t fan);
//void FanCheckTemp(Fan_t fan,int16_t setTemp,int16_t curTemp);
//void FanCheckHum(Fan_t fan,int16_t setHum,int16_t curHum);
void FanCheckTempHum(Fan_t fan,int16_t setTemp,int16_t curTemp,int16_t setHum,int16_t curHum);


void HeaterInit(Heater_t *heater);
void HeaterSetPercent(uint16_t percent);
void HeaterSave(Heater_t heater);
void HeaterCheck(Heater_t heater,int16_t setTemp,int16_t curTemp);

extern Heater_t defaultHeater;
extern Fan_t defaultFan;

#endif