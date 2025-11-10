#ifndef DMA_TRANSMIT_H
#define DMA_TRANSMIT_H

#include "main.h"
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

uint8_t Rxlen = 0;
// receive Comand string from DMA and send it to the queue
void RspUART4ReceiveIDLEFromISR()
{
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        
        HAL_UART_DMAStop(&huart1);
        
        Rxlen = __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(Uart_Recv_BinarySemHandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

#endif