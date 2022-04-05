#ifndef __BUZZER_H_
#include "main.h"
#define	BUZZER_PORT	BuzzerOut_GPIO_Port
#define BUZZER_PIN	BuzzerOut_Pin

#define BUZZER_ON()	HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_SET);
#define BUZZER_OFF()	HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_RESET);

typedef struct{
	uint8_t active;
	uint16_t duration;//milisecond
	uint16_t delaybetweenrepeat;//milisecond
	uint8_t repeatstate;
	uint16_t counter_on;
	uint16_t counter_repeat;
} Buzzer_t;


void BuzzerOn(uint16_t duration);
void BuzzerRepeatStart(uint16_t duration,uint16_t delaybetweenrepeat);
void BuzzerRepeatStop();
extern Buzzer_t Buzzer;
#endif