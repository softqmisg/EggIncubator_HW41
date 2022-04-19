#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_
#include "main.h"
#define	BUZZER_PORT	BuzzerOut_GPIO_Port
#define BUZZER_PIN	BuzzerOut_Pin

#define BUZZER_ON()	HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_SET);
#define BUZZER_OFF()	HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_RESET);

// Rythm1			1 //100ms ON,2000ms OFF
//	Rythm2			2	//50ms ON,30ms OFF,50ms ON,50ms OFF,200ms ON,500ms OFF
//	Rythm3			3	//100ms ON,500ms OFF
typedef enum{SilentRhythm=0,Rhythm1,Rhythm2,Rhythm3} RhythmTypes_t;
typedef struct {
	uint16_t onduration;//ms
	uint16_t offduration;//ms
} Rhythm_t;
typedef struct{
	Rhythm_t *rhythmProg;
	RhythmTypes_t type;
	uint8_t numofrhythms;
	uint16_t indexrhythm;
	uint16_t counterTime;
	uint8_t stateoffon;
	uint8_t mute;
	uint8_t repeatactive;
	uint8_t active;
} Buzzer_t;

void BuzzerSetRhythm(RhythmTypes_t type,uint8_t repeat);
void BuzzerMute(uint8_t mute);
void BuzzerInit();
extern Buzzer_t Buzzer;
#endif