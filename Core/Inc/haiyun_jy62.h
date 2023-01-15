/*
 * haiyun_jy62.h
 *
 *  Created on: Jan 14, 2023
 *      Author: 58294
 */

#ifndef INC_HAIYUN_JY62_H_
#define INC_HAIYUN_JY62_H_
#endif /* INC_HAIYUN_JY62_H_ */

#include "stm32f0xx_hal.h"

typedef struct jy62{
	UART_HandleTypeDef *huart;
	// accel, gyro and angles
	short accel[3];  // 4 + 4 + 4 bytes x y z
	short gyro[3];	 // 4 + 4 + 4 bytes x y z
	short angle[3];	 // 4 + 4 + 4 bytes roll pitch yaw

	uint32_t lastUpdate;
	uint8_t buffer[16];
} TypeDef_JY62;

void jy62_init(TypeDef_JY62* imu, UART_HandleTypeDef* ipt_huart);
void jy62_DMA_read(TypeDef_JY62* imu);
void jy62_decoder(TypeDef_JY62* imu);
void jy62_calib(TypeDef_JY62* imu);
void jy62_AngInit(TypeDef_JY62* imu);
