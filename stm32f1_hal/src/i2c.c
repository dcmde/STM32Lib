#include "i2c.h"

// For more details on the STM32F103 I2C have a look at the reference manuel page 759.

ERROR_t I2C_Initialization(I2C_t *i2c) {

    // Init variables.
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_TypeDef *I2C_Periph;
    GPIO_TypeDef *I2C_GPIO;
    uint16_t I2C_PinSDA, I2C_PinSCL;
    uint32_t I2C_RCC_Periph, I2C_RCC_GPIO;

    if (i2c->I2Cx == I2C1) {
        I2C_Periph = I2C1;
        I2C_PinSDA = GPIO_Pin_7;
        I2C_PinSCL = GPIO_Pin_6;
        I2C_GPIO = GPIOB;
        I2C_RCC_Periph = RCC_APB1Periph_I2C1;
        I2C_RCC_GPIO = RCC_APB2Periph_GPIOB;
    } else if (i2c->I2Cx == I2C2) {
        I2C_Periph = I2C2;
        I2C_PinSDA = GPIO_Pin_11;
        I2C_PinSCL = GPIO_Pin_10;
        I2C_GPIO = GPIOB;
        I2C_RCC_Periph = RCC_APB1Periph_I2C2;
        I2C_RCC_GPIO = RCC_APB2Periph_GPIOB;
    } else {
        return ERROR_BAD_DEVICE;
    }

    // I2C configuration
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = i2c->own_dev_addr;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = i2c->speed;

    // Enable I2C and GPIOB clocks.
    RCC_APB1PeriphClockCmd(I2C_RCC_Periph, ENABLE);
    RCC_APB2PeriphClockCmd(I2C_RCC_GPIO, ENABLE);

    // Configure I2C pins: SCL and SDA
    GPIO_InitStructure.GPIO_Pin = I2C_PinSCL | I2C_PinSDA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);

    // Apply I2C configuration.
    I2C_Init(I2C_Periph, &I2C_InitStructure);

    // I2C Peripheral Enable
    I2C_Cmd(I2C_Periph, ENABLE);
    return NO_ERROR;
}

ERROR_t I2C_Write(I2C_t *i2c, uint8_t *buffer, uint8_t size) {
    uint8_t dev_address = i2c->dev_addr;
    uint8_t stop_flag = i2c->stop_flag;
    I2C_TypeDef *I2C_Periph = i2c->I2Cx;

    // Shift the address. Needed because the bit 0 is for R/W.
    dev_address <<= 1;

    while (I2C_GetFlagStatus(I2C_Periph, I2C_FLAG_BUSY));

    // Send START condition.
    I2C_GenerateSTART(I2C_Periph, ENABLE);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_MODE_SELECT));

    // Send the device address for write.
    I2C_Send7bitAddress(I2C_Periph, dev_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send register address to write the value into device.
    I2C_Cmd(I2C_Periph, ENABLE);

    for (uint8_t i = 0; i < size; ++i) {
        // Send data and wait for acknowledge.
        I2C_SendData(I2C_Periph, buffer[i]);
        while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    if (stop_flag != 0) {
        // Send stop condition.
        I2C_GenerateSTOP(I2C_Periph, ENABLE);
    }
    return NO_ERROR;
}

ERROR_t I2C_WriteByte(I2C_t *i2c, const uint8_t reg_address, const uint8_t reg_value) {
    I2C_TypeDef *I2C_Periph = i2c->I2Cx;
    uint8_t dev_address = i2c->dev_addr;

    // Shift the address. Needed because the bit 0 is for R/W.
    dev_address <<= 1;

    while (I2C_GetFlagStatus(I2C_Periph, I2C_FLAG_BUSY));

    // Send START condition.
    I2C_GenerateSTART(I2C_Periph, ENABLE);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_MODE_SELECT));

    // Send the device address for write.
    I2C_Send7bitAddress(I2C_Periph, dev_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send register address to write the value into device.
    I2C_Cmd(I2C_Periph, ENABLE);
    I2C_SendData(I2C_Periph, reg_address);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send register value and send stop condition.
    I2C_SendData(I2C_Periph, reg_value);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send stop condition.
    I2C_GenerateSTOP(I2C_Periph, ENABLE);
    return NO_ERROR;
}

