#include "stm32f407xx_spi_driver.h"

/*
 * Helper functions
 */
static void SPI_TXE_InterruptHandle(SPI_Handle_t *pSPIHandle);
static void SPI_RXNE_InterruptHandle(SPI_Handle_t *pSPIHandle);
static void SPI_OVR_ErrInterruptHandle(SPI_Handle_t *pSPIHandle);

/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             - Enabling clock for SPI
 *
 * @param[in]         -SPI_RegDef_t *pSPIx
 * @param[in]         -EnorDi
 * @param[in]         -
 *
 * @return            - None
 *
 * @Note              -

 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		   if(pSPIx == SPI1)
		   {
			   SPI1_CLK_EN();
		   }
		   else if(pSPIx == SPI2)
		   {
			   SPI2_CLK_EN();
		   }
		   else if(pSPIx == SPI3)
		   {
			   SPI3_CLK_EN();
		   }
		   else if(pSPIx == SPI4)
		   {
			   SPI4_CLK_EN();
		   }
		   else if(pSPIx == SPI5)
		   {
			   SPI5_CLK_EN();
		   }
	 }
		    else
		    {
		       // To do
		    }
}

/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	uint32_t tempRegValue =0;

	SPI_PeriClockControl(pSPIHandle->pSPIx,ENABLE);

	// Device mode configuration -> Master or slave(Choosing master)
	tempRegValue |= ((pSPIHandle->SPIConfig.SPI_DeviceMode)<<2);

	//Bus configuration
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		tempRegValue &= ~((1<<15));
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		tempRegValue |= ((1<<15));
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		tempRegValue &= ~((1<<15));
		tempRegValue |= ((1<<10));
	}

	//Data frame configuration
	tempRegValue |= ((pSPIHandle->SPIConfig.SPI_DFF)<<11);

	//Clock speed configuration
	tempRegValue |= ((pSPIHandle->SPIConfig.SPI_SclkSpeed)<<3);

	//CPOL CONFIGURATION
	tempRegValue |= ((pSPIHandle->SPIConfig.SPI_CPOL)<<1);

	//CPHA CONFIGURATION
	tempRegValue |= ((pSPIHandle->SPIConfig.SPI_CPHA)<<0);

	//SSM CONFIGURATION
	tempRegValue |= ((pSPIHandle->SPIConfig.SPI_SSM)<<9);

	//assign value to CR1 register
	pSPIHandle->pSPIx->CR1 = tempRegValue;



}

void SPI_DeInit(SPI_RegDef_t *pSPIx);


uint8_t SPI_getFlagStatus(SPI_RegDef_t *pSPIx,uint32_t flag)
{
	if(pSPIx->SR & flag)
	{
		return FLAG_SET;
	}

	return FLAG_RESET;

}


/*
 * Data Send and Receive, Txe(if "0" then not empty, else empty)
 */

void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len>0)
	{
		while ((pSPIx->SR & (1U<<1)) == 0) { }
		//check dff bit in cr1
		if(pSPIx->CR1 & (1<<11))
		{
			//16 bit DFF
			//1. load the data in to the DR
			pSPIx->DR =   *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}
		else
		{
			 //8 bit DFF
			pSPIx->DR =   *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << 6);
	}else
	{
		pSPIx->CR1 &=  ~(1 << 6);
	}


}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1<<8);
	}
	else
	{
		pSPIx->CR1 |= ~(1<<8);
	}

}
/*****************************************************************
 * @fn          - SPI_IRQConfig
 *
 * @brief       - This function configures interrupt
 *
 * @param[in]   - IRQ Interrupt number
 * @param[in]   - Macro: Enable/Disable
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(IRQNumber <= 31)
        {
            /* Program ISER0 register */
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            /* Program ISER1 register (32 to 63) */
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            /* Program ISER2 register (64 to 95) */
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    }
    else
    {
        if(IRQNumber <= 31)
        {
            /* Program ICER0 register */
            *NVIC_ICER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            /* Program ICER1 register (32 to 63) */
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            /* Program ICER2 register (64 to 95) */
            *NVIC_ICER2 |= (1 << (IRQNumber % 64));
        }
    }
}


/*****************************************************************
 * @fn          - SPI_IRQPriorityConfig
 *
 * @brief       - This function configures interrupt priority
 *
 * @param[in]   - IRQ Interrupt number
 * @param[in]   - IRQ interrupt priority
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;

    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

uint8_t SPI_SendData_IT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t Curr_state;
	Curr_state = pSPIHandle->TxState;

	if(Curr_state != SPI_BUSY_IN_TX)
	{
		//1.Save transmit buffer address and length information in global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen     = Len;

		//2. Mark the state of SPI transmission in busy so that no other code can take over the SPI
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3. Enable TXEIE control bit to get interrupt whenever TXE flag is set in CR
		pSPIHandle->pSPIx->CR2 |= (1 << 7) ;
	}
    return Curr_state;
}
//--------------------------------------------------------------------------------------------------//
uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
		uint8_t Curr_state;
		Curr_state = pSPIHandle->RxState;

		if(Curr_state != SPI_BUSY_IN_RX)
		{
			//1.Save transmit buffer address and length information in global variables
			pSPIHandle->pRxBuffer = pRxBuffer;
			pSPIHandle->RxLen     = Len;

			//2. Mark the state of SPI transmission in busy so that no other code can take over the SPI
			pSPIHandle->RxState = SPI_BUSY_IN_RX;

			//3. Enable TXEIE control bit to get interrupt whenever TXE flag is set in CR
			pSPIHandle->pSPIx->CR2 |= (1 << 6) ;
		}
	    return Curr_state;
}

/*****************************************************************
 * @fn          - SPI_IRQHandling
 *
 * @brief       - This function handle interrupts
 *
 * @param[in]   - Handle structure
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
    uint8_t temp1;
    uint8_t temp2;

    /* Check for TXE */
    temp1 = pHandle->pSPIx->SR & (1 << 1);
    temp2 = pHandle->pSPIx->CR2 & (1 << 7);

    if(temp1 && temp2)
    {
        /* Handle TXE */
        SPI_TXE_InterruptHandle(pHandle);
    }

    /* Check for RXNE */
    temp1 = pHandle->pSPIx->SR & (1 << 0);
    temp2 = pHandle->pSPIx->CR2 & (1 << 6);

    if(temp1 && temp2)
    {
        /* Handle RXNE */
        SPI_RXNE_InterruptHandle(pHandle);
    }

    /* Check for OVR flag */
    temp1 = pHandle->pSPIx->SR & (1 << 6);
    temp2 = pHandle->pSPIx->CR2 & (1 << 5);

    if(temp1 && temp2)
    {
        /* Handle OVR Error */
        SPI_OVR_ErrInterruptHandle(pHandle);
    }

}

