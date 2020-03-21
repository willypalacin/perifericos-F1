
#include "stm32f4xx.h"
void delay(int counter)
{
	int i;
	for (i = 0; i < counter * 10000; i++) {}
}
int main(void)
{
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_StructInit(&gpio); gpio.GPIO_Pin = GPIO_Pin_13; gpio.GPIO_Mode = GPIO_Mode_OUT; GPIO_Init(GPIOG, &gpio);
	GPIO_SetBits(GPIOG, GPIO_Pin_13);
	while (1) {
		GPIO_SetBits(GPIOG, GPIO_Pin_13); // LED ON delay(400);
		GPIO_ResetBits(GPIOG, GPIO_Pin_13); // LED OFF delay(400);
	}
}
