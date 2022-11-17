/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-12     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdio.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>
#include <tool.h>
#include <drivers/pin.h>

#define RUN_LED  GET_PIN(A, 0)

int main(void){
    /*运行灯*/
    rt_pin_mode(RUN_LED, PIN_MODE_OUTPUT);

    while (1){
        rt_pin_write(RUN_LED, PIN_LOW);
        rt_thread_mdelay(1000);
        rt_pin_write(RUN_LED, PIN_HIGH);
        rt_thread_mdelay(1000);
    }
    return RT_EOK;
}




