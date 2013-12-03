/**
* @file main.c
*	@author Group 1: Christian Despatie, Lena Hsieh, Surbhi Gupta & Kishen Shakespeare
* @version 1.0
*
*	@brief  Program main
*
*/
#include "arm_math.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "TIM_Config.h"
#include "Interrupt_Conf.h"
#include "SPI_Controller.h"
#include "Radio_Driver.h"
#include "pwm.h"
#include "servo.h"
#include "lcd.h"
#include "cstdio"
#include "keypad.h"

// Signals
const int32_t TICK_25HZ = 0x01;
const int32_t INTERRUPT_RX = 0x02;
const int32_t MOTOR_FLAG = 0x03;
const int32_t KEYPAD_MODE = 0x04;
const int32_t TICK_5HZ = 0x05;

// Variables
uint8_t alpha_angle = 0x00;
uint8_t beta_angle = 0x00;

// LS302DL calibration values 
const int XOFFSET = 10; 
const int YOFFSET = -27; 
const int ZOFFSET = 104; 

int m_alpha = 90;
int m_beta = 90; 

int keypad_flag = 0;

extern int row;
extern int column;

char s_alpha [20];
char s_beta [20];

// Thread IDs
osThreadId tid_thread1;
osThreadId tid_thread2;
osThreadId tid_thread3;
osThreadId tid_thread4;
osThreadId tid_thread5;

// Avg filter
const int depth = 4;

struct averageFilter { 
    int buffer[depth]; 
    int index; 
    int average; 
    int numValues; 
}; 

/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void broadcast_thread(void const * argument);
osThreadDef(broadcast_thread, osPriorityNormal, 1, 0);

/*!
 @brief Thread to receive alpha and beta values and send them to motor control thread
 @param argument Unused
 */
void receive_thread(void const * argument);
osThreadDef(receive_thread, osPriorityNormal, 1, 0);

/*!
 @brief Thread to control and move the motors
 @param argument Unused
 */
void motor_control_thread(void const * argument);
osThreadDef(motor_control_thread, osPriorityNormal, 1, 0);

/*!
 @brief Thread to perform keypad operations
 @param argument Unused
 */
void keypad_control_thread(void const * argument);
osThreadDef(keypad_control_thread, osPriorityNormal, 1, 0);

/*!
 @brief Thread to perform LCD display
 @param argument Unused
 */
void screen_control_thread(void const * argument);
osThreadDef(screen_control_thread, osPriorityNormal, 1, 0);

/*!
 @brief Handler for TIM3 interrupt (25Hz)
 */
void TIM3_IRQHandler(void) {	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
		// Do something
		osSignalSet(tid_thread1, TICK_25HZ);
		osSignalSet(tid_thread2, TICK_25HZ);
		osSignalSet(tid_thread4, TICK_25HZ);
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	}
}

/*!
 @brief Handler for TIM2 interrupt
 */
void TIM2_IRQHandler(void) {	
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		osSignalSet(tid_thread5, TICK_5HZ);
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
}

/*!
 @brief Handler for EXTI1 interrupt
 */
void EXTI1_IRQHandler(void) {
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {		
		printf("Data rec interrupt.\n");
		osSignalSet(tid_thread2, INTERRUPT_RX);
		
    // Clear the EXTI line 1 pending bit 
    EXTI_ClearITPendingBit(EXTI_Line1);
	}
}


/*! 
 @brief Moving average filter for integer values (acceleration) 
 @param newMeasure The new value to be inserted into the filter 
 @param filter The previous state of the filter struct 
 */
void movingAverageFilter(int32_t newMeasure, struct averageFilter *filter) { 
    int i = 0; 
    int32_t sum = 0; 
      
    if (filter->index == depth - 1) { 
        filter->index = 0; 
    } else { 
        filter->index++; 
    } 
      
    if (filter->numValues < depth) { 
        filter->numValues = filter->numValues + 1; 
    } 
      
    filter->buffer[filter->index] = newMeasure; 
      
    for (i = 0; i < filter->numValues; i++) { 
        sum = sum + filter->buffer[i]; 
    } 
      
    filter->average = sum / filter->numValues; 
      
}


/*!
 @brief Program entry point
 */
int main (void) {
	
	// Initialize SPI for communicating with radio
	init_SPI1();
	
	// Configure TIM3/TIM2
	configure_TIM3();
	configure_TIM2();
	
	// Configure interrupt.
	configure_RX_interrupt();
	
	// Configure radio
	configure_cc2500();
	
	// Configure PWM for servos
	PWM_configure();
	
	// Configure LCD
	lcd_init();
	
	// Configure Keypad
	keypad_configuration();
	
	osDelay(50);
	printf("Everything configured\n");
	osDelay(50);
	
	// Start thread
	//tid_thread1 = osThreadCreate(osThread(broadcast_thread), NULL);
	
	tid_thread2 = osThreadCreate(osThread(receive_thread), NULL);
	
	tid_thread3 = osThreadCreate(osThread(motor_control_thread), NULL);

	tid_thread4 = osThreadCreate(osThread(keypad_control_thread), NULL);
	
	tid_thread5 = osThreadCreate(osThread(screen_control_thread), NULL);

	while(1){
		osDelay(osWaitForever);
	}
}

