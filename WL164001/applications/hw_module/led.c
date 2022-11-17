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

#define     DBG_TAG "LED"
#define     DBG_LVL DBG_LOG
#include    <rtdbg.h>

rt_err_t led_init(rt_device_t led)
{
    if (led == RT_NULL) {
        LOG_E("led device is not exit.");
        return -RT_ERROR;
    }
    char str[6];
    rt_device_open(led, RT_DEVICE_OFLAG_RDWR);
    for(int j=0; j<2; j++)
    {
        for(int i=0; i<APP_NUM_MAIN; i++)
        {
            rt_sprintf(str, "%s%d%s%d","IO",j,"_",i);
            rt_device_control(led, OUTPUT, str);
        }
    }
    return RT_EOK;
}

rt_err_t led_only_red(int i, WL164001_t board)
{
    if (i>=APP_NUM) {
        LOG_E("check the number of led_work.");
        return -RT_ERROR;
    }
    int off = 0;
    char red[6], green[6];

    off = i - board.pos;
    rt_sprintf(red, "%s%d%s%d", "IO", 0, "_", off);
    rt_sprintf(green, "%s%d%s%d", "IO", 1, "_", off);
//    LOG_D("%s",red);
    rt_device_write(board.led, PIN_HIGH, red, 0); //在位亮红灯
    rt_device_write(board.led, PIN_LOW, green, 0);
    return RT_EOK;
}

rt_err_t led_only_green(int i, WL164001_t board)
{
    if (i>=APP_NUM) {
        LOG_E("check the number of led_work.");
        return -RT_ERROR;
    }
    int off = 0;
    char red[6], green[6];

    off = i - board.pos;
    rt_sprintf(red, "%s%d%s%d", "IO", 0, "_", off);
    rt_sprintf(green, "%s%d%s%d", "IO", 1, "_", off);
    rt_device_write(board.led, PIN_LOW, red, 0); //不在位亮绿灯
    rt_device_write(board.led, PIN_HIGH, green, 0);
    return RT_EOK;
}

void key_status_led(WL164001_t board)
{
    int i;
    for (i = board.pos; i < APP_NUM_MAIN + board.pos; i++) {
        if (key[i].key_status == key_in && key[i].last_key_status != key_in) {
            led_only_red(i, board);
            key[i].last_key_status = key[i].key_status;
        }

        if (key[i].key_status == key_out && key[i].last_key_status != key_out) {
            led_only_green(i, board);
            key[i].last_key_status = key[i].key_status;
        }
    }

}

