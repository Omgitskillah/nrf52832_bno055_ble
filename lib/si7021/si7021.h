#ifndef _SI7021_
#define _SI7021_

bool si7021_init(void);
float read_humidity(void);
float read_temperature(void);

extern uint32_t sernum_a;
extern uint32_t sernum_b;


#endif
