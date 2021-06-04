#include "bme280.h"
#include "i2c.h"

#define I2C_ADDR        0x77

uint8_t BME280_Init(BME280_InitTypeDef *BME280_InitStruct) {
    // Check the parameters.
    assert_param(BME280_IS_OVS_T(BME280_InitStruct->OVS_T));
    assert_param(BME280_IS_OVS_P(BME280_InitStruct->OVS_P));
    assert_param(BME280_IS_OVS_H(BME280_InitStruct->OVS_H));

    // Check that the device is present by reading the id register.
    // The value should be equal to the device BME280_DEFAULT_ID.
    if (BME280_ReadRegister(BME280_ID) != BME280_DEFAULT_ID) {
        return 1;
    }

    // Set the humidity register first for the changes to be effective.
    BME280_WriteRegister(BME280_CTRL_HUM, BME280_InitStruct->OVS_H);

    // Set the ctrl_mes register.
    BME280_WriteRegister(BME280_CTRL_MES, BME280_InitStruct->OVS_P |
                                          BME280_InitStruct->OVS_T |
                                          BME280_InitStruct->MODE);
    return 0;
}

void BME280_ReadConfigurationParameters(BME280_CompParam *BME280_CompParamStruct) {
    // Array for the read register. The first addresses follow each other.
    uint8_t registerReadout[24] = {0};
    // Read registers for Temp and pressure.
    BME280_ReadRegisters(BME280_DIG_T1, registerReadout, 24);
    // Fill the structure.
    BME280_CompParamStruct->DIG_T1 = (registerReadout[1] << 8) | registerReadout[0];
    BME280_CompParamStruct->DIG_T2 = (registerReadout[3] << 8) | registerReadout[2];
    BME280_CompParamStruct->DIG_T3 = (registerReadout[5] << 8) | registerReadout[4];
    BME280_CompParamStruct->DIG_P1 = (registerReadout[7] << 8) | registerReadout[6];
    BME280_CompParamStruct->DIG_P2 = (registerReadout[9] << 8) | registerReadout[8];
    BME280_CompParamStruct->DIG_P3 = (registerReadout[11] << 8) | registerReadout[10];
    BME280_CompParamStruct->DIG_P4 = (registerReadout[13] << 8) | registerReadout[12];
    BME280_CompParamStruct->DIG_P5 = (registerReadout[15] << 8) | registerReadout[14];
    BME280_CompParamStruct->DIG_P6 = (registerReadout[17] << 8) | registerReadout[16];
    BME280_CompParamStruct->DIG_P7 = (registerReadout[19] << 8) | registerReadout[18];
    BME280_CompParamStruct->DIG_P8 = (registerReadout[21] << 8) | registerReadout[20];
    BME280_CompParamStruct->DIG_P9 = (registerReadout[23] << 8) | registerReadout[22];
    // This register is not follow by another register.
    BME280_CompParamStruct->DIG_H1 = BME280_ReadRegister(BME280_DIG_H1);
    // Read the reaming registers.
    BME280_CompParamStruct->DIG_H2 = (registerReadout[1] << 8) | registerReadout[0];
    BME280_CompParamStruct->DIG_H3 = registerReadout[2];
    BME280_CompParamStruct->DIG_H4 = (registerReadout[3] << 4) | (registerReadout[4] & 0x0F);
    BME280_CompParamStruct->DIG_H5 = (registerReadout[5] << 4) | (registerReadout[4] & 0xF0);
    BME280_CompParamStruct->DIG_H6 = registerReadout[6];
}


// Returns temperature in DegC, resolution is 0.01 DegC. Output value of �5123� equals 51.23 DegC.
// t_fine carries fine temperature as global value
int32_t t_fine;

