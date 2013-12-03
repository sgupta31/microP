/**
* @file pwm.c
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief  Configurations for PWM and TIM4. Used for the servo motors
*
*/
#include "pwm.h"

uint16_t PrescalerValue_t = 0, Period = 0;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //Set up the time base structure
TIM_OCInitTypeDef  TIM_OCInitStructure; //Set up the Output Compare structure

/**
	* @brief Enable TIM4 clock and configure GPIO for PWM
	*/
void PWM_GPIO(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* Set the mode to alternating function for PWM */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Connect TIM4 pins to GPIO pin sources */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);  

}

/**
	* @brief Configure the TIM4 parameters
	*/
void PWM_TIM(void){
	/* Compute the prescaler value */
	//SystemCoreClock max value is 72MHz for TIM4
  PrescalerValue_t = 72-1;
	Period = 20000-1;

  /* Time base configuration  for TIM4 */
  TIM_TimeBaseStructure.TIM_Period = Period;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue_t;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //Set the ouput compare structure mode to PWM1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1500;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	/* PWM1 Mode configuration: Channels 1 and 2 */
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);

  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

/**
	* @brief A function to call all the PWM configuration functions at once
	*/
void PWM_configure(void){
	PWM_GPIO();
	PWM_TIM();
}

