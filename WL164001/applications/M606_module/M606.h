/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-28     liwentai       the first version
 */
#ifndef APPLICATIONS_RFID_MODULE_DRV_M606_H_
#define APPLICATIONS_RFID_MODULE_DRV_M606_H_

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>
#include <stdio.h>

#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])
/* M606 码值更新状态 */
#define M606_OK                      0               /**< 更新并打印 */
#define M606_FAULT                   1               /**< 更新但全0，不打印 */
#define M606_SAME                    2               /**< 不更新 */

/*
 * 定义M606连续检测的RFID个数
 * {0000000000,0000000000,0000000000,0000000000,0000000000,0000000000,0000000000,}
 * 1          /2         /3         /4         /5         /6         /7
 */
#define M606_RFID_NUM  6

/* M606的串口输出帧长度，若不能达到此长度是RTT串口驱动满包分包，和半包分包中断的影响，需要屏蔽*/
#define ONE_DATA_MAXLEN  80

/* M606的串口输出帧格式*/
#define RFID_SERIAL_CONFIG  \
{                           \
    BAUD_RATE_38400,        \
    DATA_BITS_8,            \
    STOP_BITS_1,            \
    PARITY_NONE,            \
    BIT_ORDER_LSB,          \
    NRZ_NORMAL,             \
    512,                    \
    0                       \
}

/*
 * M606配置表，偏移值pos表示该M606在输出的key起始编号
 */
#define M606_config_0           \
    {                           \
        .name      = "M606_0",  \
        .uart_name = "uart3",   \
        .pos       = 0          \
    }

#define M606_config_1           \
    {                           \
        .name      = "M606_1",  \
        .uart_name = "uart2",   \
        .pos       = 6          \
    }

typedef struct M606_device* M606_device_t;

struct M606_ops
{
    rt_err_t    (*scan_rfid)    (M606_device_t M606);
    int         (*rfid_printf)  (int i);
};

struct M606_device
{
    rt_device_t dev;
    struct rt_messagequeue rx_mq;
    int pos;
    char msg_pool[4000];

    struct M606_ops ops;
};

struct rt_M606_config
{
    M606_device_t dev;
    const char* name;
    const char* uart_name;
    rt_off_t pos;
};

struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};


M606_device_t M606_device_find(const char *M606_name);
rt_err_t scan_rfid(M606_device_t M606);
int rfid_printf(int i);

#endif /* APPLICATIONS_RFID_MODULE_DRV_M606_H_ */
