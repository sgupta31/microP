/**
* @file Interrupt_Conf.c
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief Configuration of External Interrupts
*
*	@section DESCRIPTION
*	
* Provides initial configuration for all EXTI and NVIC operations.
*
*/

#include "Interrupt_Conf.h"
/*!
	@brief Configures EXTI and NVIC for the user button
*/
void configure_EXTI_PushButton(void) {
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	// Enable SYSCFG Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	// Connect PA0 pin to Line0 on EXTI
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	// Configure EXTI Line0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	// Enable Line0 and set to lowest priority interrupt
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}
/*!
	@brief Configures EXTI and NVIC for the LS302DL tap detect
*/
void configure_EXTI_ClickDetect(void) {
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);
	
	
	// Configure EXTI Line1
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	
	// Enable Line1 and set to lowest priority interrupt
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

/*!
	@brief Configures RX interrupts
*/
void configure_RX_interrupt(void) {
	
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	// Enable GPIOA Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	// Configure PA1 pin as input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Enable SYSCFG Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	// Connect PA1 pin to Line1 on EXTI
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
	
	// Configure EXTI Line1
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	// Enable Line0 and set to lowest priority interrupt
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}
