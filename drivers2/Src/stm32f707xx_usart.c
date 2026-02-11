/*
 * stm32f707xx_usart.c
 *
 */

#include "stm32f407xx_usart_driver.h"

extern uint32_t RCC_GetPCLK1Value(void);
extern uint32_t RCC_GetPCLK2Value(void);
/*
 * Peripheral Clock setup
 */
/*****************************************************************
 * @fn          - USART_PeriClockControl
 *
 * @brief       - This function enables or disables peripheral
 *                clock for the given U(S)ART port
 *
 * @param[in]   - Base address of the U(S)ART peripheral
 * @param[in]   - Macros: Enable or Disable
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pUSARTx == USART1)
        {
        	USART1_CLK_EN();
        }
        else if(pUSARTx == USART2)
        {
        	USART2_CLK_EN();
        }
        else if(pUSARTx == USART6)
        {
        	USART6_CLK_EN();
        }
    }
    else
    {
        if(pUSARTx == USART1)
        {
        	USART1_CLK_DI();
        }
        else if(pUSARTx == USART2)
        {
        	USART2_CLK_DI();
        }
        else if(pUSARTx == USART6)
        {
        	USART6_CLK_DI();
        }
    }
}

/*****************************************************************
 * @fn          - USART_PeripheralControl
 *
 * @brief       - This function sets U(S)ART peripheral control
 *
 * @param[in]   - Base address of the U(S)ART peripheral
 * @param[in]   - Enable or Disable command
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        pUSARTx->CR1 |= (1 << USART_CR1_UE);
    }
    else
    {
        pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
    }
}

/*********************************************************************
 * @fn      		  - USART_Init
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs

 */
void USART_Init(USART_Handle_t *pUSARTHandle)
{

	//Temporary variable
	uint32_t tempreg=0;

/******************************** Configuration of CR1******************************************/

	//Implement the code to enable the Clock for given USART peripheral
	USART_PeriClockControl(pUSARTHandle->pUSARTx,ENABLE);

	//Enable USART Tx and Rx engines according to the USART_Mode configuration item
	if ( pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		//Implement the code to enable the Receiver bit field
		tempreg|= (1 << USART_CR1_RE);
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		//Implement the code to enable the Transmitter bit field
		tempreg|= (1 << USART_CR1_TE);

	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields
		tempreg |= ( ( 1 << USART_CR1_RE) | ( 1 << USART_CR1_TE) );
	}

    //Implement the code to configure the Word length configuration item
	tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M ;


    //Configuration of parity control bit fields
	if ( pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		//Implement the code to enaBle the parity control
		tempreg |= ( 1 << USART_CR1_PCE);

		//Implement the code to enable EVEN parity
		//Not required because by default EVEN parity will be selected once you enable the parity control

	}else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD )
	{
		//Implement the code to enable the parity control
	    tempreg |= ( 1 << USART_CR1_PCE);

	    //Implement the code to enable ODD parity
	    tempreg |= ( 1 << USART_CR1_PS);

	}

   //Program the CR1 register
	pUSARTHandle->pUSARTx->CR1 = tempreg;

/******************************** Configuration of CR2******************************************/

	tempreg=0;

	//Implement the code to configure the number of stop bits inserted during USART frame transmission
	tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;

	//Program the CR2 register
	pUSARTHandle->pUSARTx->CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

	tempreg=0;

	//Configuration of USART hardware flow control
	if ( pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//Implement the code to enable CTS flow control
		tempreg |= ( 1 << USART_CR3_CTSE);


	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//Implement the code to enable RTS flow control
		tempreg |= ( 1 << USART_CR3_RTSE);

	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Implement the code to enable both CTS and RTS Flow control
		 tempreg |= (1 << USART_CR3_CTSE);
		 tempreg |= (1 << USART_CR3_RTSE);
	}


	pUSARTHandle->pUSARTx->CR3 = tempreg;

/******************************** Configuration of BRR(Baudrate register)******************************************/

	//Implement the code to configure the baud rate
	//We will cover this in the lecture. No action required here
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}


/******************************************************************
 * @fn          - USART_ReceiveData
 *
 * @brief       - This function receives data over U(S)ART
 *
 * @param[in]   - Pointer to U(S)ART Handle Structure
 * @param[in]   - Pointer to Rx Buffer
 * @param[in]   - Length of Rx Buffer
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Length)
{
    /* Looping over until "Length" of bytes are transferred */
    for(uint32_t i = 0 ; i < Length; i++)
    {
        /* Waiting till TXE flag is set in SR */
        while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

        /* Checking USART_WordLength item for 9BIT or 8BIT in a frame */
        if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            /* Receiving 9bit data in frame */

            /* Checking if USART_ParityControl control is used or not */
            if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                /* No parity is used. 9bits will be of user data */

                /* Reading only first 9 bits. Masking DR with 0x01FF */
                *((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

                /* Incrementing pRxBuffer address */
                pRxBuffer++;
                pRxBuffer++;
            }
            else
            {
                /* Parity is used. 8bits will be user data and 1 bit is parity */
                /* Reading only 7 bits. Masking DR with 0xFF */
                *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

                /* Incrementing pRxBuffer address */
                pRxBuffer++;
            }
        }
        else
        {
            /* Receiving 8bit data in a frame */
            /* Checking if USART_ParityControl is used or not */
            if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                /* No parity is used. 9bits will be of user data */
                /* Reading 8 bits from DR */
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
            }

            else
            {
                /* Parity is used. 7bits will be user data and 1 bit is parity */
                /* Reading only 7 bits. Masking DR with 0x7F */
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
            }

            /* Incrementing Rx Buffer */
            pRxBuffer++;
        }
    }
}