/*****************************************************************/
static void SPI_TXE_InterruptHandle(SPI_Handle_t *pSPIHandle)
{
	// check the DFF bit in CR1
   if( pSPIHandle->pSPIx->CR1 & (1 << 11) )
   {
       /* Load data into data register */
       /* 16 bit */
       pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
       pSPIHandle->TxLen--;
       pSPIHandle->TxLen--;
       (uint16_t*)pSPIHandle->pTxBuffer++;
   }
   else
   {
       /* 8 bit */
       pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
       pSPIHandle->TxLen--;
       pSPIHandle->pTxBuffer++;
   }

   if(!pSPIHandle->TxLen)
   {
       /* Tx is zero. Close SPI communication and inform application about it.
        * Prevents interrupts from setting up of TXE flag. */
       SPI_CloseTransmission(pSPIHandle);
       SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
   }
}


/*****************************************************************
* @fn          - SPI_RXNE_InterruptHandle
*
* @brief       - This function handles RXNE in interrupt mode
*
* @param[in]   - Pointer to SPI Handle structure
*
* @return      - None
*
* @Note        - None
*
*****************************************************************/
static void SPI_RXNE_InterruptHandle(SPI_Handle_t *pSPIHandle)
{
	//Do rxing as per the dff
   if( pSPIHandle->pSPIx->CR1 & (1 << 11) )
   {
       /* Load data from data register into buffer */
       /* 16 bit */
       *((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
       pSPIHandle->RxLen--;
       pSPIHandle->RxLen--;
       (uint16_t*)pSPIHandle->pRxBuffer++;
   }
   else
   {
       /* 8 bit */
       *pSPIHandle->pRxBuffer = pSPIHandle->pSPIx->DR;
       pSPIHandle->RxLen--;
       pSPIHandle->pRxBuffer++;
   }

   if(!pSPIHandle->RxLen)
   {
       /* Rx is zero. Close SPI communication and inform application about it.
        * Prevents interrupts from setting up of RXNE flag. */
       SPI_CloseReception(pSPIHandle);
       SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
   }
}


/*****************************************************************
* @fn          - SPI_OVR_ErrInterruptHandle
*
* @brief       - This function handles OVR_ERR in
*                interrupt mode
*
* @param[in]   - Pointer to SPI Handle structure
*
* @return      - None
*
* @Note        - None
*
*****************************************************************/
static void SPI_OVR_ErrInterruptHandle(SPI_Handle_t *pSPIHandle)
{
   uint8_t temp;

   /* Clear OVR flag */
   if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
   {
       temp = pSPIHandle->pSPIx->DR;
       temp = pSPIHandle->pSPIx->SR;
   }
   (void)temp;

   /* Inform application */
   SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}
/*****************************************************************
 * @fn          - SPI_CloseTransmission
 *
 * @brief       - This function close SPI transmission
 *
 * @param[in]   - Handle structure
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
    pSPIHandle->pSPIx->CR2 &= ~(1 << 7);
    pSPIHandle->pTxBuffer = 0;
    pSPIHandle->TxLen = 0;
    pSPIHandle->TxState = SPI_READY;
}


/*****************************************************************
 * @fn          - SPI_CloseReception
 *
 * @brief       - This function close SPI reception
 *
 * @param[in]   - Handle structure
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
    pSPIHandle->pSPIx->CR2 &= ~(1 << 6);
    pSPIHandle->pRxBuffer = 0;
    pSPIHandle->RxLen = 0;
    pSPIHandle->RxState = SPI_READY;
}


/*****************************************************************
 * @fn          - SPI_ClearOVRFlag
 *
 * @brief       - This function clears OVR flag
 *
 * @param[in]   - Base address of the SPI peripheral
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
    uint8_t temp;
    temp = pSPIx->DR;
    temp = pSPIx->SR;
    (void)temp;
}


/*****************************************************************
 *               Helper functions implementation                 *
 *****************************************************************/
/*****************************************************************
 * @fn          - SPI_ApplicationEventCallback
 *
 * @brief       - Application event callback function
 *
 * @param[in]   - Handle structure
 * @param[in]   - Application event
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEvent)
{
    /* This is a week implementation. The application may override this function. */
}
//---------------------------------------------------------------------------------------------------//
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);
//uint8_t I2C_DeviceMode(I2C_RegDef_t *I2Cx);
