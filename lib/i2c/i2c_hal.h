#ifndef _I2C_HAL_
#define _I2C_HAL_

/*Global Defines */

//----------------

/* External APIs */
void twi_init (uint32_t _scl, uint32_t _sda, nrf_drv_twi_frequency_t _freq);
void i2c_write( uint8_t _address, uint8_t const *p_data, uint8_t length);
uint8_t i2c_read( uint8_t _address, uint8_t _length);

#endif