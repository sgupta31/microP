/**
* @file servo.h
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief  Functions to initialize operate the servo motors
*
*/

#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f4xx.h"

/**
	* @brief Rotate the alpha servo motor
	* @param alpha -the alpha angle to rotate to
	*/
void alpha_motor(uint8_t alpha);

/**
	* @brief Rotate the beta servo motor
	* @param beta -the beta angle to rotate to
	*/
void beta_motor(uint8_t beta);

#endif
