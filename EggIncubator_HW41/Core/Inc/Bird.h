#ifndef INC_BIRD_H
#define INC_BIRD_H
#include "main.h"
#include "user_time.h"
#define NumOfBirds	10
typedef enum {Manual=0,Morgh=1,MorghShakhdar=2,Boghalamoon=3,Belderchin=4,Gharghavol=5,Kabk=6,Ordak=7,Ghaz=8,ShotorMorgh=9} BirdType_t;
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
	Prog_t pProgs[5];
} Bird_t;

extern Bird_t	defaultBirds[NumOfBirds];
void BirdInit(Bird_t *bird);
void BirdReadManual(Bird_t *bird);
void BirdSaveManual(Bird_t bird);
void BirdLoadDefault(void );

#endif