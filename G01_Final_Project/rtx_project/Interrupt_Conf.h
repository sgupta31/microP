/**
* @file Interrupt_Conf.h
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
#ifndef __INTERRUPT_CONF_H
#define __INTERRUPT_CONF_H

#include "stm32f4xx.h"

/*!
	@brief Configures EXTI and NVIC for the user button
*/
void configure_EXTI_PushButton(void);

/*!
	@brief Configures EXTI and NVIC for the LS302DL tap detect
*/
void configure_EXTI_ClickDetect(void);

/*!
	@brief Configures RX interrupts
*/
void configure_RX_interrupt(void);

#endif

