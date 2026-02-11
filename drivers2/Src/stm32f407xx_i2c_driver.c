/*
 * stm32f407xx_i2c_driver.c
 *Author: ajayp
 */
#include "stm32f407xx_i2c_driver.h"


static void I2C_GenerateStartCondition(I2C_Regdef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_Regdef_t *pI2Cx,uint8_t SlaveAddress);
static void I2C_ExecuteAddressPhaseRead(I2C_Regdef_t *pI2Cx,uint8_t SlaveAddress);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);

uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PreScaler[4] = { 2, 4 , 8, 16};


/*********************************************************************
 * @fn      		  - I2C_GenerateStartCondition
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
static void I2C_GenerateStartCondition(I2C_Regdef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}


/*********************************************************************
 * @fn      		  - I2C_GenerateStopCondition
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
 void I2C_GenerateStopCondition(I2C_Regdef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}
/*********************************************************************
 * @fn      		  - I2C_ExecuteAddressPhaseWrite
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
static void I2C_ExecuteAddressPhaseWrite(I2C_Regdef_t *pI2Cx,uint8_t SlaveAddress)
{
	SlaveAddress = SlaveAddress << 1;
	SlaveAddress &= ~(1);
	pI2Cx->DR = SlaveAddress;
}

/*********************************************************************
 * @fn      		  - I2C_ExecuteAddressPhaseRead
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
static void I2C_ExecuteAddressPhaseRead(I2C_Regdef_t *pI2Cx,uint8_t SlaveAddress)
{
	SlaveAddress = SlaveAddress << 1;
	SlaveAddress |= 1;
	pI2Cx->DR = SlaveAddress;
}
/*********************************************************************
 * @fn      		  - I2C_ClearADDRFlag
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
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
    uint32_t dummy_read;

    /* Checking for device mode */
    if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
    {
        /* Device is in master mode */
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            if(pI2CHandle->RxSize == 1)
            {
                /* Disabling ACK */
                I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);

                /* Clearing ADDR flag, reading SR1 and SR2 */
                dummy_read = pI2CHandle->pI2Cx->SR1;
                dummy_read = pI2CHandle->pI2Cx->SR2;
                (void)dummy_read;
            }
        }
        else
        {
            /* Clearing ADDR flag, reading SR1 and SR2 */
            dummy_read = pI2CHandle->pI2Cx->SR1;
            dummy_read = pI2CHandle->pI2Cx->SR2;
            (void)dummy_read;
        }
    }
    else
    {
        /* Device is in slave mode */
        /* Clearing ADDR flag, reading SR1 and SR2 */
        dummy_read = pI2CHandle->pI2Cx->SR1;
        dummy_read = pI2CHandle->pI2Cx->SR2;
        (void)dummy_read;
    }
}





