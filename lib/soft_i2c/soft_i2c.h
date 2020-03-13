#ifndef _SOFT_I2C_HAL_
#define _SOFT_I2C_HAL_

/*Global Defines */

//----------------

/* External APIs */
extern uint32_t _scl; 
extern uint32_t _sda;

void i2c_init();
bool i2c_write(uint8_t device_address, uint8_t* pbuffer, uint8_t buffer_length);
bool i2c_read(uint8_t device_address, uint8_t* pbuffer, uint8_t bytes_count);

#endif
