///*
// * Copyright (c) 2006-2021, RT-Thread Development Team
// *
// * SPDX-License-Identifier: Apache-2.0
// *
// * Change Logs:
// * Date           Author       Notes
// * 2022-10-18     liwentai       the first version
// */
#ifndef APPLICATIONS_I2C_MODULE_I2C_OPS_H_
#define APPLICATIONS_I2C_MODULE_I2C_OPS_H_

#include <board.h>

#define  NCA_ADDR        0x42
#define  NCA_ADDR_WR     0x42
#define  NCA_ADDR_RD     0x43


//寄存器指令
#define  REGISTER_0_INPUT_PORT                  0X00
#define  REGISTER_1_INPUT_PORT                  0X01
#define  REGISTER_0_OUTPUT_PORT                 0X02
#define  REGISTER_1_OUTPUT_PORT                 0X03
#define  REGISTER_0_POLARITY_INVERSION_PORT     0X04
#define  REGISTER_1_POLARITY_INVERSION_PORT     0X05
#define  REGISTER_0_CONFIGURATION_PORT          0X06
#define  REGISTER_1_CONFIGURATION_PORT          0X07


/*
 *
 */

typedef enum
{
    RED_LED,
    GREEN_LED,
    TYPE_MAX
}led_type;

rt_uint8_t  I2C_Start(void);
void        I2C_Stop(void);
rt_uint8_t  I2C_Send_Byte(rt_uint8_t i2c_data);


rt_uint8_t nca9555_write(rt_uint8_t address,
                              rt_uint8_t command,
                              rt_uint16_t length,
                              rt_uint8_t *data);

void nca9555_read(rt_uint8_t address,
                       rt_uint8_t command,
                       rt_uint16_t length,
                       rt_uint8_t *data);

void nca9555_write_pin(int index, int type, rt_uint8_t flag);

rt_uint8_t nca9555_read_pin(int index, int type);



#endif /* APPLICATIONS_I2C_MODULE_I2C_OPS_H_ */
