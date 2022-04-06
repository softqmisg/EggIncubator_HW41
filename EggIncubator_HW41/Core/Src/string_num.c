/*
 * string_num.c
 *
 *  Created on: Mar 30, 2022
 *      Author: mehdi
 */
//#include "stm32f0xx_hal.h"
#include <stdint.h>
#include "string_num.h"
#include <stdio.h>
//void ftoa(float a ,uint8_t *integer_part,uint8_t *fractional_part)
//{
//	   *integer_part=(uint8_t)a;
//	   *fractional_part=(int)((a-(double)*integer_part)*10.0);
//}
char ftoa_str[16];
char * ftoa(double a )
{
	uint8_t integer_part=(uint8_t)a;
	uint8_t fractional_part=(int)(((double)a-(double)integer_part)*10.0);
	sprintf(ftoa_str,"%d.%d",integer_part,fractional_part);
	return ftoa_str;

}
