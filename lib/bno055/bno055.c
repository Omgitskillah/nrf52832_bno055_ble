#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*system includes*/
#include "nrf_delay.h"

/* System log info inlcudes */
#ifdef LOG_BNO055
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#endif

#define HW_I2C

#ifdef HW_I2C
#include "i2c_hal.h"
#else
#include "soft_i2c.h"
#endif

#include "pca10040.h"
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "bno055.h"
#include "bno055_conf.h"

/**
 * Bno055 notes (TODO stash)
 * -----------------
 * fx to control power mode
 * fx to axis remap
 * Sensor configs (acc, gyro, mgtm)
 * Unit selection fx
 * Data output format fx
 * fx to get calibration data
 * Handle interrupts and how to set them
 * Self test mode 
*/

uint32_t BNO_SDA = 26;
uint32_t BNO_SCL = 27;
uint32_t BNO_INT = 2;
uint32_t BNO_PS0 = 3;

/* fx declaration */
bool bno055_write_reg(uint8_t _reg, uint8_t _data);
bool bno055_read_reg(uint8_t _reg, uint8_t *_buff, uint8_t _length);
bool bno055_get_chip_ID(uint8_t *_buff);
bool read_data_regs(uint8_t * regs, uint8_t num_of_regs,  uint16_t * _buff, uint8_t _buff_len);


/**
 * fx: void bno055_write_reg(uint8_t _reg, uint8_t _data)
 * Write data to bno055 device
 * 
 * Arguments
 * ---------------
 * _reg - bno055 register to write to
 * _data - single byte data to write to specified reg
 * 
 * return
 * ---------------
 * void
 */
bool bno055_write_reg(uint8_t _reg, uint8_t _data)
{
  uint8_t _buff[2] = {_reg, _data};
  //write sequence
#ifdef HW_I2C
  if( !i2c_write( bno055_address, _buff, sizeof(_buff), false ) ) return false;
#else
  if( !i2c_write( bno055_address, _buff, sizeof(_buff) ) ) return false;
#endif
  // debug data here
  return true;
}

/**
 * fx: void bno055_read_reg(uint8_t _reg, uint8_t *_buff, uint8_t _length)
 * Read a specific number of bytes from a specific register
 * 
 * Arguments
 * ---------------
 * _reg - bno055 register to read from 
 * _buff - pointer to populate with read bytes
 * _length - number of bytes to read
 * 
 * return
 * ---------------
 * true if successful 
 */
bool bno055_read_reg(uint8_t _reg, uint8_t *_buff, uint8_t _length)
{
  uint8_t reg_buff[1];
  reg_buff[0] = _reg;
  //write sequence
#ifdef HW_I2C
  if( !i2c_write( bno055_address, reg_buff, sizeof(reg_buff), false ) ) return false;
#else
  if( !i2c_write( bno055_address, reg_buff, sizeof(reg_buff) ) ) return false;
#endif
  //Read sequence
  if( !i2c_read( bno055_address, _buff, _length) ) return false;

#ifdef LOG_BNO055
    NRF_LOG_INFO("reg data: %s");
#endif
  return true;
}

/**
 * fx: bool bno055_get_chip_ID(uint8_t *_buff)
 * Reads the chip ID from reg and populates _buff
 * get_chip_ID(&id);
 * 
 * Arguments
 * ---------------
 * *_buff - pointer to populate with reg id
 * 
 * return
 * ---------------
 * true if successful 
 */
bool bno055_get_chip_ID(uint8_t *_buff)
{
  // get the chip id
  return bno055_read_reg( (uint8_t)REG_CHIP_ID, _buff, LEN_CHIP_ID);
}

/**
 * fx: bool bno055_set_op_mode(uint8_t _mode)
 * sets the bno055 operation mode
 * must cast the def to uint8_t
 * 
 * Arguments
 * ---------------
 * _mode - desired mode
 * 
 * return
 * ---------------
 * true if successful 
 */
