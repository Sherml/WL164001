/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-03     liwentai       the first version
 */
#include <stat.h>

#define     DBG_TAG "STAT"
#define     DBG_LVL DBG_LOG
#include    <rtdbg.h>

rt_err_t stat_init(rt_device_t stat)
{
    char str[6];
    if (stat == RT_NULL) {
        LOG_E("stat device is not exit.");
        return -RT_ERROR;
    }
    rt_device_open(stat, RT_DEVICE_OFLAG_RDWR);

    for(int j=0; j<2; j++){
        for(int i=0; i< APP_NUM_MAIN; i++){
            rt_sprintf(str, "%s%d%s%d","IO",j,"_",i);
            rt_device_control(stat, INPUT, str);
            key[i].key_status = key_in;
            key[i].last_key_status = key_out;
        }
    }
    return RT_EOK;
}

static rt_uint8_t get_amr(int i, WL164001_t board)
{
    int off = 0;
    if (i>=APP_NUM) {
        LOG_E("check the number of stat_work.");
        return -RT_ERROR;
    }
    char str[6];
    rt_uint8_t amr_status;
    off = i - board.pos;
    rt_sprintf(str, "%s%d%s%d", "IO", 0, "_", off);
    LOG_D("%s",str);
    amr_status = rt_device_read(board.stat, 0, str, 0);
    return amr_status;
}

static rt_uint8_t get_lock(int i, WL164001_t board)
{
    int off = 0;
    if (i>=APP_NUM) {
        LOG_E("check the number of stat_work.");
        return -RT_ERROR;
    }
    char str[6];
    rt_uint8_t lock_status;
    off = i - board.pos;
    rt_sprintf(str, "%s%d%s%d", "IO", 0, "_", off);
    lock_status = rt_device_read(board.stat, 0, str, 0);
    return lock_status;
}

/**
 * @brief 钥匙在位检测消抖写入钥匙状态
 * @param amr 在位检测传感器设备句柄
 */
static rt_uint8_t check_amr[APP_NUM][7] = { 0 };
void check_key(WL164001_t board)
{
    int sum, i, index ;

    rt_uint8_t amr_status = 0;

    for (index = board.pos; index < APP_NUM_MAIN + board.pos; index++) {
        amr_status = get_amr(index, board);
        sum = amr_status;
        for(i = 0;i < 6;i++){
            check_amr[index][i] = check_amr[index][i+1];
            sum += check_amr[index][i];
        }
        check_amr[index][6] = amr_status;

        if (sum > 4) {
            key[index].key_status = key_out;
        } else {
            key[index].key_status = key_in;
        }
    }
}

/**
 * @brief 电磁铁状态检测消抖写入磁铁闭合情况
 * @param amr 在位检测传感器设备句柄
 */
static rt_uint8_t check_elec[APP_NUM][7] = { 0 };
void check_lock(WL164001_t board)
{
    int sum, i, index ;

    rt_uint8_t lock_stat = 0;

    for (index = 0; index < APP_NUM; index++) {
        lock_stat = get_lock(index, board);
        sum = lock_stat;
        for(i = 0;i < 6;i++){
            check_elec[index][i] = check_elec[index][i+1];
            sum += check_elec[index][i];
        }
        check_elec[index][6] = lock_stat;

        if (sum > 4) {
            key[index].lock_status = lock;
        } else {
            key[index].lock_status = unlock;
        }
    }
}


