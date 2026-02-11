/*
 * stm32f407xx.h
 *
 *  Created on: Aug 16, 2025
 *      Author: ajayp
 */
#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>


/*
 * ARM Cortex Mx Processor NVIC ISERx Register Addresses
 */
#define NVIC_ISER0      ( (volatile uint32_t*)0xE000E100 )
#define NVIC_ISER1      ( (volatile uint32_t*)0xE000E104 )
#define NVIC_ISER2      ( (volatile uint32_t*)0xE000E108 )
#define NVIC_ISER3      ( (volatile uint32_t*)0xE000E10C )
#define NVIC_ISER4      ( (volatile uint32_t*)0xE000E110 )
#define NVIC_ISER5      ( (volatile uint32_t*)0xE000E114 )
#define NVIC_ISER6      ( (volatile uint32_t*)0xE000E118 )
#define NVIC_ISER7      ( (volatile uint32_t*)0xE000E11C )

/*
 * ARM Cortex Mx Processor NVIC ICERx Register Addresses
 */
#define NVIC_ICER0      ( (volatile uint32_t*)0XE000E180 )
#define NVIC_ICER1      ( (volatile uint32_t*)0XE000E184 )
#define NVIC_ICER2      ( (volatile uint32_t*)0XE000E188 )
#define NVIC_ICER3      ( (volatile uint32_t*)0XE000E18C )
#define NVIC_ICER4      ( (volatile uint32_t*)0XE000E190 )
#define NVIC_ICER5      ( (volatile uint32_t*)0XE000E194 )
#define NVIC_ICER6      ( (volatile uint32_t*)0XE000E198 )
#define NVIC_ICER7      ( (volatile uint32_t*)0XE000E19C )
//---------------------------------------------------------------------------------------------------------//
/*
 * ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR   ( (volatile uint32_t*)0XE000E400 )
//---------------------------------------------------------------------------------------------------------//
/*
 * ARM Cortex Mx Processor number of priority bits implemented in Priority Register
 */
#define NO_PR_BITS_IMPLEMENTED  4
//---------------------------------------------------------------------------------------------------------//

#define ENABLE		1
#define DISABLE		0
#define SET			ENABLE
#define RESET		DISABLE
#define GPIO_SET	SET
#define GPIO_RESET	RESET
#define FLAG_SET	SET
#define FLAG_RESET RESET

//GPIOx STRUCTURE
typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
}GPIO_Regdef_t;

#define GPIOA  ((GPIO_Regdef_t*)GPIOA_BASE)
#define GPIOB  ((GPIO_Regdef_t*)GPIOB_BASE)
#define GPIOC  ((GPIO_Regdef_t*)GPIOC_BASE)
#define GPIOD  ((GPIO_Regdef_t*)GPIOD_BASE)
#define GPIOE  ((GPIO_Regdef_t*)GPIOE_BASE)
#define GPIOH  ((GPIO_Regdef_t*)GPIOH_BASE)
/*-------------------------------------------------------------------------------------------*/

