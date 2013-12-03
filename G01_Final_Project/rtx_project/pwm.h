/**
* @file pwm.h
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief  Configurations for PWM and TIM4. Used for the servo motors
*
*/
#ifndef __PWM_H
#define __PWM_H

#include "stm32f4xx_conf.h"

/**
	* @brief Enable TIM4 clock and configure GPIO for PWM
	*/
void PWM_GPIO(void);

/**
	* @brief Configure the TIM4 parameters
	*/
void PWM_TIM(void);

/**
	* @brief A function to call all the PWM configuration functions at once
	*/
void PWM_configure(void);

#endif
