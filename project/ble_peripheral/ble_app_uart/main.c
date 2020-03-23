#include <stdint.h>
#include <string.h>

#include "nrf_delay.h"
#include "ble_uart_peripheral.h"


#include "htu21d.h"

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
    /*set the scl and sda pins based on "pca10040.h"
     defaults to _scl = ARDUINO_2_PIN; and _sda = ARDUINO_3_PIN;
     if nothing is set
    */
//    _scl = ARDUINO_6_PIN; 
//    _sda = ARDUINO_7_PIN;

    ble_uart_peripheral_init();
    htu21d_init();

    uint16_t temperature = 0;
    uint16_t humidity = 0;
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
        temperature = htu21d_nh_temperature();
        humidity = htu21d_nh_rhumidity();
        uint8_t hello[] = "hello world";
        uint8_t print_htu[35] = {0};

        sprintf(print_htu, "temperature: %d, humidity: %d\n", temperature, humidity);
        
//        ble_uart_peripheral_send_to_ble(hello, sizeof(hello));
//        ble_uart_peripheral_print_to_uart(hello, sizeof(hello));

//        ble_uart_peripheral_send_to_ble(print_htu, sizeof(print_htu));
//        ble_uart_peripheral_print_to_uart(print_htu, sizeof(print_htu));
        ble_uart_peripheral_print_to_uart(".......................", sizeof(print_htu));
        nrf_delay_ms(1000); //delay for 1000ms
        ble_uart_peripheral_idle_state_handle();
    }
}