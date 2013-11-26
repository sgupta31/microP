#ifndef __LCD_H
#define __LCD_H

#include <inttypes.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define HD44780_GPIO_CLOCK1 	RCC_AHB1Periph_GPIOE
#define HD44780_GPIO_CLOCK2 	RCC_AHB1Periph_GPIOB


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


#define HD44780_TWO_LINE_ENABLE		    0x38
#define HD44780_8_BIT_MODE              0x30

#define HD44780_DISPLAY_ON				0x0C
#define HD44780_CURSOR_UNDERLINE  0x02
#define HD44780_CURSOR_BLINK 			0x09
#define HD44780_CURSOR_BLINK_UNDERLINE 			0x0F


#define HD44780_CLEAR_DISPLAY			0x01
#define HD44780_LOCATION_COMMAND        0x80
/* function to return the right enum if VAL == 0 or VAL == 1 */
/* function needed to remove warning when GPIO_WriteBit expects an enum */
/* whereas an int is provided instead */

#define BIT_ACTION(VAL) ((VAL) == 0 ? Bit_RESET: Bit_SET)

/**
 * Initialize the display
 *
 * - Sets up GPIO pins (PE3-PE8)
 * - Initialize nibble mode
 * - Enables the display
 *
 * @param void
 * @returns void
 */
void hd44780_init(void);


/**
 * @param val: value to write to display register
 * @param rs_line: either RS_COMMAND or RS_CHAR
 *
 * @returns void
 */

void hd44780_send_data(uint8_t val, uint8_t rs_line);

/**
 * Sets the enable line to low, then to high again
 *
 * Doing so makes the data in line D7-D4 be written to the register of the
 * display.
 * Depending on rs_line, the RS GPIO will be set to '1' or '0'
 *
 * @param rs_line: either RS_COMMAND or RS_CHAR
 * @returns void
 */
void hd44780_enable_write(uint8_t rs_line);

/**
 * Clears the display -- erases any text on the display
 *
 * @param void
 * @returns void
 */
void hd44780_clear_display(void);

/**
 * Write characters to the display (maybe modify it to consider cursor position)
 *
 * @param text: String to write to display
 * @param length: length of text to print to display
 *
 * @returns void
 */
void hd44780_write_char(char *text, uint8_t length);

/**
 * Move the cursor to location specified in parameter
 *
 * @param location: location to move cursor to
 * @returns void
 */
void hd44780_move_cursor(uint8_t location);

void hd44780_move_second_line(void);

void hd44780_blink_cursor(void) ;

#endif
