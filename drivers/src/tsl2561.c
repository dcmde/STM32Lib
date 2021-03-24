#include "tsl2561.h"

ERROR_t TSL2561_Init(TSL2561_t* TSL2561){
	
	uint8_t buffer[7];
	
	ERROR_t error;
	
	// Read the device ID register to make sure we are communicating with a TSL2561.
	error = TSL2561->ReadBuffer(TSL2561->device_address, 0x80 | TSL2561_ID, buffer, 1);
	
	// Check communication status.
	if(error != NO_ERROR){return error;}
	
	// Check that the device we are trying to comunicate with is a TSL2561 and not
	// a TSL2560.
	if(!(buffer[0] & 1 << 4)){
		//return ERROR_BAD_DEVICE;
	}
	
	// Set all registers according to the init struct.
	buffer[0] = TSL2561->InitStruct->POWER;
	buffer[1] = TSL2561->InitStruct->GAIN | TSL2561->InitStruct->INTEGRATION_TIME;
	buffer[2] = TSL2561->InitStruct->THRESHOLD_CH1;				// Only lower bits.
	buffer[3] = TSL2561->InitStruct->THRESHOLD_CH1 >> 4;	// Only upper bits.
	buffer[4] = TSL2561->InitStruct->THRESHOLD_CH2;				// Only lower bits.
	buffer[5] = TSL2561->InitStruct->THRESHOLD_CH2 >> 4;	// Only upper bits.
	buffer[6] = TSL2561->InitStruct->INTR | TSL2561->InitStruct->PERSIST;

	error = TSL2561_WriteRegisters(TSL2561, TSL2561_CONTROL, buffer, sizeof(buffer));
	
	// Check communication status.
	if(error != NO_ERROR){return error;}
	
	// Read again the CONTROL register to be sure the power is on if it was set accordingly.
	error = TSL2561_ReadRegisters(TSL2561, TSL2561_CONTROL, buffer, 1);
	
	// Check that the device is working as expected. The value of the register is changed in a wierd manner we write 0x03 and read 0x33.
	if((buffer[0] & 0x0F) != TSL2561->InitStruct->POWER){
			return ERROR_INIT_FAILED;
	}
	return NO_ERROR;
}

ERROR_t TSL2561_Read(TSL2561_t* TSL2561){
	
	uint8_t buffer[4];
	
	ERROR_t error;
	
	error = TSL2561_ReadRegisters(TSL2561, TSL2561_DATA_0_LOW, buffer, sizeof(buffer));
	
	// Check communication status.
	if(error != NO_ERROR){return error;}
	
	TSL2561->ChannelVisibleIR = (uint16_t)(buffer[1] << 4) | buffer[0];
	TSL2561->ChannelIR = (uint16_t)(buffer[3] << 4) | buffer[2];
	
	// If the package is T, FN or CL. Doesn't work for CS package.
	// For 0 < CH1/CH0  0.50 Lux = 0.0304  CH0 - 0.062  CH0  ((CH1/CH0)1.4)
	// For 0.50 < CH1/CH0  0.61 Lux = 0.0224  CH0 - 0.031  CH1
	// For 0.61 < CH1/CH0  0.80 Lux = 0.0128  CH0 - 0.0153  CH1
	// For 0.80 < CH1/CH0  1.30 Lux = 0.00146  CH0 - 0.00112  CH1
	// For CH1/CH0 > 1.30	Lux = 0
	
	return NO_ERROR;
}

ERROR_t TSL2561_ReadRegisters(TSL2561_t* TSL2561, uint8_t reg_addr, uint8_t* buffer, uint8_t size){
	
	ERROR_t error;
	
	// The register address has to be a SMBus like frame COMMAND.
	error = TSL2561->ReadBuffer(TSL2561->device_address, 0x80 | reg_addr, buffer, size);
	
	// Check communication status.
	if(error != NO_ERROR){return error;}
	
	return NO_ERROR;
}

ERROR_t TSL2561_WriteRegisters(TSL2561_t* TSL2561, uint8_t reg_addr, uint8_t* buffer, uint8_t size){
	
	ERROR_t error;
	
	uint8_t tmp[size+1];
	
	// Add the byte count register that is not used but present in the protocol.
	for(uint8_t i = 0; i < size; ++i){
		tmp[i+1] = buffer[i];
	}
	
	// The address register of the device has to be a valid write COMMAND frame.
	error = TSL2561->WriteBuffer(TSL2561->device_address, 0x90 | reg_addr, tmp, size+1);
	
	// Check communication status.
	if(error != NO_ERROR){return error;}
	
	return NO_ERROR;
}
