/**
* @file lcd.c
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief  Functions to initialize operate the LCD
*
*/

#include "lcd.h"
#include "cmsis_os.h"

/**
 * @brief Initializes the GPIO pins that will be used with the LCD display
 */
static void gpio_init() {
	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable the GPIO clock for port B and E */
	RCC_AHB1PeriphClockCmd(GPIO_CLOCK1, ENABLE);  
	RCC_AHB1PeriphClockCmd(GPIO_CLOCK2, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* Rs line */
	GPIO_InitStructure.GPIO_Pin = RS;
	GPIO_Init(GPIO_PORT_RS, &GPIO_InitStructure);

	/* Enable line */
	GPIO_InitStructure.GPIO_Pin = E;
	GPIO_Init(GPIO_PORT_E, &GPIO_InitStructure);

	/* set the Enable pin to '0' */
  /* For writing to display, data is transferred only on the high to low */
  /* transition of this signal */
	GPIO_ResetBits(GPIO_PORT_E, E);

  /* Now initialize the data line, DB7 - DB0 */
  /* See the header file for the exact pin assignments */
	
	GPIO_InitStructure.GPIO_Pin = D0 | D1 | D2 | D3 | D4;
	GPIO_Init(GPIO_PORT_E, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = D5 | D6 | D7;
	GPIO_Init(GPIO_PORT_B, &GPIO_InitStructure);
	
}

/**
	* @brief Initialize the LCD to its default state
	*/
void lcd_init() {

	/* Initialize the GPIO pins first */
	gpio_init();
	
	/* Initialize to 8 Bit mode */
	send_data(EIGHT_BIT_MODE, RS_COMMAND);

	/* Turn on the display */
	send_data(DISPLAY_ON, RS_COMMAND);
}

/**
	* @brief Sets the data pins (D7-D0) individually to 0 or 1
	* @param data -the data to send to the data lines
	* @param rs_line -value of the RS line
	*/
void send_data(uint8_t data, uint8_t rs_line) {

	uint8_t bitValue = 0x01 & data;
	/* Right shift the data parameter and AND with 0x01 to get the corresponding bit */
	GPIO_WriteBit(GPIO_PORT_E, D0, convert_to_bitaction(bitValue));
	
	bitValue = 0x01 & (data >>= 1);
	GPIO_WriteBit(GPIO_PORT_E, D1, convert_to_bitaction(bitValue));

	bitValue = 0x01 & (data >>= 1);
	GPIO_WriteBit(GPIO_PORT_E, D2, convert_to_bitaction(bitValue));

	bitValue = 0x01 & (data >>= 1);
	GPIO_WriteBit(GPIO_PORT_E, D3, convert_to_bitaction(bitValue)); 

	bitValue = 0x01 & (data >>= 1);
	GPIO_WriteBit(GPIO_PORT_E, D4, convert_to_bitaction(bitValue));

	bitValue = 0x01 & (data >>= 1);
	GPIO_WriteBit(GPIO_PORT_B, D5, convert_to_bitaction(bitValue));
    
	bitValue = 0x01 & (data >>= 1);
	GPIO_WriteBit(GPIO_PORT_B, D6, convert_to_bitaction(bitValue));
    
	bitValue = 0x01 & (data >>= 1);
	GPIO_WriteBit(GPIO_PORT_B, D7, convert_to_bitaction(bitValue));
       
	/* write the data to the display */
	osDelay(1);
	
	/* Set the RS line to high/low based on rs_line parameter */
	/* rs_line = 0, command */
	/* rs_line = 1, character */
	GPIO_WriteBit(GPIO_PORT_RS, RS, convert_to_bitaction(rs_line));
    
	osDelay(1);
	
  /* To send character/command do: Enable pin high, delay, low */ 
	/* Set the Enable pin high */
	GPIO_SetBits(GPIO_PORT_E, E);
	
	/* Delay */
	osDelay(1);
    
	/* Set the Enable pin low */
	GPIO_ResetBits(GPIO_PORT_E, E);

	osDelay(1);
}

/**
	* @brief Write the characters given by text parameter to the display
	* @param text -the characters to display on the LCD
	* @param length -length of the text
	*/
void write_char(char *text, uint8_t length) {
	uint8_t i;
	
	/* Loop through the charactes to send */
	for(i = 0; i < length; ++i) {
		/* Send the char */
		send_data(*text, RS_CHAR);

		/* Increment to next char */
		++text;
	}
}

/**
	* @brief Clear the display
	* Data lines (D7-D0) = 00000001 = 0x01
	*/
void clear_display() {
	send_data(CLEAR_DISPLAY, RS_COMMAND);
}

/**
	* @brief Move the cursor to a location on the screen based on location parameter
	* @param location -the int of the location to move to
	*/
void move_cursor(uint8_t location) {
	send_data(LOCATION_COMMAND | location, RS_COMMAND);
}

/**
	* @brief Helper Function return the BitAction depending on the data parameter
	* @return Bit_SET if data=1, Bit_RESET if data=0
	*/
BitAction convert_to_bitaction(uint8_t data) {
	if (data) 
	{
		return Bit_SET;
	} 
	else 
	{
		return Bit_RESET;
	}
}
