#ifndef __SERVO_H
#define __SERVO_H

/**
	* @file servo.h
	* @brief Functions to initialize operate the servo motors
	*/

/**
	* @brief Rotate the alpha servo motor
	* @param alpha -the alpha angle to rotate to
	*/
void alpha_motor(float alpha);

/**
	* @brief Rotate the beta servo motor
	* @param beta -the beta angle to rotate to
	*/
void beta_motor(float beta);

#endif
