/**
* @file TIM_Config.c
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

#include "TIM_Config.h"

uint16_t PrescalerValue = 0;
uint16_t PrescalerValue2 = 0;
uint16_t PrescalerValue3 = 0;

/**
*	@brief Configure the TIM3 timer to operate at 25Hz and generate interrupts
*	@post Interrupts every 25Hz
*/
void configure_TIM3(void) {
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	printf("Configuring TIM3...\n");
	
	// Enable TIM3 Clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// Enable TIM3 Global Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Compute Prescaler value
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1638375) - 1;
	
	// Time base config
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	// Output Compare Timing Mode configuration for c. 1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	
	// Enable TIM3 Interrupts
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	
	// Enable TIM3 Counter
	TIM_Cmd(TIM3, ENABLE);
	
	printf("TIM3 Configured.\n");
	
}

/**
*	@brief Configure the TIM3 timer to operate at 20Hz and generate interrupts
*	@post Interrupts every 20Hz
*/
void configure_TIM2(void) {
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	printf("Configuring TIM2...\n");
	
	// Enable TIM2 Clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// Enable TIM2 Global Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Compute Prescaler value (for 20Hz)
	PrescalerValue3 = (uint16_t) ((SystemCoreClock / 2) / 327675) - 1;
	
	// Time base config
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue3;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	// Output Compare Timing Mode configuration for c. 1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	// Enable TIM2 Interrupts
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	
	// Enable TIM2 Counter
	TIM_Cmd(TIM2, ENABLE);
	
}

/**
*	@brief Configure TIM4 to run PWM on the LEDs & enable device
* @param period_set Defintes the period/max counter variable of the timer
* @param freq_set Defines the frequency of the timer clock (sets prescaler)
*/
void configure_TIM4_LEDS(int period_set, int freq_set) {
	
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	// Enable Clock on TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	// Enable GPIOD Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	// Config GPIO for LEDS (Pins 12-15)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// Connect TIM4 to LEDs
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4); 
	
	// Calculate prescaler value
	PrescalerValue2 = (uint16_t) ((SystemCoreClock /2) / freq_set) - 1;
	
	// Configure time base
	TIM_TimeBaseStructure.TIM_Period = period_set;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue2;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// Channel 1 config
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	// Channel 2 config
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	// Channel 3 config
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	// Channel 4 config
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	// Preload ARR value and enable TIM4
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);

}
