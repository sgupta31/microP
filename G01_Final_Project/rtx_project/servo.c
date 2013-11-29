#include "servo.h"
#include "pwm.h"

/**
	* @file servo.c
	* @brief Functions to initialize operate the servo motors
	*/
	
extern float alpha;
extern float beta;
float angle_delta = (1.0);

/**
	* @brief Rotate the alpha servo motor
	* @param alpha -the alpha angle to rotate to
	*/
void alpha_motor(float alpha){
	
		/* Based on HS422 data sheet, the servo motor's operating angle is 45 degrees/ 400 microsec  */
		/* Inital state is at 1100 microsec */
		if ((alpha < 180) && (alpha >=0)) {
			TIM3->CCR1 = 1100 + (alpha * (400.0/45.0));
		}
}

/**
	* @brief Rotate the beta servo motor
	* @param beta -the beta angle to rotate to
	*/
void beta_motor(float beta){
	
		/* Based on HS422 data sheet, the servo motor's operating angle is 45 degrees/ 400 microsec  */
		/* Inital state is at 1100 microsec */
		if ((beta < 180) && (beta >=0)) {
			TIM3->CCR2 = 1100 + (beta * (400.0/45.0));
		}
}
