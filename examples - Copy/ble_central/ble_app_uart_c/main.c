/**
 * BNO055 Quaternion data over Bluetooth Lowe Energy UART
 * Demo project
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "ble_uart.h"
#include "bno055.h"

#ifdef LOG_ENABLED  
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#endif

/**@brief Function for initializing the nrf log module. */
static void log_init(void)
{
#ifdef LOG_ENABLED    
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
#endif
}

int main(void)
{
    // Initialize.
    ble_uart_init();

    
    log_init(); //Stubbed out internally

    // Start execution.
    #ifdef LOG_ENABLED  
    NRF_LOG_INFO("BLE UART central example started.");
    #endif
    ble_uart_scan_start();

    // Enter main loop.
    for (;;)
    {
        ble_uart_idle_state_handle();
    }
}
