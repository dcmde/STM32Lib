/**
  * @file    bme280.h
  * @author  Couet David
  * @version V1.0
  * @date    24-April-2019
  * @brief   Pressure sensor BME280 driver.
	*/

#ifndef BME280_H
#define BME280_H

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------- Includes ---------------------------------*/

#include "stdint.h"

/*--------------------------------- Defines ----------------------------------*/

/**
  * @brief Registers addresses of the device.
	*/
#define BME280_ID                    0xD0
#define BME280_RESET            0xE0
#define BME280_CTRL_HUM        0xF2
#define BME280_STATUS            0xF3
#define BME280_CTRL_MES        0xF4
#define BME280_CONFIG            0xF5
#define BME280_PRESS_MSB    0xF7
#define BME280_PRESS_LSB    0xF8
#define BME280_PRESS_XLSB    0xF9
#define BME280_TEMP_MSB        0xFA
#define BME280_TEMP_LSB        0xFB
#define BME280_TEMP_XLSB    0xFC
#define BME280_HUM_MSB        0xFD
#define BME280_HUM_LSB        0xFE
/**
  * @brief Starting address of compensation parameters registers.
	*				 The type and the length is specifeid.
	*/
#define BME280_DIG_T1            0x88        // unsigned short
#define BME280_DIG_T2            0x8A        // signed short
#define BME280_DIG_T3            0x8C        // signed short

#define BME280_DIG_P1            0x8E        // unsigned short
#define BME280_DIG_P2            0x90        // signed short
#define BME280_DIG_P3            0x92        // signed short
#define BME280_DIG_P4            0x94        // signed short
#define BME280_DIG_P5            0x96        // signed short
#define BME280_DIG_P6            0x98        // signed short
#define BME280_DIG_P7            0x9A        // signed short
#define BME280_DIG_P8            0x9C        // signed short
#define BME280_DIG_P9            0x9E        // signed short

#define BME280_DIG_H1            0xA1        // unsigned char
#define BME280_DIG_H2            0xE1        // signed short
#define BME280_DIG_H3            0xE3        // unsigned char
#define BME280_DIG_H4            0xE4        // signed short
#define BME280_DIG_H5            0xE5        // signed short
#define BME280_DIG_H6            0xE7        // signed char

/**
  * @brief Check function input parameters.
	*/
#define BME280_IS_OVS_T(x)  ((x == OVS_T_0) || \
                                                         (x == OVS_T_1) || \
                                                         (x == OVS_T_2) || \
                                                         (x == OVS_T_4) || \
                                                         (x == OVS_T_8) || \
                                                         (x == OVS_T_16))

#define BME280_IS_OVS_P(x)  ((x == OVS_P_0) || \
                                                         (x == OVS_P_1) || \
                                                         (x == OVS_P_2) || \
                                                         (x == OVS_P_4) || \
                                                         (x == OVS_P_8) || \
                                                         (x == OVS_P_16))

#define BME280_IS_OVS_H(x)  ((x == OVS_H_0) || \
                                                         (x == OVS_H_1) || \
                                                         (x == OVS_H_2) || \
                                                         (x == OVS_H_4) || \
                                                         (x == OVS_H_8) || \
                                                         (x == OVS_H_16))
/**
  * @brief Default ID of the device. Used to check that the device is present.
	*				 Id of mass production device 0x58. Other possibilities are 0x56, 0x57 or 0x60.
	*/
#define BME280_DEFAULT_ID        0x58

/*--------------------------------- Enums ------------------------------------*/

/**
  * @brief Sensor mode of the device.
	*/
typedef enum {
    MODE_SLEEP = 0x00,
    MODE_FORCE = 0x01,
    MODE_NORMAL = 0x03
} BME280_MODE;

/**
  * @brief Oversampling configuration of the pressure.
	*/
typedef enum {
    OVS_P_0 = 0x00,
    OVS_P_1 = 0x04,
    OVS_P_2 = 0x08,
    OVS_P_4 = 0x0C,
    OVS_P_8 = 0x10,
    OVS_P_16 = 0x14
} BME280_OVS_P;

/**
  * @brief Oversampling configuration of the temperature.
	*/
typedef enum {
    OVS_T_0 = 0x00,
    OVS_T_1 = 0x20,
    OVS_T_2 = 0x40,
    OVS_T_4 = 0x60,
    OVS_T_8 = 0x80,
    OVS_T_16 = 0xA0
} BME280_OVS_T;

/**
  * @brief Oversampling configuration of the humidity.
	*/
typedef enum {
    OVS_H_0 = 0x00,
    OVS_H_1 = 0x01,
    OVS_H_2 = 0x02,
    OVS_H_4 = 0x03,
    OVS_H_8 = 0x04,
    OVS_H_16 = 0x05
} BME280_OVS_H;

/*--------------------------------- Structure --------------------------------*/

/**
  * @brief Configuration structure of the device.
	*/
typedef struct {
    BME280_MODE MODE;
    BME280_OVS_H OVS_H;
    BME280_OVS_T OVS_T;
    BME280_OVS_P OVS_P;
} BME280_InitTypeDef;

/**
  * @brief Contain all the compensation parameters of the device.
	*/
typedef struct {
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
} BME280_CompParam;

typedef struct {
    int32_t Temp;
    uint32_t Pres;
    uint32_t Hum;
} BME280_SensorsValues;

typedef struct {
    uint8_t dev_address;
    BME280_InitTypeDef initStruct;
    BME280_CompParam compParam;
    BME280_SensorsValues sensorsValues;

    uint8_t (*ReadRegister)(uint8_t register_address);

    void (*ReadRegisters)(uint8_t register_address, uint8_t *buffer, uint8_t size);

    void (*WriteRegister)(uint8_t register_address, uint8_t value);
} BME280_t;

/*--------------------------- Functions prototype ----------------------------*/

uint8_t BME280_Init(BME280_t *bme280);

void BME280_ReadConfigurationParameters(BME280_t *bme280);

void BME280_ReadSensors(BME280_t *bme280);

#ifdef __cplusplus
}
#endif

#endif // BME280_H
