#include "lcd.h"
#include "cmsis_os.h"

/**
	* @file lcd.c
	* @brief Functions to initialize operate the LCD
	* 
	* LCD Driver
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
	GPIO_InitStructure.GPIO_Pin = D0;
	GPIO_Init(GPIO_PORT_D0, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D1;
	GPIO_Init(GPIO_PORT_D1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D2;
	GPIO_Init(GPIO_PORT_D2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D3;
	GPIO_Init(GPIO_PORT_D3, &GPIO_InitStructure);  
    
 	GPIO_InitStructure.GPIO_Pin = D4;
	GPIO_Init(GPIO_PORT_D4, &GPIO_InitStructure);  
    
 	GPIO_InitStructure.GPIO_Pin = D5;
	GPIO_Init(GPIO_PORT_D5, &GPIO_InitStructure);  
    
 	GPIO_InitStructure.GPIO_Pin = D6;
	GPIO_Init(GPIO_PORT_D6, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = D7;
	GPIO_Init(GPIO_PORT_D7, &GPIO_InitStructure);    
	
}

/**
	* @brief Initialize the LCD to its default state
	*/
void lcd_init() {

	/* Initialize the GPIO pins first */
	gpio_init();

	/* Make sure we are in 8-bit mode before we switch to nibble mode */
	/* We want to be in a reset state first */
	send_data(EIGHT_BIT_MODE, RS_COMMAND);

	/* Enable two lines display */
	send_data(TWO_LINE_ENABLE, RS_COMMAND);

	/* Turn on the display */
	send_data(DISPLAY_ON, RS_COMMAND);
}

/**
	* @brief Sets the data pins (D7-D0) individually to 0 or 1
	* @param val -the data to send to the data lines
	* @param rs_line -value of the RS line
	*/
void send_data(uint8_t val, uint8_t rs_line) {

	/* Right shift the val parameter and AND with 0x01 to get the corresponding bit */
	GPIO_WriteBit(GPIO_PORT_D0, D0, convert_to_bitaction(0x01 & val));
	
	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_D1, D1, convert_to_bitaction(0x01 & val));

	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_D2, D2, convert_to_bitaction(0x01 & val));

	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_D3, D3, convert_to_bitaction(0x01 & val)); 

	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_D4, D4, convert_to_bitaction(0x01 & val));

	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_D5, D5, convert_to_bitaction(0x01 & val));
    
	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_D6, D6, convert_to_bitaction(0x01 & val));
    
	val >>= 1;
	GPIO_WriteBit(GPIO_PORT_D7, D7, convert_to_bitaction(0x01 & val));
       
	/* write the data to the display */
	enable_write(rs_line);
}

/**
	* @brief Enables write to the LCD
	* @param rs_line -value of the RS line
	*/
void enable_write(uint8_t rs_line) {

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
	* @brief Move the cursor to the second line
	*/
void move_second_line() {
    move_cursor(40);
}

/**
	* @brief Make the cursor blink
	* Data lines (D7-D0) = 00001001 = 0x09
	*/
void blink_cursor() {
	send_data(CURSOR_BLINK, RS_COMMAND);
}

/**
	* @brief Funcion return the BitAction depending on the val parameter
	* @return Bit_SET if val=1, Bit_RESET if val=0
	*/
BitAction convert_to_bitaction(uint8_t val) {
	if (val) 
	{
		return Bit_SET;
	} 
	else 
	{
		return Bit_RESET;
	}
}
