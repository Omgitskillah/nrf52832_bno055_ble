#ifndef _BLE_UART_
#define _BLE_UART_

void ble_uart_init();
void ble_uart_start_scan();
void ble_uart_idle_handle();
bool send_over_ble_uart(uint8_t * p_data, uint8_t data_len);

#endif