/*********************************************************************
 * @fn      		  - RCC_GetPCLK1Value
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
uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1,SystemClk;

	uint8_t clksrc,temp,ahbp,apb1p;

	clksrc = ((RCC->CFGR >> 2) & 0x3);

	if(clksrc == 0 )
	{
		SystemClk = 16000000;
	}else if(clksrc == 1)
	{
		SystemClk = 8000000;
	}else if (clksrc == 2)
	{
		SystemClk = RCC_GetPLLOutputClock();
	}

	//for ahb
	temp = ((RCC->CFGR >> 4 ) & 0xF);

	if(temp < 8)
	{
		ahbp = 1;
	}else
	{
		ahbp = AHB_PreScaler[temp-8];
	}



	//apb1
	temp = ((RCC->CFGR >> 10 ) & 0x7);

	if(temp < 4)
	{
		apb1p = 1;
	}else
	{
		apb1p = APB1_PreScaler[temp-4];
	}

	pclk1 =  (SystemClk / ahbp) /apb1p;

	return pclk1;
}
/*********************************************************************
 * @fn      		  - I2C_PeripheralControl
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
void I2C_PeripheralControl(I2C_Regdef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
		//pI2cBaseAddress->CR1 |= I2C_CR1_PE_Bit_Mask;
	}
	else
	{
		pI2Cx->CR1 &= ~(1 << 0);
	}

}


/*********************************************************************
 * @fn      		  - I2C_PeriClockControl
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
void I2C_PeriClockControl(I2C_Regdef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_CLK_EN();
		}else if (pI2Cx == I2C2)
		{
			I2C2_CLK_EN();
		}else if (pI2Cx == I2C3)
		{
			I2C3_CLK_EN();
		}
	}
	else
	{
		//TODO
	}

}
/*********************************************************************
 * @fn      		  - I2C_Init
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
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0;

	//Clock initialization
	I2C_PeriClockControl(pI2CHandle->pI2Cx,ENABLE);

	//Acknowledgment control - Enable or Disable
	tempreg |= ((pI2CHandle->I2C_Config.I2C_ACKControl) << 10);
	pI2CHandle->pI2Cx->CR1= tempreg;

	//Freq field in CR2
	tempreg = 0;
    tempreg |= RCC_GetPCLK1Value()/1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);

	//Device address config
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1<<14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	//CCR calculations
	uint16_t ccr_value = 0;
	tempreg = 0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//mode is standard mode
		ccr_value = (RCC_GetPCLK1Value() / ( 2 * pI2CHandle->I2C_Config.I2C_SCLSpeed ) );
		tempreg |= (ccr_value & 0xFFF);
	}else
	{
		//mode is fast mode
		tempreg |= ( 1 << 15);
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPCLK1Value() / ( 3 * pI2CHandle->I2C_Config.I2C_SCLSpeed ) );
		}else
		{
			ccr_value = (RCC_GetPCLK1Value() / ( 25 * pI2CHandle->I2C_Config.I2C_SCLSpeed ) );
		}
			tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	//TRISE Configuration
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//mode is standard mode
		tempreg = (RCC_GetPCLK1Value() /1000000U) + 1 ;

	}
	else
	{
		//mode is fast mode
		tempreg = ( (RCC_GetPCLK1Value() * 300) / 1000000000U ) + 1;

	}

	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);
}
/*********************************************************************
 * @fn      		  - I2C_GetFlagStatus
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
uint8_t I2C_GetFlagStatus(I2C_Regdef_t *pI2Cx , uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}
/*********************************************************************
 * @fn      		  - I2C_ManageAcking
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
void I2C_ManageAcking(I2C_Regdef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		//enable the ack
		pI2Cx->CR1 |= ( 1 << I2C_CR1_ACK);
	}else
	{
		//disable the ack
		pI2Cx->CR1 &= ~( 1 << I2C_CR1_ACK);
	}
}
/*********************************************************************
 * @fn      		  - I2C_MasterSendData
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
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
    //1. Generate start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm the start condition is completed by checking the SB Flag in the SR1
	//   note: until SB is cleared, the SCL will be stretched to low
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	//3. Now send address of the slave with R/w bit to 0 (Write operation)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

	//5. clear the ADDR flag according to its software sequence
	//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle);

	//6. send the data until len becomes 0

		while(Len > 0)
		{
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE) ); //Wait till TXE is set
			pI2CHandle->pI2Cx->DR = *pTxbuffer;
			pTxbuffer++;
			Len--;
		}

		//7. when Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
		//   Note: TXE=1 , BTF=1 , means that both SR and DR are empty and next transmission should begin
		//   when BTF=1 SCL will be stretched (pulled to LOW)

		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE) );

		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF) );


		//8. Generate STOP condition and master need not to wait for the completion of stop condition.
		//   Note: generating STOP, automatically clears the BTF
		if(Sr == I2C_DISABLE_SR )
		{
		   I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}

}
/*********************************************************************
 * @fn      		  - I2C_MasterReceiveData
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
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	//1. Generate start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm the start condition is completed by checking the SB Flag in the SR1
	//   note: until SB is cleared, the SCL will be stretched to low
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	//3. Now send address of the slave with R/w bit to 1 (Read operation)
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR) )
	{

	}

	//procedure to read only 1 byte from slave
		if(Len == 1)
		{
			//Disable Acking
			I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);


			//clear the ADDR flag
			I2C_ClearADDRFlag(pI2CHandle);

			//wait until  RXNE becomes 1
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

			//generate STOP condition
			if(Sr == I2C_DISABLE_SR )
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

			//read data in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;

		}


	    //procedure to read data from slave when Len > 1
		if(Len > 1)
		{
			//clear the ADDR flag
			I2C_ClearADDRFlag(pI2CHandle);

			//read the data until Len becomes zero
			for ( uint32_t i = Len ; i > 0 ; i--)
			{
				//wait until RXNE becomes 1
				while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

				if(i == 2) //if last 2 bytes are remaining
				{
					//Disable Acking
					I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

					//generate STOP condition
					if(Sr == I2C_DISABLE_SR )
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

				}

				//read the data from data register in to buffer
				*pRxBuffer = pI2CHandle->pI2Cx->DR;

				//increment the buffer address
				pRxBuffer++;
			}
		}

		//re-enable ACKing
		if(pI2CHandle->I2C_Config.I2C_ACKControl== I2C_ACK_ENABLE)
		{
			I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
		}

}

void I2C_Byte_Read(I2C_Handle_t *pI2CHandle,uint8_t saddr, uint8_t maddr,uint8_t *data)
{

	while(pI2CHandle->pI2Cx->SR2 & I2C_FLAG_BUSY);

	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,saddr);

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

	I2C_ClearADDRFlag(pI2CHandle);

	pI2CHandle->pI2Cx->DR = maddr;  // Transmitting memory address

	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE));

	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);   // Restart transmission

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,saddr);

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

	I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

	I2C_ClearADDRFlag(pI2CHandle);

	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

	*data++ = pI2CHandle->pI2Cx->DR;
}

void I2C_Burst_Read(I2C_Handle_t *pI2CHandle,uint8_t saddr, uint8_t maddr,uint8_t length,uint8_t *data)
{
	while(pI2CHandle->pI2Cx->SR2 & I2C_FLAG_BUSY);

	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,saddr);

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

	I2C_ClearADDRFlag(pI2CHandle);

	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE));

	pI2CHandle->pI2Cx->DR = maddr;  // Transmitting memory address

	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE));

	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);   // Restart transmission

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,saddr);

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

	I2C_ClearADDRFlag(pI2CHandle);

	I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);

	if(length > 1)
	{
		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//read the data until Len becomes zero
		for ( uint32_t i = length ; i > 0 ; i--)
		{
			//wait until RXNE becomes 1
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

			if(i == 2) //if last 2 bytes are remaining
			{
						//Disable Acking
						I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

						//generate STOP condition
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

			}

					//read the data from data register in to buffer
					*data++ = pI2CHandle->pI2Cx->DR;
		}
	}
			I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
}

void I2C_Burst_Write(I2C_Handle_t *pI2CHandle,uint8_t saddr, uint8_t maddr,uint8_t length,uint8_t *data)
{
	while(pI2CHandle->pI2Cx->SR2 & I2C_FLAG_BUSY);

	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,saddr);

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

	I2C_ClearADDRFlag(pI2CHandle);

	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE));

	pI2CHandle->pI2Cx->DR = maddr;  // Transmitting memory address

	for(int i=0; i < length; i++)
	{
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE)){};

		 pI2CHandle->pI2Cx->DR  = *data++;
	}

	while(!(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF)));

	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}

uint8_t  I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);


		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);


		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);


	}

	return busystate;

}

uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{

 	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);


		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);


		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}

	return busystate;
}

/*
 * IRQ Configuration and ISR handling
 */
