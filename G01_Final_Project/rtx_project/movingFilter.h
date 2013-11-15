#ifndef MOVINGFILTER_H
#define MOVINGFILTER_H
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"

/**
	* @file movingFilter.h
	* @brief This is to set up the moving average filter
	*/
	
/**
	* @brief Initialize the moving average filter and get the average value of the measurements
	* @param value to be filtered
  * @param m is a pointer to the movingFilterState struct
	* @return average is a float of the calculated average value
	*/
struct movingFilterState{
	int index;
	float buffer[10];
	float average;
};

/**
	* @brief Initialize the moving average filter and get the average value of the measurements
	* @param value is an uint16_t argument with the measurement value
  * @param m is a pointer to the movingFilterState struct
	* @return average is a double with the calculated average value
	*/
float movingFilter(float value, struct movingFilterState* m);

#endif //MOVINGFILTER_H
