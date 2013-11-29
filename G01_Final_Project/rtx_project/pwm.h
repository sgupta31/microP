#ifndef __PWM_H
#define __PWM_H

#include "stm32f4xx_conf.h"

/**
	* @file pwm.h
	* @brief Configurations for PWM and TIM3. Used for the servo motors.
	*/

/**
	* @brief Enable TIM3 clock and configure GPIO for PWM
	*/
void PWM_GPIO(void);

/**
	* @brief Configure the TIM3 parameters
	*/
void PWM_TIM(void);

/**
	* @brief Configure the NVIC for TIM3 interrupt
	*/
void PWM_NVIC(void);

/**
	* @brief A function to call all the PWM configuration functions at once
	*/
void PWM_configure(void);

#endif
