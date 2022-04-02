#ifndef I2C_HAL_H
#define I2C_HAL_H
//==============================================================================
// S E N S I R I O N AG, Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//==============================================================================
// Project : SHT2x Sample Code (V1.2)
// File : I2C_HAL.h
// Author : MST
// Controller: NEC V850/SG3 (uPD70F3740)
// Compiler : IAR compiler for V850 (3.50A)
// Brief : I2C Hardware abstraction layer
//==============================================================================
//---------- Includes ----------------------------------------------------------
#include "main.h"

#define	I2CSWSDA_PORT	SHT_SDA_GPIO_Port
#define	I2CSWSDA_PIN	SHT_SDA_Pin

#define	I2CSWSCL_PORT	SHT_SCL_GPIO_Port	
#define	I2CSWSCL_PIN	SHT_SCL_Pin

//---------- Defines -----------------------------------------------------------
//I2C ports
//The communication on SDA and SCL is done by switching pad direction
//For a low level on SCL or SDA, direction is set to output. For a high level on
//SCL or SDA, direction is set to input. (pull up resistor active)
#define SDA_HIGH() 	HAL_GPIO_WritePin(I2CSWSDA_PORT,I2CSWSDA_PIN,GPIO_PIN_SET)
#define SDA_LOW() 		HAL_GPIO_WritePin(I2CSWSDA_PORT,I2CSWSDA_PIN,GPIO_PIN_RESET)
#define SCL_HIGH() 	HAL_GPIO_WritePin(I2CSWSCL_PORT ,I2CSWSCL_PIN,GPIO_PIN_SET)
#define SCL_LOW() 		HAL_GPIO_WritePin(I2CSWSCL_PORT,I2CSWSCL_PIN,GPIO_PIN_RESET)
#define SDA_INPUT() (uint8_t)HAL_GPIO_ReadPin(I2CSWSDA_PORT,I2CSWSDA_PIN)
#define SCL_INPUT()  (uint8_t)HAL_GPIO_ReadPin(I2CSWSCL_PORT,I2CSWSCL_PIN)
//---------- Enumerations ------------------------------------------------------
// I2C level
typedef enum{
	LOW = 0,
	HIGH = 1,
}etI2cLevel;
// I2C acknowledge
typedef enum{
	ACK = 0,
	NO_ACK = 1,
}etI2cAck;
typedef enum{
	ACK_ERROR = 0x01,
	TIME_OUT_ERROR = 0x02,
	CHECKSUM_ERROR = 0x04,
	UNIT_ERROR = 0x08
}etError;
//==============================================================================
//==============================================================================
void DelayMicroSeconds (uint32_t nbrOfUs);
void SCL_CONF(void);
void SDA_CONF(void);
void SCL_OUTPUT(etI2cLevel level);
void SDA_OUTPUT(etI2cLevel level);

//==============================================================================
void I2c_Init ();
//==============================================================================
//Initializes the ports for I2C interface
//==============================================================================
void I2c_StartCondition ();
//==============================================================================
// writes a start condition on I2C-bus
// input : -
// output: -
// return: -
// note : timing (delay) may have to be changed for different microcontroller
// _____
// SDA: |_____
// _______
// SCL : |___
//==============================================================================
void I2c_StopCondition ();
//==============================================================================
// writes a stop condition on I2C-bus
// input : -
// output: -
// return: -
// note : timing (delay) may have to be changed for different microcontroller
// SDA: _____|
// _______
// SCL : ___|
//===========================================================================
uint8_t I2c_WriteByte (uint8_t txByte);
//===========================================================================
// writes a byte to I2C-bus and checks acknowledge
// input: txByte transmit byte
// output: -
// return: error
// note: timing (delay) may have to be changed for different microcontroller
//===========================================================================
uint8_t I2c_ReadByte (etI2cAck ack);
//===========================================================================
// reads a byte on I2C-bus
// input: rxByte receive byte
// output: rxByte
// note: timing (delay) may have to be changed for different microcontroller
#endif