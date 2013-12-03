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

#include "Radio_Driver.h"

/**
	* @brief Initialize the cc2500 chip
	*/
void configure_cc2500(void) {
	
	// Done this way for testing purposes...
	// Can do it all in one loop instead, much nicer code!
	
	// Configure the CC2500 as per spec
	
	send_one_byte(0x00, 0x00, 0x0B, SMARTRF_SETTING_FSCTRL1);
	send_one_byte(0x00, 0x00, 0x0C, SMARTRF_SETTING_FSCTRL0);
	send_one_byte(0x00, 0x00, 0x0D, SMARTRF_SETTING_FREQ2);
	send_one_byte(0x00, 0x00, 0x0E, SMARTRF_SETTING_FREQ1);
	send_one_byte(0x00, 0x00, 0x0F, SMARTRF_SETTING_FREQ0);
	send_one_byte(0x00, 0x00, 0x10, SMARTRF_SETTING_MDMCFG4);
	send_one_byte(0x00, 0x00, 0x11, SMARTRF_SETTING_MDMCFG3);
	send_one_byte(0x00, 0x00, 0x12, SMARTRF_SETTING_MDMCFG2);
	send_one_byte(0x00, 0x00, 0x13, SMARTRF_SETTING_MDMCFG1);
	send_one_byte(0x00, 0x00, 0x14, SMARTRF_SETTING_MDMCFG0);
	send_one_byte(0x00, 0x00, 0x0A, SMARTRF_SETTING_CHANNR);
	send_one_byte(0x00, 0x00, 0x15, SMARTRF_SETTING_DEVIATN);
	send_one_byte(0x00, 0x00, 0x21, SMARTRF_SETTING_FREND1);
	send_one_byte(0x00, 0x00, 0x22, SMARTRF_SETTING_FREND0);
	//send_one_byte(0x00, 0x00, 0x17, SMARTRF_SETTING_MCSM1);
	send_one_byte(0x00, 0x00, 0x18, SMARTRF_SETTING_MCSM0);
	send_one_byte(0x00, 0x00, 0x19, SMARTRF_SETTING_FOCCFG);
	send_one_byte(0x00, 0x00, 0x1A, SMARTRF_SETTING_BSCFG);
	send_one_byte(0x00, 0x00, 0x1B, SMARTRF_SETTING_AGCCTRL2);
	send_one_byte(0x00, 0x00, 0x1C, SMARTRF_SETTING_AGCCTRL1);
	send_one_byte(0x00, 0x00, 0x1D, SMARTRF_SETTING_AGCCTRL0);
	send_one_byte(0x00, 0x00, 0x23, SMARTRF_SETTING_FSCAL3);
	send_one_byte(0x00, 0x00, 0x24, SMARTRF_SETTING_FSCAL2);
	send_one_byte(0x00, 0x00, 0x25, SMARTRF_SETTING_FSCAL1);
	send_one_byte(0x00, 0x00, 0x26, SMARTRF_SETTING_FSCAL0);
	send_one_byte(0x00, 0x00, 0x29, SMARTRF_SETTING_FSTEST);
	send_one_byte(0x00, 0x00, 0x2C, SMARTRF_SETTING_TEST2);
	send_one_byte(0x00, 0x00, 0x2D, SMARTRF_SETTING_TEST1);
	send_one_byte(0x00, 0x00, 0x2E, SMARTRF_SETTING_TEST0);
	send_one_byte(0x00, 0x00, 0x03, SMARTRF_SETTING_FIFOTHR);
	send_one_byte(0x00, 0x00, 0x00, SMARTRF_SETTING_IOCFG2);
	send_one_byte(0x00, 0x00, 0x02, SMARTRF_SETTING_IOCFG0D);
	send_one_byte(0x00, 0x00, 0x07, SMARTRF_SETTING_PKTCTRL1);
	send_one_byte(0x00, 0x00, 0x08, SMARTRF_SETTING_PKTCTRL1);
	send_one_byte(0x00, 0x00, 0x09, SMARTRF_SETTING_ADDR);
	send_one_byte(0x00, 0x00, 0x09, SMARTRF_SETTING_PKTLEN);
	
	//send_one_byte(0x00, 0x00, 0x04, SMARTRF_SETTING_SYNC1);
	//send_one_byte(0x00, 0x00, 0x05, SMARTRF_SETTING_SYNC0);
	
}

