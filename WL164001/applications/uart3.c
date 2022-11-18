/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-18     liwentai       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>
#include <stdio.h>
#include <tool.h>

#define DBG_TAG "uart3"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define DATA_CMD_END                     '\n'       /* 结束位设置为 \r，即回车符 */
#define ONE_DATA_MAXLEN                  80         /* 不定长数据的最大长度 */

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

#define APP_NUM 6
/* 定义全局的钥匙信息 */
key_info_t key[APP_NUM];
static rt_uint8_t check_id[APP_NUM] = { 0 };

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/**
 * @brief       用于将字符串转为十六进制数组，使用sscanf，在RTT和keil中最后可能会出现不一样的数据分割表现
 * @param str   输入的字符串
 * @param out   存储十六进制的数组地址
 * @return      十六进制数组组数
 */
static int str2hex(char *str, uint8_t *out)
{
    if (str == NULL || out == NULL)
        return -1;

    int i = 0, ret = 0;

    ret = (strlen(str) / (2 * sizeof(char))) + strlen(str) % (2 * sizeof(char));

    for (i = 0; i < ret; i++){
        sscanf(str + 2 * i, "%02X", (unsigned int*)(out + i ));
//        LOG_D("%02X ",out[i]);
    }
    return ret;
}

/**
 * @brief       用于将     单一M606输出    的原始数组分割，以便映射到具体的硬件设备
 * @param ch    原始的字符串数据
 */
static void code_split(char *ch, int pos)
{
    const char *p = "{,}";
    char *ret;

    int i = pos;
    ret = strtok(ch, p);

    while ((ret)&&(i<6 + pos)){
        if (strlen(ret) != 10) {
            ret = strtok(NULL, p);
            i++;
            continue;
        }
        strncpy(key[i].user_data.ch, ret, 10);
        LOG_D("key[%02d]: %s ", i, ret);

        str2hex(key[i].user_data.ch, key[i].user_data.code);
        ret = strtok(NULL, p);
        i++;
    }

}

rt_err_t scan_rfid(char* data)
{
    int i = 0, j = 0;

    code_split(data, 0);

    for (i = 0; i < 6; i++){
        if ((memcmp(key[i].user_data.code,key[i].cur_rfid_code,5)) && (check_id[i] < 2)) {
            check_id[i]++;
            continue;
        }
        check_id[i] = 0;
        for (j = 0; j < 5; j++){
            key[i].last_rfid_code[j] = key[i].cur_rfid_code[j];
            key[i].cur_rfid_code[j] = key[i].user_data.code[j];
        }
    }
    return RT_EOK;
}

/**
 * @brief   用于打印码值
 */
int rfid_printf(int i)
{
    int j = 0;

    if (rt_memcmp(key[i].last_rfid_code, key[i].cur_rfid_code, 5) == 0){
        return 2;
    }else
    {
        memcpy(key[i].last_rfid_code, key[i].cur_rfid_code, 5);
//        /* 钥匙在位筛除 */
//        if (key[i].key_status != key_in)
//            return 1;
        /* 打印码值 */
        rt_kprintf("KEY[%02d] RFID:", i);
        for (j = 0; j < 5; j++){
            rt_kprintf("%02X ", key[i].cur_rfid_code[j]);
        }
        rt_kprintf("\n");
        return 0;
    }
}

/* 接收数据回调函数 */
static rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    if (size > 0)
    {
        rt_sem_release(&rx_sem);
    }
    return RT_EOK;
}

static char uart_sample_get_char(void)
{
    char ch;

    while (rt_device_read(serial, 0, &ch, 1) == 0)
    {
        rt_sem_control(&rx_sem, RT_IPC_CMD_RESET, RT_NULL);
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    }
    return ch;
}

/* 数据解析线程 */
static void data_parsing(void)
{
    char ch;
    int flag = 0;
    char data[ONE_DATA_MAXLEN];
    static int i = 0;
    int j;

    while (1)
    {

        ch = uart_sample_get_char();

        if (ch == '{')
        {
            flag++;
        }
        if (flag) {

            if(ch == DATA_CMD_END)
            {
                data[i++] = ch;
//                rt_kprintf("%s",data);
                scan_rfid(data);
                for (j = 0; j < 6; ++j) {
                    rfid_printf(j);
                }

                i = 0;
                flag = 0;
                continue;
            }
            i = (i >= ONE_DATA_MAXLEN-1) ? ONE_DATA_MAXLEN-1 : i;
            data[i++] = ch;
        }


    }
}

static int uart_data_sample()
{
    rt_err_t ret = RT_EOK;

    /* 查找系统中的串口设备 */
    serial = rt_device_find("uart3");
    if (!serial)
    {
        rt_kprintf("find uart3 failed!\n");
        return RT_ERROR;
    }

    struct serial_configure config = RFID_SERIAL_CONFIG;
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);
    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_rx_ind);

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial", (void (*)(void *parameter))data_parsing, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}
INIT_COMPONENT_EXPORT(uart_data_sample);