/*****************************************************************
 * @fn          - I2C_IRQInterruptConfig
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
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            /* Program ICER1 register (32 to 63) */
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            /* Program ICER2 register (64 to 95) */
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    }
}

void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;

    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	//Interrupt handling for both master and slave mode of a device

	uint32_t temp1,temp2,temp3;

	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB);

	//1.Handle for interrupt generated by SB event
	//Note: SB is only applicable for master mode

	if(temp1 && temp3)
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,pI2CHandle->DevAddr);
		}
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,pI2CHandle->DevAddr);
		}
	}

	//2. Handle for interrupt generated by ADDR event
	//Note: When master mode : Address is set
    //      When Slave mode  : Address is matched with Own address
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);

	if(temp1 && temp3)
	{
		I2C_ClearADDRFlag(pI2CHandle);
	}

	//3.Handle for interrupt generated by BTF
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);
	if(temp1 && temp3)
	{
			/* BTF flag is set */
		    if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		    {
		        /* TXE flag is also set */
		         if(pI2CHandle->pI2Cx->SR1 && (1 << I2C_SR1_TXE))
		         {
		        	 //BTF, TXE = 1
		        	 if(pI2CHandle->TxLen == 0)
		             {
		                  /* Generate STOP condition */
		                  if(pI2CHandle->Sr == I2C_DISABLE_SR)
		                  {
		                     I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		                  }

		                   /* Reset all member elements of the handle structure */
		                   I2C_CloseSendData(pI2CHandle);

		                   /* Notify application about transmission complete */
		                   I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
		             }
		          }
		       }
		       else if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		       {
		           ;
		       }
	}

	//4.Handle for interrupt generated by STOPF
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);
	if(temp1 && temp3)
	{
		//STOF flag is set
		//Clear the STOPF ( i.e 1) read SR1 2) Write to CR1 )
	    pI2CHandle->pI2Cx->CR1 |= 0x0000;

	    /* STOPF generated by master */
	    I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
	 }

	//5.Handle for interrupt generated by TXE
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE);
	/* Handling for interrupt generated by TXE event */
	if(temp1 && temp2 && temp3)
	{
	    /* Checking device mode */
	    if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	    {
	    	//TXE flag is set
			//We have to do the data transmission
	        if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
	        {
	              I2C_MasterHandleTXEInterrupt(pI2CHandle);
	        }
	    }
	    else
	    {
	        /* Checking if slave is really in Tx mode */
	        if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA))
	        {
	              I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_REQ);
	         }
	     }
	 }

	//5.Handle for interrupt generated by RXXE
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RXNE);
	 if(temp1 && temp2 && temp3)
	 {
	        /* Checking device mode */
	        if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	        {
				//The device is master
	            /* RXNE flag is set */
	            if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
	            {
	                I2C_MasterHandleRXNEInterrupt(pI2CHandle);
	            }
	        }
	        else
	        {
	            /* Checking if slave is really in Rx mode */
	            if(!(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA)))
	            {
	                I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_RCV);
	            }
	        }
	   }

}

