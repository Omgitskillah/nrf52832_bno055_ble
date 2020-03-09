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

#include "bno055.h"
#include "i2c_hal.h"
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
 * Read output data registers (from page 34)
 * Handle interrupts and how to set them
 * Self test mode 
*/

/* fx declaration */
bool bno055_write_reg(uint8_t _reg, uint8_t _data);
bool bno055_read_reg(uint8_t _reg, uint8_t *_buff, uint8_t _length);
bool bno055_get_chip_ID(uint8_t *_buff);
bool read_grv_data(uint16_t * _grv_data_arr);
bool read_data_regs(uint8_t * regs, uint16_t * _buff);

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
  uint8_t bno055w = bno055_address | 0x00;
  //write sequence
  if(!i2c_write( bno055w, _buff, sizeof(_buff), false)) return false;
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
  uint8_t reg_buff[1] = {_reg};
  uint8_t bno055w = bno055_address | 0x00;
  uint8_t bno055r = bno055_address | 0x01;
  //write sequence
  if(!i2c_write( bno055w, reg_buff, 1, true)) return false;
  //Read sequence
  if(!i2c_read( bno055r, _buff, _length)) return false;

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
  // must delay at least 7ms for all chages except to CONFIGMODE where the dalay is at least 19ms
  nrf_delay_ms(OPR_MODE_DELAY);
  return true;
}

/**
 * fx: read_data_regs(uint8_t * regs, uint16_t * _buff)
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
bool read_data_regs(uint8_t * regs, uint16_t * _buff)
{
  uint8_t bytes[6];
  for(int i = 0; i < sizeof(regs); i++)
  {
    if(!bno055_read_reg( regs[i], &bytes[i], LEN_DATA)) return false; 
    // if at any point this sequence fails, return false
  }
  //populate  buffer argument
  _buff[0] = (uint16_t)(bytes[4] << 8) | bytes[5];
  _buff[1] = (uint16_t)(bytes[2] << 8) | bytes[3];
  _buff[2] = (uint16_t)(bytes[0] << 8) | bytes[1];  
}

/**
 * fx: bool read_grv_data(uint16_t * _grv_data_arr)
 * reads grv reg data and stores it in a uint16_t data array of size 3
 * _grv_data_arr[0] = x, _grv_data_arr[1] = y, _grv_data_arr[2] = z
 * 
 * Arguments
 * ---------------
 * _grv_data_arr - array to hold the data, must be uint16_t and size 3
 * 
 * return
 * ---------------
 * true if successful 
 */
bool read_grv_data(uint16_t * _grv_data_arr)
{
  uint8_t regs[6] = {
    (uint8_t)REG_GRV_Data_Z_MSB,
    (uint8_t)REG_GRV_Data_Z_LSB,
    (uint8_t)REG_GRV_Data_Y_MSB,
    (uint8_t)REG_GRV_Data_Y_LSB,
    (uint8_t)REG_GRV_Data_X_MSB,
    (uint8_t)REG_GRV_Data_X_LSB};
  
  return read_data_regs(regs, _grv_data_arr);
}