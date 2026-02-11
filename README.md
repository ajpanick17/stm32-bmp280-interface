# STM32 BMP280 Interface
Bare-metal STM32 peripheral driver project featuring custom GPIO, I2C, and SPI implementations to interface an BMP280 Digital Pressure sensor, with real-time pressure and temeprature displayed on an SSD1306 OLED.

# Overview
This project demonstrates low-level peripheral driver development on an STM32 microcontroller without using HAL or vendor libraries for core peripherals.

The following drivers were implemented from scratch:

GPIO driver

I2C driver

SPI driver

These drivers are used to communicate with an BMP280 sensor.
Sensor data is displayed on an SSD1306 OLED display.

The project focuses on:

Register-level programming

Peripheral initialization

Bus communication protocols

Modular driver design

Embedded system integration

# Folder structure
stm32-adxl-interface
    README.md
    main.c

    drivers
        inc
            stm32f4xx.h
            gpio.h
            i2c.h
            spi.h
            bmp280.h
            ssd1306.h

        src
            gpio.c
            i2c.c
            spi.c
            bmp280.c
            ssd1306.c

# Hardware Used

STM32F411RE

BOSCH BMP280 Digital sensor

SSD1306 OLED display

Breadboard and jumper wires

# Demo picture

![BMP280](https://github.com/user-attachments/assets/37240172-1a79-4715-acb7-dd6797e4df59)

# Author
Ajay Gopinathapanicker

Embedded Firmware Engineer
