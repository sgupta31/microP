#include "arm_math.h"
#include "stm32f4xx.h"
#include "pwm.h"

int ticks;
int angle[4] = {0,0,0,0};

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
		TIM3->CCR1 = 300 /*600 + ((angle[0] + 90) * 10)*/;
		TIM3->CCR2 = 300;
		TIM3->CCR3 = 300;
		TIM3->CCR4 = 300;
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
