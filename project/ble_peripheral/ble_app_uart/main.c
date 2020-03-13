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

extern uint32_t _scl;
extern uint32_t _sda;

/**@brief Application main function.
 */
int main(void)
{
    uint8_t bno055_id = 0;
    /*set the scl and sda pins based on "pca10040.h"
     defaults to _scl = ARDUINO_2_PIN; and _sda = ARDUINO_3_PIN;
     if nothing is set
    */
//    _scl = ARDUINO_6_PIN; 
//    _sda = ARDUINO_7_PIN;

    ble_uart_peripheral_init();
    bno055_init(&bno055_id); // init the bno and read the id
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
        uint8_t print_id[20] = {0};

        sprintf(print_id, "bno055 id: %d \n", bno055_id);
        
        ble_uart_peripheral_send_to_ble(hello, sizeof(hello));
        ble_uart_peripheral_print_to_uart(hello, sizeof(hello));

        ble_uart_peripheral_send_to_ble(print_id, sizeof(print_id));
        ble_uart_peripheral_print_to_uart(print_id, sizeof(print_id));

        nrf_delay_ms(1000); //delay for 1000ms
        ble_uart_peripheral_idle_state_handle();
    }
}