#include "user_sht.h"
#include "user_eeprom.h"
#include "eeprom.h"
#include "sht2x_BB.h"
Sht_t defaultSht={
	.error=0,
	.calibHum=DEFAULT_CALIB_HUM,
	.calibTemp=DEFAULT_CALIB_TEMP,
	.humidity=0,
	.temperature=0
};
void ShtInit(Sht_t *sensor)
{
	sensor->error=0;
	EEReadByte((uint8_t *)&(sensor->calibTemp),2,EE_ADD_CALIB_TEMP);			
	EEReadByte((uint8_t *)&(sensor->calibHum),2,EE_ADD_CALIB_HUM);			
}
void ShtReadSensor(Sht_t *sensor)
{
	float tmp;
	sensor->error|=SHT2x_MeasureHM(TEMP, &tmp);
	sensor->temperature=(int16_t)(tmp*10.0)+sensor->calibTemp;
	HAL_Delay(70);
	sensor->error|=SHT2x_MeasureHM(HUMIDITY, &tmp);
	sensor->humidity=(int16_t)(tmp*10.0)+sensor->calibTemp;
	HAL_Delay(25);
}

void ShtSave(Sht_t sensor)
{
		EEWriteByte((uint8_t *)&sensor.calibTemp,2,EE_ADD_CALIB_TEMP);			
		EEWriteByte((uint8_t *)&sensor.calibHum,2,EE_ADD_CALIB_HUM);		
}