/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-03     liwentai       the first version
 */
#include "led.h"

rt_device_t led_init(const char* dev_name)
{
    char str[6];
    rt_device_t led= RT_NULL;

    led= rt_device_find(dev_name);
    rt_device_open(led, RT_DEVICE_OFLAG_RDWR);
    for(int j=0; j<2; j++)
    {
        for(int i=0; i<APP_NUM; i++)
        {
            rt_sprintf(str, "%s%d%s%d","IO",j,"_",i);
            rt_device_control(led, OUTPUT, str);
        }
    }
    return led;
}

rt_device_t led2_init()
{
    char str[6];
    rt_device_t led2 = RT_NULL;

    led2 = rt_device_find("LED2");
    rt_device_open(led2, RT_DEVICE_OFLAG_RDWR);
    for(int j=0; j<2; j++)
    {
        for(int i=0; i<APP_NUM; i++)
        {
            rt_sprintf(str, "%s%d%s%d","IO",j,"_",i);
            rt_device_control(led2, OUTPUT, str);

        }
    }
    return led2;
}

