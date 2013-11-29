#include "keypad.h"

/**
	* @file keypad.c
	* @brief Functions to initialize operate keypad
	*/
	
/* 2-D array for keypad button mapping */
int A[4][3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } , { 10, 0, 11 } };

int column;
int row;	
extern float alpha;
extern float beta;
	
/* ROWS */
GPIO_InitTypeDef GPIO_InitStructure1;

/* COLUMNS */
GPIO_InitTypeDef GPIO_InitStructure2;

/**
	* @brief Initialize the keypad
	* 
	* -Sets up the GPIO pins (PD0-PD7) and enable port D clock
	* -Sets row pins (PD1, PD2, PD3, PD4) in output mode and no pull	
	* -Sets column pins (PD5, PD6, PD7) in input mode and pullup
	*
	*/
void keypad_configuration() {
	RCC_AHB1PeriphClockCmd(KEYPAD_GPIO_CLOCK, ENABLE);
	
	/* ROW: output mode, No PULL, initial button state = 0 */
	
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD1| KEYPAD_PD2| KEYPAD_PD3| KEYPAD_PD4;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	/* COLUMN: input mode, PULL UP, initial button state = 1 */
	
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD5| KEYPAD_PD6| KEYPAD_PD7;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
}

/**
	* @brief Switch rows to input mode and columns to output mode
	* 
	* -Sets column pins (PD5, PD6, PD7) in output mode and no pull
 	* -Sets row pins (PD1, PD2, PD3, PD4) in input mode and pullup
	*
	*/
void row_check(void) {
	
	/* PD5, PD6, PD7 in output mode */
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
		
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD5| KEYPAD_PD6| KEYPAD_PD7;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	/* PD1, PD2, PD3, PD4 in input mode */
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD1| KEYPAD_PD2| KEYPAD_PD3| KEYPAD_PD4;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
}

/**
	* @brief Switch rows to output mode and columns to input mode
	* 
	* -Sets row pins (PD1, PD2, PD3, PD4) in output mode and no pull	
	* -Sets column pins (PD5, PD6, PD7) in input mode and pullup
	*
	*/
void column_check(void) {
	
	/* PD1, PD2, PD3, PD4 in input mode */
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
		
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD1| KEYPAD_PD2| KEYPAD_PD3| KEYPAD_PD4;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	/* PD5, PD6, PD7 in input mode */
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD5| KEYPAD_PD6| KEYPAD_PD7;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
}

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
void keypad_press_check() {
	
	int isPressed = 0;
	
	/* Check columns for 0 */
	if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD5) == 0) {
		column = 0;
		isPressed = 1;
	}
	
	if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD6) == 0) {
		column = 1;
		isPressed = 1;
	}
	
	if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD7) == 0) {
		column = 2;
		isPressed = 1;
	}
	
	/* Check rows only when a 0 was detected on one of the column inputs */
	if(isPressed){													
		/* Set rows as input and column as ouputs */
		row_check();

		if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD1) == 1 && GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD2) == 1 && 
			GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD3) == 1 && GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD4) == 0) {
			row = 3;
		}
		
		if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD1) == 0) {
			row = 0;
		}
		
		if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD2) == 0) {
			row = 1;
		}
		
		if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD3) == 0) {
			row = 2;
		}
		
		/* Set columns as input and rows as ouputs */
		column_check();

	}
}

/**
	* @brief Determine the key number pressed based on the row and column parameters
	* @param row -the row number of the corresponding key
	* @param column -the column number of the corresponding key
	* @return the key pressed on the keypad
	*
	* -Use the 2D array to map the row and column parameters to the key pressed
	*
	*/
int get_key_pressed(int row, int column)
{
	return A[row][column];
}

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
void keypad_control(int key){
	switch(key){
		case 2: 
			alpha--;
			break;
		case 4:
			beta--;
			break;
		case 6: 
			beta++;
			break;
		case 8: 
			alpha++;
			break;
	};
}
