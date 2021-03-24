/**
  * @file    TSL2561.h
  * @author  Couet David
  * @version V1.0
  * @date    2-July-2019
  * @brief   Luminosity sensor TSL2561 driver. Based on the TSL2561 datasheet.
	*/
	
#ifndef TSL2561_H
#define TSL2561_H

/*--------------------------------- Includes ---------------------------------*/
#include "stdint.h"
#include "error_type.h"

/*--------------------------------- Defines ----------------------------------*/

/** 
	* @defgroup Registers_addresses 
  * @{
  */
#define TSL2561_CONTROL						0x00
#define TSL2561_TIMING						0x01
#define TSL2561_THRESHOLD_L_LOW		0x02
#define TSL2561_THRESHOLD_L_HIGH	0x03
#define TSL2561_THRESHOLD_H_LOW		0x04
#define TSL2561_THRESHOLD_H_HIGH	0x05
#define TSL2561_INTERRUPT					0x06
#define TSL2561_CRC								0x08
#define TSL2561_ID								0x0A
#define TSL2561_DATA_0_LOW				0x0C
#define TSL2561_DATA_0_HIGH				0x0D
#define TSL2561_DATA_1_LOW				0x0E
#define TSL2561_DATA_1_HIGH				0x0F
/**
  * @}
  */

/*--------------------------------- Enums ------------------------------------*/

/** 
  * @defgroup Registers_configuration
  * @{
  */

/**
  * @enum TSL2561_POWER
	* @biref Power the device up and down.
	* @note	If a 0x03 is written, the value returned during a read cycle will be 0x03.
	*/
typedef enum {
	TSL2561_ON = 0x03,
	TSL2561_OFF = 0x00
} TSL2561_POWER;

/**
  * @enum TSL2561_GAIN
	* @brief Select the gain.
	*/
typedef enum {
	TSL2561_GAIN_1 = 0x00,
	TSL2561_GAIN_16 = 0x10
} TSL2561_GAIN;

/**
  * @enum TSL2561_MANUAL
	* @brief Control integration in manual mode.
	*/
typedef enum {
	TSL2561_MANUAL_ON = 0x08,
	TSL2561_MANUAL_OFF = 0
} TSL2561_MANUAL;

/**
  * @enum TSL2561_INTEGRATION_TIME
	* @brief Selects the integration time for each conversion.
	* @note If TSL2561_INTEGRATION_MANUAL is selected then TSL2561_MANUAL is 
  *				responsible to control the integration time.
	*/
typedef enum {
	TSL2561_INTEGRATION_13_MS_7 = 0x00,
	TSL2561_INTEGRATION_101_MS = 0x01,
	TSL2561_INTEGRATION_402_MS = 0x02,
	TSL2561_INTEGRATION_MANUAL = 0x03
} TSL2561_INTEGRATION_TIME;

/**
  * @enum TSL2561_INTR;
	* @brief Control the interruption type.
	*/
typedef enum {
	TSL2561_OUTPUT_DISABLED = 0x00,
	TSL2561_LEVEL = 0x10,
	TSL2561_SMB_ALERT = 0x20,
	TSL2561_TEST_MODE = 0x30
} TSL2561_INTR;

/**
  * @enum TSL2561_PERSIST
	* @brief Control the interrupt persistance.
	*/
typedef enum {
	TSL2561_EVERY_ADC_CONV,
	TSL2561_OUTSIDE_THRESHOLD,
	TSL2561_2_INTEGRETION,
	TSL2561_3_INTEGRETION,
	TSL2561_4_INTEGRETION,
	TSL2561_5_INTEGRETION,
	TSL2561_6_INTEGRETION,
	TSL2561_7_INTEGRETION,
	TSL2561_8_INTEGRETION,
	TSL2561_9_INTEGRETION,
	TSL2561_10_INTEGRETION,
	TSL2561_11_INTEGRETION,
	TSL2561_12_INTEGRETION,
	TSL2561_13_INTEGRETION,
	TSL2561_14_INTEGRETION,
	TSL2561_15_INTEGRETION
} TSL2561_PERSIST;

/**
  * @}
  */

/*--------------------------------- Structures -------------------------------*/
/**
  * @struct TSL2561_InitTypedef
	* @brief Structure containing register values for device initiatisation.
	*/
typedef struct {
	TSL2561_POWER POWER;
	TSL2561_GAIN GAIN;
	TSL2561_INTEGRATION_TIME INTEGRATION_TIME;
	TSL2561_INTR INTR;
	TSL2561_PERSIST PERSIST;
	uint16_t THRESHOLD_CH1;
	uint16_t THRESHOLD_CH2;
} TSL2561_InitTypedef;

/**
	* @struct TSL2561_t
	* @brief Structure defining an TSL2561 object.
	*/
typedef struct {
	TSL2561_InitTypedef* InitStruct;
	ERROR_t (*ReadBuffer)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, uint8_t size);
	ERROR_t (*WriteBuffer)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, uint8_t size);
	uint8_t device_address;
	uint16_t ChannelVisibleIR;
	uint16_t ChannelIR;
	uint16_t lux;
} TSL2561_t;

/*--------------------------- Functions prototype ----------------------------*/
/**
  * @about Initialize a @TSL2561_t object.
  * @brief Initialize the device according to the @TSL2561_InitTypedef
  *        Everything has to be configured by hand as it will override the default registers values on the device.
  *		   As a future improvement a new function can be done setting the device in a standard mode.
  * @param A pointer to a @TSL2561_t variable.
  * @retval Error encountered.
  */
ERROR_t TSL2561_Init(TSL2561_t* TSL2561);

/**
  * @about Read the light intensity given by the device.
  * @brief Two registers giving the light intensity need to be read and then the lux has to be computed. This is 
  *        no done yet.
  * @param A pointer to a @TSL2561_t variable.
  * @retval Error encountered.
  */
ERROR_t TSL2561_Read(TSL2561_t* TSL2561);

/**
  * @about Read a specify register or a succession of registers of the device.
  * @brief As the protocol is partially using the SMBus protocol. All the communication protocol logic for reading 
  *        registers is in this function.
  * @param A pointer to a @TSL2561_t variable.
  * @param The register address to be read.
  * @param A pointer to an array.
  * @param The number of byte to read. (Has to be at most as big as the buffer size.)
  * @retval Error encountered.
  */
ERROR_t TSL2561_ReadRegisters(TSL2561_t* TSL2561, uint8_t reg_addr, uint8_t* buffer, uint8_t size);

/**
  * @about Write a specify register or a succession of registers of the device.
  * @brief As the protocol is partially using the SMBus protocol. All the communication protocol logic for writing
  *        registers is in this function.
  * @param A pointer to a @TSL2561_t variable.
  * @param First register address to write.
  * @param A pointer to an array containing all desired values.
  * @param The number of byte/registers to write.
  */
ERROR_t TSL2561_WriteRegisters(TSL2561_t* TSL2561, uint8_t reg_addr, uint8_t* buffer, uint8_t size);

#endif // TSL2561_H
