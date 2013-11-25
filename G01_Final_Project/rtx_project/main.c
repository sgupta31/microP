#include "arm_math.h"
#include "stm32f4xx.h"
#include "pwm.h"
#include "servo.h"

int ticks;

/**
  * @brief Program entry point
  */
int main (void) {
	PWM_configure();
	ticks = 0;
	
	while(1) {
		
		while(!ticks);
		ticks = 0;
		printf("main\n");
		
		float alpha = 0.0;
		float beta = 0.0;
		
		//alpha_motor(alpha);
		beta_motor(beta);
		
// 		TIM3->CCR2 = 1050 + (servo_angle * (450.0/45.0));
// 		
// 		servo_angle += angle_delta;
// 		
// 		if ((servo_angle >= 90) || (servo_angle <= -90)) {
// 			angle_delta = -angle_delta;
// 			servo_angle += angle_delta;
// 		}
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
