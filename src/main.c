/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
			
//CONSTANTS
#define TRUE 1
#define USER_BUTTON GPIO_Pin_0
#include "stm32f4xx.h"

//CONSTANTS
#define TRUE 1
#define USER_BUTTON GPIO_Pin_0
#define INJECTOR_GPIO_A GPIO_Pin_2
#define INJECTOR_GPIO_B GPIO_Pin_4
#define INJECTOR_GPIO_C GPIO_Pin_6



//VARIABLES
static uint16_t tics = 0;
uint16_t btn_pressed = 0;
uint16_t rev_min = 0;
uint16_t frec = 2000;
uint16_t array_cargar[16] = {479,239,159,119,95,79,67,59,52,47,42,39,35,33,31};
uint16_t flag_decrementa = 1;











//Configuració simuladors switch
void init_switch(void){
                GPIO_InitTypeDef gpio_a;

                //El configurem en el GPIOD
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

                GPIO_StructInit(&gpio_a);
                gpio_a.GPIO_Pin = INJECTOR_GPIO_A;
                //Input mode
                gpio_a.GPIO_Mode = GPIO_Mode_IN;
                //Activem pull-up (pq simulem, no soldem switch)
                gpio_a.GPIO_PuPd = GPIO_PuPd_UP;
                //Carreguem configuració pin
                GPIO_Init(GPIOD, &gpio_a);


                GPIO_InitTypeDef gpio_b;

                //El configurem en el GPIOD
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

                GPIO_StructInit(&gpio_b);
                gpio_b.GPIO_Pin = INJECTOR_GPIO_B;
                //Input mode
                gpio_b.GPIO_Mode = GPIO_Mode_IN;
                //Activem pull-up (pq simulem, no soldem switch)
                gpio_b.GPIO_PuPd = GPIO_PuPd_UP;
                //Carreguem configuració pin
                GPIO_Init(GPIOD, &gpio_b);


                GPIO_InitTypeDef gpio_c;

                //El configurem en el GPIOD
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

                GPIO_StructInit(&gpio_c);
                gpio_c.GPIO_Pin = INJECTOR_GPIO_C;
                //Input mode
                gpio_c.GPIO_Mode = GPIO_Mode_IN;
                //Activem pull-up (pq simulem, no soldem switch)
                gpio_c.GPIO_PuPd = GPIO_PuPd_UP;
                //Carreguem configuració pin
                GPIO_Init(GPIOD, &gpio_c);
}

//Configuració de la interrupció del injector a del switch
void init_injector_a_interrupt1(void){


                //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
                EXTI_InitTypeDef EXTI_InitStruct;
                NVIC_InitTypeDef NVIC_InitStruct;
                /* Tell system that you will use PD2 for EXTI_Line2 */
                SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);
                /* PD2 is connected to EXTI_Line2 */
                EXTI_InitStruct.EXTI_Line = EXTI_Line2;
                /* Enable interrupt */
                EXTI_InitStruct.EXTI_LineCmd = ENABLE;
                /* Interrupt mode */
                EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
                /* Triggers on rising and falling edge */
                //Posem que interrompeixi amb flanc de pujada i de baixada
                EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
                /* Add to EXTI */
                EXTI_Init(&EXTI_InitStruct);
                /* Add IRQ vector to NVIC */
                /* PD2 is connected to EXTI_Line2, which has EXTI2_IRQn vector */
                NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
                /* Set priority */
                NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
                /* Set sub priority */
                NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
                /* Enable interrupt */
                NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
                /* Add to NVIC */
                NVIC_Init(&NVIC_InitStruct);

}

//Configuració de la interrupció del injector b del switch
void init_injector_a_interrupt2(void){


                //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

                EXTI_InitTypeDef EXTI_InitStruct;
                NVIC_InitTypeDef NVIC_InitStruct;
                /* Tell system that you will use PD4 for EXTI_Line4 */
                SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource4);
                /* PD4 is connected to EXTI_Line4 */
                EXTI_InitStruct.EXTI_Line = EXTI_Line4;
                /* Enable interrupt */
                EXTI_InitStruct.EXTI_LineCmd = ENABLE;
                /* Interrupt mode */
                EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
                /* Triggers on rising and falling edge */
                //Posem que interrompeixi amb flanc de pujada i de baixada
                EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
                /* Add to EXTI */
                EXTI_Init(&EXTI_InitStruct);
                /* Add IRQ vector to NVIC */
                /* PD4 is connected to EXTI_Line4, which has EXTI4_IRQn vector */
                NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
                /* Set priority */
                //No li assignem cap prioritat
                NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
                /* Set sub priority */
                NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
                /* Enable interrupt */
                NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
                /* Add to NVIC */
                NVIC_Init(&NVIC_InitStruct);

}

//Configuració de la interrupció del injector c del switch
void init_injector_a_interrupt3(void){


                //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

                EXTI_InitTypeDef EXTI_InitStruct;
                NVIC_InitTypeDef NVIC_InitStruct;
                /* Tell system that you will use PD6 for EXTI_Line6 */
                SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);
                /* PD6 is connected to EXTI_Line6 */
                EXTI_InitStruct.EXTI_Line = EXTI_Line6;
                /* Enable interrupt */
                EXTI_InitStruct.EXTI_LineCmd = ENABLE;
                /* Interrupt mode */
                EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
                /* Triggers on rising and falling edge */
                //Posem que interrompeixi amb flanc de pujada i de baixada
                EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
                /* Add to EXTI */
                EXTI_Init(&EXTI_InitStruct);
                /* Add IRQ vector to NVIC */
                /* PD6 is connected to EXTI_Line6, which has EXTI2_IRQn vector */
                NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
                /* Set priority */
                NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
                /* Set sub priority */
                NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
                /* Enable interrupt */
                NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
                /* Add to NVIC */
                NVIC_Init(&NVIC_InitStruct);

}

