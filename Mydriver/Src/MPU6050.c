/*
 * MPU6050.c
 *
 *  Created on: 21 Tem 2022
 *      Author: Lenovo
 */

#include "MPU6050.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c1;

static double gyro_sens;
static double accel_sens;

static double look_up_gyro_sens[] = { 131.0, 65.5, 32.8, 16.4 };
static double look_up_accel_sens[] = { 16.384, 8.192, 4.096, 2.048 };

static MPU6050_SensorStatus_t MPU6050_Write_Reg(uint8_t addr, uint8_t val) {
	HAL_StatusTypeDef ret = HAL_ERROR;
	ret = HAL_I2C_Mem_Write(&hi2c1, DEVICE_ADDR_AD0, addr, 1, &val, 1,
			HAL_TIMEOUT);

	if (HAL_ERROR == ret)
		return SENSOR_ERROR;
	else
		return SENSOR_OK;
}

static MPU6050_SensorStatus_t MPU6050_Read_Reg(uint8_t addr, uint8_t *buf,
		uint8_t size) {
	HAL_StatusTypeDef ret = HAL_ERROR;
	ret = HAL_I2C_Mem_Read(&hi2c1, DEVICE_ADDR_AD0, addr, 1, buf, size,
			HAL_TIMEOUT);

	if (HAL_ERROR == ret)
		return SENSOR_ERROR;
	else
		return SENSOR_OK;
}

static MPU6050_SensorStatus_t MPU6050_Read_Raw_Value(int16_t *ax, int16_t *ay,
		int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz) {
	MPU6050_SensorStatus_t state = SENSOR_ERROR;
	uint8_t accel_buf[6] = { 0 };
	uint8_t gyro_buf[6] = { 0 };

	state = MPU6050_Read_Reg(ACCEL_XOUT_H_REG, accel_buf, 6);

	state = MPU6050_Read_Reg(GYRO_XOUT_H_REG, gyro_buf, 6);

	*ax = (int16_t) (accel_buf[0] << 8 | accel_buf[1]);
	*ay = (int16_t) (accel_buf[2] << 8 | accel_buf[3]);
	*az = (int16_t) (accel_buf[4] << 8 | accel_buf[5]);

	*gx = (int16_t) (gyro_buf[0] << 8 | gyro_buf[1]);
	*gy = (int16_t) (gyro_buf[2] << 8 | gyro_buf[3]);
	*gz = (int16_t) (gyro_buf[4] << 8 | gyro_buf[5]);

	return state;

}

MPU6050_SensorStatus_t MPU6050_Begin(MPU6050_Sensor_t *mpu6050) {

	uint8_t data_w;
	MPU6050_SensorStatus_t state = HAL_ERROR;
	uint8_t buf;
	state = HAL_I2C_IsDeviceReady(&hi2c1, DEVICE_ADDR_AD0, 1, HAL_TIMEOUT);
	if (SENSOR_ERROR == state)
		return SENSOR_ERROR;

	// Device Reset

	data_w = DEVICE_RESET_Mask;
	state = MPU6050_Write_Reg(PWR_MGMT_1_REG, data_w);
	HAL_Delay(10);

	/* Pwr mgt sleep up */

	state = MPU6050_Write_Reg(PWR_MGMT_1_REG, 0x00); // Internal clk 8mhz
	MPU6050_Read_Reg(PWR_MGMT_1_REG, &buf, 1);
	/* Select Filter band with */
	data_w = mpu6050->BandWith;
	state = MPU6050_Write_Reg(CONFIG_REG, data_w);
	MPU6050_Read_Reg(CONFIG_REG, &buf, 1);
	/*Select Accel config */
	data_w = mpu6050->AccelRange << FS_SEL_ACCEL_Pos;
	state = MPU6050_Write_Reg(ACCEL_CONFIG_REG, data_w);
	MPU6050_Read_Reg(ACCEL_CONFIG_REG, &buf, 1);
	/*Select Gyro config */
	data_w = mpu6050->GyroRange << FS_SEL_GYRO_Pos;
	state = MPU6050_Write_Reg(GYRO_CONFIG_REG, data_w);
	MPU6050_Read_Reg(GYRO_CONFIG_REG, &buf, 1);

	/* Select gyro and accel senstive depends on config register*/
	gyro_sens = look_up_gyro_sens[mpu6050->GyroRange];
	accel_sens = look_up_accel_sens[mpu6050->AccelRange];

	return state;
}

