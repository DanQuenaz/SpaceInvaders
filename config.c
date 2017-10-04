/*
Este arquivo contem a implementação das funções do arquivo config.h
*/

#include "config.h"

GPIO_InitTypeDef gpioInit;
TIM_TimeBaseInitTypeDef timTBInit;
TIM_OCInitTypeDef timOCInit;
NVIC_InitTypeDef nvicInit;
EXTI_InitTypeDef EXTI_InitStructure;


int32_t volatile msTicks = 0; // Interval counter in ms



// Set the SysTick interrupt interval to 1ms
void SysTick_Init (void) {
	if (SysTick_Config (SystemCoreClock / 1)) {
	// handle error
	}
}
// SysTick Interrupt Handler function called automatically
void SysTick_Handler (void) {
	msTicks++; // Increment counter
}

void startTIM(){
	RCC_PCLK1Config(RCC_HCLK_Div8);//Prescaler APB1 = 18M
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE);
	
	nvicInit.NVIC_IRQChannel = TIM4_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInit.NVIC_IRQChannelSubPriority = 0;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init (&nvicInit);
	
	timTBInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timTBInit.TIM_CounterMode = TIM_CounterMode_Up;
	timTBInit.TIM_Period = 0xFFFF;//Valor do ARR 
	timTBInit.TIM_Prescaler = 8999;//Valor do prescaler do Contador -> 18M/9k = 9KHz
	TIM_TimeBaseInit(TIM4, &timTBInit);
	
	timOCInit.TIM_OCMode = TIM_OCMode_Timing;
	timOCInit.TIM_OCPolarity = TIM_OCPolarity_High;
	timOCInit.TIM_OutputState = TIM_OutputState_Enable;
	timOCInit.TIM_Pulse = 100;//Valor do comparador
	TIM_OC1Init(TIM4, &timOCInit);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
	
	TIM_ITConfig(TIM4, (TIM_IT_CC1), ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	
}

void configADC(){
	GPIO_InitTypeDef gpio;
	ADC_InitTypeDef adcInit;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	gpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &gpioInit);
	
	adcInit.ADC_ContinuousConvMode = DISABLE;
	adcInit.ADC_DataAlign = ADC_DataAlign_Right;
	adcInit.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adcInit.ADC_Mode = ADC_Mode_Independent;
	adcInit.ADC_NbrOfChannel = 1;
	adcInit.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &adcInit);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_1Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
	ADC_GetCalibrationStatus(ADC1);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == RESET);
	
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
}


void EXTI_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	
	gpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &gpioInit);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init (&EXTI_InitStructure);
	
	nvicInit.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 11;
	nvicInit.NVIC_IRQChannelSubPriority = 0;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init (&nvicInit);	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	gpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &gpioInit);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init (&EXTI_InitStructure);
	
	nvicInit.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 10;
	nvicInit.NVIC_IRQChannelSubPriority = 0;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init (&nvicInit);
}
