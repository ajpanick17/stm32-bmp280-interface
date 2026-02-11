#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PuPdcontrol;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFuncMode;
}GPIO_PinConfig_t;

typedef struct
{
	GPIO_Regdef_t *pGPIOx;
	GPIO_PinConfig_t Gpio_PinConfig;
}GPIO_Handle_t;

//defining PIN numbers
#define GPIO_PIN_0	0
#define GPIO_PIN_1	1
#define GPIO_PIN_2	2
#define GPIO_PIN_3	3
#define GPIO_PIN_4	4
#define GPIO_PIN_5	5
#define GPIO_PIN_6	6
#define GPIO_PIN_7	7
#define GPIO_PIN_8	8
#define GPIO_PIN_9	9
#define GPIO_PIN_10	10
#define GPIO_PIN_11	11
#define GPIO_PIN_12	12
#define GPIO_PIN_13	13
#define GPIO_PIN_14	14
#define GPIO_PIN_15	15


//defining possible modes of pin
#define GPIO_MODE_INPUT    0
#define GPIO_MODE_OUTPUT   1
#define GPIO_MODE_ALTFN    2
#define GPIO_MODE_ANALOG   3
#define GPIO_MODE_IT_FT    4
#define GPIO_MODE_IT_RT    5
#define GPIO_MODE_IT_RF    6


//defining possible TYPE of pin
#define GPIO_MODE_O_PP 	   0
#define GPIO_MODE_O_OD     1


//defining possible speed of pin
#define GPIO_MODE_LOW_SPEED 	   0
#define GPIO_MODE_MED_SPEED        1
#define GPIO_MODE_FAST_SPEED 	   2
#define GPIO_MODE_HIGH_SPEED       3


//defining possible PULL-UP of pin
#define GPIO_MODE_NO_PUP 	   	0
#define GPIO_MODE_PUP        	1
#define GPIO_MODE_PUD 	   		2
#define GPIO_MODE_RESERVED      3

void GPIO_PeriClockControl(GPIO_Regdef_t *pGPIOx, uint8_t EnOrDi);

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_Regdef_t *pGPIOx);

uint8_t  GPIO_ReadFromInputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_Regdef_t *pGPIOx);

void GPIO_WriteToOutputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber, uint8_t value);

void GPIO_ToggleOutputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber);
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority,uint8_t IRQEnOrDI);

void GPIO_IRQHandling(uint8_t PinNumber);


















#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
