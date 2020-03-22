#include "stm32f4xx.h"

//CONSTANTS
#define TRUE 1
#define USER_BUTTON GPIO_Pin_0


//VARIABLES
static uint16_t tics = 0;

uint16_t btn_pressed = 0;
uint16_t rev_min = 500;



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
//Inicializo el timer4
void TM_TIMER_Init(void) {
	uint16_t periodo;
	uint16_t msTimer;
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;

	/* Enable clock for TIM4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
/*    
    TIM4 is connected to APB1 bus, which has on F407 device 42MHz clock                 
    But, timer has internal PLL, which double this frequency for timer, up to 84MHz     
    Remember: Not each timer is connected to APB1, there are also timers connected     
    on APB2, which works at 84MHz by default, and internal PLL increase                 
    this to up to 168MHz                                                             
    
    Set timer prescaller
    Timer count frequency is set with
    
    timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)        
    
    In our case, we want a max frequency for timer, so we set prescaller to 0         
    And our timer will have tick frequency        
    
    timer_tick_frequency = 84000000 / (0 + 1) = 84000000
*/
	TIM_BaseStruct.TIM_Prescaler = 0;
	/* Count up */
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
/*
    Set timer period when it have reset
    First you have to know max value for timer
    In our case it is 16bit = 65535
    To get your frequency for PWM, equation is simple
    
    PWM_frequency = timer_tick_frequency / (TIM_Period + 1)
    
    If you know your PWM frequency you want to have timer period set correct
    
    TIM_Period = timer_tick_frequency / PWM_frequency - 1
    
    In our case, for 10Khz PWM_frequency, set Period to
    
    
    
    If you get TIM_Period larger than max timer value (in our case 65535),
    you have to choose larger prescaler and slow down timer tick frequency
*/  msTimer = 60000 / 7500;
	periodo = 84000000 / (1000 * msTimer) - 1;
	TIM_BaseStruct.TIM_Period = periodo; /* Al ritmo de la variable rpms */
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;
	/* Initialize TIM4 */
	TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
	/* Start count on TIM4 */
	TIM_Cmd(TIM4, ENABLE);
}


void TIM2_IRQHandler(void){
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){

	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	  TM_TIMER_Init(); //Cargo otra vez el timer 4

  }
}

void TIM4_IRQHandler(void){
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){

	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	  if (GPIO_ReadInputDataBit(GPIOA, USER_BUTTON) == TRUE){
		  //Boto pulsat!!!
		  btn_pressed = 1;
		  rev_min = (rev_min % 7500) + 500;
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
	//while (TRUE) {
		//calcula_temps_injecció();
		//espera_interrupció();
	//}
}

