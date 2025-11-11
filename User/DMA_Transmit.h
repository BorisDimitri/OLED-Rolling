#ifndef DMA_TRANSMIT_H
#define DMA_TRANSMIT_H

#include "main.h"
#include "string.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern TIM_HandleTypeDef htim2;

static uint8_t DMA_receive_buffer[20];
static uint8_t Rxlen;

void vreceiveLightCommand();
void vChangeLightPWMMaximum(uint8_t MAXCCR);
void vLightPWM();

#endif
