#include "i2c.h"

// For more details on the STM32F103 I2C have a look at the reference manuel page 759.

ERROR_t I2C_Initialization(const uint8_t i2c, const uint8_t own_address, const uint32_t speed) {

    // Init variables.
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_TypeDef *I2C_Periph;
    GPIO_TypeDef *I2C_GPIO;
    uint16_t I2C_PinSDA, I2C_PinSCL;
    uint32_t I2C_RCC_Periph, I2C_RCC_GPIO;

    switch (i2c) {
        case 1:
            I2C_Periph = I2C1;
            I2C_PinSDA = GPIO_Pin_7;
            I2C_PinSCL = GPIO_Pin_6;
            I2C_GPIO = GPIOB;
            I2C_RCC_Periph = RCC_APB1Periph_I2C1;
            I2C_RCC_GPIO = RCC_APB2Periph_GPIOB;
            break;
        case 2:
            I2C_Periph = I2C2;
            I2C_PinSDA = GPIO_Pin_11;
            I2C_PinSCL = GPIO_Pin_10;
            I2C_GPIO = GPIOB;
            I2C_RCC_Periph = RCC_APB1Periph_I2C2;
            I2C_RCC_GPIO = RCC_APB2Periph_GPIOB;
            break;
        default:
            return ERROR_BAD_DEVICE;
    }

    // I2C configuration
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = own_address;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = speed;

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

ERROR_t I2C1_Initialization(const uint32_t speed) {
    return I2C_Initilazation(1, I2C_OWN_ADDR, speed);
}

ERROR_t I2C2_Initialization(const uint32_t speed) {
    return I2C_Initilazation(2, I2C_OWN_ADDR, speed);
}

ERROR_t I2C_Write(uint8_t i2c, uint8_t dev_address, uint8_t stop_flag, uint8_t *buffer, uint8_t size) {

    I2C_TypeDef *I2C_Periph;

    switch (i2c) {
        case 1:
            I2C_Periph = I2C1;
            break;
        case 2:
            I2C_Periph = I2C2;
            break;
        default:
            return ERROR_BAD_DEVICE;
    }

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

ERROR_t I2C1_Write(uint8_t dev_address, uint8_t stop_flag, uint8_t *buffer, uint8_t size) {
    return I2C_Write(1, dev_address, stop_flag, buffer, size);
}

ERROR_t I2C2_Write(uint8_t dev_address, uint8_t stop_flag, uint8_t *buffer, uint8_t size) {
    return I2C_Write(2, dev_address, stop_flag, buffer, size);
}

ERROR_t I2C_WriteByte(const uint8_t i2c, uint8_t dev_address, const uint8_t reg_address, const uint8_t reg_value) {

    I2C_TypeDef *I2C_Periph;

    switch (i2c) {
        case 1:
            I2C_Periph = I2C1;
            break;
        case 2:
            I2C_Periph = I2C2;
            break;
        default:
            return ERROR_BAD_DEVICE;
    }

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

ERROR_t I2C_WriteBuffer(const uint8_t i2c, uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t size) {

    I2C_TypeDef *I2C_Periph;

    switch (i2c) {
        case 1:
            I2C_Periph = I2C1;
            break;
        case 2:
            I2C_Periph = I2C2;
            break;
        default:
            return ERROR_BAD_DEVICE;
    }

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

ERROR_t I2C_ReadBuffer(const uint8_t i2c, uint8_t dev_address, const uint8_t reg_address, uint8_t *buffer,
                       uint8_t num_bytes_read) {

    I2C_TypeDef *I2C_Periph;

    switch (i2c) {
        case 1:
            I2C_Periph = I2C1;
            break;
        case 2:
            I2C_Periph = I2C2;
            break;
        default:
            return ERROR_BAD_DEVICE;
    }

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

ERROR_t I2C_Read(uint8_t i2c, uint8_t dev_address, uint8_t *buffer, uint8_t num_bytes_read) {

    I2C_TypeDef *I2C_Periph;

    switch (i2c) {
        case 1:
            I2C_Periph = I2C1;
            break;
        case 2:
            I2C_Periph = I2C2;
            break;
        default:
            return ERROR_BAD_DEVICE;
    }

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

ERROR_t I2C1_Read(uint8_t dev_address, uint8_t *buffer, uint8_t num_bytes_read) {
    return I2C_Read(1, dev_address, buffer, num_bytes_read);
}

ERROR_t I2C2_Read(uint8_t dev_address, uint8_t *buffer, uint8_t num_bytes_read) {
    return I2C_Read(2, dev_address, buffer, num_bytes_read);
}

ERROR_t I2C1_WriteBuffer(uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t size) {
    return I2C_WriteBuffer(1, dev_address, reg_address, buffer, size);
}

ERROR_t I2C2_WriteBuffer(uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t size) {
    return I2C_WriteBuffer(2, dev_address, reg_address, buffer, size);
}

ERROR_t I2C1_ReadBuffer(uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t num_bytes_read) {
    return I2C_ReadBuffer(1, dev_address, reg_address, buffer, num_bytes_read);
}

ERROR_t I2C2_ReadBuffer(uint8_t dev_address, uint8_t reg_address, uint8_t *buffer, uint8_t num_bytes_read) {
    return I2C_ReadBuffer(2, dev_address, reg_address, buffer, num_bytes_read);
}