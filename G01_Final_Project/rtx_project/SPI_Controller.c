/**
* @file SPI_Controller.c
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief Functions to initialize SPI configuration
*
*/

#include "SPI_Controller.h"

/**
	* @brief Initialize the SPI
	*/
void init_SPI1(void) {
	
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	// Enable GPIO bank A clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	// Enable GPIO bank E clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	// Enable SPI clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/*
	SPI Pins:
	- PA5: SCK
	- PA6: MISO
	- PA7: MOSI
	- PE7: CSn
	*/
	
	// Setup PA 5/6/7 for alt function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Connect pins to SPI
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	// Setup PE7 pin as CS
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	// Set PE7 HIGH
	GPIOE->BSRRL |= GPIO_Pin_7;
	
	
	// Configure SPI
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 	// set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     										// transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 										// one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        										// clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      										// data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; 	// set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 	// SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;										// data is transmitted MSB first
	SPI_Init(SPI1, &SPI_InitStruct); 
	
	SPI_Cmd(SPI1, ENABLE); // enable SPI1
	
}

/**
	* @brief Send data on SPI
	* @param data- data to be transmitted to the SPI data register
	* @return received data from SPI data register
	*/
uint8_t SPI1_send(uint8_t data) {
	
	SPI1->DR = data; 														// write data to be transmitted to the SPI data register
	while( !(SPI1->SR & SPI_I2S_FLAG_TXE) ); 		// wait until transmit complete
	while( !(SPI1->SR & SPI_I2S_FLAG_RXNE) ); 	// wait until receive complete
	while( SPI1->SR & SPI_I2S_FLAG_BSY ); 			// wait until SPI is not busy anymore
	return SPI1->DR; 														// return received data from SPI data register
	
}
