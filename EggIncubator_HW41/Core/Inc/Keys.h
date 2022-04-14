#ifndef INC_KEYS_H_
#define INC_KEYS_H_
#include "main.h"
#define KEYSNUM	4

#define KEYUP_PORT				KeyUP_GPIO_Port
#define KEYUP_PIN					KeyUP_Pin

#define KEYDOWN_PORT			KeyDown_GPIO_Port
#define KEYDOWN_PIN				KeyDown_Pin

#define KEYSETTING_PORT		KeySetting_GPIO_Port
#define KEYSETTING_PIN		KeySetting_Pin

#define KEYLEDBUZZER_PORT	KeyLedBuzzer_GPIO_Port
#define KEYLEDBUZZER_PIN	KeyLedBuzzer_Pin

#define SHORTPRESS_MIN			3//*10 milisecond
#define SHORTPRESS_MAX			40//*10 milisecond

#define LONGPRESS_MIN				200//*10 milisecond
#define LONGLONGPRESS_MIN		20//*10 milisecond


#define KEYUP					0
#define KEYSETTING		1
#define KEYDOWN				2
#define KEYLEDBUZZER	3

typedef enum {Press=0,Release} KeyState_t;

typedef struct{
	KeyState_t	RawState;
	KeyState_t 	Prev_RawState;
	uint8_t ShortPress;
	uint8_t	LongPress;
	uint8_t LongLongPress;
	uint8_t counter;
	GPIO_TypeDef *port;
	uint16_t 			pin;
} Keys_t;

extern Keys_t Keys[KEYSNUM];
void KeysInit();
#endif