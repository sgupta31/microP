/**
* @file SPI_Controller.h
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief Functions to initialize SPI configuration
*
*/
#ifndef __SPI_CONTROLLER_H
#define __SPI_CONTROLLER_H

#include <stm32f4xx.h>
#include <stm32f4xx_spi.h>

/**
	* @brief Initialize the SPI
	*/
void init_SPI1(void);

/**
	* @brief Send data on SPI
	* @param data- data to be transmitted to the SPI data register
	* @return received data from SPI data register
	*/
uint8_t SPI1_send(uint8_t data);

#endif
