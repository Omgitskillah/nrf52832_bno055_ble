#ifndef _I2C_HAL_
#define _I2C_HAL_

/*Global Defines */

//----------------

/* External APIs */
bool twi_init (uint32_t _scl, uint32_t _sda);
bool i2c_write( uint8_t _address, uint8_t const *_p_data, uint8_t _length, bool _restart);
bool i2c_read( uint8_t _address, uint8_t * _buff, uint8_t _length);

#endif