#include "DMA_Transmit.h"


uint8_t StringToUint8(uint8_t* str, uint8_t len)
{
    uint8_t result = 0;
    for(uint8_t i=0;i<len-1;i++)
        result = result * 10 + str[i] - '0';
    return result;
}

void receiveLightCommand()
{
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        
        HAL_UART_DMAStop(&huart1);
        
        Rxlen = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
        
        HAL_UART_Transmit_DMA(&huart1,DMA_receive_buffer,Rxlen);
        
        
        //__HAL_TIM_SetCompare(htim2, TIM_CHANNEL_1, duty);
        
        Rxlen = 0;
        memset(DMA_receive_buffer,0,sizeof(DMA_receive_buffer));
    }
    HAL_UART_Receive_DMA(&huart1, (uint8_t*)DMA_receive_buffer, 10);
}

uint8_t LightPWMFlag = 0;
uint16_t duty_num = 10;
void LightPWM()
{
    if(LightPWMFlag==0) duty_num = duty_num + 10;	
    if(LightPWMFlag==1) duty_num = duty_num - 10;	
    
    if(duty_num > 500) LightPWMFlag=1;	
    if(duty_num < 10) LightPWMFlag=0;	
    __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,duty_num);
}

