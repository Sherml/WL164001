/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-01     liwentai       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <drv_i2c_nca9555.h>

#define NCA9555_DEVICE_NAME "LED"

rt_device_t dev = RT_NULL;

static rt_thread_t nca9555;
static void nca9555_entry(void *parameter);

static void nca9555_entry(void *parameter)
{
    char str[6];
    rt_sprintf(str, "%s%d%s%d","IO",0,"_",1);

     /*第二位可以有两种控制引脚方式：OUTPUT = 0, INPUT=1
              第三位使用字符串选择引脚，格式必须类似于 "IO0_3"*/

    rt_device_control(dev, OUTPUT, "IO0_1");
    rt_device_control(dev, OUTPUT, "IO0_2");
    rt_device_control(dev, OUTPUT, "IO0_3");

    rt_device_control(dev, OUTPUT, "IO1_1");
    rt_device_control(dev, OUTPUT, "IO1_2");
    rt_device_control(dev, OUTPUT, "IO1_3");

//    rt_uint8_t pin = 0;

    while(1)
    {
        //第二位 pos 用于指定引脚电平，第三位用于指定引脚，第四位无效
        rt_device_write(dev, PIN_LOW, str, 0);

        //第二位 pos 无效，第三位用于指定读取引脚，第四位无效，将返回引脚逻辑电平


        if(rt_device_read(dev, 0, str, 0)==PIN_LOW){
            rt_kprintf("%02x\n",rt_device_read(dev, 0, str, 0));
        }
        rt_thread_mdelay(500);

        rt_device_write(dev, PIN_HIGH, str, 0);
        if(rt_device_read(dev, 0, "IO0_1", 0)==PIN_HIGH){
            rt_kprintf("%02x\n",rt_device_read(dev, 0, "IO0_1", 0));
        }
        rt_thread_mdelay(500);
    }

}

static void nca9555_sample(int argc, char *argv[])
{
    char name[RT_NAME_MAX];
    if (argc == 2)
    {
        rt_strncpy(name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(name, NCA9555_DEVICE_NAME, RT_NAME_MAX);
    }

    dev = rt_device_find(name);
    rt_device_open(dev, RT_DEVICE_OFLAG_RDWR);

    nca9555 = rt_thread_create("nca9555", nca9555_entry, RT_NULL, 1024, 20, 15);
    rt_thread_startup(nca9555);
}
MSH_CMD_EXPORT(nca9555_sample, use the nca9555 device);
