#ifndef INC_EEPROM_H
#define INC_EEPROM_H
#include "main.h"
#include "i2c.h"
#define EE_BASEADDRESS	0xa0
#define EE_ADDRESSSIZE	I2C_MEMADD_SIZE_8BIT
#define	EE_I2C							hi2c1
#define EEWP_Pin	EE_WP_Pin
#define EEWP_Port	EE_WP_GPIO_Port

void EEWriteByte(uint8_t *pData,uint8_t len,uint16_t memaddress);
void EEWriteStr(char *pData,uint16_t memaddress);
void EEWriteDouble(double *pData,uint8_t len,uint16_t memaddress);

void EEReadByte(uint8_t *pData,uint8_t len,uint16_t memaddress);
void EEReadStr(char *pData,uint16_t memaddress);
void EEReadDouble(double *pData,uint8_t len,uint16_t memaddress);
#endif