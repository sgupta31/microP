/**
* @file lcd.c
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief  Functions to initialize operate the LCD
*
*/

#ifndef __LCD_H
#define __LCD_H

#include <inttypes.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/* Clocks for port E and B */
#define GPIO_CLOCK1 	RCC_AHB1Periph_GPIOE
#define GPIO_CLOCK2 	RCC_AHB1Periph_GPIOB

/* Pin definitions */
#define RS GPIO_Pin_9
#define GPIO_PORT_RS GPIOE

#define E GPIO_Pin_10
#define GPIO_PORT_E	GPIOE

#define GPIO_PORT_E GPIOE
#define GPIO_PORT_B GPIOB

/* GPIO port E */
#define D0 GPIO_Pin_11
#define D1 GPIO_Pin_12
#define D2 GPIO_Pin_13
#define D3 GPIO_Pin_14
#define D4 GPIO_Pin_15

/* GPIO port B */
#define D5 GPIO_Pin_10
#define D6 GPIO_Pin_11
#define D7 GPIO_Pin_12

#define RS_COMMAND 0
#define RS_CHAR	1

/* Commands */
#define EIGHT_BIT_MODE 0x30
#define DISPLAY_ON 0x0C
#define CLEAR_DISPLAY 0x01
#define LOCATION_COMMAND 0x80

/**
	* @brief Initialize the LCD to its default state
	*/
void lcd_init(void);

/**
	* @brief Sets the data pins (D7-D0) individually to 0 or 1
	* @param data -the data to send to the data lines
	* @param rs_line -value of the RS line
	*/
void send_data(uint8_t data, uint8_t rs_line);

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
	* @brief Funcion return the BitAction depending on the val parameter
	* @return Bit_SET if data=1, Bit_RESET if data=0
	*/
BitAction convert_to_bitaction(uint8_t data);

#endif
