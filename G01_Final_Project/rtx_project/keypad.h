#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "stm32f4xx_conf.h"

/**
	* @file keypad.h
	* @brief Functions to initialize operate keypad
	*/
	
/* Clock for Port D */
#define KEYPAD_GPIO_CLOCK 	RCC_AHB1Periph_GPIOD

/* Keypad pins are connected to port D */
#define GPIO_PORT_D		      GPIOD

/* Row definition */
#define KEYPAD_PD1					GPIO_Pin_1
#define KEYPAD_PD2					GPIO_Pin_2
#define KEYPAD_PD3					GPIO_Pin_3
#define KEYPAD_PD4					GPIO_Pin_4

/* Column definition */
#define KEYPAD_PD5					GPIO_Pin_5
#define KEYPAD_PD6					GPIO_Pin_6
#define KEYPAD_PD7					GPIO_Pin_7

/**
	* @brief Initialize the keypad
	* 
	* -Sets up the GPIO pins (PD0-PD7) and enable port D clock
	* -Sets row pins (PD1, PD2, PD3, PD4) in output mode and no pull	
	* -Sets column pins (PD5, PD6, PD7) in input mode and pullup
	*
	*/
void keypad_configuration(void);

/**
	* @brief Switch rows to input mode and columns to output mode
	* 
	* -Sets column pins (PD5, PD6, PD7) in output mode and no pull
 	* -Sets row pins (PD1, PD2, PD3, PD4) in input mode and pullup
	*
	*/
void row_check(void);

/**
	* @brief Switch rows to output mode and columns to input mode
	* 
	* -Sets row pins (PD1, PD2, PD3, PD4) in output mode and no pull	
	* -Sets column pins (PD5, PD6, PD7) in input mode and pullup
	*
	*/
void column_check(void);

/**
	* @brief Determine the corresponding column and row of the key pressed
	*
	* -Scanning algorithm
	* -Pull the column inputs to see if a 0 is detected
	* -When a column is 0, set isPressed variable to 1
	* -Call row_check() function to switch the column and row configuration (rows as inputs)
	* -Pull the row input to see if a 0 is detected
	* -Call column_check() function to revert column and row configuration (columns as inputs)
	*
	*/
void keypad_press_check(void);

/**
	* @brief Determine the key number pressed based on the row and column parameters
	* @param row -the row number of the corresponding key
	* @param column -the column number of the corresponding key
	* @return the key pressed on the keypad
	*
	* -Use the 2D array to map the row and column parameters to the key pressed
	*
	*/
int get_key_pressed(int row, int column);

/**
	* @brief Increase/decrease alpha and beta based on the key
	* @param key -the key pressed
	*
	* key=2 -> alpha--
	* key=8 -> alpha++
	* key=4 -> beta--
	* key=6 -> beta++
	*
	*/
void keypad_control(int key);

#endif
