#ifndef INC_USER_SHT_H
#define INC_USER_SHT_H
#include "main.h"
#include "sht2x_BB.h"
typedef struct{
	uint8_t error;
	int16_t humidity;
	int16_t temperature;
	int16_t calibHum;
	int16_t calibTemp;
} Sht_t;

void ShtInit(Sht_t *sensor);
void ShtSave(Sht_t sensor);
void ShtReadSensor(Sht_t *sensor);
extern Sht_t defaultSht;
#endif