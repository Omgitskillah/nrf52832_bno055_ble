#ifndef _BNO055_H_
#define _BNO055_H_
#include "ble_advdata.h"

typedef enum
{
  STANDARD_I2C,
  HID_OVER_I2C
}bno_i2c_protocol;

bool bno055_init(uint8_t * _id);
bool read_eul_heading_data(uint16_t * _data_arr);
bool read_eul_roll_data(uint16_t * _data_arr);
bool read_eul_pitch_data(uint16_t * _data_arr);
bool read_qua_data(uint16_t * _data_arr);
bool read_acc_data(uint16_t * _data_arr);
bool read_mag_data(uint16_t * _data_arr);
bool read_gyr_data(uint16_t * _data_arr);
bool read_lia_data(uint16_t * _data_arr);
bool read_grv_data(uint16_t * _data_arr);
void bno055_set_i2c_protocol(bno_i2c_protocol _ps);

#endif
