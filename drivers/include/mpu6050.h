/**
  * @file    mpu6050.h
  * @author  Couet David
  * @version V1.0
  * @date    2-August-2019
  * @brief   IMU mpu6050 driver.
	*/

#ifndef MPU_6050_H
#define MPU_6050_H

/*--------------------------------- Includes ---------------------------------*/
#include <error_type.h>
#include <stdint.h>
#include <math.h>

/*--------------------------------- Defines ----------------------------------*/


/**
  * @defgroup Registers_addresses
  * @{
  */
#define MPU6050_SELF_TEST_X 		0x0D
#define MPU6050_SELF_TEST_Y 		0x0E
#define MPU6050_SELF_TEST_Z 		0x0F
#define MPU6050_SELF_TEST_A			0x10

#define MPU6050_SMPLRT_DIV			0x19
#define MPU6050_CONFIG					0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_FIFO_EN					0x23

#define MPU6050_I2C_MST_CTRL		0x24
#define MPU6050_I2C_SLV0_ADDR		0x25
#define MPU6050_I2C_SLV0_REG		0x26
#define MPU6050_I2C_SLV0_CTRL		0x27
#define MPU6050_I2C_SLV1_ADDR		0x28
#define MPU6050_I2C_SLV1_REG		0x29
#define MPU6050_I2C_SLV1_CTRL		0x2A
#define MPU6050_I2C_SLV2_ADDR		0x2B
#define MPU6050_I2C_SLV2_REG		0x2C
#define MPU6050_I2C_SLV2_CTRL		0x2D
#define MPU6050_I2C_SLV3_ADDR		0x2E
#define MPU6050_I2C_SLV3_REG		0x2F
#define MPU6050_I2C_SLV3_CTRL		0x30
#define MPU6050_I2C_SLV4_ADDR		0x31
#define MPU6050_I2C_SLV4_REG		0x32
#define MPU6050_I2C_SLV4_DO			0x33
#define MPU6050_I2C_SLV4_CTRL		0x34
#define MPU6050_I2C_SLV4_DI			0x35
#define MPU6050_I2C_MST_STATUS	0x36

#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A

#define MPU6050_ACCEL_XOUT_H		0x3B
#define	MPU6050_ACCEL_XOUT_L 		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40

#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42

#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48

#define MPU6050_I2C_MST_DELAY_CTRL	0x67
#define MPU6050_SIGNAL_PATH_RESET		0x68
#define MPU6050_USER_CTRL						0x6A

#define MPU6050_PWR_MGMT_1					0x6B
#define MPU6050_PWR_MGMT_2					0x6C

#define MPU6050_FIFO_COUNTH					0x72
#define MPU6050_FIFO_COUNTL					0x73
#define MPU6050_FIFO_R_W						0x74
#define MPU6050_WHO_AM_I						0x75
/**
  * @}
  */

// MPU6050 i2c address.
#define MPU6050_I2C_ADDRESS 	0x68

/*--------------------------------- Enums ------------------------------------*/

/**
  * @defgroup Registers_configuration
  * @{
  */

/**
  * @}
  */

typedef enum{
    MPU6050_I2C_ADDR_0x68 = 0x68,   /* AD0 logic low */
    MPU6050_I2C_ADDR_0x69 = 0x69    /* AD0 logic high */
}MPU6050_I2C_ADDR;

/*--------------------------------- Structures -------------------------------*/

typedef struct {

}MPU6050_InitTypedef;

typedef struct {
    MPU6050_InitTypedef* InitStruct;
    ERROR_t (*ReadBuffer)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, uint8_t size);
    ERROR_t (*WriteBuffer)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, uint8_t size);
    MPU6050_I2C_ADDR device_address;
    int16_t* accel;
    int16_t* gyro;
}MPU6050_t;

typedef struct {
    float r;
    float theta;
    float phi;
}MPU6050_SphericalParam;

/*--------------------------- Functions prototype ----------------------------*/

ERROR_t MPU6050_Init(MPU6050_t* MPU6050);
ERROR_t MPU6050_ReadAccel(MPU6050_t* MPU6050);
ERROR_t MPU6050_ReadGyro(MPU6050_t* MPU6050);
/**
  * @brief Compute the spherical parameters of the acceleration according the acceleration
  *        values along each axis.
  */
ERROR_t MPU6050_AccelSphericalParam(MPU6050_t* MPU6050, MPU6050_SphericalParam* SphericalP);
#endif // MPU_6050_H
