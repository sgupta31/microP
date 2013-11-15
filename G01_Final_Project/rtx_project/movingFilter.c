#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "movingFilter.h"

/**
	* @file movingFilter.c
	* @brief This is to set up the moving average filter
	*/
	
/**
	* @brief Initialize the moving average filter and get the average value of the measurements
	* @param value to be filtered
  * @param m is a pointer to the movingFilterState struct
	* @return average is a float of the calculated average value
	*/
float movingFilter(float value, struct movingFilterState *m){
	
	/* Initialize the sum and average to 0 and declare i */
	float sum = 0;
	float average = 0; 
	int i;
	
	/* Store the measurement into the buffer array at the index indicated by the index value */
	m->buffer[m->index] = value;
	
	/* When you reach the end of the buffer array, circle back the index to be 0 */
	if (m->index != 9){
		m->index ++;
	} else {
		m->index = 0;
	}
	
	/* Increment the sum by adding the measurements in the buffer array */
	for (i=0; i<10; i++) {
			sum += m->buffer[i];
	}
	
	/* Calculate the average of the buffer array and store the value in average */
	m->average = sum/10.0;
	average = m->average;
	
	return average;
}