//RCC STRUCTURE
typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
			 uint32_t RESERVED1[2];
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	 	     uint32_t RESERVED2[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	uint32_t RESERVED3[2];
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	uint32_t RESERVED4[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	uint32_t RESERVED5[2];
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	uint32_t RESERVED6[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	uint32_t RESERVED7[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	uint32_t RESERVED8;
	volatile uint32_t DCKCFGR;
}RCC_Regdef_t;

#define RCC    ((RCC_Regdef_t*)RCC_BASE)
/*-------------------------------------------------------------------------------------------*/

//EXTI PERIPHERAL STRUCTURE
typedef struct
{
	volatile uint32_t IMR;    /*!< EXTI Interrupt mask register,            Address offset: 0x00 */
	volatile uint32_t EMR;    /*!< EXTI Event mask register,                Address offset: 0x04 */
	volatile uint32_t RTSR;   /*!< EXTI Rising trigger selection register,  Address offset: 0x08 */
	volatile uint32_t FTSR;   /*!< EXTI Falling trigger selection register, Address offset: 0x0C */
	volatile uint32_t SWIER;  /*!< EXTI Software interrupt event register,  Address offset: 0x10 */
	volatile uint32_t PR;     /*!< EXTI Pending register,                   Address offset: 0x14 */
} EXTI_TypeDef;

#define EXTI    ((EXTI_TypeDef*)EXTI_BASE)
/*-------------------------------------------------------------------------------------------*/
//SYSCFG PERIPHERAL STRUCTURE
typedef struct
{
	volatile uint32_t MEMRMP;       /*!< SYSCFG memory remap register,                      Address offset: 0x00      */
	volatile uint32_t PMC;          /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04      */
	volatile uint32_t EXTICR[4];    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
	volatile uint32_t CMPCR;        /*!< SYSCFG Compensation cell control register,         Address offset: 0x20      */
} SYSCFG_TypeDef;

#define SYSCFG    ((SYSCFG_TypeDef*)SYSCFG_BASE)
/*-------------------------------------------------------------------------------------------*/

//SPI PERIPHERAL STRUCTURE
typedef struct
{
	volatile uint16_t CR1; uint16_t _pad0;
	volatile uint16_t CR2; uint16_t _pad1;
	volatile uint16_t SR;  uint16_t _pad2;
	volatile uint16_t DR;  uint16_t _pad3;
	volatile uint16_t CRCPR; uint16_t _pad4;
	volatile uint16_t RXCRCR; uint16_t _pad5;
	volatile uint16_t TXCRCR; uint16_t _pad6;
	volatile uint16_t I2SCFGR; uint16_t _pad7;
	volatile uint16_t I2SPR; uint16_t _pad8;
}SPI_RegDef_t;

#define SPI1 ((SPI_RegDef_t*)SPI1_BASE)
#define SPI2 ((SPI_RegDef_t*)SPI2_BASE)
#define SPI3 ((SPI_RegDef_t*)SPI3_BASE)
#define SPI4 ((SPI_RegDef_t*)SPI4_BASE)
#define SPI5 ((SPI_RegDef_t*)SPI5_BASE)


//I2C PERIPHERAL STRUCTURE
typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t DR;
	volatile uint32_t SR1;
	volatile uint32_t SR2;
	volatile uint32_t CCR;
	volatile uint32_t TRISE;
	volatile uint32_t FLTR;
}I2C_Regdef_t;

#define I2C1 ((I2C_Regdef_t*)I2C1_BASE)
#define I2C2 ((I2C_Regdef_t*)I2C2_BASE)
#define I2C3 ((I2C_Regdef_t*)I2C3_BASE)
/*-------------------------------------------------------------------------------------------*/

//USART PERIPHERAL STRUCTURE
typedef struct
{
	volatile uint32_t SR;   /* USART status register,                   Address offset: 0x00 */
	volatile uint32_t DR;   /* USART data register,                     Address offset: 0x04 */
	volatile uint32_t BRR;  /* USART baud rate register,                Address offset: 0x08 */
	volatile uint32_t CR1;  /* USART control register 1,                Address offset: 0x0C */
	volatile uint32_t CR2;  /* USART control register 2,                Address offset: 0x10 */
	volatile uint32_t CR3;  /* USART control register 3,                Address offset: 0x14 */
	volatile uint32_t GTPR; /* USART guard time and prescaler register, Address offset: 0x18 */
}USART_RegDef_t;

#define USART1 ((USART_RegDef_t*)USART1_BASE)
#define USART2 ((USART_RegDef_t*)USART2_BASE)
#define USART6 ((USART_RegDef_t*)USART6_BASE)
/*----------------------------------------------------------------------------------------------------------------------------*/

//Define FLASH AND RAM BASE ADDRESSES
#define FLASH_BASE   	 0x08000000UL
#define SRAM1_BASE   	 0x20000000UL
#define SRAM 	     	 SRAM1_BASE
#define ROM 		 	 0x1FFF0000UL

//Define BUSES ADDRESSES
#define APB1PERIPH_BASE  0x40000000UL
#define APB2PERIPH_BASE	 0x40010000UL
#define AHB1PERIPH_BASE  0x40020000UL
#define AHB2PERIPH_BASE	 0x50000000UL

//Define PERIPHERAL BASE ADDRESSES FOR GPIOx
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000UL)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00UL)

#define CRC_BASE              (AHB1PERIPH_BASE + 0x3000UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x3C00UL)
#define DMA1_BASE             (AHB1PERIPH_BASE + 0x6000UL)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x6400UL)

//Define PERIPHERAL BASE ADDRESSES FOR AHB2
#define USB_OTG_FS_PERIPH_BASE (AHB2PERIPH_BASE + 0x0000UL)

//Define PERIPHERAL BASE ADDRESSES FOR APB1
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00UL)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800UL)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00UL)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000UL)
#define I2S2ext_BASE          (APB1PERIPH_BASE + 0x3400UL)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800UL)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00UL)
#define I2S3ext_BASE          (APB1PERIPH_BASE + 0x4000UL)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400UL)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800UL)
#define I2C3_BASE             (APB1PERIPH_BASE + 0x5C00UL)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000UL)

