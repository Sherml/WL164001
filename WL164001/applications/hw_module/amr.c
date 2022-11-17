/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-03     liwentai       the first version
 */
#include "amr.h"

rt_device_t amr_init(const char* dev_name)
{
    char str[6];
    rt_device_t amr = RT_NULL;

    amr = rt_device_find(dev_name);
    rt_device_open(amr, RT_DEVICE_OFLAG_RDWR);

    for(int j=0; j<2; j++){
        for(int i=0; i<APP_NUM; i++){
            rt_sprintf(str, "%s%d%s%d","IO",j,"_",i);
            rt_device_control(amr, INPUT, str);
            key[i].key_status = key_in;
            key[i].last_key_status = key_out;
        }
    }
    return amr;
}

//rt_device_t amr_init()
//{
//    char str[6];
//    rt_device_t amr = RT_NULL;
//
//    amr = rt_device_find("AMR1");
//    rt_device_open(amr, RT_DEVICE_OFLAG_RDWR);
//
//    for(int j=0; j<2; j++){
//        for(int i=0; i<APP_NUM; i++){
//            rt_sprintf(str, "%s%d%s%d","IO",j,"_",i);
//            rt_device_control(amr, INPUT, str);
//            key[i].key_status = key_in;
//            key[i].last_key_status = key_out;
//        }
//    }
//    return amr;
//}

/**
 * @brief 钥匙在位检测消抖写入钥匙状态
 * @param amr 在位检测传感器设备句柄
 */
static rt_uint8_t check_amr[APP_NUM][7] = { 0 };
void check_key(rt_device_t amr)
{
    char str[6];
    int sum, i, index ;

    rt_uint8_t amr_status = 0;

    for (index = 0; index < APP_NUM; index++) {
        rt_sprintf(str, "%s%d%s%d", "IO", 1, "_", index);
        amr_status = rt_device_read(amr, 0, str, 0);
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
void check_lock(rt_device_t amr)
{
    char str[6];
    int sum, i, index ;

    rt_uint8_t lock_stat = 0;

    for (index = 0; index < APP_NUM; index++) {
        rt_sprintf(str, "%s%d%s%d", "IO", 0, "_", index);
        lock_stat = rt_device_read(amr, 0, str, 0);
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


