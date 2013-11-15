#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4_discovery_lis302dl.h"
#include "stm32f4xx_gpio.h"

/**
	* @file initialization.c
	* @brief The functions to setup the ADC, accelerometer, tap and user button.
	*/

/**
	* @brief Setup the ADC driver configuration parameters
	*/
void ADC_config(){

	// Enable the ADC interface clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// Declare structure
	ADC_InitTypeDef ADC1_InitTypeDef;

	// Initializing the ADC structure
	ADC_StructInit(&ADC1_InitTypeDef);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);

	// After the initializations, enable the ADC
	ADC_Cmd(ADC1, ENABLE);

	// Enable Temperature Sensor
	ADC_TempSensorVrefintCmd(ENABLE); 
	
}

/**
	* @brief LIS302DL driver for reading accelerations and interrupt structures for external interrupts
	*/
void accelerometer_config(void){
	
		/* Define an accelerometer struct and provide values for the fields of the data structure */
		LIS302DL_InitTypeDef accelerometer;
		accelerometer.Axes_Enable = LIS302DL_XYZ_ENABLE; //Enable XYZ axes
		accelerometer.Output_DataRate = LIS302DL_DATARATE_100; //Meets required output rate at least 25 times per second and is less than 400
		accelerometer.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE; //Set the power mode to active
		accelerometer.Self_Test = LIS302DL_SELFTEST_NORMAL; 
		accelerometer.Full_Scale = LIS302DL_FULLSCALE_2_3; //Using a smaller range for more precision
		LIS302DL_Init(&accelerometer);
	
		/* Enable the accelerometer interrupt line */
		LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;
		LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_LATCHED; //Enable the interrupt status register to latch interrupt requests
		LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_XYZ_ENABLE; //Enable the single click interrupt on XYZ axes
		LIS302DL_InterruptStruct.DoubleClick_Axes =	LIS302DL_CLICKINTERRUPT_XYZ_DISABLE; //Disable the double click interrupt on XYZ axes
		LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);
}

/**
	* @brief Configure GPIO pins, EXTI structure, EXTI interrupt line EXTILine1 to detect single taps
	*/
void tap_config(void) {
		uint8_t ctrl = 0;
	
		/* Enable RCC_AHB1Periph_GPIOE clock since it is connected to the external interrupt for the accelerometer */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		/* Enable the APB2 peripheral SYSCFG since EXTI is connected to it */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

		/* Enable GPIO and connect the GPIO pin GPIO_Pin_1 to the accelerometer interrupt line */
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //LIS302DL_SPI_INT1_PIN is defined as GPIO_Pin_1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOE, &GPIO_InitStructure); //LIS302DL_SPI_INT2_GPIO_PORT is defined as GPIOE

		//Enable EXTI and configure EXTI Line0 
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_InitStructure.EXTI_Line = EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //Work on rising edge of the clock 
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		//Enable and set EXTI Line1 Interrupt 
		NVIC_InitTypeDef NVIC_InitStructEXTI;
		NVIC_InitStructEXTI.NVIC_IRQChannel = EXTI1_IRQn;
		
		/* Setting the preemtion priority of the tap to be lower than the user button interrupt */
		NVIC_InitStructEXTI.NVIC_IRQChannelPreemptionPriority = 0x01;
		NVIC_InitStructEXTI.NVIC_IRQChannelSubPriority = 0x00;
		NVIC_InitStructEXTI.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructEXTI);
		
		/* Connect the EXTILine1 to accelerometer interrupt pin */
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);

		/* Configure Interrupt control register: enable click interrupt1 */
		/* I2CFG2,I2CFG1,I2CFG0 set to 1 and I1CFG2,I1CFG1,I1CFG0 set to 0 */
		/* 111000 = 0x38 in hex */ 
		ctrl = 0x038;
		LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG3_ADDR, 1);
		
		/* Enable Interrupt generation on single click on all axes*/
		/* Single_X, Single_Y, Single_Z values are set to 1 ; 10101 = 0x15 in hex */
		ctrl = 0x15;
		LIS302DL_Write(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);

		/* Configure Click Threshold on X/Y axis (10 x 0.5g) 0xAA*/
		ctrl = 0xFF ;
		LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSY_X_REG_ADDR, 1);

		/* Configure Click Threshold on Z axis (10 x 0.5g) 0x0A*/
		ctrl = 0x0F;
		LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSZ_REG_ADDR, 1);

		/* Configure Time Limit */
		ctrl = 0x03;
		LIS302DL_Write(&ctrl, LIS302DL_CLICK_TIMELIMIT_REG_ADDR, 1);
		
		/* Configure Latency */
		uint8_t latency = 0xFA;
		LIS302DL_Write(&latency, LIS302DL_CLICK_LATENCY_REG_ADDR, 1);
		
		//click window not required for single click
}

/**
	* @brief Initialize user button configuration
	*/
void userbutton_config(void){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	
	// Configure user button
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_0;	/* Setup the user button connected to pin 0 */
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure2);
	
	//Enable EXTI and configure EXTI Line0
	EXTI_InitTypeDef EXTI_InitStructure2;
	EXTI_InitStructure2.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure2.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure2.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure2.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure2);

	//Enable and set EXTI Line0 Interrupt 
	NVIC_InitTypeDef NVIC_InitStructure2;
	NVIC_InitStructure2.NVIC_IRQChannel = EXTI0_IRQn;
	
	/* Setting the preemption priority of user button be higher than the tap interrupt */
	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure2); 
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
}

/**
	* @brief Initialize the ADC, accelerometer, tap and user button configurations
	*/
void initialization(void) {
	ADC_config();
	accelerometer_config();
	tap_config();
	userbutton_config();
}
