/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-16     liwentai       the first version
 */
#include "M606.h"
#include <tool.h>

#define DBG_TAG "M606"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

struct rt_M606_config M606_config[]=
{
        M606_config_0,
#ifdef USING_SLAVE_BOARD
        M606_config_1,
#endif
};

static rt_err_t M606_rx(rt_device_t dev, rt_size_t size)
{
    M606_device_t M606 = (M606_device_t)(dev->user_data);

    struct rx_msg msg;
    rt_err_t result;
    msg.dev = dev;
    msg.size = size;

    result = rt_mq_send(&M606->rx_mq, &msg, sizeof(msg));
    if ( result == -RT_EFULL){
//        rt_kprintf("%s message queue full！\n",dev->parent.name);
    }
    return result;
}

/**
 * @brief   M606的初始化函数
 * @param   uart_name 用于M606与RT串口设备连接，函数中已经以DMA接收打开设备
 * @param   pos 用于M606与读码钥匙口编号对齐
 * @return  返回设备句柄
 */
static M606_device_t M606_init(const char* uart_name, rt_off_t pos)
{
    M606_device_t M606;
    struct serial_configure config = RFID_SERIAL_CONFIG;

    RT_ASSERT(uart_name);

    M606 = rt_calloc(1, sizeof(struct M606_device));
    if (M606 == RT_NULL) {
        LOG_E("Can't allocate memory for M606 device on '%s' ", uart_name);
        rt_free(M606);
    }

    M606->pos = pos;
    M606->dev = rt_device_find(uart_name);

    M606->dev->user_data = (void*)M606;
    rt_device_control(M606->dev, RT_DEVICE_CTRL_CONFIG, &config);
    rt_device_set_rx_indicate(M606->dev, M606_rx);

    rt_mq_init(&M606->rx_mq,
                "rx_mq",
                M606->msg_pool,
                sizeof(struct rx_msg),
                sizeof(M606->msg_pool),
                RT_IPC_FLAG_FIFO);

    rt_device_open(M606->dev, RT_DEVICE_FLAG_DMA_RX);

    M606->ops.scan_rfid     =   scan_rfid;
    M606->ops.rfid_printf   =   rfid_printf;

    return M606;
}

static rt_err_t rt_device_M606_register(M606_device_t dev, const char* M606_name)
{
    rt_device_t device = RT_NULL;
    device = rt_calloc(1, sizeof(struct rt_device));
    if (device == RT_NULL) {
        LOG_E("can't allocate memory for M606 device");
        free(device);
    }

    /* register device */
    device->type = RT_Device_Class_Miscellaneous;

    device->init = RT_NULL;
    device->open = RT_NULL;
    device->close = RT_NULL;
    device->read = RT_NULL;
    device->write = RT_NULL;
    device->control = RT_NULL;

    device->user_data = (void*) dev;

    return rt_device_register(device, M606_name, RT_DEVICE_FLAG_STANDALONE);
}

M606_device_t M606_device_find(const char *M606_name)
{
    M606_device_t M606;
    rt_device_t dev = rt_device_find(M606_name);
    if (dev == RT_NULL) {
        LOG_E("device %s is not exist", M606_name);
        return RT_NULL;
    }

    M606 = (M606_device_t)dev->user_data;
    return M606;
}

static int rt_hw_M606_init(void)
{
    int i;
    for (i = 0; i < ITEM_NUM(M606_config); i++)
    {
        M606_config[i].dev = M606_init(M606_config[i].uart_name, M606_config[i].pos);
        if (M606_config[i].dev == RT_NULL) {
            LOG_E("M606 device on %s init failed.", M606_config[i].uart_name);
            return -RT_ERROR;
        }
        rt_device_M606_register(M606_config[i].dev, M606_config[i].name);
    }
    for (i = 0; i < APP_NUM; i++){
        for (int j = 0; j < 5; j++){
            key[i].cur_rfid_code[j] = 0xFF;
            key[i].last_rfid_code[j] = 0xFF;
        }
    }
    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_hw_M606_init);
