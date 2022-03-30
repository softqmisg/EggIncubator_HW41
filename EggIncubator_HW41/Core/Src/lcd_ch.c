/*
 * lcd_ch.c
 *
 *  Created on: Sep 17, 2019
 *      Author: mehdi
 */
#include "lcd_ch.h"

uint8_t custom_char1[][8] = {
		   {6, 0, 1, 25, 30, 8, 24, 0 },//TO
		   {0, 0, 1, 25, 25, 30, 0, 0 },//SE
		   {0, 0, 12, 20, 28, 7, 0, 0 },//EM
		   {0, 24, 9, 11, 30, 0, 0, 0 },//MHV
		   {0, 0, 0, 3, 11, 9, 27, 0 },//RGH
			 {3, 16, 18, 21, 21, 30, 0,0 },//AS
			 {0, 0, 1, 19, 11, 15, 24,0 },//D
			 {0, 1, 2, 4, 4, 28, 0, 0 }//K
   };
   uint8_t custom_char2[][8] = {
		   {0, 2, 3, 18, 18, 18, 12, 0 },//S
		   {0, 23, 21, 7, 1, 1, 14, 0 },//V
		   {0, 0, 4, 1, 30, 0, 0, 0 },//N
		   {3, 0, 7, 5, 31, 0, 0, 0 },//GH
		   {4, 10, 10, 17, 31, 31, 14,0 },//DROP
			 {4, 10, 10, 10, 10, 27, 31,14 },//thermo
			 {24, 24, 0, 3, 4, 4, 3,0 },//'C
			 {0, 0, 0, 0, 0, 0, 0, 0 }//ALARM oFF
   };	 
/*
 * micro second delay for LCD
 */
 void lcd_delay_us(uint16_t delay_us)
{
	while(delay_us)
	{
		for(uint8_t i=0;i<100;i++)
			__ASM("nop");
		delay_us--;
	}
}
/*
 * toggle EN pin of LCD
 */
void toggle_EN_pin(void)
{
	HAL_GPIO_WritePin(LCDE_PORT, LCDE_PIN, GPIO_PIN_SET);
	//HAL_Delay(1);
	lcd_delay_us(200);	//140
	HAL_GPIO_WritePin(LCDE_PORT, LCDE_PIN, GPIO_PIN_RESET);
}
/*
 * send 8 bit value to LCD
 *  first send top nibble and then down nibble
 */
void LCD_4bit_send(unsigned char lcd_data)
{

	uint8_t top_nibble_ones=(lcd_data>>4)<<LCDDB4_OFFSET,down_nibble_ones=(lcd_data&0x0F)<<LCDDB4_OFFSET;
	uint8_t top_nibble_zeros=(~top_nibble_ones)&LCDDATA_MASK,down_nibble_zeros=(~down_nibble_ones)&LCDDATA_MASK;

	HAL_GPIO_WritePin(LCDDB4_PORT, top_nibble_ones, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCDDB4_PORT, top_nibble_zeros, GPIO_PIN_RESET);
	toggle_EN_pin();

	HAL_GPIO_WritePin(LCDDB4_PORT, down_nibble_ones, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCDDB4_PORT, down_nibble_zeros, GPIO_PIN_RESET);
	toggle_EN_pin();
}
/*
 * send command  or data value to LCD
 * mode: DAT(data)/CMD(command)
 */
void LCD_send(unsigned char value, unsigned char mode)
{
	switch(mode)
	{
		case DAT:
			HAL_GPIO_WritePin(LCDRS_PORT, LCDRS_PIN, GPIO_PIN_SET);
		break;
		case CMD:
			HAL_GPIO_WritePin(LCDRS_PORT, LCDRS_PIN, GPIO_PIN_RESET);
		break;
	}
	LCD_4bit_send(value);
}
/*
 * clear LCD and go home
 */
void LCD_clear_home(void)
{
	LCD_send(LCD16X2_CLEAR_DISPLAY, CMD);
	LCD_send(LCD16X2_CURSOR_HOME, CMD);
}
/*
 * initilize lcd
 */
void LCD_init(void)
{
	HAL_GPIO_WritePin(LCDWR_PORT, LCDWR_PIN, GPIO_PIN_RESET);
	HAL_Delay(10);
	//three time send 8 bit interface then 4 bit interface command
	LCD_send(0x33,CMD);
	LCD_send(0x32,CMD);
	///////////////////////////////////////////////////////
	LCD_send(LCD16X2_FUNCTION_SET| LCD16X2_4BIT_INTERFACE |LCD16X2_2LINE_MODE |LCD16X2_5X7DOT_FORMAT,CMD);
	LCD_send(LCD16X2_DISPLAY_CURSOR_ON_OFF| LCD16X2_DISPLAY_ON| LCD16X2_CURSOR_UNDERLINE_OFF|LCD16X2_CURSOR_BLINK_OFF,CMD);
	LCD_clear_home();
	LCD_send(LCD16X2_CHARACTER_ENTRY_MODE|LCD16X2_INCREMENT|LCD16X2_DISPLAY_SHIFT_OFF,CMD);
}

