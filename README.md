# BNO055 data over BLE using the nRF52832

## Overview
This repo contains bring-up firmware to collect and transmit IMU data.

## Prerequisite
In order to build the project on Segger Embedded Studio, the nRF5_SDK_15.2.0_9412b96 at this [link](https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v15.x.x/nRF5_SDK_15.2.0_9412b96.zip).
Install the nRF5_SDK_15.2.0_9412b96 folder in the git root folder. 

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

## Inlcude Directory paths 
### (relative to example ses folder)

```C
../../../config
../../../../../../components
../../../../../../components/ble/ble_advertising
../../../../../../components/ble/ble_db_discovery
../../../../../../components/ble/ble_dtm
../../../../../../components/ble/ble_link_ctx_manager
../../../../../../components/ble/ble_racp
../../../../../../components/ble/ble_services/ble_ancs_c
../../../../../../components/ble/ble_services/ble_ans_c
../../../../../../components/ble/ble_services/ble_bas
../../../../../../components/ble/ble_services/ble_bas_c
../../../../../../components/ble/ble_services/ble_cscs
../../../../../../components/ble/ble_services/ble_cts_c
../../../../../../components/ble/ble_services/ble_dfu
../../../../../../components/ble/ble_services/ble_dis
../../../../../../components/ble/ble_services/ble_gls
../../../../../../components/ble/ble_services/ble_hids
../../../../../../components/ble/ble_services/ble_hrs
../../../../../../components/ble/ble_services/ble_hrs_c
../../../../../../components/ble/ble_services/ble_hts
../../../../../../components/ble/ble_services/ble_ias
../../../../../../components/ble/ble_services/ble_ias_c
../../../../../../components/ble/ble_services/ble_lbs
../../../../../../components/ble/ble_services/ble_lbs_c
../../../../../../components/ble/ble_services/ble_lls
../../../../../../components/ble/ble_services/ble_nus
../../../../../../components/ble/ble_services/ble_nus_c
../../../../../../components/ble/ble_services/ble_rscs
../../../../../../components/ble/ble_services/ble_rscs_c
../../../../../../components/ble/ble_services/ble_tps
../../../../../../components/ble/common
../../../../../../components/ble/nrf_ble_gatt
../../../../../../components/ble/nrf_ble_qwr
../../../../../../components/ble/nrf_ble_scan
../../../../../../components/ble/peer_manager
../../../../../../components/boards
../../../../../../components/drivers_nrf/usbd
../../../../../../components/libraries/atomic
../../../../../../components/libraries/atomic_fifo
../../../../../../components/libraries/atomic_flags
../../../../../../components/libraries/balloc
../../../../../../components/libraries/bootloader/ble_dfu
../../../../../../components/libraries/bsp
../../../../../../components/libraries/button
../../../../../../components/libraries/cli
../../../../../../components/libraries/crc16
../../../../../../components/libraries/crc32
../../../../../../components/libraries/crypto
../../../../../../components/libraries/csense
../../../../../../components/libraries/csense_drv
../../../../../../components/libraries/delay
../../../../../../components/libraries/ecc
../../../../../../components/libraries/experimental_section_vars
../../../../../../components/libraries/experimental_task_manager
../../../../../../components/libraries/fds
../../../../../../components/libraries/fifo
../../../../../../components/libraries/fstorage
../../../../../../components/libraries/gfx
../../../../../../components/libraries/gpiote
../../../../../../components/libraries/hardfault
../../../../../../components/libraries/hci
../../../../../../components/libraries/led_softblink
../../../../../../components/libraries/log
../../../../../../components/libraries/log/src
../../../../../../components/libraries/low_power_pwm
../../../../../../components/libraries/mem_manager
../../../../../../components/libraries/memobj
../../../../../../components/libraries/mpu
../../../../../../components/libraries/mutex
../../../../../../components/libraries/pwm
../../../../../../components/libraries/pwr_mgmt
../../../../../../components/libraries/queue
../../../../../../components/libraries/ringbuf
../../../../../../components/libraries/scheduler
../../../../../../components/libraries/sdcard
../../../../../../components/libraries/slip
../../../../../../components/libraries/sortlist
../../../../../../components/libraries/spi_mngr
../../../../../../components/libraries/stack_guard
../../../../../../components/libraries/strerror
../../../../../../components/libraries/svc
../../../../../../components/libraries/timer
../../../../../../components/libraries/twi_mngr
../../../../../../components/libraries/twi_sensor
../../../../../../components/libraries/uart
../../../../../../components/libraries/usbd
../../../../../../components/libraries/usbd/class/audio
../../../../../../components/libraries/usbd/class/cdc
../../../../../../components/libraries/usbd/class/cdc/acm
../../../../../../components/libraries/usbd/class/hid
../../../../../../components/libraries/usbd/class/hid/generic
../../../../../../components/libraries/usbd/class/hid/kbd
../../../../../../components/libraries/usbd/class/hid/mouse
../../../../../../components/libraries/usbd/class/msc
../../../../../../components/libraries/util
../../../../../../components/nfc/ndef/conn_hand_parser
../../../../../../components/nfc/ndef/conn_hand_parser/ac_rec_parser
../../../../../../components/nfc/ndef/conn_hand_parser/ble_oob_advdata_parser
../../../../../../components/nfc/ndef/conn_hand_parser/le_oob_rec_parser
../../../../../../components/nfc/ndef/connection_handover/ac_rec
../../../../../../components/nfc/ndef/connection_handover/ble_oob_advdata
../../../../../../components/nfc/ndef/connection_handover/ble_pair_lib
../../../../../../components/nfc/ndef/connection_handover/ble_pair_msg
../../../../../../components/nfc/ndef/connection_handover/common
../../../../../../components/nfc/ndef/connection_handover/ep_oob_rec
../../../../../../components/nfc/ndef/connection_handover/hs_rec
../../../../../../components/nfc/ndef/connection_handover/le_oob_rec
../../../../../../components/nfc/ndef/generic/message
../../../../../../components/nfc/ndef/generic/record
../../../../../../components/nfc/ndef/launchapp
../../../../../../components/nfc/ndef/parser/message
../../../../../../components/nfc/ndef/parser/record
../../../../../../components/nfc/ndef/text
../../../../../../components/nfc/ndef/uri
../../../../../../components/nfc/t2t_lib
../../../../../../components/nfc/t2t_lib/hal_t2t
../../../../../../components/nfc/t2t_parser
../../../../../../components/nfc/t4t_lib
../../../../../../components/nfc/t4t_lib/hal_t4t
../../../../../../components/nfc/t4t_parser/apdu
../../../../../../components/nfc/t4t_parser/cc_file
../../../../../../components/nfc/t4t_parser/hl_detection_procedure
../../../../../../components/nfc/t4t_parser/tlv
../../../../../../components/softdevice/common
../../../../../../components/softdevice/s132/headers
../../../../../../components/softdevice/s132/headers/nrf52
../../../../../../components/toolchain/cmsis/include
../../../../../../external/fprintf
../../../../../../external/segger_rtt
../../../../../../external/utf_converter
../../../../../../integration/nrfx
../../../../../../integration/nrfx/legacy
../../../../../../modules/nrfx
../../../../../../modules/nrfx/drivers/include
../../../../../../modules/nrfx/hal
../../../../../../modules/nrfx/mdk
../config
../../../../../../lib/ble_uart /ble_uart_peripheral
../../../../../../lib/i2c
../../../../../../lib/bno055
```