MPU6050_SensorStatus_t MPU6050_Calibration(MPU6050_Sensor_t *mpu6050,
		int iteration) {

	MPU6050_SensorStatus_t state = HAL_ERROR;

	int16_t accel_raw[3];
	int16_t gyro_raw[3];

	mpu6050->Offset_val.accel[0] = 0;
	mpu6050->Offset_val.accel[1] = 0;
	mpu6050->Offset_val.accel[2] = 0;

	mpu6050->Offset_val.gyro[0] = 0;
	mpu6050->Offset_val.gyro[1] = 0;
	mpu6050->Offset_val.gyro[2] = 0;

	// Calibration accelometers and gyroscope

	for (int i = 0; i < iteration; i++) {

		state = MPU6050_Read_Raw_Value(&accel_raw[0], &accel_raw[1],
				&accel_raw[2], &gyro_raw[0], &gyro_raw[1], &gyro_raw[2]);

		mpu6050->Offset_val.accel[0] += accel_raw[0];
		mpu6050->Offset_val.accel[1] += accel_raw[1];
		mpu6050->Offset_val.accel[2] += accel_raw[2];

		mpu6050->Offset_val.gyro[0] += gyro_raw[0];
		mpu6050->Offset_val.gyro[1] += gyro_raw[1];
		mpu6050->Offset_val.gyro[2] += gyro_raw[2];

	}
	mpu6050->Offset_val.accel[0] /= iteration;
	mpu6050->Offset_val.accel[1] /= iteration;
	mpu6050->Offset_val.accel[2] /= iteration;

	mpu6050->Offset_val.gyro[0] /= iteration;
	mpu6050->Offset_val.gyro[1] /= iteration;
	mpu6050->Offset_val.gyro[2] /= iteration;

	return state;
}

MPU6050_SensorStatus_t MPU6050_Read_Temp_Value(MPU6050_Sensor_t *mpu6050) {

	MPU6050_SensorStatus_t state = SENSOR_ERROR;
	uint8_t temp[2];
	int16_t temp_raw;
	state = MPU6050_Read_Reg(TEMP_OUT_H_REG, temp, 2);
	temp_raw = temp[0] << 8 | temp[1];

	/* Temperature in degrees C = (TEMP_OUT Register Value as a signed quantity)/340 + 36.53 */
	mpu6050->Temperature = (temp_raw / 340) + 36.53;

	return state;
}

MPU6050_SensorStatus_t MPU6050_Read_Value(MPU6050_Sensor_t *mpu6050) {
	MPU6050_SensorStatus_t state = SENSOR_ERROR;
	int16_t accel_raw[3];
	int16_t gyro_raw[3];
	state = MPU6050_Read_Raw_Value(&accel_raw[0], &accel_raw[1], &accel_raw[2],
			&gyro_raw[0], &gyro_raw[1], &gyro_raw[2]);

	mpu6050->Acc_val.accel_x = ((accel_raw[0] - mpu6050->Offset_val.accel[0])
			/ accel_sens) * 0.004;
	mpu6050->Acc_val.accel_y = ((accel_raw[1] - mpu6050->Offset_val.accel[1])
			/ accel_sens) * 0.004;
	mpu6050->Acc_val.accel_z = ((accel_raw[2] - mpu6050->Offset_val.accel[2])
			/ accel_sens) * 0.004;

	mpu6050->Gyro_val.gyro_x = ((gyro_raw[0] - mpu6050->Offset_val.gyro[0])
			/ gyro_sens) * 0.004;
	mpu6050->Gyro_val.gyro_y = ((gyro_raw[1] - mpu6050->Offset_val.gyro[1])
			/ gyro_sens) * 0.004;
	mpu6050->Gyro_val.gyro_x = ((gyro_raw[2] - mpu6050->Offset_val.gyro[2])
			/ gyro_sens) * 0.004;

	return state;
}

MPU6050_SensorStatus_t MPU6050_Test(MPU6050_Sensor_t *mpu6050) {
	MPU6050_SensorStatus_t state = SENSOR_ERROR;
	int16_t accel_raw[3];
	int16_t gyro_raw[3];
	state = MPU6050_Read_Raw_Value(&accel_raw[0], &accel_raw[1], &accel_raw[2],
			&gyro_raw[0], &gyro_raw[1], &gyro_raw[2]);

	mpu6050->Acc_val.accel_x = ((accel_raw[0]) / accel_sens);
	mpu6050->Acc_val.accel_y = ((accel_raw[1]) / accel_sens);
	mpu6050->Acc_val.accel_z = ((accel_raw[2]) / accel_sens);

	mpu6050->Gyro_val.gyro_x = ((gyro_raw[0]) / gyro_sens);
	mpu6050->Gyro_val.gyro_y = ((gyro_raw[1]) / gyro_sens);
	mpu6050->Gyro_val.gyro_x = ((gyro_raw[2]) / gyro_sens);
	return state;
}

uint8_t MPU6050_Get_Device_Address() {
	MPU6050_SensorStatus_t state = SENSOR_ERROR;
	uint8_t who_am_i = 0x00;

	state = MPU6050_Read_Reg(WHO_AM_I_REG, &who_am_i, 1);

	if (SENSOR_OK == state)
		return who_am_i;
	else
		return HAL_ERROR;
}
