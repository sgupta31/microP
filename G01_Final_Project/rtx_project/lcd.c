#include "lcd.h"
#include "cmsis_os.h"

/**
	* @file lcd.c
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

/**
 * @brief Initializes the GPIO pins that will be used with the LCD display
 */
static void gpio_hd44780_init() {
	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable the GPIO clock for port B and E */
	RCC_AHB1PeriphClockCmd(HD44780_GPIO_CLOCK1, ENABLE);  
	RCC_AHB1PeriphClockCmd(HD44780_GPIO_CLOCK2, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* Rs line */
	GPIO_InitStructure.GPIO_Pin = HD44780_RS;
	GPIO_Init(GPIO_PORT_RS, &GPIO_InitStructure);

	/* Enable line */
	GPIO_InitStructure.GPIO_Pin = HD44780_E;
	GPIO_Init(GPIO_PORT_E, &GPIO_InitStructure);

	/* set the Enable pin to '0' */
  /* For writing to display, data is transferred only on the high to low */
  /* transition of this signal */
	GPIO_ResetBits(GPIO_PORT_E, HD44780_E);

  /* Now initialize the data line, DB7 - DB0 */
  /* See the header file for the exact pin assignments */
	GPIO_InitStructure.GPIO_Pin = HD44780_DB0;
	GPIO_Init(GPIO_PORT_DB0, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = HD44780_DB1;
	GPIO_Init(GPIO_PORT_DB1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = HD44780_DB2;
	GPIO_Init(GPIO_PORT_DB2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = HD44780_DB3;
	GPIO_Init(GPIO_PORT_DB3, &GPIO_InitStructure);  
    
 	GPIO_InitStructure.GPIO_Pin = HD44780_DB4;
	GPIO_Init(GPIO_PORT_DB4, &GPIO_InitStructure);  
    
 	GPIO_InitStructure.GPIO_Pin = HD44780_DB5;
	GPIO_Init(GPIO_PORT_DB5, &GPIO_InitStructure);  
    
 	GPIO_InitStructure.GPIO_Pin = HD44780_DB6;
	GPIO_Init(GPIO_PORT_DB6, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = HD44780_DB7;
	GPIO_Init(GPIO_PORT_DB7, &GPIO_InitStructure);    
	
}

/**
	* @brief Initialize the LCD to its default state
	*/
void hd44780_init() {

	/* Initialize the GPIO pins first */
	gpio_hd44780_init();

	/* Make sure we are in 8-bit mode before we switch to nibble mode */
	/* We want to be in a reset state first */
	hd44780_send_data(HD44780_8_BIT_MODE, RS_COMMAND);

	/* Enable two lines display */
	hd44780_send_data(HD44780_TWO_LINE_ENABLE, RS_COMMAND);

	/* Turn on the display */
	hd44780_send_data(HD44780_DISPLAY_ON, RS_COMMAND);
}

/**
	* @brief Sets the data pins (D7-D0) individually to 0 or 1
	* @param val -the data to send to the data lines
	* @param rs_line -value of the RS line
	*/
void hd44780_send_data(uint8_t val, uint8_t rs_line) {

	/* Right shift the val parameter and AND with 0x01 to get the corresponding bit */
	GPIO_WriteBit(GPIO_PORT_DB0, HD44780_DB0, BIT_ACTION(0x01 & val));
	
	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_DB1, HD44780_DB1, BIT_ACTION(0x01 & val));

	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_DB2, HD44780_DB2, BIT_ACTION(0x01 & val));

	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_DB3, HD44780_DB3, BIT_ACTION(0x01 & val)); 

	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_DB4, HD44780_DB4, BIT_ACTION(0x01 & val));

	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_DB5, HD44780_DB5, BIT_ACTION(0x01 & val));
    
	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_DB6, HD44780_DB6, BIT_ACTION(0x01 & val));
    
	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_DB7, HD44780_DB7, BIT_ACTION(0x01 & val));
       
	/* write the data to the display */
	hd44780_enable_write(rs_line);
}

/**
	* @brief Enables write to the LCD
	* @param rs_line -value of the RS line
	*/
void hd44780_enable_write(uint8_t rs_line) {

	osDelay(1);
	
	/* Set the RS line to high/low based on rs_line parameter */
	/* rs_line = 0, command */
	/* rs_line = 1, character */
	GPIO_WriteBit(GPIO_PORT_RS, HD44780_RS, BIT_ACTION(rs_line));
    
	osDelay(1);
	
  /* To send character/command do: Enable pin high, delay, low */ 
	/* Set the Enable pin high */
	GPIO_SetBits(GPIO_PORT_E, HD44780_E);
	
	/* Delay */
	osDelay(1);
    
	/* Set the Enable pin low */
	GPIO_ResetBits(GPIO_PORT_E, HD44780_E);

	osDelay(1);
}

/**
	* @brief Write the characters given by text parameter to the display
	* @param text -the characters to display on the LCD
	* @param length -length of the text
	*/
void hd44780_write_char(char *text, uint8_t length) {
	uint8_t i;
	
	/* Loop through the charactes to send */
	for(i = 0; i < length; ++i) {
		/* Send the char */
		hd44780_send_data(*text, RS_CHAR);

		/* Increment to next char */
		++text;
	}
}

/**
	* @brief Clear the display
	* Data lines (D7-D0) = 00000001 = 0x01
	*/
void hd44780_clear_display() {
	hd44780_send_data(HD44780_CLEAR_DISPLAY, RS_COMMAND);
}

/**
	* @brief Move the cursor to a location on the screen based on location parameter
	* @param location -the int of the location to move to
	*/
void hd44780_move_cursor(uint8_t location) {
	hd44780_send_data(HD44780_LOCATION_COMMAND | location, RS_COMMAND);
}

/**
	* @brief Move the cursor to the second line
	*/
void hd44780_move_second_line() {
    hd44780_move_cursor(40);
}

/**
	* @brief Make the cursor blink
	* Data lines (D7-D0) = 00001001 = 0x09
	*/
void hd44780_blink_cursor() {
	hd44780_send_data(HD44780_CURSOR_BLINK, RS_COMMAND);
}
