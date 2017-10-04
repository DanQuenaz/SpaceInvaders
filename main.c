/*
Autor: Dan Quenaz M. S. Pimentel
*/
#include "environment.h"


int main(){
	
	SystemCoreClockUpdate(); // Update SystemCoreClock to 72 MHz
	SysTick_Init (); // Initialize SysTick Timer
	NVIC_SetPriorityGrouping(3);
	
	//Inicializa();
	configADC();
	GLCD_Initialize();
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen();
	startTIM();
	EXTI_Config();
	environment();

}
