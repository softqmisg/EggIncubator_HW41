/*
 * lcd_ch.c
 *
 *  Created on: Sep 17, 2019
 *      Author: mehdi
 */
#include "lcd_ch.h"
#include <string.h>
uint8_t MorghShakhdar_Pos_ch[]={0,1,2,3,4,5,6,1};
uint8_t Boghalamoon_Pos_ch[]={7,8,9,10,11,8,12};
uint8_t Belderchin_Pos_ch[]={7,10,13,1,14,15,12};
uint8_t Gharghavol_Pos_ch[]={9,16,9,4,8,17};
uint8_t Kabk_Pos_ch[]={18,19,20};
uint8_t Ordak_Pos_ch[]={6,1,13,20};
uint8_t Ghaz_Pos_ch[]={21,4,22};
uint8_t Shotormorgh_Pos_ch[]={3,23,16,0,1,2};
uint8_t Dasti_Pos_ch[]={13,24,23,25};
uint8_t Morgh_Pos_ch[]={0,1,2};
uint8_t Timer_Pos_ch[]={26,27,11,16};
uint8_t Charkhsh_Pos_ch[]={14,16,28,29};
uint8_t Dama_Pos_ch[]={13,11,4};
uint8_t Rotobat_Pos_ch[]={1,30,8,7,31};
uint8_t Zaman_Pos_ch[]={22,11,4,12};
uint8_t Rooz_Pos_ch[]={1,32,22};


