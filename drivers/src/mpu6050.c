#include "mpu6050.h"

ERROR_t MPU6050_Init(MPU6050_t* MPU6050){
	
	// Variable to transfer to i2c.
	uint8_t data;
	ERROR_t error;
	
	// Set the MPU in running mode.
	error = MPU6050->ReadBuffer(MPU6050->device_address, MPU6050_PWR_MGMT_1, &data, 1);
	if(error != NO_ERROR){
		return error;
	}
	data &= ~((1 <<5)|(1 <<6));

	error = MPU6050->WriteBuffer(MPU6050->device_address, MPU6050_PWR_MGMT_1, &data, 1);
	if(error != NO_ERROR){
		return error;
	}

	// Set the gyroscope configuration. +- 1000�/s
	error = MPU6050->ReadBuffer(MPU6050->device_address, MPU6050_GYRO_CONFIG, &data, 1);
	if(error != NO_ERROR){
		return error;
	}

	data &= ~(1 <<3);
	data |= (1 <<4);
	error = MPU6050->WriteBuffer(MPU6050->device_address, MPU6050_GYRO_CONFIG, &data, 1);
	if(error != NO_ERROR){
		return error;
	}

	// Set the acceleration configuration. +- 8g
	error = MPU6050->ReadBuffer(MPU6050->device_address, MPU6050_ACCEL_CONFIG, &data, 1);
	if(error != NO_ERROR){
		return error;
	}

	data &= ~(1 <<3);
	data |= (1 <<4);

	error = MPU6050->WriteBuffer(MPU6050->device_address, MPU6050_ACCEL_CONFIG, &data, 1);
	if(error != NO_ERROR){
		return error;
	}

	return NO_ERROR;
}

ERROR_t MPU6050_ReadAccel(MPU6050_t* MPU6050){

	// Variable to receive bytes from read_buffer.
	uint8_t data[6];
	ERROR_t error;

	// Read acceleration values.
	error = MPU6050->ReadBuffer(MPU6050->device_address, MPU6050_ACCEL_XOUT_H, data, 6);
	if(error != NO_ERROR){
		return error;
	}

	MPU6050->accel[0] = (data[0] << 8) | (data[1]);
	MPU6050->accel[1] = (data[2] << 8) | (data[3]);
	MPU6050->accel[2] = (data[4] << 8) | (data[5]);

	return NO_ERROR;
}

ERROR_t MPU6050_ReadGyro(MPU6050_t* MPU6050){

	// Variable to receive bytes from read_buffer.
	uint8_t data[6];
	ERROR_t error;

	// Read gyroscope values.
	error = MPU6050->ReadBuffer(MPU6050->device_address, MPU6050_GYRO_XOUT_H, data, 6);
	if(error != NO_ERROR){
		return error;
	}

	MPU6050->gyro[0] = (data[0] << 8) | (data[1]);
	MPU6050->gyro[1] = (data[2] << 8) | (data[3]);
	MPU6050->gyro[2] = (data[4] << 8) | (data[5]);

	return NO_ERROR;
}

ERROR_t MPU6050_AccelSphericalParam(MPU6050_t* MPU6050, MPU6050_SphericalParam* sphericalP){
	float ax = MPU6050->accel[0];
	float ay = MPU6050->accel[1];
	float az = MPU6050->accel[2];

	sphericalP->r = sqrt(pow(ax,2) + pow(ay,2) + pow(az,2));
	sphericalP->theta = atan2(sqrt(pow(ax,2) + pow(ay,2)), az);
	sphericalP->phi = atan2(ay,ax);
	return NO_ERROR;
}
