#include <stdbool.h>
#include <stdint.h>
/*system includes*/
#include "nrf_delay.h"
#include "ble_uart_peripheral.h"

#include "soft_i2c.h"
#include "htu21d_config.h"
#include "htu21d.h"

uint8_t buffer[50];

bool htu21d_nh_get_data(uint8_t _command, uint32_t _delay, uint16_t *_data_buffer);

bool htu21d_init()
{
  // set to 
  i2c_init(ARDUINO_SDA_PIN, ARDUINO_SCL_PIN);
  nrf_delay_ms((uint8_t)HTU21D_STRATUP_DELAY);
}

uint16_t htu21d_nh_temperature()
{
    bool _ret = false;
    uint16_t _temperature;
    
    _ret = htu21d_nh_get_data((uint8_t)MEASURE_TEMPERATURE_NOHOLD, MEASURE_TEMPERATURE_DELAY, &_temperature);

    sprintf(buffer, "htu21d temperature: %d\n", _temperature);
    ble_uart_peripheral_print_to_uart(buffer, sizeof(buffer));
    // Only return a valid value
    return _ret;
}

uint16_t htu21d_nh_rhumidity()
{
    bool _ret = false;
    uint16_t _relative_humidity;

    _ret = htu21d_nh_get_data((uint8_t)MEASURE_HUMIDITY_NOHOLDE, MEASURE_HUMIDITY_DELAY, &_relative_humidity);

    sprintf(buffer, "htu21d humidity: %d\n", _relative_humidity);
    ble_uart_peripheral_print_to_uart(buffer, sizeof(buffer));
    // Only return a valid value
    return _ret;
}

bool htu21d_nh_get_data(uint8_t _command, uint32_t _delay, uint16_t *_data_buffer)
{
    bool _ret = false;
    uint8_t command[1] = {_command};
    uint8_t attempts = 3; // Maximum number of attempts to get an ACK from the HTU

    if( !i2c_write((uint8_t)HTU21D_ADDRESS, command, sizeof(command)) ) return false;

    uint8_t read_buffer[3];
    for( int i = 0; i < attempts; i++ )
    {
        _ret = i2c_read((uint8_t)HTU21D_ADDRESS, read_buffer, sizeof(read_buffer));
        if( _ret ) break;
        // else
        nrf_delay_ms(_delay); // wait a few ms then try again
    }

    if( !_ret ) return false;
    //else get temperature
    *_data_buffer = (read_buffer[1] & 0xFC) | (uint16_t)(read_buffer[0] << 8) ; // ignore the crc

    return true;    
}


