#ifndef INC_USER_TIME_H
#define INC_USER_TIME_H
#include "main.h"
typedef struct {
	uint16_t hr;
	uint8_t min;
	uint8_t sec;
	uint16_t day;
} Time_t;
void TimeSave(Time_t t,uint16_t add);
void TimeInit(Time_t *t);
void IncTime(Time_t *time,Time_t inc);
void IncTimesec(Time_t *time);
#endif