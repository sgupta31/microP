#include "keypad.h"



/* ROW: output, 0, No PULL */
GPIO_InitTypeDef GPIO_InitStructure1;

/* COLUMN: input, 1, PULL UP */
GPIO_InitTypeDef GPIO_InitStructure2;

/**
 * Initialize the keypad
 *
 * - Sets up GPIO pins (PD0-PD7)
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
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD0;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD1;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD2;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD3;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	/* COLUMN: input, 1, PULL UP */
	
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD5;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD6;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD7;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
}

void row_check(void) {
/* Row to column */
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
		
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD0;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD1;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD2;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD3;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	/* Column to row */
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD5;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD6;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD7;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
}

void column_check(void) {
	/* Row to column */
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
		
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD5;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD6;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	GPIO_InitStructure2.GPIO_Pin = KEYPAD_PD7;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure2);
	
	/* Column to row */
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD0;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD1;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD2;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIO_Pin = KEYPAD_PD3;
	GPIO_Init(GPIO_PORT_D, &GPIO_InitStructure1);

}

void keypad_press_check() {
	
	int isPressed = 0;
	//Check columns first
	if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD5) == 0) {
		printf("Column 1\n");
		isPressed = 1;
	}
	
	if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD6) == 0) {
		printf("Column 2\n");
		isPressed = 1;
	}
	
	if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD7) == 0) {
		printf("Column 3\n");
		isPressed = 1;
	}
	
	if(isPressed){
		
		column_check();

		if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD0) == 0) {
			printf("Row 4\n");
		}
		
		if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD1) == 0) {
			printf("Row 1\n");
		}
		
		if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD2) == 0) {
			printf("Row 2\n");
		}
		
		if (GPIO_ReadInputDataBit(GPIO_PORT_D, KEYPAD_PD3) == 0) {
			printf("Row 3\n");
		}
		
		row_check();

	}
}
