/*
 * lcd_ch.h
 *
 *  Created on: Sep 17, 2019
 *      Author: mehdi
 */

#ifndef INC_LCD_CH_H_
#define INC_LCD_CH_H_
#include "main.h"
/*
 * Pin out definition
 */
#define LCDDB4_OFFSET	3
#define LCDDATA_MASK	0x0F<<LCDDB4_OFFSET
#define LCDDB4_PORT	LCD_D4_GPIO_Port
#define LCDDB4_PIN	LCD_D4_Pin

#define LCDWR_PORT	LCD_RW_GPIO_Port
#define LCDWR_PIN	LCD_RW_Pin

#define LCDRS_PORT	LCD_RS_GPIO_Port
#define LCDRS_PIN	LCD_RS_Pin

#define LCDE_PORT	LCD_E_GPIO_Port
#define LCDE_PIN	LCD_E_Pin

/*
 * data or command mode
 */
#define DAT 1
#define CMD 0


/*
 * LCD Command Code
 */

#define LCD16X2_CLEAR_DISPLAY			0x01
#define LCD16X2_CURSOR_HOME				0x02
#define LCD16X2_CHARACTER_ENTRY_MODE	0x04
#define LCD16X2_DISPLAY_CURSOR_ON_OFF	0x08
#define LCD16X2_DISPLAY_CURSOR_SHIFT 	0x10
#define LCD16X2_FUNCTION_SET			0x20
#define LCD16X2_SET_CGRAM_ADDRESS	 	0x40
#define LCD16X2_SET_DDRAM_ADDRESS	 	0x80
/*
 * Value of commands
 */
/* Character entry mode instructions */
#define LCD16X2_INCREMENT				0x02	// Initialization setting
#define LCD16X2_DECREMENT				0x00
#define LCD16X2_DISPLAY_SHIFT_ON		0x01
#define LCD16X2_DISPLAY_SHIFT_OFF		0x00	// Initialization setting
/* Display cursor on off instructions */
#define LCD16X2_DISPLAY_ON	 			0x04
#define LCD16X2_DISPLAY_OFF	 			0x00	// Initialization setting
#define LCD16X2_CURSOR_UNDERLINE_ON	 	0x02
#define LCD16X2_CURSOR_UNDERLINE_OFF	0x00	// Initialization setting
#define LCD16X2_CURSOR_BLINK_ON	 		0x01
#define LCD16X2_CURSOR_BLINK_OFF	 	0x00	// Initialization setting
/* Display cursor shift instructions */
#define LCD16X2_DISPLAY_SHIFT			0x08
#define LCD16X2_CURSOR_MOVE				0x00
#define LCD16X2_RIGHT_SHIFT				0x04
#define LCD16X2_LEFT_SHIFT				0x00
/* Function set instructions */
#define LCD16X2_8BIT_INTERFACE			0x10	// Initialization setting
#define LCD16X2_4BIT_INTERFACE			0x00
#define LCD16X2_2LINE_MODE				0x08
#define LCD16X2_1LINE_MODE				0x00	// Initialization setting
#define LCD16X2_5X10DOT_FORMAT			0x04
#define LCD16X2_5X7DOT_FORMAT			0x00	// Initialization setting
/* Busy flag bit location */
#define LCD16X2_BUSY_FLAG				0x80

/** LCD display and cursor attributes --------------------------------------- */
#define LCD16X2_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF	(LCD16X2_DISPLAY_OFF | \
	LCD16X2_CURSOR_UNDERLINE_OFF | LCD16X2_CURSOR_BLINK_OFF)
#define LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF		(LCD16X2_DISPLAY_ON | \
	LCD16X2_CURSOR_UNDERLINE_OFF | LCD16X2_CURSOR_BLINK_OFF)
#define LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_ON		(LCD16X2_DISPLAY_ON | \
	LCD16X2_CURSOR_UNDERLINE_OFF | LCD16X2_CURSOR_BLINK_ON)
#define LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_OFF		(LCD16X2_DISPLAY_ON | \
	LCD16X2_CURSOR_UNDERLINE_ON | LCD16X2_CURSOR_BLINK_OFF)
#define LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_ON		(LCD16X2_DISPLAY_ON | \
	LCD16X2_CURSOR_UNDERLINE_ON | LCD16X2_CURSOR_BLINK_ON)
//////////////////////////////////////////////////////////////////////////////////////
/** Display size ------------------------------------------------------------ */
// Number of visible lines of the display (1 or 2)
#define LCD16X2_LINES			2
// Visible characters per line of the display
#define LCD16X2_DISP_LENGTH		16
// DDRAM address of first char of line 1
#define LCD16X2_START_LINE_1	0x00
// DDRAM address of first char of line 2
#define LCD16X2_START_LINE_2	0x40
///////////////////////////////////////////////////////////////
#define MORGHSHAKHDAR_STR	0	
#define BOGHALAMOON_STR		1
#define BELDERCHIN_STR		2
#define	GHARGHAVOL_STR		3
#define KABK_STR					4
#define ORDAK_STR					5
#define GHAZ_STR					6
#define SHOTORMORGH_STR		7
#define DASTI_STR					8
#define MORGH_STR					9
#define TIMER_STR					10
#define CHARKHSH_STR			11
#define DAMA_STR					12
#define ROTOBAT_STR				13
#define ZAMAN_STR					14
#define ROOZ_STR					15

///////////////////////////////////////////////////////////////
void LCD_backlight(uint8_t onoff);
void LCD_init(void);
void LCD_putstr(char *lcd_string);
void LCD_putchar(char char_data);
void LCD_clear_home(void);
void LCD_gotoxy(unsigned char x_pos, unsigned char y_pos);
void LCD_putstrpos(char *lcd_string,unsigned char x_pos, unsigned char y_pos);
void LCD_put_custom_char( uint8_t location);
void LCD_putpersian(uint8_t StringCode,uint8_t posx,uint8_t posy);
void LCD_clearrow(uint8_t row);

#endif /* LCD_CH_H_ */
