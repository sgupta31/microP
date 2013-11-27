#include "arm_math.h"
#include "stm32f4xx.h"
#include "pwm.h"
#include "servo.h"
#include "lcd.h"
#include "keypad.h"

int ticks;

/**
  * @brief Program entry point
  */
int main (void) {
 	PWM_configure();
	hd44780_init();
	keypad_configuration();
	
	ticks = 0;
	printf("main\n");
	while(1) {
		
		while(!ticks);
		ticks = 0;
		
// 		float alpha = 0.0;
// 		float beta = 0.0;
// 		
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