//RSI del boto a del switch (INJECTOR A)
void EXTI2_IRQHandler(void) {
    //Mirem si s'ha interromput
    if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
               //Posem a 0 el flag de la interrupcio
               EXTI_ClearITPendingBit(EXTI_Line2);
               //CODI
    }
}

//RSI del boto b del switch (INJECTOR B)
void EXTI4_IRQHandler(void) {
    //Mirem si s'ha interromput
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
               //Posem a 0 el flag de la interrupcio
               EXTI_ClearITPendingBit(EXTI_Line4);
               //CODI
    }
}

//RSI del boto c del switch (INJECTOR C)
void EXTI9_5_IRQHandler(void) {
    //Mirem si s'ha interromput
    if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
               //Posem a 0 el flag de la interrupcio
               EXTI_ClearITPendingBit(EXTI_Line6);
               //CODI
    }
}








void delay(int counter)
{
	int i;
	for (i = 0; i < counter * 10000; i++) {}
}

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




void configuraGPIOG13() {
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOG, &gpio);
}


void TIM2_IRQHandler(void){
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){

	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);


	  if (GPIO_ReadInputDataBit(GPIOA, USER_BUTTON) == TRUE){
		  //Boto pulsat!!!
		  btn_pressed = 1;
		  if (rev_min == 15)  flag_decrementa = -1;
		  if (rev_min == 0) flag_decrementa = 1;

		  rev_min = (rev_min + flag_decrementa);
		  TM_TIMER_Init();
		  TM_PWM_Init();
		  TM_LEDS_Init();
		  GPIO_SetBits(GPIOG, GPIO_Pin_13);
	  } else{
		  btn_pressed=0;
		  GPIO_ResetBits(GPIOG, GPIO_Pin_13);
	  }
	  //INTTIM_Config(100);*/

  }
}

void TM_TIMER_Init(void) {
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
	TIM_BaseStruct.TIM_Prescaler = 10499;
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

	TIM_Period = 84000000 / 10000 - 1 = 8399

	If you get TIM_Period larger than max timer value (in our case 65535),
	you have to choose larger prescaler and slow down timer tick frequency
*/
    TIM_BaseStruct.TIM_Period = array_cargar[rev_min]; //El 16 es del prescaler.
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
	/* Initialize TIM4 */
    TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
	/* Start count on TIM4 */
    TIM_Cmd(TIM4, ENABLE);
}

void TM_PWM_Init(void) {
    TIM_OCInitTypeDef TIM_OCStruct;

    /* Common settings */

    /* PWM mode 2 = Clear on compare match */
    /* PWM mode 1 = Set on compare match */
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

    uint16_t max_tics = array_cargar[rev_min];

/*
    To get proper duty cycle, you have simple equation

    pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1

    where DutyCycle is in percent, between 0 and 100%

    25% duty cycle:     pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
    50% duty cycle:     pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
    75% duty cycle:     pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
    100% duty cycle:    pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399

    Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time
*/      uint16_t  pulse_length = ((max_tics + 1) * 25) / 100 - 1;
        TIM_OCStruct.TIM_Pulse = pulse_length; /* 25% duty cycle */
        TIM_OC1Init(TIM4, &TIM_OCStruct);
        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

        pulse_length = ((max_tics + 1) * 50) / 100 - 1;
        TIM_OCStruct.TIM_Pulse = pulse_length; /* 50% duty cycle */
        TIM_OC2Init(TIM4, &TIM_OCStruct);
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

        pulse_length = ((max_tics + 1) * 75) / 100 - 1;
        TIM_OCStruct.TIM_Pulse = pulse_length; /* 75% duty cycle */
        TIM_OC3Init(TIM4, &TIM_OCStruct);
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

        TIM_OCStruct.TIM_Pulse = max_tics; /* 100% duty cycle */
        TIM_OC4Init(TIM4, &TIM_OCStruct);
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

void TM_LEDS_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	    /* Clock for GPIOD */
	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	    /* Alternating functions for pins */

	    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	    	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	    	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	    	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);


	    /* Set pins */
	    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	    GPIO_Init(GPIOD, &GPIO_InitStruct);
}


void init_clock(void) {
	RCC_DeInit();

	//Activem cristall extern HSE
	RCC_HSEConfig(RCC_HSE_ON);
	//Esperem a que s'activi
	RCC_WaitForHSEStartUp();

	//Configurem el PLL per tenir sysclock sigui de 168 MHz
	RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);
	//Activem el PLL
	RCC_PLLCmd(ENABLE);
	//Esperem a que el PLL estigui actiu
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

	//AHB a 168MHz
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	//APB1 a 42MHz
	RCC_PCLK1Config(RCC_HCLK_Div4);
	//APB2 a 84MHz
	RCC_PCLK2Config(RCC_HCLK_Div2);

	//Posem el PLL al clock
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}

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

void inicialitza_sistema(void){
	init_clock();
	init_button();
	TM_TIMER_Init();
	TM_PWM_Init();
	TM_LEDS_Init();
}

int main(void) {
	inicialitza_sistema();
	INTTIM_Config(2);
    configuraGPIOG13();
    while(1) {}
    //TM_TIMER_Init(); //Timer4

}
