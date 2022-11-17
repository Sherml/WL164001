/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-03     liwentai       the first version
 */
#include "elec.h"

#define     DBG_TAG "ELEC"
#define     DBG_LVL DBG_LOG
#include    <rtdbg.h>

rt_err_t elec_open(int i, WL164001_t board){
    char open[6],close[6];
    rt_sprintf(open, "%s%d%s%d","IO", 0, "_", i - board.pos);
    rt_sprintf(close, "%s%d%s%d","IO", 1, "_", i - board.pos);

    if (i - board.pos > APP_NUM_MAIN) {
        LOG_E("open lock: the chosen pin is conflict with board hardware.");
        return -RT_ERROR;
    }

    rt_device_control(board.elec, OUTPUT, close);
    rt_device_write(board.elec, PIN_LOW, close, 0);

    rt_device_control(board.elec, OUTPUT, open);
    rt_device_write(board.elec, PIN_HIGH, open, 0);
    rt_thread_mdelay(50);
    rt_device_write(board.elec, PIN_LOW, open, 0);

    return RT_EOK;
}

rt_err_t elec_close(int i, WL164001_t board){
    char open[6],close[6];
    rt_sprintf(open, "%s%d%s%d","IO", 0, "_", i - board.pos);
    rt_sprintf(close, "%s%d%s%d","IO", 1, "_", i - board.pos);

    if (i - board.pos > APP_NUM_MAIN) {
        LOG_E("close lock: the chosen pin is conflict with board hardware.");
        return -RT_ERROR;
    }

    rt_device_control(board.elec, OUTPUT, open);
    rt_device_write(board.elec, PIN_LOW, open, 0);

    rt_device_control(board.elec, OUTPUT, close);
    rt_device_write(board.elec, PIN_HIGH, close, 0);
    rt_thread_mdelay(50);
    rt_device_write(board.elec, PIN_LOW, close, 0);

    return RT_EOK;
}

rt_err_t elec_init(rt_device_t elec)
{
    if (elec == RT_NULL) {
        LOG_E("elec device is not exit.");
        return -RT_ERROR;
    }
    rt_device_open(elec, RT_DEVICE_OFLAG_RDWR);
    return RT_EOK;
}




