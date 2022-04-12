#include "eeprom.h"
#include <string.h>
void EEWriteByte(uint8_t *pData,uint8_t len,uint16_t memaddress)
{
	HAL_GPIO_WritePin(EEWP_Port,EEWP_Pin,GPIO_PIN_RESET);
	for(uint8_t i=0;i<len;i++)
	{
		while(HAL_I2C_IsDeviceReady(&EE_I2C, EE_BASEADDRESS, 1, HAL_MAX_DELAY) != HAL_OK);
		HAL_I2C_Mem_Write(&EE_I2C,EE_BASEADDRESS,memaddress+i,EE_ADDRESSSIZE,(uint8_t *)&pData[i],1,HAL_MAX_DELAY);
		HAL_Delay(10);
	}
		HAL_GPIO_WritePin(EEWP_Port,EEWP_Pin,GPIO_PIN_SET);
}
void EEReadByte(uint8_t *pData,uint8_t len,uint16_t memaddress)
{
	while(HAL_I2C_IsDeviceReady(&EE_I2C, EE_BASEADDRESS, 1, HAL_MAX_DELAY) != HAL_OK);
	HAL_I2C_Mem_Read(&EE_I2C,EE_BASEADDRESS,memaddress,EE_ADDRESSSIZE,pData,len,HAL_MAX_DELAY);
	HAL_Delay(10);
}
/*
*/
void EEWriteStr(char *pData,uint16_t memaddress)
{
	EEWriteByte((uint8_t *)pData,strlen(pData),memaddress);
	uint8_t endch=0;
	EEWriteByte(&endch,1,memaddress+strlen(pData));
}
void EEReadStr(char *pData,uint16_t memaddress)
{
	uint8_t i=0;
	do{
		while(HAL_I2C_IsDeviceReady(&EE_I2C, EE_BASEADDRESS, 1, HAL_MAX_DELAY) != HAL_OK);
		HAL_I2C_Mem_Read(&EE_I2C,EE_BASEADDRESS,memaddress,EE_ADDRESSSIZE,(uint8_t *)&pData[i],1,HAL_MAX_DELAY);
		memaddress++;
		HAL_Delay(10);
	}while(pData[i++]!=0);
}
/*
*/
void EEWriteDouble(double *pData,uint8_t len,uint16_t memaddress)
{
	HAL_GPIO_WritePin(EEWP_Port,EEWP_Pin,GPIO_PIN_RESET);
	uint8_t fData[2];
	for(uint8_t i=0;i<len;i++)
	{
		uint8_t integer_part=(uint8_t) pData[i];
		uint16_t x=(uint16_t)(pData[i]*10.0);
		uint16_t y=(uint16_t)((double)integer_part*10.0);
		uint8_t fractional_part=x-y;
		fData[0]=integer_part;
		fData[1]=fractional_part;
		while(HAL_I2C_IsDeviceReady(&EE_I2C, EE_BASEADDRESS, 1, HAL_MAX_DELAY) != HAL_OK);
		HAL_I2C_Mem_Write(&EE_I2C,EE_BASEADDRESS,memaddress,EE_ADDRESSSIZE,fData,2,HAL_MAX_DELAY);
		memaddress+=2;
		HAL_Delay(10);
	}
	HAL_GPIO_WritePin(EEWP_Port,EEWP_Pin,GPIO_PIN_SET);
}
void EEReadDouble(double *pData,uint8_t len,uint16_t memaddress)
{
	uint8_t fData[2];
	for(uint8_t i=0;i<len;i++)
	{
		while(HAL_I2C_IsDeviceReady(&EE_I2C, EE_BASEADDRESS, 1, HAL_MAX_DELAY) != HAL_OK);
		HAL_I2C_Mem_Read(&EE_I2C,EE_BASEADDRESS,memaddress,EE_ADDRESSSIZE,fData,2,HAL_MAX_DELAY);
		pData[i]=(double)fData[0]+(double)fData[1]/10.0;
		memaddress+=2;
		HAL_Delay(10);
	}	
}
