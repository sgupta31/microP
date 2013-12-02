#include "arm_math.h"
#include "stm32f4xx.h"
#include "pwm.h"
#include "servo.h"
#include "lcd.h"
#include "keypad.h"

int ticks;
extern int row;
extern int column;
float alpha = 0.0;
float beta = 0.0;

/**
  * @brief Program entry point
  */
int main (void) {
 	PWM_configure();
	lcd_init();
	keypad_configuration();
	
	ticks = 0;
	printf("main\n");
	while(1) {
		
		while(!ticks);
		ticks = 0;

// 		alpha_motor(alpha);
// 		beta_motor(beta);
		
// 		hd44780_clear_display();
// 		hd44780_blink_cursor();
// 		osDelay(1000);
//     hd44780_write_char("uP", 2);
//     osDelay(1000);
//     hd44780_write_char(" is", 3);
//     osDelay(1000);
//     hd44780_move_second_line();
// 		osDelay(1000);
// 		hd44780_write_char("awesome!", 8);
//     osDelay(5000);

		keypad_press_check();
		printf("key pressed = %d\n", get_key_pressed(row, column));
		
	}
}

void TIM3_IRQHandler(void)
{
	/* Checking if the TIM3 interrupt has occured */
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // Clear the TIM3 interrupt pending bit
		ticks = 1;
	}
}

