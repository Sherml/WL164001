/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-02     liwentai       the first version
 */
#ifndef APPLICATIONS_HW_MODULE_HW_ALL_WORK_C_
#define APPLICATIONS_HW_MODULE_HW_ALL_WORK_C_

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "M606_module/M606.h"


/* 单板最大同类器件个数 */
#define APP_NUM_MAIN 6

/* 副板使用设置 */
//#define USING_SLAVE_BOARD
#ifdef USING_SLAVE_BOARD
#define BOARD_NUM 2
#define APP_NUM 12
#endif
#ifndef USING_SLAVE_BOARD
#define BOARD_NUM 1
#define APP_NUM 6
#endif

#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])

typedef enum
{
    key_out = 1,     //取走
    key_in,      //插入
    key_erro,    //不匹配
} key_status_t;

typedef enum
{
    unlock,
    lock,
} lock_status_t;

typedef struct
{
    char ch[11];
    rt_uint8_t code[5];
} key_code_t;

//钥匙状态
typedef struct tagLockRfid
{
    rt_uint8_t cur_rfid_code[5];
    rt_uint8_t last_rfid_code[5];
    key_code_t user_data;
    key_status_t key_status;
    key_status_t last_key_status;
    lock_status_t lock_status;

} key_info_t;

typedef struct WL164001_board
{
    const char* led_name;
    const char* stat_name;
    const char* elec_name;
    const char* M606_name;
    int pos;

    rt_device_t led;
    rt_device_t stat;
    rt_device_t elec;
    M606_device_t M606;
} WL164001_t;

extern WL164001_t board[BOARD_NUM];
extern key_info_t key[APP_NUM];



#endif /* APPLICATIONS_HW_MODULE_HW_ALL_WORK_C_ */
