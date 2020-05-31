/*
* I2C HAL for nRF52832
*/

/* System includes */
#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"

/* System log info inlcudes */
//#ifdef LOG_I2C_HAL
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
//#endif

/*lib specific includes */
#include "i2c_hal.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID     0

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = true;

/* data buffer */
uint8_t i2c_data;

/**
 * Write data to device on I2C bus
 * 
 * Arguments
 * ---------------
 * _address - I2C address for device
 * p_data - pointer to data to be writen 
 * length - the length of data to write in bytes
 * 
 * return
 * ---------------
 * true if successful
 */
bool i2c_write( uint8_t _address, uint8_t const *_p_data, uint8_t _length, bool _restart)
{
    // Note that there is no error check during this process
    do
    {
        __WFE();
    }while (m_xfer_done == false);
    ret_code_t err_code;
    m_xfer_done = false;
    err_code = nrf_drv_twi_tx(&m_twi, _address, _p_data, _length, _restart);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false);
    return (err_code == 0) ? true : false;
}

/**
 * Write data to device on I2C bus
 * 
 * Arguments
 * ---------------
 * _address - I2C address for device
 * p_data - pointer to data buffer
 * length - the length of data to read
 * 
 * return
 * ---------------
 * true if successful
 */
bool i2c_read( uint8_t _address, uint8_t * _buff, uint8_t _length)
{
    // Note that there is no error check during this process
    /* Read 1 byte from the specified address - skip 3 bits dedicated for fractional part of temperature. */
    do
    {
        __WFE();
    }while (m_xfer_done == false);
    m_xfer_done = false;
    ret_code_t err_code = nrf_drv_twi_rx(&m_twi, _address, _buff, _length);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false);
    return (err_code == 0) ? true : false;
}

/**
 * handle data from i2c bus
 */
__STATIC_INLINE void data_handler(uint8_t data)
{

#ifdef LOG_I2C_HAL
    //log data for now, 
    //TODO: present data appropriately here
    NRF_LOG_INFO("i2c data: %d", data);
#endif
}

/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
                data_handler(i2c_data);
            }
            m_xfer_done = true;
            break;
        default:
            break;
    }
}

/**
 * @brief UART initialization.
 */
bool i2c_init (uint32_t _sda, uint32_t _scl)
{
    ret_code_t err_code;
    
    const nrf_drv_twi_config_t twi_device = {
       .scl                = _scl,
       .sda                = _sda,
       .frequency          = NRF_DRV_TWI_FREQ_100K, // NRF_DRV_TWI_FREQ_400K for bno055
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH, //TODO: consider making this configurable
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_device, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);

    return (err_code == 0) ? true : false;
}