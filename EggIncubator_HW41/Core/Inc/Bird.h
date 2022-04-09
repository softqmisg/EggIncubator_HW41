#ifndef INC_BIRD_H
#define INC_BIRD_H
#include "main.h"
#define NumOfBirds	10
typedef enum {Manual=0,Morgh,ShotorMorgh,Ghaz,Ordak,Kabk,Gharghavol,Belderchin,Boghalamoon,MorghShakhdar} BirdType_t;
typedef struct{
	int16_t humidty;
	int16_t temperature;
	uint16_t	durationDays;
} Prog_t;

typedef struct {
	uint16_t hr;
	uint8_t min;
	uint8_t sec;
	uint16_t day;
} DateTime_t;
typedef struct{
	BirdType_t Type;
	uint16_t TotalDurationDays;
	uint16_t HatchTotalDays;
	uint16_t NumofProg;
	Prog_t *pProgs;
	DateTime_t curDateTime;
} Bird_t;

extern Bird_t	defaultBirds[NumOfBirds];
void BirdInit(Bird_t *bird);
void BirdReadManual(Bird_t *bird);
void BirdSaveManual(Bird_t bird);
#endif