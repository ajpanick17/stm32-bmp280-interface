/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Ajay Gopinathapanicker
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "stm32f407XX_gpio_driver.h"
#include "stm32f407XX_i2c_driver.h"
#include "bmp280.h"
#include "ssd1306.h"

//Structure member definition for BMP280 sensor
BMP280_CALIBRATION_DATA Bmp280_CalibData;
BMP280_REG bmp280_reg;

char arr[28] = {0xAE,0x20,0x10,0xB0,0xC8,0x00,0x10,0x40,0x81,0xFF,0xA1,0xA6,0xA8,0x3F,0xA4,0xD3,0x00,0xD5,0xF0,0xD9,0x22,0xDA,0x12,0xDB,0x20,0x8D,0x14,0xAF};

//Global handle definition for I2C
I2C_Handle_t I2C1handle;

extern int32_t Final_temperature;
extern int32_t Final_pressure;
int32_t t_fine;
extern uint32_t raw_pressure_data;
extern int32_t raw_temp_data;
uint8_t DisplayLine1[32];
uint8_t DisplayLine2[32];

#define SCB_CPACR_ADDRESS  (0xE000ED88UL)
#define SCB_CPACR          (*(volatile uint32_t *)(SCB_CPACR_ADDRESS))

void I2C_GPIO_PIN_INIT(void);
void I2C1_INIT(void);
void GPIO_ButtonInit(void);
extern void ssd1306_print(int page, const char *text);

void delay(volatile uint32_t count)
{
    while(count--) {
        __asm__("nop");      // do nothing
    }
}

int main(void)
{
	SCB_CPACR |= (0xFUL << 20);  //FPU

	GPIO_ButtonInit();
	I2C_GPIO_PIN_INIT();
	I2C1_INIT();

	/* I2C IRQ configurations */
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV,ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER,ENABLE);

	I2C_PeripheralControl(I2C1,ENABLE);
	I2C_ManageAcking(I2C1,ENABLE);

	 SSD1306_Init();

	bmp_sensor_init(&bmp280_reg);
	bmp_read_calibration_data(&Bmp280_CalibData);

	while(1)
	{
		delay(1000000);
		bmp_raw_data_read(&Bmp280_CalibData);
		Final_temperature = bmp_compensate_temp(&Bmp280_CalibData,raw_temp_data,&t_fine);
		Final_pressure = bmp_compensate_pressure(&Bmp280_CalibData,raw_pressure_data);
		snprintf(DisplayLine1, sizeof(DisplayLine1), "Temperature: %u C", Final_temperature);
		snprintf(DisplayLine2, sizeof(DisplayLine2), "Pressure:%ul hPa", Final_pressure);

		SSD1306_GotoXY(0,0);
		SSD1306_Puts(DisplayLine1,&Font_7x10,1);
		SSD1306_GotoXY(0,30);
		SSD1306_Puts(DisplayLine2,&Font_7x10,1);
	    SSD1306_UpdateScreen();


	}

}

void I2C_GPIO_PIN_INIT(void)
{

	/*
	 * PB8 --> SCL-->AF04
	 * PB9 --> SDA-->AF04
	 * PC13 -> PUSH BUTTON
	 */

	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.Gpio_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.Gpio_PinConfig.GPIO_PinAltFuncMode = 4;
	I2CPins.Gpio_PinConfig.GPIO_PinOPType = GPIO_MODE_O_OD;
	I2CPins.Gpio_PinConfig.GPIO_PuPdcontrol = GPIO_MODE_PUP;
	I2CPins.Gpio_PinConfig.GPIO_PinSpeed = GPIO_MODE_FAST_SPEED;

	//SCL
	I2CPins.Gpio_PinConfig.GPIO_PinNumber = GPIO_PIN_8;
	GPIO_Init(&I2CPins);

	//SDA
	I2CPins.Gpio_PinConfig.GPIO_PinNumber = GPIO_PIN_9;
	GPIO_Init(&I2CPins);

}

void I2C1_INIT(void)
{
	I2C1handle.pI2Cx = I2C1;
	I2C1handle.I2C_Config.I2C_ACKControl    = I2C_ACK_ENABLE;
	I2C1handle.I2C_Config.I2C_DeviceAddress = 0x61;
	I2C1handle.I2C_Config.I2C_FMDutyCycle   = I2C_FM_DUTY_2;
	I2C1handle.I2C_Config.I2C_SCLSpeed      = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1handle);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOCtn;

	//this is btn gpio configuration
	GPIOCtn.pGPIOx = GPIOC;
	GPIOCtn.Gpio_PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	GPIOCtn.Gpio_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT;
	GPIOCtn.Gpio_PinConfig.GPIO_PinSpeed = GPIO_MODE_FAST_SPEED;
	GPIOCtn.Gpio_PinConfig.GPIO_PuPdcontrol = GPIO_MODE_PUP;

	GPIO_Init(&GPIOCtn);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv)
{
    if(AppEv == I2C_EV_TX_CMPLT)
    {
    	//Do nothing yet
    }
    else if (AppEv == I2C_EV_RX_CMPLT)
    {
    	//Do nothing yet
    }
    else if (AppEv == I2C_ERROR_AF)
    {
        /* Master ACK failure happens when slave fails to send ACK
    	   for the byte sent from the master */
        I2C_CloseSendData(pI2CHandle);

        /* Generating stop condition to release the bus */
        I2C_GenerateStopCondition(I2C1);

        /* Hanging in infinite loop */
        while(1);
    }
}

void I2C1_EV_IRQHandler(void)
{
    I2C_EV_IRQHandling(&I2C1handle);
}


void I2C1_ER_IRQHandler(void)
{
    I2C_ER_IRQHandling(&I2C1handle);
}

