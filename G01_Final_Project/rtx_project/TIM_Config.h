/**
* @file TIM_Config.h
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief Configures TIM3/4 for producing 25Hz interrupt, click interrupt, and PWM.
*
*	@section DESCRIPTION
*	
* Provides configuration and setup of TIM3 to give 25Hz clock rate as well as generate
* interrupts when detecting a single click, and also provides functions to configure
* TIM4 to connect to the LEDs and provide hardware PWM functionality.
*
*/

#ifndef __TIM_Config_H
#define __TIM_Config_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery_lis302dl.h"

#define TIM_ARR                          ((uint16_t)1900)
#define TIM_CCR                          ((uint16_t)1000)

/**
*	@brief Configure TIM4 to run PWM on the LEDs & enable device
* @param period_set Defintes the period/max counter variable of the timer
* @param freq_set Defines the frequency of the timer clock (sets prescaler)
*/
void configure_TIM4_LEDS(int period_set, int freq_set);

/**
*	@brief Configure the TIM3 timer to operate at 25Hz and generate interrupts
*	@post Interrupts every 25Hz
*/
void configure_TIM3(void);

/**
*	@brief Configure the TIM3 timer to operate at 20Hz and generate interrupts
*	@post Interrupts every 20Hz
*/
void configure_TIM2(void);
#endif
