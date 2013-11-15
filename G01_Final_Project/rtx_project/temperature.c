#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "cmsis_os.h"
#include "movingFilter.h"

/**
	* @file temperature.c
	* @brief Threads used in the temperature mode and helper functions
	*/
	
uint_fast16_t ADC_value; 

/* external variables */
extern int32_t signal_set;
extern int32_t signal_temp;
extern osThreadId tid_temp_LED;
extern int mode;
extern int submode;

double initial_temperature = 30;	/* Initialize initial_temperature to 30 degrees Celcius */
double temperature;
int i = 0;

double convert_to_temperature(double ADC_average);
void compare_temperatures(double temperature);
void turn_led_on(int i);
void turn_led_off(int i);
void delay(int time_to_delay);
void flash();

/**
  * @brief The temperature thread to calculate the temperature in degrees Celsius
	*/
void temp_thread() {
	
	struct movingFilterState mFilter;
	mFilter.index = 0;
	mFilter.average = 0.0;
	
	while (1) {
		/* os wait for temperature signal to be set */
		osSignalWait(signal_set, osWaitForever);
		
		/* Get the ADC_value readings */
		ADC_SoftwareStartConv(ADC1);
		ADC_value = ADC_GetConversionValue(ADC1); // Result available in ADC1.DR
		
		/* Calculate the average by the moving average filter */
		double average = movingFilter(ADC_value, &mFilter);
		
		/* Convert the calculated average into temperature in Celsius */
		temperature = convert_to_temperature(average);
		
		//printf("temp = %f\n", temperature);

		/* Set the signal for the temp_LED_thread if mode = 0 */
		if (!mode) {
			osSignalSet(tid_temp_LED, signal_set);
		}
	}
}

/**
  * @brief The temperature LED thread (mode = 0) to call functions based on submode variable.
	*/
void temp_LED_thread() {
	while (1) {
		/* os wait for signal to be set for temperature LED thread */
		osSignalWait(signal_set, osWaitForever);
		
		if (submode) {
			/* submode = 1 --> flashing mode */
			flash();
		} else {
			/* submode = 0 --> temperature trend display mode */
			compare_temperatures(temperature);
		}

	}
}

/**
	* @brief Convert the calculated average value from voltage format to temperature format in Celsius 
	* @param ADC_average is a double argument with the average value
	* @return temperature is a double with the calculated temperature value in Celsius
	*/
double convert_to_temperature(double ADC_average){
	
	/* Declare Vadc and temperature, initialize the values of Vdd, V25 an avgSlope as seen in the datasheet */
	double Vadc, temperature;
	double Vdd = 3; 							// Vdd is 3V
  double V25= 0.76; 						// Voltage at 25 degrees C
  double avgSlope= 0.0025; 			// Average slope in mV/degree C
	
	/* Use formulae to convert the measurement value to Vadc and then Vadc to temperature value in Celsius */
	Vadc=(ADC_average/4095)*Vdd;
  temperature=((Vadc-V25)/avgSlope) + 25;
	
	return temperature;
}

/**
	* @brief Display the LEDs clockwise/counterclockwise depending if the temperature increased/decreased by 2 degrees Celsius (submode = 0)
	* @param temperature is a double of the current temperature of the processor
	*/
void compare_temperatures(double temperature){

	if(temperature - initial_temperature >= 0.2){
		/* Temperature increased by 2 degree Celcius */
		turn_led_off(i); /* Turn current LED off */

		/* Wrap around logic for the LEDs */
		if (i == 3) {
			i = 0;
		} else {
			i++;
		}
		initial_temperature = temperature;
		
	} else if(initial_temperature - temperature >= 0.2){
		/* Temperature decreased by 2 degree Celcius */
		turn_led_off(i); /* Turn current LED off */
		
		/* Wrap around logic for the LEDs */	
		if (i == 0) {
			i = 3;
		} else {
			i--;
		}
		initial_temperature = temperature;
		
	}
	
	turn_led_on(i); /* Turn the LED with label i on */
}

/**
	* @brief Flash the four LEDs (submode = 1)
	*/
void flash() {
	turn_led_on(0);
	turn_led_on(1);
	turn_led_on(2);
	turn_led_on(3);

	delay(1000000);
	
	turn_led_off(0);
	turn_led_off(1);
	turn_led_off(2);
	turn_led_off(3);
	
	delay(1000000);

}

/**
	* @brief Turn on the LED label with the int i
	* @param i is an int of the LED label of the LED to turn on
	*/
void turn_led_on(int i) {
	/* Use a switch statement to turn on the LED with the label i */
	switch (i) {
		case 0:
			TIM4->CCR1 = 400;
			break;
		case 1:
			TIM4->CCR2 = 400;
			break;
		case 2:
			TIM4->CCR3 = 400;
			break;
		case 3:
			TIM4->CCR4 = 400;
			break;
		default:
			TIM4->CCR1 = 400;
			break;
	}
}

/**
	* @brief Turn off the LED label with the int i
	* @param i is an int of the LED label of the LED to turn off
	*/
void turn_led_off(int i) {
	/* Use a switch statement to turn off the LED with the label i */
	switch (i) {
		case 0:
			TIM4->CCR1 = 0;
			break;
		case 1:
			TIM4->CCR2 = 0;
			break;
		case 2:
			TIM4->CCR3 = 0;
			break;
		case 3:
			TIM4->CCR4 = 0;
			break;
		default:
			TIM4->CCR4 = 0;
			break;
	}
}

/**
	* @brief The delay function delays for a time equal to time_to_delay
	* @param time_to_delay is an int of the time to delay
	*/
void delay(int time_to_delay){
	while(time_to_delay != 0)
	 time_to_delay--;
}
