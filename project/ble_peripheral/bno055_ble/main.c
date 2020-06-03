#include <stdint.h>
#include <string.h>

#include "nrf_delay.h"
#include "ble_uart_peripheral.h"
#include "i2c_hal.h"
#include "bno055.h"

#define LOG_BLE_UART_PERIPHERAL

#ifdef LOG_BLE_UART_PERIPHERAL
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#endif

#ifdef LOG_BLE_UART_PERIPHERAL
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
} 
#endif

/**@brief Application main function.
 */
int main(void)
{
    uint8_t bno_id = 0;
    uint8_t output_data[100] = {0};
#ifdef LOG_BLE_UART_PERIPHERAL    
    log_init();
    NRF_LOG_INFO("Debug logging for UART over RTT started.");
    NRF_LOG_FLUSH();
#endif
    
    ble_uart_peripheral_init();

    if( !bno055_init(&bno_id) )
    {
        ble_uart_peripheral_print_to_uart("NO BNO055 detected", 18);
#ifdef LOG_BLE_UART_PERIPHERAL 
        NRF_LOG_INFO("NO BNO055 detected");
        NRF_LOG_FLUSH();
#endif
    }
    
    ble_uart_peripheral_advertising_start();


    // Enter main loop.
    for (;;)
    {

        uint16_t qua_data[4];
        
        if( read_qua_data(qua_data) )
        {
          sprintf(output_data, "%02X:%d,%d,%d,%d\n", bno_id, qua_data[0], qua_data[1], qua_data[2], qua_data[3]);

          ble_uart_peripheral_send_to_ble(output_data, sizeof(output_data));
          ble_uart_peripheral_print_to_uart(output_data, sizeof(output_data));
#ifdef LOG_BLE_UART_PERIPHERAL 
          NRF_LOG_INFO(output_data);
          NRF_LOG_FLUSH();
#endif
        }
        else
        {
          ble_uart_peripheral_send_to_ble("data error", 10);
#ifdef LOG_BLE_UART_PERIPHERAL 
          NRF_LOG_INFO("data error");
          NRF_LOG_FLUSH();
#endif
        }

#ifdef LOG_BLE_UART_PERIPHERAL 
        NRF_LOG_FLUSH();
#endif
        nrf_delay_ms(10); //delay for 10ms
        ble_uart_peripheral_idle_state_handle();
    }
}