ERROR_t I2C_WriteBuffer(I2C_t *i2c, uint8_t reg_address, uint8_t *buffer, uint8_t size) {
    I2C_TypeDef *I2C_Periph = i2c->I2Cx;
    uint8_t dev_address = i2c->dev_addr;

    // Shift the address. Needed because the bit 0 is for R/W.
    dev_address <<= 1;

    while (I2C_GetFlagStatus(I2C_Periph, I2C_FLAG_BUSY));

    // Send START condition.
    I2C_GenerateSTART(I2C_Periph, ENABLE);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_MODE_SELECT));

    // Send the device address for write.
    I2C_Send7bitAddress(I2C_Periph, dev_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send register address to write the value into device.
    I2C_Cmd(I2C_Periph, ENABLE);
    I2C_SendData(I2C_Periph, reg_address);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    for (uint8_t i = 0; i < size; ++i) {
        // Send data and wait for acknowledge.
        I2C_SendData(I2C_Periph, buffer[i]);
        while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    // Send stop condition.
    I2C_GenerateSTOP(I2C_Periph, ENABLE);
    return NO_ERROR;
}

ERROR_t I2C_Read(I2C_t *i2c, uint8_t *buffer, uint8_t num_bytes_read) {
    I2C_TypeDef *I2C_Periph = i2c->I2Cx;
    uint8_t dev_address = i2c->dev_addr;

    // Shift the address. Needed because the bit 0 is for R/W.
    dev_address <<= 1;

    // Send START condition to read.
    I2C_GenerateSTART(I2C_Periph, ENABLE);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_MODE_SELECT));

    // Send the device address for write.
    I2C_Send7bitAddress(I2C_Periph, dev_address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // While the total number of byte was not read.
    while (num_bytes_read--) {

        // If the last byte is going to be read.
        if (num_bytes_read < 1) {
            // Send NACK and stop condition.
            I2C_AcknowledgeConfig(I2C_Periph, DISABLE);
            I2C_GenerateSTOP(I2C_Periph, ENABLE);
        }

        // Get the incoming data and put them into the array.
        while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_RECEIVED));
        *buffer++ = I2C_ReceiveData(I2C_Periph);
    }

    // Enable ACK for other reception.
    I2C_AcknowledgeConfig(I2C_Periph, ENABLE);
    return NO_ERROR;
}

uint8_t I2C_ReadByte(I2C_t *i2c, const uint8_t reg_address) {
    I2C_TypeDef *I2C_Periph = i2c->I2Cx;
    uint8_t dev_address = i2c->dev_addr;
    uint8_t value;

    // Shift the address. Needed because the bit 0 is for R/W.
    dev_address <<= 1;

    while (I2C_GetFlagStatus(I2C_Periph, I2C_FLAG_BUSY));

    // Send START condition to write the register address to read.
    I2C_GenerateSTART(I2C_Periph, ENABLE);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_MODE_SELECT));

    // Send the device address for write.
    I2C_Send7bitAddress(I2C_Periph, dev_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send register address to write the value into device.
    I2C_Cmd(I2C_Periph, ENABLE);
    I2C_SendData(I2C_Periph, reg_address);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send START condition to read the register.
    I2C_GenerateSTART(I2C_Periph, ENABLE);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_MODE_SELECT));

    // Send the device address for read.
    I2C_Send7bitAddress(I2C_Periph, dev_address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // Send NACK and stop condition.
    I2C_AcknowledgeConfig(I2C_Periph, DISABLE);
    I2C_GenerateSTOP(I2C_Periph, ENABLE);

    // Get the incoming data.
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_RECEIVED));
    value = I2C_ReceiveData(I2C_Periph);

    // Enable ACK for other reception.
    I2C_AcknowledgeConfig(I2C_Periph, ENABLE);
    return value;
}

ERROR_t I2C_ReadBuffer(I2C_t *i2c, const uint8_t reg_address, uint8_t *buffer, uint8_t num_bytes_read) {
    I2C_TypeDef *I2C_Periph = i2c->I2Cx;
    uint8_t dev_address = i2c->dev_addr;

    // Shift the address. Needed because the bit 0 is for R/W.
    dev_address <<= 1;

    while (I2C_GetFlagStatus(I2C_Periph, I2C_FLAG_BUSY));

    // Send START condition to write the register address to read.
    I2C_GenerateSTART(I2C_Periph, ENABLE);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_MODE_SELECT));

    // Send the device address for write.
    I2C_Send7bitAddress(I2C_Periph, dev_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send register address to write the value into device.
    I2C_Cmd(I2C_Periph, ENABLE);
    I2C_SendData(I2C_Periph, reg_address);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send START condition to read the register.
    I2C_GenerateSTART(I2C_Periph, ENABLE);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_MODE_SELECT));

    // Send the device address for read.
    I2C_Send7bitAddress(I2C_Periph, dev_address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // While the total number of byte was not read.
    while (num_bytes_read--) {

        // If the last byte is going to be read.
        if (num_bytes_read < 1) {
            // Send NACK and stop condition.
            I2C_AcknowledgeConfig(I2C_Periph, DISABLE);
            I2C_GenerateSTOP(I2C_Periph, ENABLE);
        }

        // Get the incoming data and put them into the array.
        while (!I2C_CheckEvent(I2C_Periph, I2C_EVENT_MASTER_BYTE_RECEIVED));
        *buffer++ = I2C_ReceiveData(I2C_Periph);
    }

    // Enable ACK for other reception.
    I2C_AcknowledgeConfig(I2C_Periph, ENABLE);
    return NO_ERROR;
}