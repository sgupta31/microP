#ifndef INITIALIZATION_H
#define INITIALIZATION_H

/**
	* @file initialization.h
	* @brief The functions to setup the ADC, accelerometer, tap and user button.
	*/
	
/**
	* @brief Setup the ADC driver configuration parameters
	*/
void ADC_config(void);

/**
	* @brief LIS302DL driver for reading accelerations and interrupt structures for external interrupts
	*/
void accelerometer_config(void);

/**
	* @brief Configure GPIO pins, EXTI structure, EXTI interrupt line EXTILine0 to detect single taps
	*/
void tap_config(void);

/**
	* @brief Initialize user button configuration
	*/
void userbutton_config(void);

/**
	* @brief Initialize the ADC, accelerometer, tap and user button configurations
	*/
void initialization(void);

#endif //INITIALIZATION_H

