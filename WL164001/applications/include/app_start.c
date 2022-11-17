/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-16     liwentai       the first version
 */
#include "app.h"

#define     DBG_TAG "app_start"
#define     DBG_LVL DBG_LOG
#include    <rtdbg.h>

rt_sem_t app_flag = RT_NULL;
static app_module_t obj[] =
{
        MOD_LED,
        MOD_AMR,
        MOD_ELEC,
        MOD_RFID,
        MOD_FLASH,
};

int app_open()
{
    app_flag = rt_sem_create("app_flag", 0, RT_IPC_FLAG_FIFO);

    for (int i = 0; i < ITEM_NUM(obj); i++) {
        /*初始化模块*/
        obj[i].thread = rt_thread_create(obj[i].name,
                                         obj[i].entry,
                                         RT_NULL,
                                         obj[i].stack_size,
                                         obj[i].priority,
                                         obj[i].tick);
        if (obj[i].thread == RT_NULL)
        {
            LOG_E("can't allocate memory for thread %s",obj[i].name);
            free(obj[i].thread);
        }else {
            rt_thread_startup(obj[i].thread);
        }
    }
    return RT_EOK;
}

INIT_COMPONENT_EXPORT(app_open);