/******************************************************************
 * @fn          - USART_SendDataInterrupt
 *
 * @brief       - This function sends data over U(S)ART
 *                in interrupt mode
 *
 * @param[in]   - Pointer to U(S)ART Handle Structure
 * @param[in]   - Pointer to Tx Buffer
 * @param[in]   - Length of Tx Buffer
 *
 * @return      - Tx state
 *
 * @Note        - None
 *
 *****************************************************************/
uint8_t USART_SendDataInterrupt(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Length)
{
    uint8_t txstate = pUSARTHandle->TxBusyState;

    if(txstate != USART_BUSY_IN_TX)
    {
        pUSARTHandle->TxLen = Length;
        pUSARTHandle->pTxBuffer = pTxBuffer;
        pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

        /* Enabling interrupt for TXE */
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

        /* Enabling interrupt for TC */
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
    }

    return txstate;

}

/******************************************************************
 * @fn          - USART_ReceiveDataInterrupt
 *
 * @brief       - This function receives data over U(S)ART
 *                in interrupt mode
 *
 * @param[in]   - Pointer to U(S)ART Handle Structure
 * @param[in]   - Pointer to Rx Buffer
 * @param[in]   - Length of Rx Buffer
 *
 * @return      - Rx state
 *
 * @Note        - None
 *
 *****************************************************************/
uint8_t USART_ReceiveDataInterrupt(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Length)
{
    uint8_t rxstate = pUSARTHandle->RxBusyState;

    if(rxstate != USART_BUSY_IN_RX)
    {
        pUSARTHandle->RxLen = Length;
        pUSARTHandle->pRxBuffer = pRxBuffer;
        pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

        /* Enabling RXNE interrupt */
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
    }

    return rxstate;
}

/*****************************************************************
 * @fn          - USART_GetFlagStatus
 *
 * @brief       - This function returns if bit in register is
 *                set or not
 *
 * @param[in]   - Base address of the U(S)ART peripheral
 * @param[in]   - Name of flag
 *
 * @return      - Flag status (True/False)
 *
 * @Note        - None
 *
 *****************************************************************/
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t FlagName)
{
    if(pUSARTx->SR & FlagName)
    {
        return FLAG_SET;
    }
    return FLAG_RESET;
}

/*******************************************************************
 * @fn          - USART_SetBaudRate
 *
 * @brief       - This function sets U(S)ART Baudrate
 *
 * @param[in]   - Base address of the U(S)ART peripheral
 * @param[in]   - Baud rate value
 *
 * @return      - None
 *
 * @Note        - None
 *
 ********************************************************************/
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{
    /* Holds APB clock */
    uint32_t PCLKx;

    uint32_t usartdiv;

    /* Hold Mantissa and Fraction values */
    uint32_t M_part,F_part;

    uint32_t tempreg = 0;

    /* Getting value of APB bus clock into the variable PCLKxn */
    if(pUSARTx == USART1 || pUSARTx == USART6)
    {
        /* USART1 and USART6 are hanging on APB2 bus */
        PCLKx = RCC_GetPCLK2Value();
    }
    else
    {
        PCLKx = RCC_GetPCLK1Value();
    }

    /* Checking for OVER8 configuration bit */
    if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
    {
        /* OVER8 = 1. Over sampling by 8 */
        usartdiv = ((25 * PCLKx) / (2 * BaudRate));
    }
    else
    {
        /* OVER8 = 0. Over sampling by 16 */
        usartdiv = ((25 * PCLKx) / (4 * BaudRate));
    }

    /* Calculating Mantissa part */
    M_part = usartdiv/100;

    /* Placing Mantissa part in appropriate bit position. USART_BRR */
    tempreg |= M_part << 4;

    /* Extracting fractional part */
    F_part = (usartdiv - (M_part * 100));

    /* Calculating final fractional part */
    if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
    {
        /* OVER8 = 1. Over sampling by 8 */
        F_part = (((F_part * 8) + 50) / 100) & ((uint8_t)0x07);
    }
    else
    {
        /* OVER8 = 0. Over sampling by 16 */
        F_part = ((( F_part * 16) + 50) / 100) & ((uint8_t)0x0F);
    }

    /* Placing fractional part in appropriate bit position. USART_BRR */
    tempreg |= F_part;

    /* Copying value of tempreg in to BRR register */
    pUSARTx->BRR = tempreg;
}
