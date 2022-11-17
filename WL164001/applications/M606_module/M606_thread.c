/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-10     liwentai       the first version
 */
#include "M606.h"





int M606_open()
{
    rt_thread_t rfid_action = RT_NULL;


    rfid_action = rt_thread_create("rfid_getcode", rfid, RT_NULL, 2048, 17, 15);

    rt_thread_startup(rfid_action);


    return RT_EOK;
}
MSH_CMD_EXPORT(M606_open, Open M606 device with RFID function);