/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void broadcast_thread(void const *argument) {
	
	uint8_t received = 0x00;
	
	uint8_t send1 = 0x00;
	uint8_t send2 = 0x00;
	
	// Angles / Buffer variables 
  int32_t accelBuffer[16]; 
  double alpha = 0.0; 
  double beta = 0.0; 
	 
	// Initialize filter structs 
  struct averageFilter currentAverageFilterX; 
  struct averageFilter currentAverageFilterY; 
  struct averageFilter currentAverageFilterZ; 
  currentAverageFilterX.index = 0; 
  currentAverageFilterX.numValues = 0; 
  currentAverageFilterY.index = 0; 
  currentAverageFilterY.numValues = 0; 
  currentAverageFilterZ.index = 0; 
  currentAverageFilterZ.numValues = 0; 
	
	while(1) {
			
		// Wait for 25Hz interrupt signal
		osSignalWait(TICK_25HZ, osWaitForever);
		
		LIS302DL_ReadACC(accelBuffer); 
          
    // Calibration w\ offsets 
    accelBuffer[0] += XOFFSET; 
    accelBuffer[1] += YOFFSET; 
    accelBuffer[2] += ZOFFSET; 
          
    // Run values through moving average filter 
    movingAverageFilter(accelBuffer[0], &currentAverageFilterX); 
    movingAverageFilter(accelBuffer[1], &currentAverageFilterY); 
    movingAverageFilter(accelBuffer[2], &currentAverageFilterZ); 
          
    // Calculate angles 
    alpha = atan( currentAverageFilterX.average / ( sqrt((currentAverageFilterY.average *  
    currentAverageFilterY.average) + (currentAverageFilterZ.average * currentAverageFilterZ.average) ) ) ); 
    beta = atan( currentAverageFilterY.average / ( sqrt((currentAverageFilterX.average *  
    currentAverageFilterX.average) + (currentAverageFilterZ.average * currentAverageFilterZ.average) ) ) ); 
          
    // Convert to degrees 
    alpha = alpha * (180 / PI); 
    beta = beta * (180 / PI); 
		
		alpha += 90;
		beta += 90;
		
		send1 = (int) alpha;
		send2 = (int) beta;
		
		printf("S1: 0x%X\n", send1);
		printf("S2: 0x%X\n", send2);
		
		// Push '5' to TX FIFO for transmission (0x3F)
		received = send_one_byte(0x00, 0x00, 0x3F, send1);
		// Push '5' to TX FIFO for transmission (0x3F)
		received = send_one_byte(0x00, 0x00, 0x3F, send2);
		
		// Send TX command strobe
		cmd_strobe_TX();
		
		osDelay(50);
		
		cmd_strobe_IDLE();
		
		// print out status byte
		printf("Received: 0x%X\n", received);
		
	}
}

/*!
 @brief Thread to receive alpha and beta values and send them to motor control thread
 @param argument Unused
 */
void receive_thread(void const *argument) {
	
	uint8_t num_RX_bytes = 0x00;
	uint8_t received_value = 0x00;
	
	uint8_t tempSender = 0x00;
	
	while(1) {
		
		// If in keypad mode, wait for keypad mode to end and do not receive data.
		if (keypad_flag == 1) {
			osSignalWait(KEYPAD_MODE, osWaitForever);
		}
		
		// Set cc2500 to RX mode
		cmd_strobe_RX();
		
		// Wait for data to be recieved by cc2500
		//osSignalWait(INTERRUPT_RX, osWaitForever);
		osSignalWait(TICK_25HZ, osWaitForever);
		
		// Get # bytes in the RX FIFO
		num_RX_bytes = get_RX_bytes();
		
		// If there is nothing in the RX FIFO but interrupt was triggered,
		// Flush and go idle. 
		if (num_RX_bytes == 0x00) {
			cmd_strobe_IDLE();
			cmd_strobe_RXFLUSH();
		}

		// Receive alpha angle
		alpha_angle = send_one_byte(0x01, 0x00, 0x3F, 0x00);
		printf("Received value1 = 0x%X\n", alpha_angle);
		
		// Receive beta angle
		beta_angle	= send_one_byte(0x01, 0x00, 0x3F, 0x00);
		printf("Received value2 = 0x%X\n", beta_angle);
		
		// Set cc2500 to idle and flush RX FIFO
		cmd_strobe_IDLE();
		cmd_strobe_RXFLUSH();
		
		// Send signal to motor control thread
		osSignalSet(tid_thread3, MOTOR_FLAG);
		
	}
}

/*!
 @brief Thread to control and move the motors
 @param argument Unused
 */