uint8_t custom_character[][8] = {
		   {0x00, 0x00, 0x00, 0x1f, 0x05, 0x07, 0x00, 0x00 },//0//Mo
		   {0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x04, 0x18 },//1//R
		   {0x02, 0x00, 0x03, 0x04, 0x0b, 0x08, 0x08, 0x07 },//2//Gh
		   {0x04, 0x0A, 0x00, 0x15, 0x15, 0x1f, 0x00, 0x00 },//3//Sh
		   {0x00, 0x04, 0x04, 0x04, 0x04, 0x07, 0x00, 0x00 },//4//A_
			 {0x04, 0x00, 0x06, 0x11, 0x09, 0x0F, 0x10,	0x00 },//5//KhD
			 {0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00,	0x00 },//6//A
			 {0x00, 0x00, 0x00, 0x02, 0x02, 0x1e, 0x00, 0x04 },//7//B
			 {0x00, 0x00, 0x00, 0x0e, 0x0a, 0x0f, 0x04, 0x18 },//8//_Oo
			 {0x00, 0x05, 0x00, 0x07, 0x05, 0x1f, 0x00, 0x00 },//9//Gh
			 {0x00, 0x02, 0x02, 0x02, 0x02, 0x1f, 0x00, 0x00 },//10//_L_
			 {0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x05, 0x07 },//11//m
			 {0x00, 0x04, 0x00, 0x11, 0x11, 0x11, 0x0e, 0x00 },//12//N
			 {0x00, 0x00, 0x00, 0x0c, 0x02, 0x01, 0x02, 0x0c },//13//D
			 {0x00, 0x00, 0x00, 0x0e, 0x01, 0x1f, 0x00, 0x0e },//14//Ch_			 
			 {0x00, 0x00, 0x00, 0x02, 0x02, 0x1f, 0x00, 0x0c },//15//_Ie_		 			 
			 {0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x18 },//16//_R		 			 
			 {0x00, 0x01, 0x01, 0x01, 0x11, 0x11, 0x0E, 0x00 },//17//L		 			 
			 {0x01, 0x02, 0x04, 0x02, 0x01, 0x1f, 0x00, 0x00 },//18//K_		 			 
			 {0x00, 0x00, 0x00, 0x02, 0x02, 0x1f, 0x00, 0x04 },//19//_B_		 			 
			 {0x01, 0x02, 0x04, 0x02, 0x11, 0x1f, 0x00, 0x00 },//20//K		 			 
			 {0x02, 0x00, 0x03, 0x04, 0x04, 0x1f, 0x00, 0x00 },//21//Gh_		 			 
			 {0x00, 0x02, 0x00, 0x02, 0x02, 0x02, 0x04, 0x18 },//22//Z		 			 
			 {0x00, 0x00, 0x18, 0x02, 0x02, 0x1f, 0x00, 0x00 },//23//_T_		 			 
			 {0x00, 0x00, 0x00, 0x15, 0x15, 0x1f, 0x00, 0x00 },//24//S
			 {0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x12, 0x0C },//25//_Y
			 {0x10, 0x10, 0x16, 0x10, 0x10, 0x11, 0x0F, 0x00 },//26//Ta
			 {0x00, 0x00, 0x00, 0x02, 0x02, 0x1e, 0x00, 0x0c },//27//Y			 
			 {0x04, 0x00, 0x06, 0x09, 0x01, 0x1f, 0x00, 0x00 },//28//Kh_			 
			 {0x00, 0x02, 0x05, 0x00, 0x15, 0x17, 0x14, 0x1c },//29//_Sh			 
			 {0x04, 0x04, 0x04, 0x07, 0x05, 0x1f, 0x00, 0x00 },//30//t_			 
			 {0x00, 0x00, 0x0A, 0x00, 0x11, 0x1e, 0x00, 0x00 },//31//T			 
			 {0x00, 0x00, 0x0e, 0x0a, 0x0e, 0x02, 0x04, 0x18 },//32//Oo	

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
void LCD_clearrow(uint8_t row)
{
	LCD_gotoxy(0,row);
	for(uint8_t i=0;i<16;i++)
	LCD_send(' ',DAT);
}
/*
 * 	cerate custom  character in location of CGRAM
 *	data_bytes of value of  
*/
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
		LCD_send(*lcd_string++, DAT);
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
void LCD_putpersian(uint8_t StringCode,uint8_t posx,uint8_t posy,Alignment_t align)
{
	uint8_t lengthofcustom=0;
	uint8_t *posofcustomchar;
	uint8_t memstartpos=0;
	switch(StringCode)
	{
		case MORGHSHAKHDAR_STR:
			lengthofcustom=8;
			posofcustomchar=MorghShakhdar_Pos_ch;
		break;
		case BOGHALAMOON_STR:
			lengthofcustom=7;
			posofcustomchar=Boghalamoon_Pos_ch;
		break;
		case BELDERCHIN_STR:
			lengthofcustom=7;
			posofcustomchar=Belderchin_Pos_ch;
		break;
		case GHARGHAVOL_STR:
			lengthofcustom=6;
			posofcustomchar=Gharghavol_Pos_ch;
		break;		
		case KABK_STR:
			lengthofcustom=3;
			posofcustomchar=Kabk_Pos_ch;
		break;	
		case ORDAK_STR:
			lengthofcustom=4;
			posofcustomchar=Ordak_Pos_ch;
		break;	
		case GHAZ_STR:
			lengthofcustom=3;
			posofcustomchar=Ghaz_Pos_ch;
		break;	
		case SHOTORMORGH_STR:
			lengthofcustom=6;
			posofcustomchar=Shotormorgh_Pos_ch;
		break;	
		case DASTI_STR:
			lengthofcustom=4;
			posofcustomchar=Dasti_Pos_ch;
		break;
		case MORGH_STR:
			lengthofcustom=3;
			posofcustomchar=Morgh_Pos_ch;
		break;
		case TIMER_STR:
			lengthofcustom=4;
			posofcustomchar=Timer_Pos_ch;
		break;
		case CHARKHSH_STR:
			lengthofcustom=4;
			posofcustomchar=Charkhsh_Pos_ch;
		break;
		case DAMA_STR:
			lengthofcustom=3;
			posofcustomchar=Dama_Pos_ch;
		break;
		case ROTOBAT_STR:
			lengthofcustom=5;
			posofcustomchar=Rotobat_Pos_ch;
			memstartpos=3;
		break;
		case ZAMAN_STR:
			lengthofcustom=4;
			posofcustomchar=Zaman_Pos_ch;
		break;
		case ROOZ_STR:
			lengthofcustom=3;
			posofcustomchar=Rooz_Pos_ch;
			memstartpos=4;
		break;
	}
	for( uint8_t i=0;i<lengthofcustom;i++)
		LCD_create_custom_char(i+memstartpos,custom_character[posofcustomchar[i]]);
	switch(align)
	{
		case AlignLeft:
			posx=0;
		break;
		case AlignCenter:
			posx=(16-lengthofcustom)/2;			
			break;
		case AlignRight:
			posx=16-lengthofcustom;
		break;
	}
	LCD_gotoxy(posx,posy);
	for( uint8_t i=0;i<lengthofcustom;i++)
		LCD_send(lengthofcustom-1-i+memstartpos,DAT);//(lengthofcustom-1-i);
}
/*
*/
void LCD_putstralign(char *lcd_str,uint8_t x_pos,uint8_t y_pos,Alignment_t align)
{
	switch(align)
	{
		case AlignLeft:
			x_pos=0;
		break;
		case AlignCenter:
			x_pos=(16-strlen(lcd_str))/2;			
			break;
		case AlignRight:
			x_pos=16-strlen(lcd_str);
		break;
	}
	LCD_putstrpos(lcd_str,x_pos,y_pos);
}
void LCD_clear_size(uint8_t x,uint8_t y,uint8_t size)
{
	LCD_gotoxy(x,y);
	for(uint8_t i=0;i<size;i++)
		LCD_send(' ',DAT);
}
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
