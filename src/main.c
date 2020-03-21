#include "stm32f4xx.h"

//CONSTANTS
#define TRUE 1

#define GREEN_LED_13 GPIO_Pin_13
#define RED_LED_14 GPIO_Pin_14
#define USER_BUTTON GPIO_Pin_0


//VARIABLES
static uint16_t tics = 0;

//Init clock
void init_clock(void){
	//Posem la config per defecte
	RCC_DeInit();
}

//Configuració boto usuari
void init_button(void){
	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_StructInit(&gpio);

	gpio.GPIO_Pin = USER_BUTTON;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOA, &gpio);
}

//Crida de tots els inits
void inicialitza_sistema(void){
	init_clock();
	init_button();

}

void main(void){
	inicialitza_sistema();
	while (TRUE) {
		calcula_temps_injecció();
		espera_interrupció();
	}
}