int32_t BME280_CompensateTemp(int32_t adc_T, BME280_CompParam *BME280_CompParamStruct) {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t) BME280_CompParamStruct->DIG_T1 << 1))) *
            ((int32_t) BME280_CompParamStruct->DIG_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) BME280_CompParamStruct->DIG_T1)) *
              ((adc_T >> 4) - ((int32_t) BME280_CompParamStruct->DIG_T1))) >> 12) *
            ((int32_t) BME280_CompParamStruct->DIG_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer. Output value of �96386� equals 96386 Pa = 963.86 hPa
uint32_t BME280_CompensatePres(int32_t adc_P, BME280_CompParam *BME280_CompParamStruct) {
    int32_t var1, var2;
    uint32_t p;

    var1 = (((int32_t) t_fine) >> 1) - (int32_t) 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t) BME280_CompParamStruct->DIG_P6);
    var2 = var2 + ((var1 * ((int32_t) BME280_CompParamStruct->DIG_P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t) BME280_CompParamStruct->DIG_P4) << 16);
    var1 = (((BME280_CompParamStruct->DIG_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) +
            ((((int32_t) BME280_CompParamStruct->DIG_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t) BME280_CompParamStruct->DIG_P1)) >> 15);

    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = (((uint32_t) (((int32_t) 1048576) - adc_P) - (var2 >> 12))) * 3125;
    if (p < 0x80000000) {
        p = (p << 1) / ((uint32_t) var1);
    } else {
        p = (p / (uint32_t) var1) * 2;
    }

    var1 = (((int32_t) BME280_CompParamStruct->DIG_P9) * ((int32_t) (((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t) (p >> 2)) * ((int32_t) BME280_CompParamStruct->DIG_P8)) >> 13;
    p = (uint32_t) ((int32_t) p + ((var1 + var2 + BME280_CompParamStruct->DIG_P7) >> 4));

    return p;
}


// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of �47445� represents 47445/1024 = 46.333 %RH
uint32_t BME280_CompensateHum(int32_t adc_H, BME280_CompParam *BME280_CompParamStruct) {
    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t) 76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t) BME280_CompParamStruct->DIG_H4) << 20) -
                    (((int32_t) BME280_CompParamStruct->DIG_H5) * v_x1_u32r)) + \
                            ((int32_t) 16384)) >> 15) *
                 (((((((v_x1_u32r * ((int32_t) BME280_CompParamStruct->DIG_H6)) >> 10) *
                      (((v_x1_u32r * ((int32_t) BME280_CompParamStruct->DIG_H3)) >> 11) \
 + ((int32_t) 32768))) >> 10) + ((int32_t) 2097152)) * ((int32_t) BME280_CompParamStruct->DIG_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r -
                 (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t) BME280_CompParamStruct->DIG_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    return (uint32_t) (v_x1_u32r >> 12);
}

/**
  * @about Read the pressure, temperature and humidity of the sensor.
  * @param 
  */
void BME280_ReadSensors(BME280_SensorsValues *BME280_Sensors, BME280_CompParam *BME280_CompParamStruct) {
    uint8_t buffer[8];
    // Read all registers.
    BME280_ReadRegisters(BME280_PRESS_MSB, buffer, 8);
    // Get the adc values fron the buffer read.
    int32_t adc_P = (int32_t) (buffer[0] << 12) | (int32_t) (buffer[1] << 4) | (int32_t) (buffer[2] >> 4);
    int32_t adc_T = (int32_t) (buffer[3] << 12) | (int32_t) (buffer[4] << 4) | (int32_t) (buffer[5] >> 4);
    int32_t adc_H = (int32_t) (buffer[6] << 7) | (int32_t) (buffer[7]);
    // Compensate the ADC values, has to start with temperature for t_fine.
    BME280_Sensors->Temp = BME280_CompensateTemp(adc_T, BME280_CompParamStruct);
    BME280_Sensors->Pres = BME280_CompensatePres(adc_P, BME280_CompParamStruct);
    BME280_Sensors->Hum = BME280_CompensateHum(adc_H, BME280_CompParamStruct);
}

/**
	* @about Read a register of the device using spi.
	* @param The address of the register to read.
	* @retval The value of the register.
	*/
uint8_t BME280_ReadRegister(const uint8_t register_address) {
    // Variable to get the returned value from device.
    uint8_t received_data;

    BME280_ReadRegisters(register_address, &received_data, 1);

    // Return the value.
    return received_data;
}

void BME280_ReadRegisters(const uint8_t register_address,
                          uint8_t *buffer,
                          const uint8_t size) {
    I2C_ReadBuffer(1, I2C_ADDR, register_address, buffer, size);
}

/**
	* @about Write into a register of the device using spi.
	* @param The address of the register to write.
	* @param The value of the register.
	*/
void BME280_WriteRegister(uint8_t register_address, uint8_t value) {
    I2C_WriteBuffer(1, I2C_ADDR, register_address, &value, 1);
}
