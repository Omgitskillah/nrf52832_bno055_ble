#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#include "soft_i2c.h"

uint32_t _scl = ARDUINO_2_PIN; //default
uint32_t _sda = ARDUINO_3_PIN; //default

// Soft i2c low level drivers
void config_pins(uint32_t _SCL, uint32_t _SDA);
void scl_low(void);
void scl_high(void);
void sda_low(void);
void sda_high(void);
bool read_sda(void);
bool read_scl(void);
void i2c_delay(void);

// i2c protoco drivers
void i2c_start(void);
void i2c_stop(void);
void ack_slave(void);
void nack_slave(void);
bool wait_ack(void);
void i2c_tx_byte(uint8_t _byte);
uint8_t i2c_rx_byte(void);


bool i2c_read(uint8_t device_address, uint8_t* pbuffer, uint8_t bytes_count)
{
	i2c_start();

	i2c_tx_byte( device_address | 0x01 );

	if(!wait_ack()) return false;

    // i2c_master_start(); //TODO; A restart is not needed for BNO055

	while ( bytes_count )
	{
        i2c_delay();

		*pbuffer = i2c_rx_byte();
        ack_slave();
		if (bytes_count == 1)
			nack_slave();
		else
			ack_slave();
		pbuffer++;
		bytes_count--;
	}

	i2c_stop();

	return true;    
}

bool i2c_write(uint8_t device_address, uint8_t* pbuffer, uint8_t buffer_length)
{
    i2c_start();
    i2c_tx_byte(device_address | 0x00);
    if(!wait_ack())
    {
        i2c_stop();
        return false;
    }
    for(int i = 0; i < buffer_length; i++)
    {
        i2c_tx_byte(pbuffer[i]);
        if(!wait_ack())
        {
            i2c_stop();
            i2c_start();
            return false;
        }
    }
    i2c_stop();

    return true;
} 

// Receive one byte from slave
uint8_t i2c_rx_byte(void)
{
    uint8_t _byte = 0;

    sda_high();
    i2c_delay();

    for(int i = 0; i < 8; i++)
    {
        _byte <<= 1;
        do
        {
            scl_high();
        } while (!read_scl()); // wait for scl to go high

        i2c_delay();

        if(read_sda())
        {
            _byte |= 0x01;
        }
        scl_low();
        i2c_delay();    
    }

    scl_low();
    sda_low();
    i2c_delay();

    return _byte;
}

// transmit one byte to slave
void i2c_tx_byte(uint8_t _byte)
{
   for(int i = 0; i < 8; i++)
   {
       scl_low();
       i2c_delay();
       if (_byte & 0x80)
       {
           sda_high();
       }
       else
       {
           sda_low();
       }
       _byte <<= 1; //shift left
       i2c_delay();
       scl_high();
       i2c_delay();
   } 
   scl_low();
   i2c_delay();
}

// Send ack to slave device
void ack_slave(void)
{
    scl_low();
	i2c_delay();
	sda_low();
	i2c_delay();
	scl_high();
	i2c_delay();
	scl_low();
	i2c_delay();
}

// Send nack to slave device
void nack_slave(void)
{
	scl_low();
	i2c_delay();
	sda_high();
	i2c_delay();
	scl_high();
	i2c_delay();
	scl_low();
	i2c_delay();
}

// wait for slave to ack received data
bool wait_ack(void)
{
    scl_low();
    i2c_delay();
    sda_high();
    i2c_delay();
    scl_high();

    while(!read_scl()); // Wait for scl to go high

    i2c_delay();

    if(read_sda())
    {
        scl_low();
        i2c_delay();

        return false;
    }

    scl_low();
    i2c_delay();

    return true;
}

void i2c_init()
{
    config_pins(_scl, _sda);
    sda_high();
    scl_high();
    sda_low();
    scl_low();
    sda_high();
    scl_high();
}

void i2c_stop(void)
{
    sda_low();
    scl_high();
    i2c_delay();
    sda_high();
    i2c_delay();
    scl_low();
    i2c_delay();
}

void i2c_start(void)
{
    sda_high();
    scl_high();
    i2c_delay();
    sda_low();
    i2c_delay();
    scl_low();
    i2c_delay();
}

// Pin drivers
void scl_low(void)
{
    nrf_gpio_pin_write(_scl, 0);
}

void scl_high(void)
{
    nrf_gpio_pin_write(_scl, 1);
}

void sda_low(void)
{
    nrf_gpio_pin_write(_sda, 0);
}

void sda_high(void)
{
    nrf_gpio_pin_write(_sda, 1);
}

bool read_scl(void)
{
    bool pin_set = nrf_gpio_pin_out_read(_scl) ? true : false;
    return pin_set; 
}

bool read_sda(void)
{
    bool pin_set = nrf_gpio_pin_out_read(_sda) ? true : false;
    return pin_set; 
}

void i2c_delay(void)
{
    nrf_delay_us(2); // minimum is about 1.3uS, rounded up
}

// set pins as output open drain 
void config_pins(uint32_t _SCL, uint32_t _SDA)
{
    nrf_gpio_cfg(
    _SCL,
    GPIO_PIN_CNF_DIR_Output,
    GPIO_PIN_CNF_INPUT_Disconnect,
    GPIO_PIN_CNF_PULL_Disabled,
    GPIO_PIN_CNF_DRIVE_S0D1,
    GPIO_PIN_CNF_SENSE_Disabled);

    nrf_gpio_cfg(
    _SDA,
    GPIO_PIN_CNF_DIR_Output,
    GPIO_PIN_CNF_INPUT_Disconnect,
    GPIO_PIN_CNF_PULL_Disabled,
    GPIO_PIN_CNF_DRIVE_S0D1,
    GPIO_PIN_CNF_SENSE_Disabled);
}

