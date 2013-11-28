#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "stm32f4xx_conf.h"

#define KEYPAD_GPIO_CLOCK 	RCC_AHB1Periph_GPIOD

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
 * Initialize the keypad
 *
 * - Sets up GPIO pins (PD0-PD7)
 * - Enables the keypad
 *
 */
void keypad_configuration(void);

void column_check(void);

void row_check(void);

void keypad_press_check(void);

int get_key_pressed(int row, int column);

void keypad_control(int key);
#endif