bool bno055_set_op_mode(uint8_t _mode)
{
  // write to op mode register
  if(!bno055_write_reg((uint8_t)REG_OPR_MODE, _mode)) return false; // no point in delaying if write failed
  // must delay at least 7ms for all changes except to CONFIGMODE where the dalay is at least 19ms
  nrf_delay_ms((uint32_t)OPR_MODE_DELAY);
  return true;
}

/**
 * fx: bool read_data_regs(uint8_t * regs, uint8_t num_of_regs,  uint16_t * _buff, uint8_t _buff_len)
 * reads data registers, appends to 16 bits and stores in array
 * _buff[0] = x, _buff[1] = y, _buff[2] = z
 * 
 * Arguments
 * ---------------
 * _buff - array to hold the data, must be uint16_t and size 3
 * regs - array of siz registers containing MSB and LSB data for three axis
 * 
 * return
 * ---------------
 * true if successful 
 */
bool read_data_regs(uint8_t * regs, uint8_t num_of_regs,  uint16_t * _buff, uint8_t _buff_len)
{
  uint8_t bytes[num_of_regs];

  for(int i = 0; i < num_of_regs; i++)
  {
    if(!bno055_read_reg( regs[i], &bytes[i], LEN_DATA)) return false; 
    // if at any point this sequence fails, return false
  }
  //populate  buffer argument
  for(int j = 0; j < _buff_len; j++)
  {
    _buff[j] = (uint16_t)(bytes[num_of_regs-((j+1)*2)] << 8) | bytes[num_of_regs-((j*2)+1)];
  } 

  return true;
}

/**
 * fx: bool bno055_init()
 * setup bno055
 * 
 * Arguments
 * -----------
 * 
 * return
 * -----------
 * true if successful
 */
bool bno055_init(uint8_t * _id)
{
  nrf_gpio_cfg_output(BNO_PS0);
  nrf_gpio_cfg_input(BNO_INT, NRF_GPIO_PIN_NOPULL);
  bno055_set_i2c_protocol(STANDARD_I2C);
  // set to 
  if( !i2c_init(BNO_SDA, BNO_SCL)) return false;

  if(!bno055_get_chip_ID(_id)) return false;
  if(!bno055_set_op_mode(NDOF)) return false;

  return true;
}

/**
 * fx: void bno055_set_i2c_protocol(bno_i2c_protocol _ps)
 * select an I2C protocol for the BNO
 * 
 * Arguments
 * -----------
 * The protocol required
 * STANDARD_I2C
 * HID_OVER_I2C
 * 
 * return
 * -----------
 * void
 */

void bno055_set_i2c_protocol( bno_i2c_protocol _ps )
{
  if( _ps == STANDARD_I2C )
  {
    nrf_gpio_pin_clear(BNO_PS0);
  }
  else
  {
    nrf_gpio_pin_set(BNO_PS0);
  } 
}

/**
 * fx: bool read_<sensor>_data(uint16_t * _data_arr)
 * reads grv reg data and stores it in a uint16_t data array of variable size
 * _<sensor>_data_arr[0] = x, _<sensor>_data_arr[1] = y, _<sensor>_data_arr[2] = z, _<sensor>_data_arr[3] = w
 * 
 * Arguments
 * ---------------
 * _<sensor>_data_arr - array to hold the data, must be uint16_t and size 3
 * 
 * return
 * ---------------
 * true if successful 
 */

bool read_grv_data(uint16_t * _data_arr)
{
  uint8_t regs[6] = {
    (uint8_t)REG_GRV_Data_Z_MSB,
    (uint8_t)REG_GRV_Data_Z_LSB,
    (uint8_t)REG_GRV_Data_Y_MSB,
    (uint8_t)REG_GRV_Data_Y_LSB,
    (uint8_t)REG_GRV_Data_X_MSB,
    (uint8_t)REG_GRV_Data_X_LSB};
  
  return read_data_regs(regs, sizeof(regs), _data_arr, 3);
}

