#include "keypad.h"

//2-D array for keypad
int A[4][3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } , { 10, 0, 11 } };

int column;
int row;	
extern float alpha;
extern float beta;
	
/* ROW: output, 0, No PULL */
GPIO_InitTypeDef GPIO_InitStructure1;

/* COLUMN: input, 1, PULL UP */
GPIO_InitTypeDef GPIO_InitStructure2;

/**
 * Initialize the keypad
 *
 * - Sets up GPIO pins (PD4-PD7)
 * - Enables the keypad
 *
 */	
void keypad_configuration() {
	RCC_AHB1PeriphClockCmd(KEYPAD_GPIO_CLOCK, ENABLE);
	
	/* ROW: output, 0, No PULL */
	
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD1| KEYPAD_PD2| KEYPAD_PD3| KEYPAD_PD4;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	/* COLUMN: input, 1, PULL UP */
	
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD5| KEYPAD_PD6| KEYPAD_PD7;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
}
//Switch back to original rows and columns
void row_check(void) {
	/* Row to column */
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
		
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD5| KEYPAD_PD6| KEYPAD_PD7;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	/* Column to row */
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD1| KEYPAD_PD2| KEYPAD_PD3| KEYPAD_PD4;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
}

//Switch rows to columns and columns to rows
void column_check(void) {
	/* Row to column */
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
		
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD1| KEYPAD_PD2| KEYPAD_PD3| KEYPAD_PD4;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	/* Column to row */
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD5| KEYPAD_PD6| KEYPAD_PD7;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
}

void keypad_press_check() {
	
	int isPressed = 0;
	
	//Check columns first
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
	
	if(isPressed){													
		//Check rows 
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
		
		//Switch back to column check
		column_check();

	}
}

//Get the key pressed
int get_key_pressed(int row, int column)
{
	return A[row][column];
}

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
