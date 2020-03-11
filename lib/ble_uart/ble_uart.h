#ifndef _BLE_UART_
#define _BLE_UART_

void ble_uart_init();
void ble_uart_scan_start();
void ble_uart_idle_handle();
bool ble_uart_send_over_ble(uint8_t * _string, uint8_t data_len);
void ble_uart_send_to_uart(uint8_t * _string);
void ble_uart_idle_state_handle(void);

#endif