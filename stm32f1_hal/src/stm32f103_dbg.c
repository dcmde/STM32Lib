#include "stm32f103_dbg.h"

#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>

//volatile uint32_t DBG_TimeStamp = 0;

/**
	* @about Redefine write function to send data through SWO.
	* @brief In order to have printf workin, we redefine the write function to 
	*				 send data through ITM. (PB3 pin)
	*/
int _write(int file, char *ptr, int len){
	
	for(uint32_t DataIdx = 0; DataIdx < len; ++DataIdx){
		// Send data through PB3 using ITM.
		ITM_SendChar(*ptr++);
	}
	// Return the number of bytes sent.
	return len;
}

///**
//	* @about Activate SW debug.
//	* @brief Set the register to activate the SWO port (PB3).
//	*    	 The port PB3 could not be used when this is activated.
//	*/
//void DBG_Init(void){
//	// Get register value.
//	uint32_t tmp = AFIO->MAPR;
//	// Clear bits of the register for debug config.
//	tmp &= ~AFIO_MAPR_SWJ_CFG;
//	// Enable SW-DP and disable JTAG.
//	tmp |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
//	// Write into the register the new config.
//	AFIO->MAPR = tmp;
//}


//void DBG_InitTimestamp(DBG_TimestampFreq dbg_timestampFreq){
//
//	TIM_TimeBaseInitTypeDef TIM_InitStruct;
//
//	DBG_Init();
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//
//	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_InitStruct.TIM_Period = (uint16_t)18*dbg_timestampFreq;
//	TIM_InitStruct.TIM_Prescaler = 3;
//	TIM_InitStruct.TIM_RepetitionCounter = 0;
//
//	TIM_TimeBaseInit(TIM3, &TIM_InitStruct);
//
//	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//	NVIC_EnableIRQ(TIM3_IRQn);
//
//	TIM_Cmd(TIM3, ENABLE);
//
//}
//
//void DBG_PrintTimestamp(void){
//    printf("%lu\n", DBG_TimeStamp);
//}

//void TIM3_IRQHandler(void){
//    ++DBG_TimeStamp;
//    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//}