/*****************************************************************
 * @fn          - I2C_ER_IRQHandling
 *
 * @brief       - Interrupt handling for different I2C errors
 *
 * @param[in]   - Pointer to I2C Handle structure
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{
    uint32_t temp1, temp2;

    /* Status of ITERREN control bit in CR2 */
    temp2 = (pI2CHandle->pI2Cx->CR2) & (1 << I2C_CR2_ITERREN);

    /***********************Check for Bus error************************************/

    /* Checking for bus error */
    temp1 = (pI2CHandle->pI2Cx->SR1) & (1 << I2C_SR1_BERR);
    if(temp1 && temp2)
    {
        /* Clear the bus error flag */
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_BERR);

        /* Notify application about error */
        I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_BERR);
    }

    /***********************Check for arbitration lost error************************************/

    /* Checking for arbitration lost error */
    temp1 = (pI2CHandle->pI2Cx->SR1) & (1 << I2C_SR1_ARLO);
    if(temp1 && temp2)
    {
        /* Clear the arbitration lost error flag */
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_ARLO);

        /* Notify application about error */
        I2C_ApplicationEventCallback(pI2CHandle, I2C_SR1_ARLO);
    }

    /***********************Check for ACK failure  error************************************/

    /* Checking for ACK failure error */
    temp1 = (pI2CHandle->pI2Cx->SR1) & (1 << I2C_SR1_AF);
    if(temp1 && temp2)
    {
        /* Clear the ACK failure error flag */
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_AF);

        /* Notify application about error */
        I2C_ApplicationEventCallback(pI2CHandle, I2C_SR1_AF);
    }

    /***********************Check for Overrun/underrun error************************************/

    /* Checking for Overrun/Underrun error */
    temp1 = (pI2CHandle->pI2Cx->SR1) & (1 << I2C_SR1_OVR);
    if(temp1 && temp2)
    {
        /* Clear the Overrun/Underrun error flag */
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_OVR);

        /* Notify application about error */
        I2C_ApplicationEventCallback(pI2CHandle, I2C_SR1_OVR);
    }

    /***********************Check for Time out error************************************/

    /* Checking for Time out error */
    temp1 = (pI2CHandle->pI2Cx->SR1) & (1 << I2C_SR1_TIMEOUT);
    if(temp1 && temp2)
    {
        /* Clear the Time out error flag */
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_TIMEOUT);

        /* Notify application about error */
        I2C_ApplicationEventCallback(pI2CHandle, I2C_SR1_TIMEOUT);
    }
}

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
    /* Disabling ITBUFEN Control Bit */
    pI2CHandle->pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);

    /* Disabling ITEVFEN Control Bit */
    pI2CHandle->pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);

    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pRxBuffer = 0;
    pI2CHandle->RxLen = 0;
    pI2CHandle->RxSize = 0;

    if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
    {
        I2C_ManageAcking(pI2CHandle->pI2Cx,ENABLE);
    }

}

