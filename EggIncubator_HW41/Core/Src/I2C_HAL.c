//==============================================================================
// S E N S I R I O N AG, Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//==============================================================================
// Project : SHT2x Sample Code (V1.2)
// File : I2C_HAL.c
// Author : MST
// Controller: NEC V850/SG3 (uPD70F3740)
// Compiler : IAR compiler for V850 (3.50A)
// Brief : I2C Hardware abstraction layer
//==============================================================================
//---------- Includes ----------------------------------------------------------
#include "I2C_HAL.h"
//==============================================================================
//#pragma push
//#pragma O0
void DelayMicroSeconds (uint32_t nbrOfUs)
{
	uint8_t k=1;
	uint32_t max=((uint32_t)nbrOfUs*43)/10;
	for(uint32_t i=0; i<max; i++)
	{ 
		__ASM("nop"); //nop's may be added for timing adjustment
	}
}
//#pragma pop
//==============================================================================
void SCL_OUTPUT(etI2cLevel level)
{
	if(level==LOW )SCL_LOW();else SCL_HIGH();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = I2CSWSCL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(I2CSWSCL_PORT, &GPIO_InitStruct);
}
//==============================================================================
void SDA_OUTPUT(etI2cLevel level)
{
	if(level==LOW ) SDA_LOW();else SDA_HIGH();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = I2CSWSDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(I2CSWSDA_PORT, &GPIO_InitStruct);
}
//==============================================================================
void SDA_CONF(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = I2CSWSDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(I2CSWSDA_PORT, &GPIO_InitStruct);
	 
}
//==============================================================================
void SCL_CONF(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = I2CSWSCL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(I2CSWSCL_PORT, &GPIO_InitStruct);
	

}
//==============================================================================
void I2c_Init ()
//==============================================================================
{
	SDA_OUTPUT(LOW);
	SCL_OUTPUT(LOW); // Set port as output for configuration

	SDA_CONF(); // I2C-bus idle mode SDA released (input)
	SCL_CONF(); // I2C-bus idle mode SCL released (input)
	HAL_Delay(20);
}
//==============================================================================
void I2c_StartCondition ()
//==============================================================================
{
	SDA_CONF();
	SCL_CONF();
	//DelayMicroSeconds(5);
	SDA_OUTPUT(LOW);
	DelayMicroSeconds(10); // hold time start condition (t_HD;STA)
	SCL_OUTPUT(LOW);
	DelayMicroSeconds(10);
}
//==============================================================================
void I2c_StopCondition ()
//==============================================================================
{
	SDA_OUTPUT(LOW);
	SCL_OUTPUT(LOW);
	//DelayMicroSeconds(1);
	SCL_CONF();
	DelayMicroSeconds(10); // set-up time stop condition (t_SU;STO)
	SDA_CONF();
	DelayMicroSeconds(10);
}
//==============================================================================
uint8_t I2c_WriteByte (uint8_t txByte)
//==============================================================================
{
	uint8_t mask,error=0;
	SDA_OUTPUT(LOW);
	SCL_OUTPUT(LOW);
	for (mask=0x80; mask>0; mask>>=1) //shift bit for masking (8 times)
	{
		if ((mask & txByte) == 0) 
			SDA_LOW();//masking txByte, write bit to SDA-Line
		else 
			SDA_HIGH();
		DelayMicroSeconds(1); //data set-up time (t_SU;DAT)
		SCL_HIGH(); //generate clock pulse on SCL
		DelayMicroSeconds(7); //SCL high time (t_HIGH)
		SCL_LOW();
		DelayMicroSeconds(7); //data hold time(t_HD;DAT)
	}
	SDA_CONF(); //release SDA-line
	SCL_HIGH(); //clk #9 for ack
	DelayMicroSeconds(2); //data set-up time (t_SU;DAT)
	if(SDA_INPUT()==(uint8_t)HIGH) 
		error=ACK_ERROR; //check ack from i2c slave
	DelayMicroSeconds(5); //data set-up time (t_SU;DAT)
	SCL_LOW();
	DelayMicroSeconds(10); //wait time to see byte package on scope
	return error; //return error code
}
//==============================================================================
uint8_t I2c_ReadByte (etI2cAck ack)
//==============================================================================
{
	uint8_t mask,rxByte=0;
	SDA_CONF(); //release SDA-line
	SCL_OUTPUT(HIGH);
	for (mask=0x80; mask>0; mask>>=1) //shift bit for masking (8 times)
	{ 
		SCL_HIGH(); //start clock on SCL-line
		DelayMicroSeconds(1); //data set-up time (t_SU;DAT)
		DelayMicroSeconds(10); //SCL high time (t_HIGH)
		if (SDA_INPUT()==1) 
			rxByte=(rxByte | mask); //read bit
		SCL_LOW();
		DelayMicroSeconds(10); //data hold time(t_HD;DAT)
	}
	SDA_OUTPUT((etI2cLevel)ack); //send acknowledge if necessary
	DelayMicroSeconds(1); //data set-up time (t_SU;DAT)
	SCL_HIGH(); //clk #9 for ack
	DelayMicroSeconds(5); //SCL high time (t_HIGH)
	SCL_LOW();
	SDA_CONF(); //release SDA-line
	DelayMicroSeconds(5); //wait time to see byte package on scope
	return rxByte; //return error code
}