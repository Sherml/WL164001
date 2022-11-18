/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-28     liwentai       the first version
 */
#include "M606.h"
#include <tool.h>

#define DBG_TAG "M606"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


/* 定义全局的钥匙信息 */
key_info_t key[APP_NUM];
static rt_uint8_t check_id[APP_NUM] = { 0 };

/**
 * @brief   用于从消息队列接收串口收到的数据，调试日志会输出接收到的数据长度，接收到的原始数据
 * @param M606 接收信息的设备句柄
 * @return  返回数据
 */
static char* rfid_get(M606_device_t M606)
{
    struct rx_msg msg;
    rt_err_t result;
    rt_uint32_t rx_length;
    static char rx_buffer[RT_SERIAL_RB_BUFSZ + 1];

    rt_memset(&msg, 0, sizeof(msg));
    result = rt_mq_recv(&M606->rx_mq, &msg, sizeof(msg), RT_WAITING_FOREVER);
    if (result == RT_EOK){
        /* 从串口读取数据*/
        rx_length = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
        LOG_D("strlen RX: %d",rx_length);
    }
    LOG_D("%s",rx_buffer);
    return rx_buffer;
}


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

    while ((ret)&&(i<M606_RFID_NUM + pos)){
        strncpy(key[i].user_data.ch, ret, 10);
        LOG_D("key[%d]: %s ", i, ret);
        str2hex(key[i].user_data.ch, key[i].user_data.code);
        ret = strtok(NULL, p);
        i++;
    }

}

/**
 * @brief               用于获取码值，并更新
 * @param M606          获取码值的设备
 * @return
 */
rt_err_t scan_rfid(M606_device_t M606)
{
    int i = 0, j = 0;

    static char data[ONE_DATA_MAXLEN];

    memset(data,'\0',sizeof(ONE_DATA_MAXLEN*sizeof(char)));
    rt_strncpy(data, rfid_get(M606), ONE_DATA_MAXLEN);
    if (strlen(data) < 40) {
        rt_strncpy(data, rfid_get(M606), ONE_DATA_MAXLEN);
    }

    code_split(data, M606->pos);

    for (i = pos; i < M606_RFID_NUM + pos; i++){
        if ((memcmp(key[i].user_data.code,key[i].last_rfid_code,5)) && (check_id[i] < 3)) {
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
    }else {
        memcpy(key[i].last_rfid_code, key[i].cur_rfid_code, 5);
        if (!(memcmp(&key[i].user_data.code[1], "\x00\x00\x00", 3)))
            return 1;
        rt_kprintf("KEY[%d] RFID:", i);
        for (j = 0; j < 5; j++){
            rt_kprintf("%02X ", key[i].cur_rfid_code[j]);
        }
        rt_kprintf("\n");
        return 0;
    }
}
