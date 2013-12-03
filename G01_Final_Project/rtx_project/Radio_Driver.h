/**
* @file Radio_Driver.c
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief Configuration of radio driver
*
*	@section DESCRIPTION
*	
* Provides initial configuration for the radio driver.
*
*/
#ifndef __RADIO_DRIVER_H
#define __RADIO_DRIVER_H

#include "SMARTRF_CC2500.h"
#include "SPI_Controller.h"
#include "cmsis_os.h"

#define CC2500_SPI_CS_GPIO_PORT					GPIOE
#define CC2500_SPI_CS_PIN								GPIO_Pin_7

#define CC2500_SPI_SO_GPIO_PORT					GPIOA
#define CC2500_SPI_SO_PIN								GPIO_Pin_6

#define CC2500_CS_LOW()       					GPIO_ResetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)
#define CC2500_CS_HIGH()      					GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)

/**
	* @brief Initialize the cc2500 chip
	*/
void configure_cc2500(void);

/**
	* @brief Send header
	* @param rw_bit- read/write bit
	* @param burst- burst mode, default = 0
	* @param address- destination address
	* @return 0
	*/
uint8_t send_header(uint8_t rw_bit, uint8_t burst, uint8_t address);

/**
	* @brief Helper function to check pin 6
	*/
uint8_t check_so_pin(void);

/**
	* @brief Send one byte of data
	* @param rw_bit- read/write bit
	* @param burst- burst mode, default = 0
	* @param address- destination address
	* @param data- data to be sent
	* @return reply received
	*/
uint8_t send_one_byte(uint8_t rw_bit, uint8_t burst, uint8_t address, uint8_t data);

/**
	* @brief Reset/set the state of port based on the state passed
	* @param State to be set
	*/
void set_cs(int state);

/**
	* @brief Send transmitter cmd strobe
	* @return reply received
	*/
uint8_t cmd_strobe_TX(void);

/**
	* @brief Get the value of received bytes
	* @return The value of bytes received
	*/
uint8_t get_RX_bytes(void);

/**
	* @brief Send receiver cmd strobe
	* @return reply received
	*/
uint8_t cmd_strobe_RX(void);

/**
	* @brief Send IDLE cmd strobe
	* @return reply received
	*/
uint8_t cmd_strobe_IDLE(void);

/**
	* @brief Send receiver flush cmd strobe
	* @return reply received
	*/
uint8_t cmd_strobe_RXFLUSH(void);

#endif

