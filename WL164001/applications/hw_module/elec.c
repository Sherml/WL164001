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

static void elec_toggle(WL164001_t board, lock_status_t stat,int i)
{
    char a_point[6],b_point[6];
    if (stat == unlock) {
        rt_sprintf(a_point, "%s%d%s%d","IO", 0, "_", i - board.pos);
        rt_sprintf(b_point, "%s%d%s%d","IO", 1, "_", i - board.pos);
    }
    if (stat == lock) {
        rt_sprintf(a_point, "%s%d%s%d","IO", 1, "_", i - board.pos);
        rt_sprintf(b_point, "%s%d%s%d","IO", 0, "_", i - board.pos);
    }
    rt_device_control(board.elec, OUTPUT, b_point);
    rt_device_write(board.elec, PIN_LOW, b_point, 0);

    rt_device_control(board.elec, OUTPUT, a_point);
    rt_device_write(board.elec, PIN_HIGH, a_point, 0);
    rt_thread_mdelay(50);
    rt_device_write(board.elec, PIN_LOW, a_point, 0);
}

/**
 * @brief 电磁铁操作函数.
 * @param i      需要动作的电磁铁索引.
 * @param stat   需要做的动作.
 * @note stat的动作有：unlock = 0; lock = 1
 * @return 电磁铁操作结果.
 *      @retval -1  失败.
 *      @retval 0   成功.
 */
rt_err_t elec_action(int i, lock_status_t stat)
{
    if (i  > APP_NUM - 1) {
        LOG_E("open lock: the chosen pin is conflict with board hardware.");
        return -RT_ERROR;
    }
    if (i >= board[BOARD_NUM-1].pos) {
        elec_toggle(board[BOARD_NUM-1], stat, i);
    }else {
        elec_toggle(board[BOARD_NUM-2], stat, i);
    }
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




