# STM32 ADXL Interface
Bare-metal STM32 peripheral driver project featuring custom GPIO, I2C, and SPI implementations to interface an ADXL accelerometer, with real-time data displayed on an SSD1306 OLED.

# Overview
This project demonstrates low-level peripheral driver development on an STM32 microcontroller without using HAL or vendor libraries for core peripherals.

The following drivers were implemented from scratch:

GPIO driver

I2C driver

SPI driver

These drivers are used to communicate with an ADXL accelerometer.
Sensor data is displayed on an SSD1306 OLED display.

The project focuses on:

Register-level programming

Peripheral initialization

Bus communication protocols

Modular driver design

Embedded system integration

# Features
Bare-metal GPIO driver

Bare-metal I2C master driver

Bare-metal SPI driver

ADXL accelerometer interfacing

Real-time sensor data output

OLED display integration using SSD1306

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

STM32 microcontroller (e.g., STM32F4 series)

ADXL accelerometer

SSD1306 OLED display

Breadboard and jumper wires

# Author
Ajay Gopinathapanicker

Embedded Firmware Engineer
