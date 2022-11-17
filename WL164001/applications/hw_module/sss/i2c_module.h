///*
// * Copyright (c) 2006-2021, RT-Thread Development Team
// *
// * SPDX-License-Identifier: Apache-2.0
// *
// * Change Logs:
// * Date           Author       Notes
// * 2022-10-12     liwentai       the first version
// */
//
//#include <rthw.h>
//#include <rtdevice.h>
//#include <board.h>
//
//#ifndef APPLICATIONS_I2C_MODULE_I2C_MODULE_H_
//#define APPLICATIONS_I2C_MODULE_I2C_MODULE_H_
//
//#define  NCA_ADDR        0x42
//#define  NCA_ADDR_WR     0x42
//#define  NCA_ADDR_RD     0x43
//
//
////GPIO输出  输入模式
//#define  GPIO_MODE_INPUT                        0x00000000u         /*!< Input Floating Mode                   */
//#define  GPIO_MODE_OUTPUT_PP                    0x00000001u         /*!< Output Push Pull Mode                 */
//
//#define  GPIO_MODE_OUTPUT_OD                    0x00000011u         /*!< Output Open Drain Mode                */
//#define  GPIO_MODE_AF_PP                        0x00000002u         /*!< Alternate Function Push Pull Mode     */
//#define  GPIO_MODE_AF_OD                        0x00000012u         /*!< Alternate Function Open Drain Mode    */
//#define  GPIO_MODE_AF_INPUT                     GPIO_MODE_INPUT     /*!< Alternate Function Input Mode         */
//#define  GPIO_MODE_ANALOG                       0x00000003u         /*!< Analog Mode  */
//
////寄存器指令
//#define  REGISTER_0_INPUT_PORT                  0X00
//#define  REGISTER_1_INPUT_PORT                  0X01
//#define  REGISTER_0_OUTPUT_PORT                 0X02
//#define  REGISTER_1_OUTPUT_PORT                 0X03
//#define  REGISTER_0_POLARITY_INVERSION_PORT     0X04
//#define  REGISTER_1_POLARITY_INVERSION_PORT     0X05
//#define  REGISTER_0_CONFIGURATION_PORT          0X06
//#define  REGISTER_1_CONFIGURATION_PORT          0X07
//
//
///*
// *
// */
//
//typedef enum
//{
//    RED_LED,
//    GREEN_LED,
//    TYPE_MAX
//}led_type;
//
//#define     test1
//
//#ifdef      test0
//struct nca9555_device
//{
//    struct rt_i2c_bus_device *i2c_bus;
//    rt_mutex_t lock;
//};
//
//
//typedef struct nca9555_device *nca_device_t;
//
//nca_device_t nca9555_init(const char *i2c_bus_name);
//void nca9555_write_pin(nca_device_t dev, int index, int type, rt_uint8_t flag);
//rt_uint8_t nca9555_read_pin(nca_device_t dev, int index, int type);
//#endif
////struct rt_nca9555_ops
////{
////    rt_err_t    (*init)(nca_device_t nca);
////    rt_err_t    (*open)(nca_device_t nca);
////    rt_err_t    (*close)(nca_device_t nca);
////    rt_err_t    (*control)(nca_device_t nca);
////};
//
//
//
//#ifdef      test1
//
//void        nca9555_init(const char *i2c_bus_name);
//rt_err_t    nca9555_write(rt_uint8_t reg, rt_uint8_t byte);
//rt_err_t    nca9555_read(rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *buf);
//void        nca9555_write_pin(int index, int type, rt_uint8_t flag);
//rt_uint8_t  nca9555_read_pin(int index, int type);
//
//static struct rt_i2c_bus_device *i2c_bus;
//
//#endif
//
//
//#endif /* APPLICATIONS_I2C_MODULE_I2C_MODULE_H_ */