//////////////////////////////////////////////////////////////////////////////////////////
void LCD_putstr(char *lcd_string)
{
	do
	{
		if((*lcd_string)<128)
			LCD_send(*lcd_string++, DAT);
		else
		{
			LCD_put_custom_char(*lcd_string-128);
			lcd_string++;
		}
	}while(*lcd_string != '\0');
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_putchar(char char_data)
{
	LCD_send(char_data, DAT);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_gotoxy(unsigned char x_pos, unsigned char y_pos)
{
	if(y_pos == 0)
	{
		LCD_send(LCD16X2_SET_DDRAM_ADDRESS  | (LCD16X2_START_LINE_1 +x_pos), CMD);
	}
	else
	{
		LCD_send(LCD16X2_SET_DDRAM_ADDRESS  | (LCD16X2_START_LINE_2 +x_pos), CMD);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_putstrpos(char *lcd_string,unsigned char x_pos, unsigned char y_pos)
{
	LCD_gotoxy(x_pos,y_pos);
	LCD_putstr(lcd_string);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_entry_inc(void)
{
	LCD_send(LCD16X2_CHARACTER_ENTRY_MODE | LCD16X2_INCREMENT |
		LCD16X2_DISPLAY_SHIFT_OFF,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_entry_dec(void)
{
	LCD_send(LCD16X2_CHARACTER_ENTRY_MODE | LCD16X2_DECREMENT |
		LCD16X2_DISPLAY_SHIFT_OFF,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_entry_inc_shift(void)
{
	LCD_send(LCD16X2_CHARACTER_ENTRY_MODE | LCD16X2_INCREMENT |
		LCD16X2_DISPLAY_SHIFT_ON,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_entry_dec_shift(void)
{
	LCD_send(LCD16X2_CHARACTER_ENTRY_MODE | LCD16X2_DECREMENT |
		LCD16X2_DISPLAY_SHIFT_ON,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_display_on(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_ON_OFF |
			LCD16X2_DISPLAY_ON|LCD16X2_CURSOR_UNDERLINE_OFF|LCD16X2_CURSOR_BLINK_OFF,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_display_off(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_ON_OFF |
			LCD16X2_DISPLAY_OFF|LCD16X2_CURSOR_UNDERLINE_OFF|LCD16X2_CURSOR_BLINK_OFF,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_cursor_on(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_ON_OFF |LCD16X2_DISPLAY_ON|
			LCD16X2_CURSOR_UNDERLINE_ON,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_cursor_off(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_ON_OFF |LCD16X2_DISPLAY_ON|
			LCD16X2_CURSOR_UNDERLINE_OFF,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_blink_on(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_ON_OFF |LCD16X2_DISPLAY_ON|
			LCD16X2_CURSOR_BLINK_ON,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_blink_off(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_ON_OFF |LCD16X2_DISPLAY_ON|
			LCD16X2_CURSOR_BLINK_OFF,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_cursor_blink_on(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_ON_OFF |LCD16X2_DISPLAY_ON|LCD16X2_CURSOR_BLINK_ON|
			LCD16X2_CURSOR_BLINK_ON,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_cursor_blink_off(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_ON_OFF |LCD16X2_DISPLAY_ON|LCD16X2_CURSOR_BLINK_OFF|
			LCD16X2_CURSOR_BLINK_OFF,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_display_shift_right(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_SHIFT |
		LCD16X2_DISPLAY_SHIFT | LCD16X2_RIGHT_SHIFT,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_display_shift_left(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_SHIFT |
		LCD16X2_DISPLAY_SHIFT | LCD16X2_LEFT_SHIFT,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_cursor_shift_left(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_SHIFT |
			LCD16X2_CURSOR_MOVE | LCD16X2_LEFT_SHIFT,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_cursor_shift_right(void)
{
	LCD_send(LCD16X2_DISPLAY_CURSOR_SHIFT |
			LCD16X2_CURSOR_MOVE | LCD16X2_RIGHT_SHIFT,CMD);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_create_custom_char(uint8_t location, uint8_t* data_bytes)
{
	int i;

	// We only have 8 locations 0-7 for custom chars
	location &= 0x07;

	// Set CGRAM address
	LCD_send(LCD16X2_SET_CGRAM_ADDRESS | (location << 3),CMD);

	// Write 8 bytes custom char pattern
	for (i = 0; i < 8; i++)
	{
		LCD_send(data_bytes[i],DAT);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_put_custom_char( uint8_t location)
{
	LCD_send(location,DAT);
}
//////////////////////////////////////////////////////////////////////////////////////////
void LCD_load_custom_char(uint8_t custom_chars[][8])
{
	   for(uint8_t i=0;i<8;i++)
				LCD_create_custom_char(i, custom_chars[i]);	
}
//////////////////////////////////////////////////////////////////////////////////////////
/*void LCD_put_custom_text(uint8_t x,uint8_t y,uint8_t text)
{
	if(text==CUSTOM_TEXT_BIRD)
	{
		LCD_gotoxy(x+4,y);LCD_put_custom_char(CUSTOM_CHAR_P);
		LCD_gotoxy(x+3,y);LCD_put_custom_char(CUSTOM_CHAR_R);
		LCD_gotoxy(x+2,y);LCD_put_custom_char(CUSTOM_CHAR_N);
		LCD_gotoxy(x+1,y);LCD_put_custom_char(CUSTOM_CHAR_D);
		LCD_gotoxy(x+0,y);LCD_put_custom_char(CUSTOM_CHAR_H);

	}
	if(text==CUSTOM_TEXT_GHOGHNOOS)
	{
		LCD_gotoxy(x+4,y);LCD_put_custom_char(CUSTOM_CHAR_GH);
		LCD_gotoxy(x+3,y);LCD_put_custom_char(CUSTOM_CHAR_GH);
		LCD_gotoxy(x+2,y);LCD_put_custom_char(CUSTOM_CHAR_NN);
		LCD_gotoxy(x+1,y);LCD_put_custom_char(CUSTOM_CHAR_V);
		LCD_gotoxy(x+0,y);LCD_put_custom_char(CUSTOM_CHAR_S);
		
	}

}
*/
