/*
 * bmp280.c
 * Author: ajayp
 */

#include "bmp280.h"
#include <string.h>

extern I2C_Handle_t I2C1handle;
uint8_t ReceivedCalibData[24];
uint8_t raw_data[6];
uint32_t raw_pressure_data;
int32_t raw_temp_data;
extern uint8_t rxComplt;

int32_t Final_temperature;
int32_t Final_pressure;

static double t_fine;


void bmp_sensor_write(uint8_t Reg_addr,uint8_t *Txdata,uint8_t Len,uint8_t SlaveAddr)
{
	//while(I2C_MasterSendDataIT(&I2C1handle,Txdata,Len,SlaveAddr,I2C_DISABLE_SR)!= I2C_READY);
	I2C_Burst_Write(&I2C1handle,SlaveAddr,Reg_addr,Len,Txdata);
}

void bmp_sensor_read(uint8_t SlaveAddr,uint8_t Reg_addr,uint8_t RxLen,uint8_t *ReceiveBuffer)
{
	//while(I2C_MasterSendDataIT(&I2C1handle,Txdata,TxLen,SlaveAddr,I2C_DISABLE_SR) != I2C_READY);
	//while(I2C_MasterReceiveDataIT(&I2C1handle,Rxdata,RxLen,SlaveAddr,I2C_DISABLE_SR) != I2C_READY);
	I2C_Burst_Read(&I2C1handle,SlaveAddr,Reg_addr,RxLen,ReceiveBuffer);
}

void bmp_sensor_init(BMP280_REG *Bmp280_reg)
{

	//1.Configuring stand-by duration, filter, and spi comm(disable)
	Bmp280_reg->Config[DEVICE_DATA] = (uint8_t)(BMP280_STANDBY_250 | BMP280_FILTER_OFF | SPI_DISABLE);
	bmp_sensor_write(BMP280_CONFIG,Bmp280_reg->Config,1,BMP280_SLAVE_ADDR);


	//2.Initialize and write over-sampling temperature,pressure and mode parameters
	Bmp280_reg->Ctrl_Meas[DEVICE_DATA] = (uint8_t)(OSRS_TEMP_X4 | OSRS_PRS_X4 | POWER_MODE_NORMAL);
	bmp_sensor_write(BMP280_CTRL_MEAS,Bmp280_reg->Ctrl_Meas,1,BMP280_SLAVE_ADDR);

}

void bmp_read_calibration_data(BMP280_CALIBRATION_DATA *Bmp280_CalibData)
{
    uint8_t Calib_addr = 0x88;
    int16_t temp[12];
    uint16_t temp2;

	bmp_sensor_read(BMP280_SLAVE_ADDR,Calib_addr,24,ReceivedCalibData);

	for (uint32_t i = 0; i < 12; ++i)
	{
		temp2 	= (uint16_t)(ReceivedCalibData[2*i] | (ReceivedCalibData[2*i + 1] << 8));
		temp[i] = (int16_t)temp2; /* bitwise identical; interpret as signed for temp */
	}

	Bmp280_CalibData->dig_T1 = (uint16_t)temp[0];
	Bmp280_CalibData->dig_T2 = temp[1];
	Bmp280_CalibData->dig_T3 = temp[2];
	Bmp280_CalibData->dig_P1 = (uint16_t)temp[3];
	Bmp280_CalibData->dig_P2 = temp[4];
	Bmp280_CalibData->dig_P3 = temp[5];
	Bmp280_CalibData->dig_P4 = temp[6];
	Bmp280_CalibData->dig_P5 = temp[7];
	Bmp280_CalibData->dig_P6 = temp[8];
	Bmp280_CalibData->dig_P7 = temp[9];
	Bmp280_CalibData->dig_P8 = temp[10];
	Bmp280_CalibData->dig_P9 = temp[11];

}

void bmp_raw_data_read(BMP280_CALIBRATION_DATA *Bmp280_CalibData)
{
	volatile uint32_t temp[3];

	bmp_sensor_read(BMP280_SLAVE_ADDR,BMP280_PRESS_MSB,6,raw_data);
	//raw_pressure_data = (raw_data[0]<<12) | (raw_data[1]<<4) | (raw_data[2]>>4);
	//raw_temp_data  	  = (raw_data[3]<<12) | (raw_data[4]<<4) | (raw_data[5]>>4);

	temp[2]=raw_data[3];
	temp[1]=raw_data[4];
	temp[0]=raw_data[5];
	raw_temp_data = (temp[2]<<12)+(temp[1]<<4)+(temp[0]>>4);

	temp[2]=raw_data[0];
	temp[1]=raw_data[1];
	temp[0]=raw_data[2];
	raw_pressure_data = (temp[2]<<12)+(temp[1]<<4)+(temp[0]>>4);

}

int32_t bmp_compensate_temp(BMP280_CALIBRATION_DATA *Bmp280_CalibData, int32_t raw_temp_data, int32_t *t_fine_out)
{
	double var1, var2;
	volatile float T;

	var1=(((double)raw_temp_data)/16384.0-((double)Bmp280_CalibData->dig_T1)/1024.0)*((double)Bmp280_CalibData->dig_T2);
	var2=((((double)raw_temp_data)/131072.0-((double)Bmp280_CalibData->dig_T1)/8192.0)*(((double)raw_temp_data)/131072.0-((double)Bmp280_CalibData->dig_T1)/8192.0))*((double)Bmp280_CalibData->dig_T3);
	t_fine = (int32_t)(var1+var2);
	T = (var1+var2)/5120.0;
	return T;
}



int32_t bmp_compensate_pressure(BMP280_CALIBRATION_DATA *Bmp280_CalibData,  int32_t raw_pressure_data)
{
	double var1, var2;
	volatile double p;

	var1=((double)t_fine/2.0)-64000.0;
	var2=var1*var1*((double)Bmp280_CalibData->dig_P6)/32768.0;
    var2=var2+var1*((double)Bmp280_CalibData->dig_P5)*2.0;
	var2=(var2/4.0)+(((double)Bmp280_CalibData->dig_P4)*65536.0);
	var1=(((double)Bmp280_CalibData->dig_P3)*var1*var1/524288.0+((double)Bmp280_CalibData->dig_P2)*var1)/524288.0;
	var1=(1.0+var1/32768.0)*((double)Bmp280_CalibData->dig_P1);

	p =1048576.0-(double)raw_pressure_data;
	p=(p-(var2/4096.0))*6250.0/var1;
	var1=((double)Bmp280_CalibData->dig_P9)*p*p/2147483648.0;
	var2=p*((double)Bmp280_CalibData->dig_P8)/32768.0;
    p=p+(var1+var2+((double)Bmp280_CalibData->dig_P7))/16.0;
    p /= 100;
	return p;

}
