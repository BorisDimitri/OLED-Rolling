#ifndef DMA_TRANSMIT_H
#define DMA_TRANSMIT_H

#include "main.h"
#include "string.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

static uint8_t DMA_receive_buffer[20];
static uint8_t Rxlen;

void receiveLightCommand();


#endif