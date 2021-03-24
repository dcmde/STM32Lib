#include "bmp280.h"

ERROR_t BMP280_Init(BMP280_t* BMP280){
    // Check pointers.
    if(BMP280->CompParamStruct == NULL || BMP280->InitStruct == NULL){
        return ERROR_NULL_POINTER;
    }
	// Check parameters.
	assert_param(BMP280_IS_MODE(BMP280->InitStruct->MODE));
	assert_param(BMP280_IS_OVS_T(BMP280->InitStruct->OVS_T));
	assert_param(BMP280_IS_OVS_P(BMP280->InitStruct->OVS_P));
	
	// Check that the device is present by reading the id register. 
	// The value should be equal to the device BMP280_DEFAULT_ID.
	if(BMP280_ReadRegister(BMP280, BMP280_ID) != BMP280_DEFAULT_ID){
		return ERROR_BAD_DEVICE;
	}
	
	// Set the ctrl_mes register.
	BMP280_WriteRegister(BMP280, BMP280_CTRL_MES, BMP280->InitStruct->OVS_P | BMP280->InitStruct->OVS_T | BMP280->InitStruct->MODE);

	return NO_ERROR;
}

ERROR_t BMP280_ReadConfigurationParameters(BMP280_t* BMP280){

	// Array for the read register. The first addresses follow each other.
	uint8_t registerReadout[24] = {0};

	// Read registers for Temp and pressure.
	BMP280_ReadRegisters(BMP280, BMP280_DIG_T1, registerReadout, 24);

	// Fill the structure.
	BMP280->CompParamStruct->DIG_T1 = (registerReadout[1] << 8) | registerReadout[0];
	BMP280->CompParamStruct->DIG_T2 = (registerReadout[3] << 8) | registerReadout[2];
	BMP280->CompParamStruct->DIG_T3 = (registerReadout[5] << 8) | registerReadout[4];
	BMP280->CompParamStruct->DIG_P1 = (registerReadout[7] << 8) | registerReadout[6];
	BMP280->CompParamStruct->DIG_P2 = (registerReadout[9] << 8) | registerReadout[8];
	BMP280->CompParamStruct->DIG_P3 = (registerReadout[11] << 8) | registerReadout[10];
	BMP280->CompParamStruct->DIG_P4 = (registerReadout[13] << 8) | registerReadout[12];
	BMP280->CompParamStruct->DIG_P5 = (registerReadout[15] << 8) | registerReadout[14];
	BMP280->CompParamStruct->DIG_P6 = (registerReadout[17] << 8) | registerReadout[16];
	BMP280->CompParamStruct->DIG_P7 = (registerReadout[19] << 8) | registerReadout[18];
	BMP280->CompParamStruct->DIG_P8 = (registerReadout[21] << 8) | registerReadout[20];
	BMP280->CompParamStruct->DIG_P9 = (registerReadout[23] << 8) | registerReadout[22];

    return NO_ERROR;
}

/**
 * @about Compensate the raw temperature value from the device into the true temperature in °C.
 * @param BMP280 structure.
 * @return The compensated value in °C, resolution is 0.01 DegC. Output value of �5123� equals 51.23 DegC.
 */
int32_t BMP280_CompensateTemp(BMP280_t* BMP280){
	int32_t var1, var2, T;
	var1 = ((((BMP280->adc_T >> 3) - ((int32_t)BMP280->CompParamStruct->DIG_T1 << 1))) * ((int32_t)BMP280->CompParamStruct->DIG_T2) ) >> 11;
	var2 = (((((BMP280->adc_T >> 4) - ((int32_t)BMP280->CompParamStruct->DIG_T1)) * ((BMP280->adc_T >> 4) - ((int32_t)BMP280->CompParamStruct->DIG_T1))) >> 12) * ((int32_t)BMP280->CompParamStruct->DIG_T3)) >> 14;
    BMP280->t_fine = var1 + var2;
	T = (BMP280->t_fine * 5 + 128) >> 8;
	return T;
}

/**
 * @about Compensate the raw pressure value from the device into the true pressure in Pa.
 * @param BMP280 structure.
 * @return The compensated pressure. Returns pressure in Pa as unsigned 32 bit integer. Output value of �96386� equals 96386 Pa = 963.86 hPa
 */
uint32_t BMP280_CompensatePres(BMP280_t* BMP280){
	int32_t var1, var2;
	uint32_t p;
	
	var1 = (((int32_t)BMP280->t_fine)>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)BMP280->CompParamStruct->DIG_P6);
	var2 = var2 + ((var1*((int32_t)BMP280->CompParamStruct->DIG_P5))<<1);
	var2 = (var2>>2) + (((int32_t)BMP280->CompParamStruct->DIG_P4)<<16);
	var1 = (((BMP280->CompParamStruct->DIG_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)BMP280->CompParamStruct->DIG_P2) * var1)>>1))>>18;
	var1 =((((32768+var1))*((int32_t)BMP280->CompParamStruct->DIG_P1))>>15);
	
	if (var1 == 0){
		return 0; // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int32_t)1048576)-BMP280->adc_P)-(var2>>12)))*3125;
	if (p < 0x80000000){
		p = (p << 1) / ((uint32_t)var1);
	}
	else{
		p = (p / (uint32_t)var1) * 2;
	}
	
	var1 = (((int32_t)BMP280->CompParamStruct->DIG_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)BMP280->CompParamStruct->DIG_P8))>>13;
	p = (uint32_t)((int32_t)p + ((var1 + var2 + BMP280->CompParamStruct->DIG_P7) >> 4));
	
	return p;
}

/**
  * @about Read the pressure, temperature of the sensor.
  * @param BMP280 structure
  * @param
  */
void BMP280_GetTempPress(BMP280_t* BMP280, BMP280_SensorValues* BMP280_Sensors){

	uint8_t buffer[8];

	// Read all registers.
	BMP280_ReadRegisters(BMP280, BMP280_PRESS_MSB, buffer, 8);

	// Get the adc values fron the buffer read.
	BMP280->adc_P = (int32_t)(buffer[0] << 12) | (int32_t)(buffer[1] << 4) | (int32_t)(buffer[2] >> 4);
	BMP280->adc_T = (int32_t)(buffer[3] << 12) | (int32_t)(buffer[4] << 4) | (int32_t)(buffer[5] >> 4);

	// Compensate the ADC values, has to start with temperature for t_fine.
	BMP280_Sensors->Temp = BMP280_CompensateTemp(BMP280);
	BMP280_Sensors->Pres = BMP280_CompensatePres(BMP280);
}



/**
	* @about Read a register of the device using spi.
	* @param The address of the register to read.
	* @retval The value of the register.
	*/
uint8_t BMP280_ReadRegister(BMP280_t* BMP280, const uint8_t register_address){

	// Variable to get the returned value from device.
	uint8_t received_data;
	
	BMP280_ReadRegisters(BMP280, register_address, &received_data, 1);
	
	// Return the value.
	return received_data;
}

ERROR_t BMP280_ReadRegisters(BMP280_t* BMP280, uint8_t register_address, uint8_t* buffer, const uint8_t size){
	return BMP280->ReadBuffer(BMP280->address, register_address, buffer, size);
}

ERROR_t BMP280_WriteRegister(BMP280_t* BMP280, uint8_t register_address, uint8_t value){
	return BMP280->WriteBuffer(BMP280->address, register_address, &value, 1);
}
