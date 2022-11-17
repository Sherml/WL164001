#ifndef _NCA9533_H_
#define _NCA9533_H_

#include <rtthread.h>

/* control cmd */
#define RT_NCA9533_CTRL_MODE     0x00

/* control value */
#define RT_NCA9555_MODE_OUTPUT  0x00
#define RT_NCA9555_MODE_INPUT   0x01
#define RT_NCA9555_POLARITY_NOR 0x00
#define RT_NCA9555_POLARITY_NOT 0x01

struct nca9555_pin_cfg
{
    rt_uint8_t pin;
    rt_uint8_t mode;
    rt_uint8_t polarity;
};

extern rt_err_t rt_nca9555_init(const char *dev_name, const char *i2c_name, rt_uint8_t i2c_addr);
extern void rt_nca9555_deinit(const char *dev_name);

#endif /* _TCA9543_H_ */
