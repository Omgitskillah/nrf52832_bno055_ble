#include <stdint.h>
#include <string.h>

#include "nrf_delay.h"
#include "ble_uart_peripheral.h"
#include "i2c_hal.h"
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
    uint8_t bno_id = 0;
    ble_uart_peripheral_init();
    bno055_init(&bno_id);
  
    // Start execution.
#ifdef LOG_BLE_UART_PERIPHERAL    
    NRF_LOG_INFO("Debug logging for UART over RTT started.");
#endif    
    ble_uart_peripheral_advertising_start();

    // Enter main loop.
    for (;;)
    {

        uint16_t qua_data[4];
        read_qua_data(qua_data);

        uint8_t output_data[100] = {0};
        sprintf(output_data, "bno055: %d; Q(w,x,y,z): %d, %d, %d, %d \n", bno_id, qua_data[0], qua_data[1], qua_data[2], qua_data[3]);

        ble_uart_peripheral_send_to_ble(output_data, sizeof(output_data));
        ble_uart_peripheral_print_to_uart(output_data, sizeof(output_data));

        nrf_delay_ms(1000); //delay for 1000ms
        ble_uart_peripheral_idle_state_handle();
    }
}