/**
  * @file    BMP280.h
  * @author  Couet David
  * @version V1.0
  * @date    24-April-2019
  * @brief   Pressure sensor BMP280 driver.
	*/
	
#ifndef BMP280_H
#define BMP280_H

/*--------------------------------- Includes ---------------------------------*/

#include <error_type.h>
#include <stdint.h>
#include <stdlib.h>
#include <stm32f10x_conf.h>

/*--------------------------------- Defines ----------------------------------*/

/**
  * @brief Registers addresses of the device.
  */
#define BMP280_ID				0xD0
#define BMP280_RESET			0xE0
#define BMP280_CTRL_HUM		0xF2
#define BMP280_STATUS			0xF3
#define BMP280_CTRL_MES		0xF4
#define BMP280_CONFIG			0xF5
#define BMP280_PRESS_MSB	0xF7
#define BMP280_PRESS_LSB	0xF8
#define BMP280_PRESS_XLSB	0xF9
#define BMP280_TEMP_MSB		0xFA
#define BMP280_TEMP_LSB		0xFB
#define BMP280_TEMP_XLSB	0xFC
#define BMP280_HUM_MSB		0xFD
#define BMP280_HUM_LSB		0xFE
/**
 * @brief Starting address of compensation parameters registers.
 *				 The type and the length is specifeid.
 */
#define BMP280_DIG_T1			0x88		// unsigned short
#define BMP280_DIG_T2			0x8A		// signed short
#define BMP280_DIG_T3			0x8C		// signed short

#define BMP280_DIG_P1			0x8E		// unsigned short
#define BMP280_DIG_P2			0x90		// signed short
#define BMP280_DIG_P3			0x92		// signed short
#define BMP280_DIG_P4			0x94		// signed short
#define BMP280_DIG_P5			0x96		// signed short
#define BMP280_DIG_P6			0x98		// signed short
#define BMP280_DIG_P7			0x9A		// signed short
#define BMP280_DIG_P8			0x9C		// signed short
#define BMP280_DIG_P9			0x9E		// signed short

#define BMP280_DIG_H1			0xA1		// unsigned char
#define BMP280_DIG_H2			0xE1		// signed short
#define BMP280_DIG_H3			0xE3		// unsigned char
#define BMP280_DIG_H4			0xE4		// signed short
#define BMP280_DIG_H5			0xE5		// signed short
#define BMP280_DIG_H6			0xE7		// signed char

/**
  * @brief Check function input parameters.
	*/

#define BMP280_IS_MODE(x)   ((x == MODE_SLEEP) || (x == MODE_FORCE) || (x == MODE_NORMAL))

#define BMP280_IS_OVS_T(x)  ((x == OVS_T_0) || \
														 (x == OVS_T_1) || \
														 (x == OVS_T_2) || \
														 (x == OVS_T_4) || \
														 (x == OVS_T_8) || \
														 (x == OVS_T_16))
														 
#define BMP280_IS_OVS_P(x)  ((x == OVS_P_0) || \
														 (x == OVS_P_1) || \
														 (x == OVS_P_2) || \
														 (x == OVS_P_4) || \
														 (x == OVS_P_8) || \
														 (x == OVS_P_16))

/**
  * @brief Default ID of the device. Used to check that the device is present.
	*				 Id of mass production device. Other possibilites are 0x56 or 0x57.
	*/
#define BMP280_DEFAULT_ID		0x58

/*--------------------------------- Enums ------------------------------------*/

/**
  * @brief Sensor mode of the device.
	*/
typedef enum{
	MODE_SLEEP = 0x00,
	MODE_FORCE = 0x01,
	MODE_NORMAL = 0x03
} BMP280_MODE;

/**
  * @brief Oversampling configuration of the pressure.
	*/
typedef enum{
	OVS_P_0 = 0x00,
	OVS_P_1 = 0x04,
	OVS_P_2 = 0x08, 
	OVS_P_4 = 0x0C,
	OVS_P_8 = 0x10,
	OVS_P_16 = 0x14
} BMP280_OVS_P;

/**
  * @brief Oversampling configuration of the temperature.
	*/
typedef enum{
	OVS_T_0 = 0x00,
	OVS_T_1 = 0x20,
	OVS_T_2 = 0x40,
	OVS_T_4 = 0x60,
	OVS_T_8 = 0x80,
	OVS_T_16 = 0xA0
} BMP280_OVS_T;

/*--------------------------------- Structure --------------------------------*/

/**
  * @brief Configuration structure of the device.
	*/
typedef struct {
	BMP280_MODE MODE;
	BMP280_OVS_T OVS_T;
	BMP280_OVS_P OVS_P;
} BMP280_InitTypeDef;

/**
  * @brief Contain all the compensation parameters of the device.
	*/
typedef struct{
	uint16_t DIG_T1;
	int16_t DIG_T2;
	int16_t DIG_T3;
	
	uint16_t DIG_P1;
	int16_t DIG_P2;
	int16_t DIG_P3;
	int16_t DIG_P4;
	int16_t DIG_P5;
	int16_t DIG_P6;
	int16_t DIG_P7;
	int16_t DIG_P8;
	int16_t DIG_P9;
	
	uint8_t DIG_H1;
	int16_t DIG_H2;
	uint8_t DIG_H3;
	int16_t DIG_H4;
	int16_t DIG_H5;
	int8_t DIG_H6;
} BMP280_CompParam;

typedef struct{
	int32_t Temp;
	uint32_t Pres;
} BMP280_SensorValues;

typedef struct{
	BMP280_InitTypeDef* InitStruct;
    BMP280_CompParam* CompParamStruct;	/* Stores all the compensation parameters of the device. */
    ERROR_t (*WriteBuffer)(uint8_t address, uint8_t reg_addr, uint8_t* buffer, uint8_t size);
    ERROR_t (*ReadBuffer)(uint8_t address, uint8_t reg_addr, uint8_t* buffer, uint8_t size);
	uint8_t address;	/* When SDO to Vcc 0x77, SDO to GND 0x76 */
	int32_t adc_T;		/* Stores the raw value of the device */
	int32_t adc_P;		/* Stores the raw value of the device */
	int32_t t_fine;		/* Variable used to compensate the pressure calculation */
} BMP280_t;

/*--------------------------- Functions prototype ----------------------------*/

/**
 * @about Initialize the BMP280. The BMP280_t structure has to be configured properly by giving valid pointers to variables.
 *        The address can be 0x77 or 0x76.
 * @param BMP280 structure. Should be properly initialized before.
 * @return Error state, ERROR_NULL_POINTER, BAD_DEVICE, NO_ERROR
 */
ERROR_t BMP280_Init(BMP280_t* BMP280);

/**
 * @about Read the values from the adc of the device.
 * @param BMP280 structure.
 * @param BMP280_Sensors structure to get the values
 */
void BMP280_GetTempPress(BMP280_t* BMP280, BMP280_SensorValues* BMP280_Values);
ERROR_t BMP280_ReadConfigurationParameters(BMP280_t* BMP280);

uint8_t BMP280_ReadRegister(BMP280_t* BMP280, uint8_t register_address);
ERROR_t BMP280_ReadRegisters(BMP280_t* BMP280, uint8_t register_address, uint8_t* buffer, uint8_t size);
ERROR_t BMP280_WriteRegister(BMP280_t* BMP280, uint8_t register_address, uint8_t value);

#endif // BMP280_H
