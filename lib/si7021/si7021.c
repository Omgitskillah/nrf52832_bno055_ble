#include <stdbool.h>
#include <stdint.h>

/*system includes*/
#include "nrf_delay.h"

/* System log info inlcudes */
#ifdef LOG_BNO055
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#endif

#include "si7021_config.h"
#include "signal.h"

uint32_t sernum_a;
uint32_t sernum_b;

void reset(void);
void read_revision(void);
void read_serial_number(void);
bool read_register(uint8_t _reg);

void reset(void)
{
    
}