//Define PERIPHERAL BASE ADDRESSES FOR APB2
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000UL)
#define USART1_BASE           (APB2PERIPH_BASE + 0x1000UL)
#define USART6_BASE           (APB2PERIPH_BASE + 0x1400UL)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000UL)
#define ADC1_COMMON_BASE      (APB2PERIPH_BASE + 0x2300UL)
/* Legacy define */
#define ADC_BASE               ADC1_COMMON_BASE
#define SDIO_BASE             (APB2PERIPH_BASE + 0x2C00UL)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000UL)
#define SPI4_BASE             (APB2PERIPH_BASE + 0x3400UL)
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x3800UL)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x3C00UL)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800UL)
#define SPI5_BASE             (APB2PERIPH_BASE + 0x5000UL)

//Clock enable macros for gpio
#define GPIOA_CLK_EN() (RCC->AHB1ENR |= (1<<0))
#define GPIOB_CLK_EN() (RCC->AHB1ENR |= (1<<1))
#define GPIOC_CLK_EN() (RCC->AHB1ENR |= (1<<2))
#define GPIOD_CLK_EN() (RCC->AHB1ENR |= (1<<3))
#define GPIOE_CLK_EN() (RCC->AHB1ENR |= (1<<4))
#define GPIOH_CLK_EN() (RCC->AHB1ENR |= (1<<7))

//Clock Disable macros for gpio
#define GPIOA_CLK_DI() (RCC->AHB1ENR &= ~(1<<0))
#define GPIOB_CLK_DI() (RCC->AHB1ENR &= ~(1<<1))
#define GPIOC_CLK_DI() (RCC->AHB1ENR &= ~(1<<2))
#define GPIOD_CLK_DI() (RCC->AHB1ENR &= ~(1<<3))
#define GPIOE_CLK_DI() (RCC->AHB1ENR &= ~(1<<4))
#define GPIOH_CLK_DI() (RCC->AHB1ENR &= ~(1<<7))

//Clock enable macros for sysconfig
#define SYSCFG_CLK_EN() (RCC->APB2ENR |= (0<<14))

//Clock enable macros for spi
#define SPI1_CLK_EN()  (RCC->APB2ENR |= (1<<12))
#define SPI2_CLK_EN()  (RCC->APB1ENR |= (1<<14))
#define SPI3_CLK_EN()  (RCC->APB1ENR |= (1<<15))
#define SPI4_CLK_EN()  (RCC->APB2ENR |= (1<<13))
#define SPI5_CLK_EN()  (RCC->APB2ENR |= (1<<20))

//CLOCK ENABLE MACROS FOR I2C
#define I2C1_CLK_EN()  (RCC->APB1ENR |= (1<<21))
#define I2C2_CLK_EN()  (RCC->APB1ENR |= (1<<22))
#define I2C3_CLK_EN()  (RCC->APB1ENR |= (1<<23))

//CLOCK DISABLE MACROS FOR I2C
#define I2C1_CLK_DI() (RCC->APB1ENR &= ~ (1<<21))
#define I2C2_CLK_DI() (RCC->APB1ENR &= ~ (1<<22))
#define I2C3_CLK_DI() (RCC->APB1ENR &= ~ (1<<23))

//CLOCK ENABLE MACROS FOR USART
#define USART1_CLK_EN()  (RCC->APB2ENR |= (1<<4))
#define USART2_CLK_EN()  (RCC->APB1ENR |= (1<<17))
#define USART6_CLK_EN()  (RCC->APB2ENR |= (1<<5))

//CLOCK DISABLE MACROS FOR USART
#define USART1_CLK_DI() (RCC->APB2ENR &= ~ (1<<4))
#define USART2_CLK_DI() (RCC->APB1ENR &= ~ (1<<17))
#define USART6_CLK_DI() (RCC->APB2ENR &= ~ (1<<5))


//GPIO PERIPHERAL RESET MACROS
#define GPIOA_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOD_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOE_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOF_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOG_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOH_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)
#define GPIOI_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8)); }while(0)

// IRQ Interrupt numbers
#define SPI_IRQ_NO_SPI1 35
#define SPI_IRQ_NO_SPI2 36
#define SPI_IRQ_NO_SPI3 51
#define SPI_IRQ_NO_SPI4 84
#define SPI_IRQ_NO_SPI5 85