bool read_lia_data(uint16_t * _data_arr)
{
  uint8_t regs[6] = {
    (uint8_t)REG_LIA_Data_Z_MBS,
    (uint8_t)REG_LIA_Data_Z_LSB,
    (uint8_t)REG_LIA_Data_Y_MBS,
    (uint8_t)REG_LIA_Data_Y_LSB,
    (uint8_t)REG_LIA_Data_X_MBS,
    (uint8_t)REG_LIA_Data_X_LSB};
  
  return read_data_regs(regs, sizeof(regs), _data_arr, 3);
}

bool read_gyr_data(uint16_t * _data_arr)
{
  uint8_t regs[6] = {
    (uint8_t)REG_GYR_DATA_Z_MSB,
    (uint8_t)REG_GYR_DATA_Z_LSB,
    (uint8_t)REG_GYR_DATA_Y_MSB,
    (uint8_t)REG_GYR_DATA_Y_LSB,
    (uint8_t)REG_GYR_DATA_X_MSB,
    (uint8_t)REG_GYR_DATA_X_LSB};
  
  return read_data_regs(regs, sizeof(regs), _data_arr, 3);
}

bool read_mag_data(uint16_t * _data_arr)
{
  uint8_t regs[6] = {
    (uint8_t)REG_MAG_DATA_Z_MSB,
    (uint8_t)REG_MAG_DATA_Z_LSB,
    (uint8_t)REG_MAG_DATA_Y_MSB,
    (uint8_t)REG_MAG_DATA_Y_LSB,
    (uint8_t)REG_MAG_DATA_X_MSB,
    (uint8_t)REG_MAG_DATA_X_LSB};
  
  return read_data_regs(regs, sizeof(regs), _data_arr, 3);
}

bool read_acc_data(uint16_t * _data_arr)
{
  uint8_t regs[6] = {
    (uint8_t)REG_ACC_DATA_Z_MSB,
    (uint8_t)REG_ACC_DATA_Z_LSB,
    (uint8_t)REG_ACC_DATA_Y_MSB,
    (uint8_t)REG_ACC_DATA_Y_LSB,
    (uint8_t)REG_ACC_DATA_X_MSB,
    (uint8_t)REG_ACC_DATA_X_LSB};
  
  return read_data_regs(regs, sizeof(regs), _data_arr, 3);
}

bool read_qua_data(uint16_t * _data_arr) // Size of 4 elements
{
  uint8_t regs[8] = {
    (uint8_t)REG_QUA_Data_z_MSB,
    (uint8_t)REG_QUA_Data_z_LSB,
    (uint8_t)REG_QUA_Data_y_MSB,
    (uint8_t)REG_QUA_Data_y_LSB,
    (uint8_t)REG_QUA_Data_x_MSB,
    (uint8_t)REG_QUA_Data_x_LSB,
    (uint8_t)REG_QUA_Data_w_MSB,
    (uint8_t)REG_QUA_Data_w_LSB};
  
  return read_data_regs(regs, sizeof(regs), _data_arr, 4);
}

bool read_eul_pitch_data(uint16_t * _data_arr) // Size of 1 elements
{
  uint8_t regs[2] = {
    (uint8_t)REG_EUL_Pitch_MSB,
    (uint8_t)REG_EUL_Pitch_LSB};
  
  return read_data_regs(regs, sizeof(regs), _data_arr, 1);
}

bool read_eul_roll_data(uint16_t * _data_arr) // Size of 1 elements
{
  uint8_t regs[2] = {
    (uint8_t)REG_EUL_Roll_MSB,
    (uint8_t)REG_EUL_Roll_LSB};
  
  return read_data_regs(regs, sizeof(regs), _data_arr, 1);
}

bool read_eul_heading_data(uint16_t * _data_arr) // Size of 1 elements
{
  uint8_t regs[2] = {
    (uint8_t)REG_EUL_Heading_MSB,
    (uint8_t)REG_EUL_Heading_LSB};
  
  return read_data_regs(regs, sizeof(regs), _data_arr, 1);
}
