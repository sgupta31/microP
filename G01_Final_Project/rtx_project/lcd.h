#ifndef __LCD_H
#define __LCD_H

#include <inttypes.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/**
	* @file lcd.h
	* @brief Functions to initialize operate the LCD
	* 
	* HD44780 Driver
	*
	* Contrast line should be connected to GND
	* R/W line should be connected to GND since we always write to LCD
	* Vss connect to GND
	* Vdd can be connected to +5V
	*
	* See the header file to see the port assignments
	*/

/* Clocks for port E and B */
#define HD44780_GPIO_CLOCK1 	RCC_AHB1Periph_GPIOE
#define HD44780_GPIO_CLOCK2 	RCC_AHB1Periph_GPIOB

/* Pin definitions */
#define HD44780_RS					GPIO_Pin_9
#define GPIO_PORT_RS		        GPIOE

#define HD44780_E 					GPIO_Pin_10
#define GPIO_PORT_E		            GPIOE

#define HD44780_DB0					GPIO_Pin_11
#define GPIO_PORT_DB0		        GPIOE

#define HD44780_DB1					GPIO_Pin_12
#define GPIO_PORT_DB1		        GPIOE

#define HD44780_DB2					GPIO_Pin_13
#define GPIO_PORT_DB2		        GPIOE

#define HD44780_DB3					GPIO_Pin_14
#define GPIO_PORT_DB3		        GPIOE

#define HD44780_DB4					GPIO_Pin_15
#define GPIO_PORT_DB4		        GPIOE

#define HD44780_DB5					GPIO_Pin_10
#define GPIO_PORT_DB5		        GPIOB

#define HD44780_DB6					GPIO_Pin_11
#define GPIO_PORT_DB6		        GPIOB

#define HD44780_DB7					GPIO_Pin_12
#define GPIO_PORT_DB7		        GPIOB

#define RS_COMMAND				0
#define RS_CHAR						1

/* Commands */
#define HD44780_TWO_LINE_ENABLE		    	0x38
#define HD44780_8_BIT_MODE              0x30
#define HD44780_DISPLAY_ON							0x0C
#define HD44780_CURSOR_UNDERLINE  			0x02
#define HD44780_CURSOR_BLINK 						0x09
#define HD44780_CURSOR_BLINK_UNDERLINE 	0x0F
#define HD44780_CLEAR_DISPLAY						0x01
#define HD44780_LOCATION_COMMAND        0x80

/* function to return the right enum if VAL == 0 or VAL == 1 */
/* function needed to remove warning when GPIO_WriteBit expects an enum */
/* whereas an int is provided instead */
#define BIT_ACTION(VAL) ((VAL) == 0 ? Bit_RESET: Bit_SET)

/**
	* @brief Initialize the LCD to its default state
	*/
void hd44780_init(void);


/**
	* @brief Sets the data pins (D7-D0) individually to 0 or 1
	* @param val -the data to send to the data lines
	* @param rs_line -value of the RS line
	*/
void hd44780_send_data(uint8_t val, uint8_t rs_line);

/**
	* @brief Enables write to the LCD
	* @param rs_line -value of the RS line
	*/
void hd44780_enable_write(uint8_t rs_line);

/**
	* @brief Write the characters given by text parameter to the display
	* @param text -the characters to display on the LCD
	* @param length -length of the text
	*/
void hd44780_write_char(char *text, uint8_t length);

/**
	* @brief Clear the display
	* Data lines (D7-D0) = 00000001 = 0x01
	*/
void hd44780_clear_display(void);

/**
	* @brief Move the cursor to a location on the screen based on location parameter
	* @param location -the int of the location to move to
	*/
void hd44780_move_cursor(uint8_t location);

/**
	* @brief Move the cursor to the second line
	*/
void hd44780_move_second_line(void);

/**
	* @brief Make the cursor blink
	* Data lines (D7-D0) = 00001001 = 0x09
	*/
void hd44780_blink_cursor(void) ;

#endif
