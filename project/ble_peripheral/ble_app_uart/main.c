#include <stdint.h>
#include <string.h>

#include "nrf_delay.h"
#include "ble_uart_peripheral.h"
#include "bno055.h"

#ifdef LOG_BLE_UART_PERIPHERAL
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#endif

/**@brief Application main function.
 */
int main(void)
{
    ble_uart_peripheral_init();

    uint8_t bno055_id = 0;
    bno055_init(&bno055_id);

#ifdef LOG_ENABLED
    log_init();
#endif    
    // Start execution.
#ifdef LOG_BLE_UART_PERIPHERAL    
    NRF_LOG_INFO("Debug logging for UART over RTT started.");
#endif    
    ble_uart_peripheral_advertising_start();

    // Enter main loop.
    for (;;)
    {
        uint8_t hello[] = "hello world";
        ble_uart_peripheral_send_to_ble(hello, 11);
        ble_uart_peripheral_print_to_uart(hello, 11);
        nrf_delay_ms(1000); //delay for 1000ms
        ble_uart_peripheral_idle_state_handle();
    }
}