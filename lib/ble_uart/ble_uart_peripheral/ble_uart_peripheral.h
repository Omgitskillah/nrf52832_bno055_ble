#ifndef _BLE_UART_PERIPHERAL_
#define _BLE_UART_PERIPHERAL_

void ble_uart_peripheral_print_to_uart(uint8_t * _string, uint32_t _len);
void ble_uart_peripheral_init(void);
void ble_uart_peripheral_advertising_start(void);
void ble_uart_peripheral_idle_state_handle(void);
void ble_uart_peripheral_send_to_ble(uint8_t * _string, uint16_t data_len);

#endif