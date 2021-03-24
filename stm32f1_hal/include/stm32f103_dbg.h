/**
  * @file    header.h
  * @author  Couet David
  * @version V1.0
  * @date    24-July-2019
  * @brief   Configure SW-DP for STM32F103
	*/

#ifndef STM32F103_DBG_H
#define STM32F103_DBG_H

/*--------------------------------- Includes ---------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"

/*--------------------------------- Enums ---------------------------------*/
typedef enum {
    DBG_TimestampFreq_103 = 1000, DBG_TimestampFreq_104 = 100, DBG_TimestampFreq_105 = 10
} DBG_TimestampFreq;

/*--------------------------- Functions prototype ----------------------------*/

/**
	* @about Activate SW debug.
	* @brief Set the register to activate the SWO port (PB3).
	*    	 The port PB3 could not be used when this is activated.
	*/
void DBG_Init(void);

void DBG_InitTimestamp(DBG_TimestampFreq dbg_timestampFreq);

void DBG_PrintTimestamp(void);

#endif // STM32F103_DBG_H
