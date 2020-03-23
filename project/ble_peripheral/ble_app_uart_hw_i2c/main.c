#include <stdint.h>
#include <string.h>

#include "nrf_delay.h"
#include "ble_uart_peripheral.h"
#include "i2c_hal.h"

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
    twi_init();
  
    // Start execution.
#ifdef LOG_BLE_UART_PERIPHERAL    
    NRF_LOG_INFO("Debug logging for UART over RTT started.");
#endif    
    ble_uart_peripheral_advertising_start();

    // Enter main loop.
    for (;;)
    {
        uint8_t hello[] = "hello world";
        uint8_t print_data[35] = {0};

        sprintf(print_data, "temperature: %d, humidity: %d\n", 20, 80);
        
        ble_uart_peripheral_send_to_ble(hello, sizeof(hello));
        ble_uart_peripheral_print_to_uart(hello, sizeof(hello));

        ble_uart_peripheral_send_to_ble(print_data, sizeof(print_data));
        ble_uart_peripheral_print_to_uart(print_data, sizeof(print_data));

        nrf_delay_ms(1000); //delay for 1000ms
        ble_uart_peripheral_idle_state_handle();
    }
}