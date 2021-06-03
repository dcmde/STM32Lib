#ifndef I2C_F103
#define I2C_F103

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "error_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_OWN_ADDR    0x02

ERROR_t I2C_Initilazation(uint8_t i2c, uint8_t own_address, uint32_t speed);

ERROR_t I2C1_Initilazation(uint32_t speed);

ERROR_t I2C2_Initilazation(uint32_t speed);

ERROR_t I2C_Write(uint8_t i2c, uint8_t dev_address, uint8_t stop_flag, uint8_t *buffer, uint8_t size);

ERROR_t I2C1_Write(uint8_t dev_address, uint8_t stop_flag, uint8_t *buffer, uint8_t size);

ERROR_t I2C2_Write(uint8_t dev_address, uint8_t stop_flag, uint8_t *buffer, uint8_t size);

ERROR_t I2C_Read(uint8_t i2c, uint8_t dev_address, uint8_t *buffer, uint8_t num_bytes_read);

ERROR_t I2C1_Read(uint8_t dev_address, uint8_t *buffer, uint8_t num_bytes_read);

ERROR_t I2C2_Read(uint8_t dev_address, uint8_t *buffer, uint8_t num_bytes_read);

ERROR_t I2C_WriteByte(uint8_t i2c, uint8_t dev_address, uint8_t reg_address, uint8_t reg_value);

ERROR_t I2C1_WriteByte(uint8_t dev_address, uint8_t reg_address, uint8_t reg_value);

ERROR_t I2C2_WriteByte(uint8_t dev_address, uint8_t reg_address, uint8_t reg_value);

ERROR_t I2C_WriteBuffer(uint8_t i2c, uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t size);

ERROR_t I2C1_WriteBuffer(uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t size);

ERROR_t I2C2_WriteBuffer(uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t size);

ERROR_t I2C_ReadBuffer(uint8_t i2c, uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t num_bytes_read);

ERROR_t I2C1_ReadBuffer(uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t num_bytes_read);

ERROR_t I2C2_ReadBuffer(uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t num_bytes_read);

#ifdef __cplusplus
}
#endif

#endif // I2C_F103
