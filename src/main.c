#include "stm32f4xx.h"

//CONSTANTS
#define TRUE 1
#define USER_BUTTON GPIO_Pin_0


//VARIABLES
static uint16_t tics = 0;

uint16_t btn_pressed = 0;



void INTTIM_Config(uint16_t numOfMilleseconds){

  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM2 gloabal Interrupt */

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  /* TIM2 clock enable */

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


  /* Time base configuration */

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  TIM_TimeBaseStructure.TIM_Period = (500*numOfMilleseconds) - 1;  // 1 MHz down to 1 KHz (1 ms)

  TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)

  //TIM_TimeBaseStructure.TIM_Prescaler = 168-1; //168MHz Clock should be down to 1MHz

  TIM_TimeBaseStructure.TIM_ClockDivision = 0;

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

  /* TIM IT enable */

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  /* TIM2 enable counter */

  TIM_Cmd(TIM2, ENABLE);

}

void TIM2_IRQHandler(void){
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){

	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	  if (GPIO_ReadInputDataBit(GPIOA, USER_BUTTON) == TRUE){
		  //Boto pulsat!!!
		  btn_pressed = 1;
	  }

  }
}

//Init clock
//NO ESTOY SEGURO DE QUE ESTË BIEN ASI...!!!
void init_clock(void){
	//Posem la config per defecte
	RCC_DeInit();

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	uint32_t freq = 310000;
	SysTick_Config(RCC_Clocks.HCLK_Frequency / freq);

}


//Configuració boto usuari
void init_button(void){
	GPIO_InitTypeDef gpio;

	//Activem el clock del GPIOA
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

