#ifndef __LCD_H
#define __LCD_H

#include <inttypes.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/**
	* @file lcd.h
	* @brief Functions to initialize operate the LCD
	* 
	* LCD Driver
	*
	* Contrast line should be connected to GND
	* R/W line should be connected to GND since we always write to LCD
	* Vss connect to GND
	* Vdd can be connected to +5V
	*
	*/

/* Clocks for port E and B */
#define GPIO_CLOCK1 	RCC_AHB1Periph_GPIOE
#define GPIO_CLOCK2 	RCC_AHB1Periph_GPIOB

/* Pin definitions */
#define RS GPIO_Pin_9
#define GPIO_PORT_RS GPIOE

#define E GPIO_Pin_10
#define GPIO_PORT_E	GPIOE

#define D0 GPIO_Pin_11
#define GPIO_PORT_D0 GPIOE

#define D1 GPIO_Pin_12
#define GPIO_PORT_D1 GPIOE

#define D2 GPIO_Pin_13
#define GPIO_PORT_D2 GPIOE

#define D3 GPIO_Pin_14
#define GPIO_PORT_D3 GPIOE

#define D4 GPIO_Pin_15
#define GPIO_PORT_D4 GPIOE

#define D5 GPIO_Pin_10
#define GPIO_PORT_D5 GPIOB

#define D6 GPIO_Pin_11
#define GPIO_PORT_D6 GPIOB

#define D7 GPIO_Pin_12
#define GPIO_PORT_D7 GPIOB

#define RS_COMMAND 0
#define RS_CHAR	1

/* Commands */
#define TWO_LINE_ENABLE 0x38
#define EIGHT_BIT_MODE 0x30
#define DISPLAY_ON 0x0C
#define CURSOR_UNDERLINE 0x02
#define CURSOR_BLINK 0x09
#define CURSOR_BLINK_UNDERLINE 0x0F
#define CLEAR_DISPLAY 0x01
#define LOCATION_COMMAND 0x80

/* function to return the right enum if VAL == 0 or VAL == 1 */
/* function needed to remove warning when GPIO_WriteBit expects an enum */
/* whereas an int is provided instead */
//#define BIT_ACTION(VAL) ((VAL) == 0 ? Bit_RESET: Bit_SET)

/**
	* @brief Initialize the LCD to its default state
	*/
void lcd_init(void);


/**
	* @brief Sets the data pins (D7-D0) individually to 0 or 1
	* @param val -the data to send to the data lines
	* @param rs_line -value of the RS line
	*/
void send_data(uint8_t val, uint8_t rs_line);

/**
	* @brief Enables write to the LCD
	* @param rs_line -value of the RS line
	*/
void enable_write(uint8_t rs_line);

/**
	* @brief Write the characters given by text parameter to the display
	* @param text -the characters to display on the LCD
	* @param length -length of the text
	*/
void write_char(char *text, uint8_t length);

/**
	* @brief Clear the display
	* Data lines (D7-D0) = 00000001 = 0x01
	*/
void clear_display(void);

/**
	* @brief Move the cursor to a location on the screen based on location parameter
	* @param location -the int of the location to move to
	*/
void move_cursor(uint8_t location);

/**
	* @brief Move the cursor to the second line
	*/
void move_second_line(void);

/**
	* @brief Make the cursor blink
	* Data lines (D7-D0) = 00001001 = 0x09
	*/
void blink_cursor(void) ;

/**
	* @brief Funcion return the BitAction depending on the val parameter
	* @return Bit_SET if val=1, Bit_RESET if val=0
	*/
BitAction convert_to_bitaction(uint8_t val);

#endif
