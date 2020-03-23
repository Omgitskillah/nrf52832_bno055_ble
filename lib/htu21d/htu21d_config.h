#ifndef _HTU21D_CONFIG_
#define _HTU21D_CONFIG_

#define HTU21D_STRATUP_DELAY            16 // Ensure we wait for over 15ms before using the device 
#define MEASURE_TEMPERATURE_DELAY       44 
#define MEASURE_HUMIDITY_DELAY          14 

#define HTU21D_ADDRESS                  0x80

#define MEASURE_TEMPERATURE_HOLD        0xE3
#define MEASURE_HUMIDITY_HOLDE          0xE5
#define MEASURE_TEMPERATURE_NOHOLD      0xF3
#define MEASURE_HUMIDITY_NOHOLDE        0xF5
#define WRITE_USER_REGISTER             0xE6
#define READ_USER_REGISTER              0xE7
#define SOFT_RESET                      0xFE

#endif