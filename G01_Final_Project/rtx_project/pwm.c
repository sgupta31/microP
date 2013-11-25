#include "pwm.h"

/**
	* @file pwm.c
	* @brief 
	*/
	
uint16_t PrescalerValue = 0, Period = 0;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //Set up the time base structure
TIM_OCInitTypeDef  TIM_OCInitStructure; //Set up the Output Compare structure

/**
	* @brief Enable TIM3 clock and configure GPIO for PWM
	*/
void PWM_GPIO(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* Set the mode to alternating function for PWM */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* Connect TIM3 pins to GPIO pin sources */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);  

}

/**
	* @brief Configure the TIM3 parameters
	*/
void PWM_TIM(void){
	/* Compute the prescaler value */
	//SystemCoreClock max value is 72MHz for TIM3
  PrescalerValue = 72-1;
	Period = 20000-1;

  /* Time base configuration  for TIM3 */
  TIM_TimeBaseStructure.TIM_Period = Period;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //Set the ouput compare structure mode to PWM1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1500;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	/* PWM1 Mode configuration: Channels 1,2,3 and 4 */
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

void PWM_NVIC(void){
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		
		/* Setting the preemtion priority of the tap to be lower than the user button interrupt */
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

void PWM_configure(void){
	PWM_GPIO();
	PWM_TIM();
	PWM_NVIC();
}

