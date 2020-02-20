# BNO055 data over BLE using the nRF52832

## Overview
This repo contains bring-up firmware to collect and transmit IMU data.

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
