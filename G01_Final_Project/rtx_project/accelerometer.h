#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

/**
	* @file accelerometer.h
	* @brief Threads for accelerometer mode and helper functions
	*/
	
/**
  * @brief The accelerometer thread to calculate tilt angles alpha and beta
	*/
void acc_thread(void);

/**
  * @brief The accelerometer LED thread (mode = 1) to call functions based on submode variable.
	*/
void acc_LED_thread(void);

/**
  * @brief PWM with TIM4 (submode = 1)
	*/
void pwm(void);

/**
	* @brief Display LEDs based on the tilt angles alpha and beta (submode = 0)
	*/
void tilting_leds(float alphaAngle, float betaAngle);

#endif //ACCELEROMETER_H