/*****************************************************************
 * @fn          - I2C_CloseSendData
 *
 * @brief       - Closing I2C communication when data is sent
 *
 * @param[in]   - Pointer to I2C Handle structure
 *
 * @return      - None
 *
 * @Note        - Disabling all interrupts
 *
 *****************************************************************/
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
    /* Disabling ITBUFEN Control Bit */
    pI2CHandle->pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);

    /* Disabling ITEVFEN Control Bit */
    pI2CHandle->pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);


    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pTxBuffer = 0;
    pI2CHandle->TxLen = 0;
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
    /* TXE flag is set */
    if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
    {
        if(pI2CHandle->TxLen > 0)
        {
            /* Loading data into DR */
            pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);

            /* Decrement Tx Length */
            pI2CHandle->TxLen--;

            /* Increment Tx buffer address */
            pI2CHandle->pTxBuffer++;
        }
    }
    /* DBG->Check comments */
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
    if(pI2CHandle->RxSize == 1)
    {
        *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
        pI2CHandle->RxLen--;
    }

    if(pI2CHandle->RxSize > 1)
    {
        if(pI2CHandle->RxLen == 2)
        {
            /* Clearing ACK bit */
            I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
        }

        /* Reading DR */
        *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
        pI2CHandle->pRxBuffer++;
        pI2CHandle->RxLen--;
    }

    if(pI2CHandle->RxLen == 0)
    {
        /* Closing I2C data reception and notifying application */

        /* Generating stop condition */
        if(pI2CHandle->Sr == I2C_DISABLE_SR)
        {
            I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
        }

        /* Closing I2C RX */
        I2C_CloseReceiveData(pI2CHandle);

        /* Notify application about transmission complete */
        I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
    }
}

uint32_t RCC_GetPLLOutputClock(void)
{
	return 0;
}

