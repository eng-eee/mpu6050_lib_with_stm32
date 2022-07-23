/*
 * MPU6050.h
 *
 *  Created on: 21 Tem 2022
 *      Author: Lenovo
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stdio.h"
#include "stdlib.h"

#define DEVICE_ADDR_AD0				(0x68<<1)
#define DEVICE_ADDR_AD1				(0x69<<1)

#define SMPLRT_DIV_REG				0x19
#define CONFIG_REG					0x1A
#define GYRO_CONFIG_REG				0x1B
#define ACCEL_CONFIG_REG			0x1C
#define FIFO_EN_REG					0x23

#define ACCEL_XOUT_H_REG			0x3B
#define ACCEL_XOUT_L_REG			0x3C
#define ACCEL_YOUT_H_REG			0x3D
#define ACCEL_YOUT_L_REG			0x3E
#define ACCEL_ZOUT_H_REG			0x3F
#define ACCEL_ZOUT_L_REG			0x40

#define TEMP_OUT_H_REG				0x41
#define TEMP_OUT_L_REG				0x42

#define GYRO_XOUT_H_REG				0x43
#define GYRO_XOUT_L_REG				0x44
#define GYRO_YOUT_H_REG				0x45
#define GYRO_YOUT_L_REG				0x46
#define GYRO_ZOUT_H_REG				0x47
#define GYRO_ZOUT_L_REG				0x48

#define SIGNAL_PATH_RESET_REG		0x68
#define	USER_CTRL_REG				0x6A
#define PWR_MGMT_1_REG				0x6B /*<!This register allows the user to configure the power mode and clock source.*/
#define PWR_MGMT_2_REG				0x6C /*<!This register allows the user to configure the frequency of wake-ups in Accelerometer Only Low Power Mode.*/
#define FIFO_COUNT_H_REG			0x72 /*<!These registers keep track of the number of samples currently in the FIFO buffer.*/
#define FIFO_COUNT_L_REG			0x73 /*<!These registers keep track of the number of samples currently in the FIFO buffer.*/

#define FIFO_R_W_REG				0x74 /*<! This register is used to read and write data from the FIFO buffer */
#define WHO_AM_I_REG				0x75

/*
 * 	EXT_SYNC_SET PARAMETER FOR CONFIG REGISTER
 */

#define TEMP_OUT_L						1
#define	GYRO_XOUT_L						2
#define	GYRO_YOUT_L						3
#define	GYRO_ZOUT_L						4
#define	ACCEL_XOUT_L					5
#define	ACCEL_YOUT_L					6
#define	ACCEL_ZOUT_L					7

#define EXT_SYNC_SET_Msk				(7<<3)
#define EXT_SYNC_SET_Pos				(3)

/*
 * 	DLPF_CFG FOR CONFIG REGISTRER
 */

#define DLPF_CFG_Msk					(7U)
#define DLPF_CFG_Pos					(0U)

/*
 * FS_SEL FOR GYRO CONFIG
 */

#define FS_SEL_GYRO_Mask				(3U<<3)
#define FS_SEL_GYRO_Pos					3

/*
 * FS_SEL FOR ACCEL CONFIG
 */

#define FS_SEL_ACCEL_Mask				(3U<<3)
#define FS_SEL_ACCEL_Pos				(3)

/*
 * SIGNAL_PATH_RESET MASK
 */

#define GYRO_RESET_Mask					(1<<2)
#define ACCEL_RESET_Mask				(1<<1)
#define TEMP_RESET_Mask					(1<<0)

/*
 * USER_CTRL_REG Mask
 *
 */

#define FIFO_EN_Mask					(1<<6)
#define I2C_MST_EN_Mask					(1<<5)
#define I2C_IF_DIS_Mask					(1<<4)
#define FIFO_RESET_Mask					(1<<2)
#define I2C_MST_RESET_Mask				(1<<1)
#define	SIG_COND_RESET_Mask				(1<<0)

/*
 * PWR_MGMT_1_REG MASK
 */

#define DEVICE_RESET_Mask				(1<<7)
#define SLEEP_Mask						(1<<6)
#define CYCLE_Mask						(1<<5)
#define TEMP_DISABLE_Mask				(1<<3)
#define CLKSEL_Mask						(7<<0)


typedef enum{
	SENSOR_OK,
	SENSOR_ERROR = !SENSOR_OK
}MPU6050_SensorStatus_t;

typedef enum{
	BAND_W260,
	BAND_W184,
	BAND_W94,
	BAND_W44,
	BAND_W21,
	BAND_W10,
	BAND_W5
}FilterBandwidth_t;

typedef enum{
	GYRO_SCALE_RANGE250,
	GYRO_SCALE_RANGE500,
	GYRO_SCALE_RANGE1000,
	GYRO_SCALE_RANGE2000
}Gyro_Scale_Range_t;

typedef enum{
	ACCEL_SCALE_RANGE2G,
	ACCEL_SCALE_RANGE4G,
	ACCEL_SCALE_RANGE8G,
	ACCEL_SCALE_RANGE16G
}Accel_Scale_Range_t;

typedef struct{
	double accel_x;
	double accel_y;
	double accel_z;
}Accel_Value_t;

typedef struct{
	double gyro_x;
	double gyro_y;
	double gyro_z;
}Gyro_Value_t;

typedef struct{
	int16_t gyro[3];
	int16_t accel[3];
}Accel_Gyro_Offset_t;

typedef struct{
	FilterBandwidth_t BandWith;
	Gyro_Scale_Range_t GyroRange;
	Accel_Scale_Range_t AccelRange;
	Accel_Gyro_Offset_t Offset_val;
	Accel_Value_t Acc_val;
	Gyro_Value_t Gyro_val;
	double Temperature;
}MPU6050_Sensor_t;



MPU6050_SensorStatus_t MPU6050_Begin(MPU6050_Sensor_t* mpu6050);
MPU6050_SensorStatus_t MPU6050_Test(MPU6050_Sensor_t* mpu6050);


MPU6050_SensorStatus_t MPU6050_Calibration(MPU6050_Sensor_t* mpu6050,int iteration);
MPU6050_SensorStatus_t MPU6050_Read_Temp_Value(MPU6050_Sensor_t* mpu6050);
MPU6050_SensorStatus_t MPU6050_Read_Value(MPU6050_Sensor_t* mpu6050);

uint8_t MPU6050_Get_Device_Address();

#endif /* INC_MPU6050_H_ */
