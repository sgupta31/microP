/**
* @file servo.c
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief  Functions to initialize operate the servo motors
*
*/

#include "servo.h"
#include "pwm.h"

/**
	* @brief Rotate the alpha servo motor
	* @param alpha -the alpha angle to rotate to
	*/
void alpha_motor(uint8_t alpha){
	
		if ((alpha < 180) && (alpha >=0)) {
			TIM4->CCR1 = 1100 + (alpha * (400/45));
		}
		
}

/**
	* @brief Rotate the beta servo motor
	* @param beta -the beta angle to rotate to
	*/
void beta_motor(uint8_t beta){
	
		if ((beta < 180) && (beta >=0)) {
			TIM4->CCR2 = 1100 + (beta * (400/45));
		}
		
}
