#include "DMA_Transmit.h"

uint16_t StringToUint16(uint8_t *str, uint8_t len)
{
    uint8_t result = 0;
    for (uint8_t i = 0; i < len - 1; i++)
        result = result * 10 + str[i] - '0';
    return result;
}

void vResetDMAReceive()
{
    Rxlen = 0;
    memset(DMA_receive_buffer, 0, sizeof(DMA_receive_buffer));
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)DMA_receive_buffer, 30);
}

void vreceiveLightCommand()
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
    {

        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);

        HAL_UART_DMAStop(&huart1);

        Rxlen = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"ok\n", 3);
        HAL_Delay(30);

        uint8_t index = 0;
        while (index < Rxlen && DMA_receive_buffer[index] != 0xFA)
            index++;
        if (index > Rxlen) // Input invalid
        {
            vResetDMAReceive();
            return;
        }

        HAL_UART_Transmit_DMA(&huart1, DMA_receive_buffer + index, 2);
        HAL_Delay(30);

        if (DMA_receive_buffer[index + 1] >= 0 && DMA_receive_buffer[index + 1] <= 100)
            vChangeLightPWMMaximum(DMA_receive_buffer[index + 1]);
        else
        {
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"Overflow\n", 9);
            HAL_Delay(30);
        }

        vResetDMAReceive();
    }
}

uint8_t LightPWMFlag = 0;
uint16_t duty_num = 10;
uint8_t MaxLightCCR = 50;
void vChangeLightPWMMaximum(uint8_t MAXCCR)
{
    MaxLightCCR = MAXCCR;
}

void vLightPWM()
{
    if (LightPWMFlag == 0)
        duty_num = duty_num + 5;
    if (LightPWMFlag == 1)
        duty_num = duty_num - 5;

    if (duty_num > MaxLightCCR)
        LightPWMFlag = 1;
    if (duty_num < 5)
        LightPWMFlag = 0;

    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, duty_num);
}

void vreceiveCommandFromComputer()
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
    {

        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);

        HAL_UART_DMAStop(&huart1);

        Rxlen = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

        //find Start byte FA
        uint8_t index = 0;
        while (index < Rxlen && DMA_receive_buffer[index] != 0xFA) index++;
        if (index >= Rxlen) // Input invalid, clear the buffer 
        {
            vResetDMAReceive();
            return;
        }
        
        // get the accurate input data length
        uint8_t datalen = index;
        while (datalen < Rxlen && DMA_receive_buffer[datalen] != 0x00) datalen ++;
        

        HAL_UART_Transmit_DMA(&huart1, DMA_receive_buffer + index, datalen);
        HAL_Delay(30);

        vResetDMAReceive();
    }
}
