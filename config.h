/*
Este arquivo contem as fun��es para configura��es dos perifericos
*/

#ifndef CONFIG_H				
#define CONFIG_H

#include "stm32f10x.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern GLCD_FONT GLCD_Font_16x24;//defini��o da fonte

void startTIM(void);//Configura o TIM
void configADC(void);//Configura o ADC
void EXTI_Config(void);//Configura o GPIO e suas interrup��es
void SysTick_Init (void);


#endif
