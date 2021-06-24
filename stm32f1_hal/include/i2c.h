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

typedef struct {
    uint8_t own_dev_addr;
    uint8_t stop_flag;
    I2C_TypeDef *I2Cx;
    uint32_t speed;
} I2C_t;

ERROR_t I2C_Initialization(I2C_t *i2c);

ERROR_t I2C_Write(I2C_t *i2c, uint8_t dev_addr, uint8_t *buffer, uint8_t size);

ERROR_t I2C_WriteByte(I2C_t *i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t reg_value);

ERROR_t I2C_WriteBuffer(I2C_t *i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t size);

ERROR_t I2C_Read(I2C_t *i2c, uint8_t dev_addr, uint8_t *buffer, uint8_t num_bytes_read);

uint8_t I2C_ReadByte(I2C_t *i2c, uint8_t dev_addr, uint8_t reg_address);

ERROR_t I2C_ReadBuffer(I2C_t *i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif // I2C_F103
