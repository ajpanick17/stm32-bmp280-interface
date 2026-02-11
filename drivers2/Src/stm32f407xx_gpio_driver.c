/*
 * stm32f407xx_gpio_driver.c
 *
 * Author: ajayp
 */

#include "stm32f407XX_gpio_driver.h"


void GPIO_PeriClockControl(GPIO_Regdef_t *pGPIOx, uint8_t EnOrDi)
{
		if(EnOrDi == ENABLE)
		{
	        if(pGPIOx == GPIOA)
	        {
	        	GPIOA_CLK_EN();
	        }
	        else if(pGPIOx == GPIOB)
	        {
	        	GPIOB_CLK_EN();
	        }
	        else if(pGPIOx == GPIOC)
	        {
	        	GPIOC_CLK_EN();
	        }
	        else if(pGPIOx == GPIOD)
	        {
	        	GPIOD_CLK_EN();
	        }
	        else if(pGPIOx == GPIOE)
	        {
	        	GPIOE_CLK_EN();
	        }
	        else if(pGPIOx == GPIOH)
	        {
	        	GPIOH_CLK_EN();
	        }
	    }
	    else
	    {
	        // Disable clock
	        if(pGPIOx == GPIOA)
	        {
	        	GPIOA_CLK_DI();
	        }
	        else if(pGPIOx == GPIOB)
	        {
	        	GPIOB_CLK_DI();
	        }
	        else if(pGPIOx == GPIOC)
	        {
	        	GPIOC_CLK_DI();
	        }
	        else if(pGPIOx == GPIOD)
	        {
	        	GPIOD_CLK_DI();
	        }
	        else if(pGPIOx == GPIOE)
	        {
	        	GPIOE_CLK_DI();
	        }
	        else if(pGPIOx == GPIOH)
	        {
	        	GPIOH_CLK_DI();
	        }
	    }
}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp;

	//Configure clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx,ENABLE);

	//Configure mode
	if(pGPIOHandle->Gpio_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		temp = ((pGPIOHandle->Gpio_PinConfig.GPIO_PinMode) << (2 * pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~( 0x3 << (2 * pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber)); //clearinG
		pGPIOHandle->pGPIOx->MODER |= temp;
	}
	else
	{
		//interrupts will code later
	}


	//Configure speed
	temp = ((pGPIOHandle->Gpio_PinConfig.GPIO_PinSpeed) << (2 * pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~( 0x3 << (2 * pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber)); //clearinG
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	//Configure PuPd
	temp = (pGPIOHandle->Gpio_PinConfig.GPIO_PuPdcontrol << ( 2 * pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber) );
	pGPIOHandle->pGPIOx->PUPDR &= ~( 0x3 << ( 2 * pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber)); //clearing
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	//4. configure the optype
	temp = (pGPIOHandle->Gpio_PinConfig.GPIO_PinOPType << pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber );
	pGPIOHandle->pGPIOx->OTYPER &= ~( 0x1 << pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber); //clearing
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	//5. configure the alt functionality
	if(pGPIOHandle->Gpio_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
	   //configure the alt function registers.
		uint8_t temp1, temp2;

		temp1 = pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->Gpio_PinConfig.GPIO_PinNumber  % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << ( 4 * temp2 ) ); //clearing
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->Gpio_PinConfig.GPIO_PinAltFuncMode << ( 4 * temp2 ) );
	}
}



void GPIO_DeInit(GPIO_Regdef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
		{
			GPIOA_REG_RESET();
		}else if (pGPIOx == GPIOB)
		{
			GPIOB_REG_RESET();
		}else if (pGPIOx == GPIOC)
		{
			GPIOC_REG_RESET();
		}else if (pGPIOx == GPIOD)
		{
			GPIOD_REG_RESET();
		}else if (pGPIOx == GPIOE)
		{
			GPIOE_REG_RESET();
		}else if (pGPIOx == GPIOH)
		{
			GPIOF_REG_RESET();
		}

}

uint8_t  GPIO_ReadFromInputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;

	value = (uint8_t )((pGPIOx->IDR >> PinNumber) & 0x00000001) ;

	return value;
}


uint16_t GPIO_ReadFromInputPort(GPIO_Regdef_t *pGPIOx)
{
	uint16_t value;

	value = (uint16_t)pGPIOx->IDR;

	return value;
}

void GPIO_WriteToOutputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber, uint8_t value)
{
	if(value == GPIO_SET)
	{
		//write 1 to the output data register at the bit field corresponding to the pin number
		pGPIOx->ODR |= ( 1 << PinNumber);
	}
	else
	{
		//write 0
		pGPIOx->ODR &= ~( 1 << PinNumber);
	}
}

void GPIO_WriteToOutputPort(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber, uint8_t value)
{
	pGPIOx->ODR  = value;
}

void GPIO_ToggleOutputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR  ^= ( 1 << PinNumber);
}
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority,uint8_t IRQEnOrDI);

void GPIO_IRQHandling(uint8_t PinNumber);