/******************************************************************************************
 *Bit position definitions of I2C peripheral
 ******************************************************************************************/
/*
 * Bit position definitions I2C_CR1
 */
#define I2C_CR1_PE						0
#define I2C_CR1_NOSTRETCH  				7
#define I2C_CR1_START 					8
#define I2C_CR1_STOP  				 	9
#define I2C_CR1_ACK 				 	10
#define I2C_CR1_SWRST  				 	15

/*
 * Bit position definitions I2C_CR2
 */
#define I2C_CR2_FREQ				 	0
#define I2C_CR2_ITERREN				 	8
#define I2C_CR2_ITEVTEN				 	9
#define I2C_CR2_ITBUFEN 			    10

/*
 * Bit position definitions I2C_OAR1
 */
#define I2C_OAR1_ADD0    				 0
#define I2C_OAR1_ADD71 				 	 1
#define I2C_OAR1_ADD98  			 	 8
#define I2C_OAR1_ADDMODE   			 	15

/*
 * Bit position definitions I2C_SR1
 */

#define I2C_SR1_SB 					 	0
#define I2C_SR1_ADDR 				 	1
#define I2C_SR1_BTF 					2
#define I2C_SR1_ADD10 					3
#define I2C_SR1_STOPF 					4
#define I2C_SR1_RXNE 					6
#define I2C_SR1_TXE 					7
#define I2C_SR1_BERR 					8
#define I2C_SR1_ARLO 					9
#define I2C_SR1_AF 					 	10
#define I2C_SR1_OVR 					11
#define I2C_SR1_TIMEOUT 				14

/*
 * Bit position definitions I2C_SR2
 */
#define I2C_SR2_MSL						0
#define I2C_SR2_BUSY 					1
#define I2C_SR2_TRA 					2
#define I2C_SR2_GENCALL 				4
#define I2C_SR2_DUALF 					7

/*
 * Bit position definitions I2C_CCR
 */
#define I2C_CCR_CCR 					 0
#define I2C_CCR_DUTY 					14
#define I2C_CCR_FS  				 	15

// I2C IRQ Interrupt numbers
#define IRQ_NO_I2C1_EV  31
#define IRQ_NO_I2C1_ER  32
#define IRQ_NO_I2C2_EV  33
#define IRQ_NO_I2C2_ER  34
#define IRQ_NO_I2C3_EV  72
#define IRQ_NO_I2C3_ER  73

/******************************************************************************************
 *			Bit position definitions of U(S)ART peripheral
 ******************************************************************************************/
/*
 * Bit position definitions USART_SR
 */
#define USART_SR_PE         0
#define USART_SR_FE         1
#define USART_SR_NF         2
#define USART_SR_ORE        3
#define USART_SR_IDLE       4
#define USART_SR_RXNE       5
#define USART_SR_TC         6
#define USART_SR_TXE        7
#define USART_SR_LBD        8
#define USART_SR_CTS        9


/*
 * Bit position definitions USART_CR1
 */
#define USART_CR1_SBK       0
#define USART_CR1_RWU       1
#define USART_CR1_RE        2
#define USART_CR1_TE        3
#define USART_CR1_IDLEIE    4
#define USART_CR1_RXNEIE    5
#define USART_CR1_TCIE      6
#define USART_CR1_TXEIE     7
#define USART_CR1_PEIE      8
#define USART_CR1_PS        9
#define USART_CR1_PCE       10
#define USART_CR1_WAKE      11
#define USART_CR1_M         12
#define USART_CR1_UE        13
#define USART_CR1_OVER8     15


/*
 * Bit position definitions USART_CR2
 */
#define USART_CR2_ADD       0
#define USART_CR2_LBDL      5
#define USART_CR2_LBDIE     6
#define USART_CR2_LBCL      8
#define USART_CR2_CPHA      9
#define USART_CR2_CPOL      10
#define USART_CR2_CLKEN     11
#define USART_CR2_STOP      12
#define USART_CR2_LINEN     14


/*
 * Bit position definitions USART_CR3
 */
#define USART_CR3_EIE       0
#define USART_CR3_IREN      1
#define USART_CR3_IRLP      2
#define USART_CR3_HDSEL     3
#define USART_CR3_NACK      4
#define USART_CR3_SCEN      5
#define USART_CR3_DMAR      6
#define USART_CR3_DMAT      7
#define USART_CR3_RTSE      8
#define USART_CR3_CTSE      9
#define USART_CR3_CTSIE     10
#define USART_CR3_ONEBIT    11


#endif /* INC_STM32F407XX_H_ */
