#ifndef DMA_TRANSMIT_H
#define DMA_TRANSMIT_H

#include "main.h"
#include "string.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

static uint8_t DMA_receive_buffer[30];
static uint8_t Rxlen;

void vResetDMAReceive(void);
void vreceiveCommandFromComputer(void);
void vreceiveLightCommand(void);
void vChangeLightPWMMaximum(uint8_t MAXCCR);
void vLightPWM();

#endif
