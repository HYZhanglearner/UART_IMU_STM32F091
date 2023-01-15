/*
 * haiyun_jy62.c
 *
 *  Created on: Jan 14, 2023
 *      Author: 58294
 */

#include "haiyun_jy62.h"
#define LENGTH_OF_BUFFER 11
const uint8_t angInit[3] = {0xFF, 0xAA, 0x52};
const uint8_t angCalib[3] = {0xFF, 0xAA, 0x67};


void jy62_init(TypeDef_JY62* imu,UART_HandleTypeDef* ipt_huart){
	// Check imu and uart port
	if(!imu) return;
	else if (!imu->huart) return;
	imu->huart = ipt_huart;

	jy62_AngInit(imu);
	imu->accel[0] = 0; imu->accel[1] = 0; imu->accel[2] = 0;
	imu->gyro[0] = 0; imu->gyro[1] = 0; imu->gyro[2] = 0;
	imu->angle[0] = 0; imu->angle[1] = 0; imu->angle[2] = 0;

	imu->lastUpdate = HAL_GetTick();
	HAL_UART_Receive_DMA(imu->huart, imu->buffer, LENGTH_OF_BUFFER);
}
void jy62_AngInit(TypeDef_JY62* imu){
	HAL_UART_Transmit_DMA(imu->huart, angInit, 3);
}

void jy62_DMA_read(TypeDef_JY62* imu){
	HAL_UART_Receive_DMA(imu->huart, imu->buffer, LENGTH_OF_BUFFER);
}

void jy62_decoder(TypeDef_JY62* imu){
	int sum = 0;
	short temp = 0;
	if(imu->buffer[0]!=0x51) return;
	switch (imu->buffer[1]) {
		// Acquiring accels
		case 0x51:
			sum = imu->buffer[0] + imu->buffer[1] + imu->buffer[2] + imu->buffer[3] +
			imu->buffer[4] + imu->buffer[5] + imu->buffer[6] + imu->buffer[7] +
			imu->buffer[8]+ imu->buffer[9];
			if(sum == imu->buffer[10]){
				imu->angle[0] = ((imu->buffer[3]<<8) |imu->buffer[2]) /32768*16 *9.8;
				imu->angle[1] = ((imu->buffer[5]<<8) |imu->buffer[3]) /32768*16 *9.8;
				imu->angle[2] = ((imu->buffer[7]<<8) |imu->buffer[6]) /32768*16 *9.8;
			}
			else return;
			break;
		// Acquiring gyros
		case 0x52:
			sum = imu->buffer[0] + imu->buffer[1] + imu->buffer[2] + imu->buffer[3] +
			imu->buffer[4] + imu->buffer[5] + imu->buffer[6] + imu->buffer[7] +
			imu->buffer[8]+ imu->buffer[9];
			if(sum == imu->buffer[10]){
				imu->gyro[0] = ((imu->buffer[3]<<8) |imu->buffer[2]) /32768*16 *9.8;
				imu->gyro[1] = ((imu->buffer[5]<<8) |imu->buffer[3]) /32768*16 *9.8;
				imu->gyro[2] = ((imu->buffer[7]<<8) |imu->buffer[6]) /32768*16 *9.8;
			}
			else return;
			break;
		// Acquiring angles
		case 0x53:
			sum = imu->buffer[0] + imu->buffer[1] + imu->buffer[2] + imu->buffer[3] +
			imu->buffer[4] + imu->buffer[5] + imu->buffer[6] + imu->buffer[7] +
			imu->buffer[8]+ imu->buffer[9];
			if(sum == imu->buffer[10]){
				imu->angle[0] = ((imu->buffer[3]<<8) |imu->buffer[2]) /32768*16 *9.8;
				imu->angle[1] = ((imu->buffer[5]<<8) |imu->buffer[3]) /32768*16 *9.8;
				imu->angle[2] = ((imu->buffer[7]<<8) |imu->buffer[6]) /32768*16 *9.8;
			}
			else return;
			break;

		  default:
			  break;

	}
}
void jy62_calib(TypeDef_JY62* imu){
	HAL_UART_Transmit_DMA(imu->huart, angCalib, 3);
}

