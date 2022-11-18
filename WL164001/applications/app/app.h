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

#define MOD_LED                          \
    {                                    \
        .name       = "LED",             \
        .entry      = led_entry,         \
        .stack_size = 1024,              \
        .priority   = 17,                \
        .tick       = 15,                \
    }

#define MOD_AMR                          \
    {                                    \
        .name       = "STAT",            \
        .entry      = stat_entry,        \
        .stack_size = 1024,              \
        .priority   = 17,                \
        .tick       = 15,                \
    }

#define MOD_ELEC                         \
    {                                    \
        .name       = "ELEC",            \
        .entry      = elec_entry,        \
        .stack_size = 1024,              \
        .priority   = 20,                \
        .tick       = 15,                \
    }

#define MOD_RFID                         \
    {                                    \
        .name       = "RFID",            \
        .entry      = rfid_entry,        \
        .stack_size = 2048,              \
        .priority   = 17,                \
        .tick       = 15,                \
    }

#define MOD_FLASH                        \
    {                                    \
        .name       = "FLASH",           \
        .entry      = flash_entry,       \
        .stack_size = 1024,              \
        .priority   = 18,                \
        .tick       = 15,                \
    }

#define BOARD_config_0                   \
    {                                    \
        .led_name       = "LED0",        \
        .stat_name      = "STAT0",       \
        .elec_name      = "ELEC0",       \
        .M606_name      = "M606_0",      \
        .pos            = 0              \
    }

#define BOARD_config_1                   \
    {                                    \
        .led_name       = "LED1",        \
        .stat_name      = "STAT1",       \
        .elec_name      = "ELEC1",       \
        .M606_name      = "M606_1",      \
        .pos            = 6              \
    }



typedef struct app_module
{
    const char*     name;
    rt_thread_t     thread;
    void            (*entry)(void*);
    rt_uint32_t     stack_size;
    rt_uint8_t      priority;
    rt_uint32_t     tick;
} app_module_t;




extern rt_sem_t app_flag;

extern void led_entry(void* parameter);
extern void stat_entry(void* parameter);
extern void elec_entry(void* parameter);
extern void rfid_entry(void* parameter);
extern void flash_entry(void* parameter);

#endif /* APPLICATIONS_INCLUDE_APP_H_ */
