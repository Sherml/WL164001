/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-16     liwentai       the first version
 */
#ifndef APPLICATIONS_INCLUDE_APP_H_
#define APPLICATIONS_INCLUDE_APP_H_

#include <tool.h>
#include <stat.h>
#include <led.h>
#include <elec.h>
#include "M606_module/M606.h"

/**
 * @addtogroup 线程定义
 */

/**@{*/

/* 钥匙指示灯模块线程 */
#define MOD_LED                          \
    {                                    \
        .name       = "LED",             \
        .entry      = led_entry,         \
        .stack_size = 1024,              \
        .priority   = 21,                \
        .tick       = 15,                \
    }

/* 钥匙在位检测模块线程 */
#define MOD_AMR                          \
    {                                    \
        .name       = "STAT",            \
        .entry      = stat_entry,        \
        .stack_size = 1024,              \
        .priority   = 21,                \
        .tick       = 15,                \
    }

/* 电磁铁状态检测模块线程 */
#define MOD_ELEC                         \
    {                                    \
        .name       = "ELEC",            \
        .entry      = elec_entry,        \
        .stack_size = 1024,              \
        .priority   = 21,                \
        .tick       = 15,                \
    }

/* RFID读码模块线程 */
#define MOD_RFID                         \
    {                                    \
        .name       = "RFID",            \
        .entry      = rfid_entry,        \
        .stack_size = 2048,              \
        .priority   = 20,                \
        .tick       = 15,                \
    }

/* FLASH模块线程 */
#define MOD_FLASH                        \
    {                                    \
        .name       = "FLASH",           \
        .entry      = flash_entry,       \
        .stack_size = 1024,              \
        .priority   = 18,                \
        .tick       = 15,                \
    }

/**@}*/

/**
 * @addtogroup 板级模块定义
 * @brief 将主板操作设备和副板设备定义为模块
 */

/**@{*/

/* 定义主板设备 */
#define BOARD_config_0                   \
    {                                    \
        .led_name       = "LED0",        \
        .stat_name      = "STAT0",       \
        .elec_name      = "ELEC0",       \
        .M606_name      = "M606_0",      \
        .pos            = 0              \
    }

/* 定义副板设备 */
#define BOARD_config_1                   \
    {                                    \
        .led_name       = "LED1",        \
        .stat_name      = "STAT1",       \
        .elec_name      = "ELEC1",       \
        .M606_name      = "M606_1",      \
        .pos            = 6              \
    }

/**@}*/

/* 模块的线程抽象 */
typedef struct app_module
{
    const char*     name;
    rt_thread_t     thread;
    void            (*entry)(void*);
    rt_uint32_t     stack_size;
    rt_uint8_t      priority;
    rt_uint32_t     tick;
} app_module_t;

/* 定义信号量用于线程通信 */
extern rt_sem_t app_flag;

/// @brief 定义线程的入口函数
extern void led_entry(void* parameter);
extern void stat_entry(void* parameter);
extern void elec_entry(void* parameter);
extern void rfid_entry(void* parameter);
extern void flash_entry(void* parameter);

#endif /* APPLICATIONS_INCLUDE_APP_H_ */
