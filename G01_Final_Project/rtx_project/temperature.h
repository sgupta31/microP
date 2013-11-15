#ifndef TEMPERATURE_H
#define TEMPERATURE_H

/**
	* @file temperature.h
	* @brief Threads used in the temperature mode and helper functions
	*/
	
/**
  * @brief The temperature thread to calculate the temperature in degrees Celsius
	*/
void temp_thread(void);

/**
  * @brief The temperature LED thread (mode = 0) to call functions based on submode variable.
	*/
void temp_LED_thread(void);

/**
	* @brief Convert the calculated average value from voltage format to temperature format in Celsius 
	* @param ADC_average is a double argument with the average value
	* @return temperature is a double with the calculated temperature value in Celsius
	*/
double convert_to_temperature(double ADC_average);

/**
	* @brief Display the LEDs clockwise/counterclockwise depending if the temperature increased/decreased by 2 degrees Celsius (submode = 0)
	* @param temperature is a double of the current temperature of the processor
	*/
void compare_temperatures(double temperature);

/**
	* @brief Flash the four LEDs (submode = 1)
	*/
void flash(void);

/**
	* @brief Turn on the LED label with the int i
	* @param i is an int of the LED label of the LED to turn on
	*/
void turn_led_on(int i);

/**
	* @brief Turn off the LED label with the int i
	* @param i is an int of the LED label of the LED to turn off
	*/
void turn_led_off(int i);

/**
	* @brief The delay function delays for a time equal to time_to_delay
	* @param time_to_delay is an int of the time to delay
	*/
void delay(int time_to_delay);

#endif //TEMPERATURE_H

