#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "TIM_Config.h"
#include "Interrupt_Conf.h"
#include "SPI_Controller.h"
#include "Radio_Driver.h"

// Signals
const int32_t TICK_25HZ = 0x01;
const int32_t INTERRUPT_RX = 0x02;

// Vars


// Thread IDs
osThreadId tid_thread1;
osThreadId tid_thread2;

/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void thread(void const * argument);
osThreadDef(thread, osPriorityNormal, 1, 0);

void thread2(void const * argument);
osThreadDef(thread2, osPriorityNormal, 1, 0);


// Handler for TIM3 interrupt (25Hz)
void TIM3_IRQHandler(void) {	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
		// Do something
		osSignalSet(tid_thread1, TICK_25HZ);
		osSignalSet(tid_thread2, TICK_25HZ);
		//printf("Interrupt hit\n");
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	}
}

void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {		
		// Do Something.
		printf("Data rec interrupt.\n");
		osSignalSet(tid_thread2, INTERRUPT_RX);
		
    // Clear the EXTI line 1 pending bit 
    EXTI_ClearITPendingBit(EXTI_Line1);
	}
}



/*!
 @brief Program entry point
 */
int main (void) {
	
	// Initialize SPI for communicating with radio
	init_SPI1();
	
	// Configure TIM3 for 25Hz interrupts
	configure_TIM3();	
	
	// Configure interrupt.
	configure_RX_interrupt();
	
	// Configure radio
	configure_cc2500();
	
	
	// Start thread
	tid_thread1 = osThreadCreate(osThread(thread), NULL);
	
	//tid_thread2 = osThreadCreate(osThread(thread2), NULL);

	while(1){
		osDelay(osWaitForever);
	}
}

void thread (void const *argument) {
	
	uint8_t received = 0x00;
	
	while(1) {
			
		// Wait for 25Hz interrupt signal
		osSignalWait(TICK_25HZ, osWaitForever);
		
		// Push '5' to TX FIFO for transmission (0x3F)
		received = send_one_byte(0x00, 0x00, 0x3F, 0xF7);
		received = send_one_byte(0x00, 0x00, 0x3F, 0xA5);
		
		// Send TX command strobe
		cmd_strobe_TX();
		
		osDelay(100);
		
		cmd_strobe_IDLE();
		
		// print out SO byte
		printf("Received: 0x%X\n", received);
		
	}
}

void thread2 (void const *argument) {
	
	uint8_t num_RX_bytes = 0x00;
	uint8_t received_value = 0x00;
		
	while(1) {
		
		cmd_strobe_RX();
		
		osSignalWait(INTERRUPT_RX, osWaitForever);
		
		num_RX_bytes = get_RX_bytes();
		
		if (num_RX_bytes == 0x00) {
			cmd_strobe_IDLE();
			cmd_strobe_RXFLUSH();
		}
			
		received_value = send_one_byte(0x01, 0x00, 0x3F, 0x00);
		printf("Received value1 = 0x%X\n", received_value);
		
		/*
		received_value = send_one_byte(0x01, 0x00, 0x3F, 0x00);
		printf("Received value2 = 0x%X\n", received_value);
		received_value = send_one_byte(0x01, 0x00, 0x3F, 0x00);
		printf("Received value3 = 0x%X\n", received_value);
		received_value = send_one_byte(0x01, 0x00, 0x3F, 0x00);
		printf("Received value4 = 0x%X\n", received_value);
		received_value = send_one_byte(0x01, 0x00, 0x3F, 0x00);
		printf("Received value5 = 0x%X\n", received_value);
				
		*/
		
		cmd_strobe_IDLE();
		cmd_strobe_RXFLUSH();
		
	}
}