void motor_control_thread(void const *argument) {
	
	struct averageFilter avgFilterAlpha;
	struct averageFilter avgFilterBeta;
	avgFilterAlpha.index = 0;
	avgFilterAlpha.numValues = 0;
	avgFilterBeta.index = 0;
	avgFilterBeta.numValues = 0;
	
	while(1) {
		
		// Wait for the RX thread to signal it has received data
		osSignalWait(MOTOR_FLAG, osWaitForever);

		movingAverageFilter(alpha_angle, &avgFilterAlpha);
		movingAverageFilter(beta_angle, &avgFilterBeta);
		
		alpha_angle = avgFilterAlpha.average;
		beta_angle = avgFilterBeta.average;
		
		// Move the motors to the new angles
		alpha_motor(alpha_angle);
		beta_motor(beta_angle);
		
		/*
		alpha = alpha_angle;
		beta = beta_angle;
		*/
		
		sprintf(s_alpha, "%d", alpha_angle);
		sprintf(s_beta, "%d", beta_angle);

	}
	
}

/*!
 @brief Thread to perform keypad operations
 @param argument Unused
 */
void keypad_control_thread(void const *argument) {
	
	int tempkey = 0;
	int manual_mode = 0;

	
//	char alpha [20];
//	char beta [20];
	
	while(1) {
		
		osSignalWait(TICK_25HZ, osWaitForever);
		
		keypad_press_check();
		tempkey = get_key_pressed(row, column);
		
		if (tempkey == 0) {
			// Do nothing
		}
		else if (tempkey == 10) {
			keypad_flag = 1;
		}
		else if (tempkey == 11) {
			keypad_flag = 0;
			osSignalSet(tid_thread2, KEYPAD_MODE);
		}
		
		// If in keypad mode, check keypress for
		// Pre-defined position & move motors.
		if (keypad_flag == 1) {
			
			if (tempkey == 1) {
				m_alpha = 170;
				m_beta = 170;
			}
			else if (tempkey == 3) {
				m_alpha = 10;
				m_beta = 10;
			}
			else if (tempkey == 5) {
				m_alpha = 90;
				m_beta = 90;
			}
			else if (tempkey == 2) {
				if (m_alpha < 180) {m_alpha += 5;}
			}
			else if (tempkey == 8) {
				if (m_alpha > 0) {m_alpha -= 5;}
			}
			else if (tempkey == 6) {
				if (m_beta < 180) {m_beta += 5;}
			}
			else if (tempkey == 4) {
				if (m_beta > 0) {m_beta -=5;}
			}
			
			// Move motors
			alpha_motor(m_alpha);
			beta_motor(m_beta);
			
			sprintf(s_alpha, "%d", m_alpha);
			sprintf(s_beta, "%d", m_beta);
			
		}
		
	}
	
}

/*!
 @brief Thread to perform LCD display
 @param argument Unused
 */
void screen_control_thread(void const *argument) {
	
	int alphalen = 0;
	int betalen = 0;
	
	clear_display();
	
	write_char("Alpha: ", 7);
	move_cursor(12);
	write_char("Beta: ", 6);
	
	while(1) {
		
		osSignalWait(TICK_5HZ, osWaitForever);
		
		if (keypad_flag == 0) {
			if (alpha_angle < 10) {
				alphalen = 1;
			}
			else if (alpha_angle >= 10 && alpha_angle < 100) {
				alphalen = 2;
			}
			else if (alpha_angle >= 100) {
				alphalen = 3;\
			}
			
			if (beta_angle < 10) {
				betalen = 1;
			}
			else if (beta_angle >= 10 && beta_angle < 100) {
				betalen = 2;
			}
			else if (beta_angle >= 100) {
				betalen = 3;\
			}
		}
		else if (keypad_flag == 1) {
			if (m_alpha < 10) {
				alphalen = 1;
			}
			else if (m_alpha >= 10 && m_alpha < 100) {
				alphalen = 2;
			}
			else if (m_alpha >= 100) {
				alphalen = 3;\
			}
			
			if (m_beta < 10) {
				betalen = 1;
			}
			else if (m_beta >= 10 && m_beta < 100) {
				betalen = 2;
			}
			else if (m_beta >= 100) {
				betalen = 3;\
			}
		}

		// Send angles to SCREEN.
		move_cursor(7);
		switch(alphalen){
			case 1:
				s_alpha[alphalen] = ' ';
				s_alpha[alphalen + 1] = ' ';
				break;
			case 2: 
				s_alpha[alphalen] = ' ';
				break;
			case 3:
				break;
		};
		alphalen = 3;
		write_char(s_alpha, alphalen);

		move_cursor(18);
		switch(betalen){
			case 1:
				s_beta[betalen] = ' ';
				s_beta[betalen + 1] = ' ';
				break;
			case 2: 
				s_beta[betalen] = ' ';
				break;
			case 3:
				break;
		};
		betalen = 3;
		write_char(s_beta, betalen);

	}
}
