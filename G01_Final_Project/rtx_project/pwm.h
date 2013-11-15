#include "stm32f4xx_conf.h"

/**
	* @file pwm.h
	* @brief Configure TIM4 for PWM (part 5)
	*/

/**
	* @brief Enable TIM3 clock and configure GPIO for PWM
	*/
void PWM_GPIO(void);

/**
	* @brief Configure the TIM3 parameters
	*/
void PWM_TIM(void);
void PWM_NVIC(void);
void PWM_configure(void);
//void configure_TIM(void);
