# BNO055 data over BLE using the nRF52832

## Overview
This repo contains bring-up firmware to collect and transmit IMU data.

## Prerequisite
1. nRF5_SDK_15.2.0_9412b96 at this [link](https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v15.x.x/nRF5_SDK_15.2.0_9412b96.zip).
2. Segger Embedded Studio 

## Hardware used
1. nRF52832
2. BNO055

## Software Requirements
1. A simplified low level BNO055 driver
2. HAL level module to read and present the following BNO055 data
   - Quatenrion data
   - Euler Vector
   - Angular Velocity
   - Acceleration Vector
   - Magnetic Field STrength Vector
   - Linear Acceleration Vector
   - Gravity Vector
   - Temperature
3. Simplified nRF52832 BLE driver
   - nRF52832 should be able to pair and stream UART data over BLE
4. Main System Application
   - The system must collect and transmit BNO055 data over BLE to a PC/Mac app at 100Hz
5. The PC/Mac app must be able to use IMU data to control orientation and position of a 3D image according to IMU data

## Setting Up

1. Download the nRF5_SDK_15.2.0_9412b96  sdk
2. Extract the contents onto your desktop
3. Clone or download the repo.
4. Copy the folders `project` and `lib` and paste them into the sdk root folder. You will then have `examples`, `project`, `lib`, `config`, `components`, `modules` etc in the same folder.  
5. Go to project `project\ble_peripheral\ble_app_uart\pca10040\s132\ses` and open `ble_app_uart_pca10040_s132.emProject` using Segger Embedded Studio
6. Compile and program the PCA10040 board
7. The device will broadcast as `bno055` over bluetooth
8. Device should be pinging `Hello World` every second