/**
	* @brief Send header
	* @param rw_bit- read/write bit
	* @param burst- burst mode, default = 0
	* @param address- destination address
	* @return 0
	*/
uint8_t send_header(uint8_t rw_bit, uint8_t burst, uint8_t address) {
	// Do something
	return 0x00;
}

/**
	* @brief Helper function to check pin 6
	*/
uint8_t check_so_pin(void) {
	
	if (CC2500_SPI_SO_GPIO_PORT->IDR & 0x20) {
		return 1;
	}
	else {
		return 0;
	}
}

/**
	* @brief Send one byte of data
	* @param rw_bit- read/write bit
	* @param burst- burst mode, default = 0
	* @param address- destination address
	* @param data- data to be sent
	* @return reply received
	*/
uint8_t send_one_byte(uint8_t rw_bit, uint8_t burst, uint8_t address, uint8_t data) {
	uint8_t reply = 0x00;
	uint8_t headerByte = 0x00;
	uint8_t statusByte = 0x00;
	
	// Set CSn pin low to begin transfer
	set_cs(0);
	
	// Wait until so pin goes low
	while (check_so_pin());

	// Construct header byte
	headerByte = (rw_bit << 7) | headerByte;
	headerByte = (burst << 6)  | headerByte;
	headerByte = address			 | headerByte;

	// Send header byte over SPI
	statusByte = SPI1_send(headerByte);
	
	// Send data over SPI
	reply = SPI1_send(data);
	
	osDelay(50);
	
	// Pull CS pin high after transmission
	set_cs(1);
	
	return reply;
	
}

/**
	* @brief Send transmitter cmd strobe
	* @return reply received
	*/
uint8_t cmd_strobe_TX(void) {
	
	uint8_t headerByte = 0x00;
	uint8_t reply = 0x00;
	
	set_cs(0);
	while(check_so_pin());
	
	headerByte = 0x35;
	
	// Send TX cmd strobe
	reply = SPI1_send(headerByte);

	// Set CS pin high.
	set_cs(1);
	
	return reply;
	
}

/**
	* @brief Send receiver cmd strobe
	* @return reply received
	*/
uint8_t cmd_strobe_RX(void) {
	uint8_t headerByte = 0x00;
	uint8_t reply = 0x00;
	
	set_cs(0);
	while(check_so_pin());
	
	headerByte = 0x34;
	
	reply = SPI1_send(headerByte);
	
	osDelay(50);
		
	set_cs(1);
	
	return reply;
}

/**
	* @brief Send IDLE cmd strobe
	* @return reply received
	*/
uint8_t cmd_strobe_IDLE(void) {
	uint8_t headerByte = 0x00;
	uint8_t reply = 0x00;
	
	set_cs(0);
	while(check_so_pin());
	
	headerByte = 0x36;
	
	reply = SPI1_send(headerByte);
	
	osDelay(50);
		
	set_cs(1);
	
	return reply;
}

/**
	* @brief Send receiver flush cmd strobe
	* @return reply received
	*/
uint8_t cmd_strobe_RXFLUSH(void) {
	uint8_t headerByte = 0x00;
	uint8_t reply = 0x00;
	
	set_cs(0);
	while(check_so_pin());
	
	headerByte = 0x3A;
	
	reply = SPI1_send(headerByte);
		
	osDelay(50);
	
	set_cs(1);
	
	return reply;
}

/**
	* @brief Get the value of received bytes
	* @return The value of bytes received
	*/
uint8_t get_RX_bytes(void) {
	uint8_t value = 0x00;
	
	set_cs(0);
	while(check_so_pin());
	
	value = SPI1_send(0xBB);
	
	SPI1_send(0x00);
	
	set_cs(1);
	
	return value;
}

/**
	* @brief Reset/set the state of port based on the state passed
	* @param State to be set
	*/
void set_cs(int state) {
	
	if (state == 0) {
		GPIOE->BSRRH |= GPIO_Pin_7;
	}
	else if(state == 1) {
		GPIOE->BSRRL |= GPIO_Pin_7;
	}
}
