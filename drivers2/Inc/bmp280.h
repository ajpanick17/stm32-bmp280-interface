/* bmp280.h
 *
 * Created on: Oct 3, 2025
 * Author: ajayp
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include "stm32f407xx.h"
#include "stm32f407xx_i2c_driver.h"

typedef struct
{
	uint8_t Device_ID;
	uint8_t Status;
	uint8_t Config[1];
	uint8_t Ctrl_Meas[1];

	uint8_t Raw_Prs_Msb;
	uint8_t Raw_Prs_Lsb;
	uint8_t Raw_Prs_Xlsb;

	uint8_t Raw_temp_Msb;
	uint8_t Raw_temp_Lsb;
	uint8_t Raw_temp_Xlsb;
}BMP280_REG;

typedef struct
{
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
}BMP280_CALIBRATION_DATA;

typedef enum {
    BMP280_ID        = 0xD0,
    BMP280_RESET     = 0xE0,
    BMP280_STATUS    = 0xF3,
    BMP280_CTRL_MEAS = 0xF4,
    BMP280_CONFIG    = 0xF5,
    BMP280_PRESS_MSB = 0xF7,
    BMP280_PRESS_LSB = 0xF8,
    BMP280_PRESS_XLSB= 0xF9,
    BMP280_TEMP_MSB  = 0xFA,
    BMP280_TEMP_LSB  = 0xFB,
    BMP280_TEMP_XLSB = 0xFC,
    BMP280_CALIB     = 0x88
} bmp280_reg_addr_t;


#define BMP280_SLAVE_ADDR  0x76
#define BMP280_RESET_VALUE 0xB6
#define BMP280_CALIB_ADDRESS 0x88
#define DEVICE_DATA 0

//Definitions of Modes
#define POWER_MODE_SLEEP  (0x0U << 0)
#define POWER_MODE_FORCED (0x1U << 0)
#define POWER_MODE_NORMAL (0x3U << 0)

//Definitions of Temperature over-sampling
#define OSRS_TEMP_SKIP   (0x0U << 5)
#define OSRS_TEMP_X1     (0x1U << 5)
#define OSRS_TEMP_X2     (0x2U << 5)
#define OSRS_TEMP_X4     (0x3U << 5)
#define OSRS_TEMP_X8     (0x4U << 5)
#define OSRS_TEMP_X16    (0x5U << 5)

//Definitions of Pressure over-sampling
#define OSRS_PRS_SKIP   (0x0U << 2)
#define OSRS_PRS_X1     (0x1U << 2)
#define OSRS_PRS_X2     (0x2U << 2)
#define OSRS_PRS_X4     (0x3U << 2)
#define OSRS_PRS_X8     (0x4U << 2)
#define OSRS_PRS_X16    (0x5U << 2)

//Definitions of Stand-by times
#define BMP280_STANDBY_05   (0x0U << 5)
#define BMP280_STANDBY_62   (0x1U << 5)
#define BMP280_STANDBY_125  (0x2U << 5)
#define BMP280_STANDBY_250  (0x3U << 5)
#define BMP280_STANDBY_500  (0x4U << 5)
#define BMP280_STANDBY_1000 (0x5U << 5)
#define BMP280_STANDBY_2000 (0x6U << 5)
#define BMP280_STANDBY_4000 (0x7U << 5)

//Definitions of Filter
#define BMP280_FILTER_OFF   (0x0U << 2)
#define BMP280_FILTER_2  	(0x1U << 2)
#define BMP280_FILTER_4  	(0x2U << 2)
#define BMP280_FILTER_8  	(0x3U << 2)
#define BMP280_FILTER_16  	(0x4U << 2)

#define SPI_ENABLE          (0x1U << 0)
#define SPI_DISABLE         (0x0U << 0)

//Function prototypes
void bmp_sensor_init(BMP280_REG *Bmp280_reg);
void bmp_sensor_write(uint8_t Reg_addr,uint8_t *Txdata,uint8_t Len,uint8_t SlaveAddr);
//void bmp_sensor_read(uint8_t *Txdata, uint8_t *Rxdata,uint8_t TxLen,uint8_t RxLen,uint8_t SlaveAddr);
void bmp_sensor_read(uint8_t SlaveAddr,uint8_t Reg_addr, uint8_t RxLen,uint8_t *ReceiveBuffer);
void bmp_read_calibration_data(BMP280_CALIBRATION_DATA *Bmp280_CalibData);
void bmp_raw_data_read(BMP280_CALIBRATION_DATA *Bmp280_CalibData);
int32_t bmp_compensate_temp(BMP280_CALIBRATION_DATA *Bmp280_CalibData, int32_t raw_temp_data, int32_t *t_fine_out);
int32_t bmp_compensate_pressure(BMP280_CALIBRATION_DATA *Bmp280_CalibData,  int32_t raw_pressure_data);

#endif /* INC_BMP280_H_ */
