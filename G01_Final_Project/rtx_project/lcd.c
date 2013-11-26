#include "lcd.h"
#include "cmsis_os.h"
/**
 * === HD44780 Driver! ===
 *
 * == General notes:
 *
 * Contrast line should be connected to GND
 * R/W line should be connected to GND since we always write to LCD
 *
 * Vss connect to GND
 * Vdd can be connected to +5V or +3V
 *
 * See the header file to see the port assignments
 *
 * Usage example:
 * int main (void) {
 *
 *   hd44780_init();
 *   
 *   while(1) {
 *       hd44780_clear_display();
 *       osDelay(1000);
 *       hd44780_write_char("uP", 2);
 *       osDelay(1000);
 *       hd44780_write_char(" is", 3);
 *       osDelay(1000);
 *       hd44780_move_second_line();
 *       hd44780_write_char("awesome!", 8);
 *       osDelay(5000);
 *   }
 * }
 */

/**
 * Initializes the GPIO pins that will be used with the LCD display
 *
 * @param void
 * @returns void
 */
static void gpio_hd44780_init(void) {
	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable the GPIO clock */
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

void hd44780_init(void) {

	/* Initialize the GPIO pins first */
	gpio_hd44780_init();

	/* make sure we are in 8-bit mode before we switch to nibble mode */
	/* we want to be in a reset state first */
    hd44780_send_data(HD44780_8_BIT_MODE, RS_COMMAND);

    /* Enable two lines display */
    hd44780_send_data(HD44780_TWO_LINE_ENABLE, RS_COMMAND);

	/* switch on the display */
	hd44780_send_data(HD44780_DISPLAY_ON, RS_COMMAND);
}

void hd44780_send_data(uint8_t val, uint8_t rs_line) {

    /* Set all the gpio pins on or off depending on variable val */
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

void hd44780_enable_write(uint8_t rs_line) {

	/* Set the rs line to low / high based on parameter rs_line */
	/* used to tell display whether to interpret data as command or char */
	osDelay(1);
	/* if rs_line == 0, interpret data as command, else, interpret data */
	/* as characters to display */
	GPIO_WriteBit(GPIO_PORT_RS, HD44780_RS, BIT_ACTION(rs_line));
    
	osDelay(1);
    
	/* Set the Enable pin high */
	GPIO_SetBits(GPIO_PORT_E, HD44780_E);
	osDelay(1);
    
	/* Set the Enable pin low -- data written to lcd */
	GPIO_ResetBits(GPIO_PORT_E, HD44780_E);
	osDelay(1);
}

void hd44780_clear_display(void) {
	hd44780_send_data(HD44780_CLEAR_DISPLAY, RS_COMMAND);
}

void hd44780_write_char(char *text, uint8_t length) {
	uint8_t i;

	for(i = 0; i < length; ++i) {
		/* write char to display */
		hd44780_send_data(*text, RS_CHAR);

		/* increment to next char to display */
		++text;
	}
}

void hd44780_move_cursor(uint8_t location) {
	hd44780_send_data(HD44780_LOCATION_COMMAND | location, RS_COMMAND);
}

void hd44780_move_second_line(void) {
    hd44780_move_cursor(40);
}

void hd44780_blink_cursor(void) {
	hd44780_send_data(HD44780_CURSOR_BLINK_UNDERLINE, RS_COMMAND);
}
