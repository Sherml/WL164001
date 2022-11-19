/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-12     liwentai       the first version
 */

#include    <rtdevice.h>
#include    <rtthread.h>
#include    <board.h>
#include    <tool.h>

#ifndef APPLICATIONS_I2C_MODULE_I2C_MODULE_H_
#define APPLICATIONS_I2C_MODULE_I2C_MODULE_H_

/**
 * @addtogroup NCA9555基本设定
 */

/**@{*/

/* NCA9555所在I2C总线 */
#define RT_I2C_NCA9555_NAME_1 "i2c1"
#define RT_I2C_NCA9555_NAME_2 "i2c2"

/* NCA9555地址 */
#define RT_I2C_STAT_ADDR     0x20
#define RT_I2C_LED_ADDR     0x21
#define RT_I2C_ELEC_ADDR    0x22

/**@}*/

/**
 * @addtogroup NCA9555控制定义
 */

/**@{*/

/* NCA9555端口控制模式 */
#define     OUTPUT      0                                       /**< 输出逻辑电平 */
#define     INPUT       1                                       /**< 输入逻辑电平 */
#define     INVERT      2                                       /**< 反转输入电平极性 */

/* 寄存器指令 */
#define  REGISTER_0_INPUT_PORT                  0X00
#define  REGISTER_1_INPUT_PORT                  0X01
#define  REGISTER_0_OUTPUT_PORT                 0X02
#define  REGISTER_1_OUTPUT_PORT                 0X03
#define  REGISTER_0_POLARITY_INVERSION_PORT     0X04
#define  REGISTER_1_POLARITY_INVERSION_PORT     0X05
#define  REGISTER_0_CONFIGURATION_PORT          0X06
#define  REGISTER_1_CONFIGURATION_PORT          0X07

/**@}*/

/**
 * @ingroup NCA9555基本设定
 *
 * @def NCA9555_config_0
 * @def NCA9555_config_1
 * @def NCA9555_config_2
 * @def NCA9555_config_3
 * @def NCA9555_config_4
 * @def NCA9555_config_5
 * 将单个NCA9555芯片抽象统一管理.
 */
#define NCA9555_config_0                     \
    {                                        \
        .addr       = RT_I2C_STAT_ADDR,      \
        .bus_name   = RT_I2C_NCA9555_NAME_1, \
        .name       = "STAT0"                \
    }

#define NCA9555_config_1                     \
    {                                        \
        .addr       = RT_I2C_LED_ADDR,       \
        .bus_name   = RT_I2C_NCA9555_NAME_1, \
        .name       = "LED0"                 \
    }

#define NCA9555_config_2                     \
    {                                        \
        .addr       = RT_I2C_ELEC_ADDR,      \
        .bus_name   = RT_I2C_NCA9555_NAME_1, \
        .name       = "ELEC0"                \
    }

#define NCA9555_config_3                     \
    {                                        \
        .addr       = RT_I2C_STAT_ADDR,      \
        .bus_name   = RT_I2C_NCA9555_NAME_2, \
        .name       = "STAT1"                \
    }

#define NCA9555_config_4                     \
    {                                        \
        .addr       = RT_I2C_LED_ADDR,       \
        .bus_name   = RT_I2C_NCA9555_NAME_2, \
        .name       = "LED1"                 \
    }

#define NCA9555_config_5                     \
    {                                        \
        .addr       = RT_I2C_ELEC_ADDR,      \
        .bus_name   = RT_I2C_NCA9555_NAME_2, \
        .name       = "ELEC1"                \
    }

/**
 * @ingroup NCA9555基本设定
 *
 * @def IO0_0
 * @def IO0_1
 * @def IO0_2
 * @def IO0_3
 * @def IO0_4
 * @def IO0_5
 * @def IO0_6
 * @def IO0_7
 * @def IO1_0
 * @def IO1_1
 * @def IO1_2
 * @def IO1_3
 * @def IO1_4
 * @def IO1_5
 * @def IO1_6
 * @def IO1_7 
 * 将NCA9555芯片16个引脚统一管理.
 */
#define IO0_0                \
    {                        \
        .name = "IO0_0",     \
        .offset = 0          \
    }

#define IO0_1                \
    {                        \
        .name = "IO0_1",     \
        .offset = 1          \
    }

#define IO0_2                \
    {                        \
        .name = "IO0_2",     \
        .offset = 2          \
    }

#define IO0_3                \
    {                        \
        .name = "IO0_3",     \
        .offset = 3          \
    }

#define IO0_4                \
    {                        \
        .name = "IO0_4",     \
        .offset = 4          \
    }

#define IO0_5                \
    {                        \
        .name = "IO0_5",     \
        .offset = 5          \
    }

#define IO0_6                \
    {                        \
        .name = "IO0_6",     \
        .offset = 6          \
    }

#define IO0_7                \
    {                        \
        .name = "IO0_7",     \
        .offset = 7          \
    }

#define IO1_0                \
    {                        \
        .name = "IO1_0",     \
        .offset = 8          \
    }

#define IO1_1                \
    {                        \
        .name = "IO1_1",     \
        .offset = 9          \
    }

#define IO1_2                \
    {                        \
        .name = "IO1_2",     \
        .offset = 10         \
    }

#define IO1_3                \
    {                        \
        .name = "IO1_3",     \
        .offset = 11         \
    }

#define IO1_4                \
    {                        \
        .name = "IO1_4",     \
        .offset = 12         \
    }

#define IO1_5                \
    {                        \
        .name = "IO1_5",     \
        .offset = 13         \
    }

#define IO1_6                \
    {                        \
        .name = "IO1_6",     \
        .offset = 14         \
    }

#define IO1_7                \
    {                        \
        .name = "IO1_7",     \
        .offset = 15         \
    }

/* 定义引脚信息对象 */
struct pin_list
{
    const char* name;
    rt_off_t offset;
};

/* 定义NCA9555对象 */
struct nca9555_device
{
    struct rt_i2c_bus_device* i2c_bus;
    rt_uint8_t addr;
};
typedef struct nca9555_device* nca_device_t;

/* 链接I2C设备与NCA9555设备 */
struct rt_nca9555
{
    nca_device_t dev;
    rt_uint8_t addr;
    const char* bus_name;
    const char* name;
};

/**
 * @ingroup NCA9555基本设定
 * @brief NCA9555的直接操作方法
 */

/**@{*/

nca_device_t nca9555_init(const char* i2c_bus_name, rt_uint8_t addr);
rt_err_t nca9555_write_data(nca_device_t dev, rt_uint8_t reg, rt_uint8_t len, rt_uint8_t* buf);
rt_err_t nca9555_read_data(nca_device_t dev, rt_uint8_t reg, rt_uint8_t len, rt_uint8_t* buf);

/**@}*/
#endif /* APPLICATIONS_I2C_MODULE_I2C_MODULE_H_ */
