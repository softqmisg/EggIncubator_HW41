#ifndef INC_BIRD_H
#define INC_BIRD_H
#include "main.h"
#include "user_time.h"
#define NumOfBirds	10
typedef enum {Manual=0,Morgh,MorghShakhdar,Boghalamoon,Belderchin,Gharghavol,Kabk,Ordak,Ghaz,ShotorMorgh} BirdType_t;
typedef struct{
	int16_t humidity;
	int16_t temperature;
	uint16_t	durationDays;
} Prog_t;

typedef struct{
	BirdType_t Type;
	uint16_t TotalDurationDays;
	uint16_t HatchTotalDays;
	uint16_t NumofProg;
	Prog_t *pProgs;
} Bird_t;

extern Bird_t	defaultBirds[NumOfBirds];
void BirdInit(Bird_t *bird);
void BirdReadManual(Bird_t *bird);
void BirdSaveManual(Bird_t bird);
#endif