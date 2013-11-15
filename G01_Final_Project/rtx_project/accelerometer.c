#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "cmsis_os.h"
#include "stm32f4_discovery_lis302dl.h"
#include "movingFilter.h"
#include "math.h"
#include "accelerometer.h"

#define PI 3.14159265;	/* PI constant */

/**
	* @file accelerometer.c
	* @brief Threads for accelerometer mode and helper functions
	*/
	
/* external variables */
extern int32_t signal_set;
extern osThreadId tid_acc_LED;
extern int mode;
extern int submode;

/* Calculated manually by Matlab. Used the Least Mean Square method from Tilt Angle Application Notes (Appendix) */
float offsetX = -0.0426;
float offsetY = -0.0407;
float offsetZ =  0.1072;

float Xacc, Yacc, Zacc, alpha, beta;
int32_t out[3];	/* To store the x, y and z values read from the accelerometer */
int brightness = 0;
int delay_time;
int up = 1;

void pwm(void);
void tilting_leds(float alphaAngle, float betaAngle);

/**
  * @brief The accelerometer thread to calculate tilt angles alpha and beta
	*/
void acc_thread(void) {
	
	/* Initialize the movingFilterState struct and the initial values */
	struct movingFilterState mfilterX, mfilterY, mfilterZ;
	mfilterX.index = mfilterY.index = mfilterZ.index = 0;
	mfilterX.average = mfilterY.average = mfilterZ.average = 0.0;
	
	while (1) {
	
		osSignalWait(signal_set, osWaitForever);
		
		/* Obtain readings from the accelerometer */
		LIS302DL_ReadACC(&out[0]);
		
		/* Calibrating the obtained readings from the accelerometer using the offset values returned by Matlab */
		Xacc = out[0]*0.001 + offsetX;
		Yacc = out[1]*0.001 + offsetY;
		Zacc = out[2]*0.001 + offsetZ;
	
		/* Filtering the calibration readings in X, Y, Z coordinates */
		movingFilter(Xacc, &mfilterX);
		movingFilter(Yacc, &mfilterY);
		movingFilter(Zacc, &mfilterZ);
	
		Xacc = mfilterX.average;
		Yacc = mfilterY.average;
		Zacc = mfilterZ.average;
		
		/* Calculate the angles alpha (roll) and beta (pitch) in degrees (see conversion from radian to degree) */
		alpha = atan(Xacc/sqrt(Yacc*Yacc + Zacc*Zacc)) * 180 / PI;
		beta = atan(Yacc/sqrt(Xacc*Xacc + Zacc*Zacc)) * 180 / PI;

		//printf("alpha = %f | beta = %f\n", alpha, beta);
		
		/* Set the signal for the acc_LED_thread if mode = 1 */
		if (mode) {
			osSignalSet(tid_acc_LED, signal_set);
		}

	}
}

/**
  * @brief The accelerometer LED thread (mode = 1) to call functions based on submode variable.
	*/
void acc_LED_thread(void) {
	while (1) {
		osSignalWait(signal_set, osWaitForever);
				
		if (submode) {
			/* submode = 1 --> PWM mode */
			pwm();
		} else {
			/* submode = 0 --> tilt mode */
			tilting_leds(alpha, beta);
		}
		
	}
}

/**
  * @brief PWM with TIM4 (submode = 1)
	*/
void pwm(void) {
	/* From off to on state */
	if (up) {
		TIM4->CCR1 = brightness;
		TIM4->CCR2 = brightness;
		TIM4->CCR3 = brightness;
		TIM4->CCR4 = brightness;

		brightness += 5;

		if (brightness > 300) {
			up = 0;
		}	
	}
	
	/* From on to off state */
	if (!up) {
		TIM4->CCR1 = brightness;
		TIM4->CCR2 = brightness;
		TIM4->CCR3 = brightness;
		TIM4->CCR4 = brightness;
		
		brightness -= 5;
		
		if (brightness == 0) {
			up = 1;
		}
	}
}

/**
	* @brief Display LEDs based on the tilt angles alpha and beta (submode = 0)
	*/
void tilting_leds(float alphaAngle, float betaAngle) {
	/* Turn off all the LEDs */
	TIM4->CCR1 = 0;
	TIM4->CCR2 = 0;
	TIM4->CCR3 = 0;
	TIM4->CCR4 = 0;
	
	/* There is a 4 degrees error*/
	/* Set the delay time based on the alpha angle */
	if (alphaAngle > 4 || alphaAngle < -4) {
		TIM4->CCR2 = 333; /* 333 is the maximum duty cycle */
		TIM4->CCR4 = 333;
		if (alphaAngle > 45 && alphaAngle <= 90) {
			delay_time = 12000000;
		} else if (alphaAngle > 2 && alphaAngle <= 45) {
			delay_time = 7000000;
		} else if (alphaAngle < -2 && alphaAngle >= -45) {
			delay_time = 4000000;
		} else if (alphaAngle < -45 && alphaAngle >= -90) {
			delay_time = 1000000;
		}
		
		/* Delay with LEDs on */
		delay(delay_time);
		
		/* Turn off the LEDs */
		TIM4->CCR2 = 0;
		TIM4->CCR4 = 0;
		
		/* Delay with LEDs of*/
		delay(delay_time);

	}
	
	/* Set the delay time based on the beta angle */
	if (betaAngle > 4 || betaAngle < -4) {
		TIM4->CCR1 = 333;
		TIM4->CCR3 = 333;
		if (betaAngle > 45 && betaAngle <= 90) {
			delay_time = 12000000;
		} else if (betaAngle > 2 && betaAngle <= 45) {
			delay_time = 7000000;
		} else if (betaAngle < -2 && betaAngle >= -45) {
			delay_time = 4000000;
		} else if (betaAngle < -45 && betaAngle >= -90) {
			delay_time = 1000000;
		}
		
		/* Delay with LEDs on */
		delay(delay_time);
		
		/* Turn off the LEDs */
		TIM4->CCR1 = 0;
		TIM4->CCR3 = 0;
		
		/* Delay with LEDs of*/
		delay(delay_time);
		
	}
}
