/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-02     liwentai       the first version
 */

#include "app.h"
#include <drv_spi.h>
#include <spi_flash_sfud.h>

#define     DBG_TAG "app_work"
#define     DBG_LVL DBG_INFO
#include    <rtdbg.h>



void led_entry(void* parameter)
{
    int i = 0;
    for (i = 0; i < BOARD_NUM; ++i) {
        if (led_init(board[i].led) != RT_EOK) {
            LOG_E("check led init.");
        }
    }
    while (1) {
        rt_sem_take(app_flag, RT_WAITING_FOREVER);
        for (i = 0; i < BOARD_NUM; ++i) {
            key_status_led(board[i]);
        }
    }
}

void stat_entry(void* parameter)
{
    int i;
    for (i = 0; i < BOARD_NUM; ++i) {
        if (stat_init(board[i].stat) != RT_EOK) {
            LOG_E("check stat init.");
        }
    }
    while (1) {
        /*钥匙在位检测*/
        for (i = 0; i < BOARD_NUM; ++i) {
            check_key(board[i]);
        }
        /*电磁铁闭合检测*/
        for (i = 0; i < BOARD_NUM; ++i) {
            check_lock(board[i]);
        }
        rt_sem_release(app_flag);
    }
}

void elec_entry(void* parameter)
{
    int i;
    for (i = 0; i < BOARD_NUM; ++i) {
        if (elec_init(board[i].elec) != RT_EOK) {
            LOG_E("check elec init.");
        }
    }
    while (1) {
//        for (i = 0; i < APP_NUM; ++i) {
//            elec_action(i, unlock);
//            rt_thread_mdelay(500);
//            elec_action(i, lock);
//            rt_thread_mdelay(500);
//        }
    }

}

static int operate_lock(int argc, char* argv[])
{
    int i = 0;
    if(argc == 2){
        i = atoi(argv[1]);
        if ((i>=0)&&(i<APP_NUM)){
            rt_kprintf("the NO.%d lock will be open...\n",i);
            rt_thread_mdelay(500);
            elec_action(i, unlock);
            rt_thread_mdelay(1000);
            elec_action(i, lock);
        }else {
            goto __exit;
        }
    }else{
        goto __exit;
    }

    return RT_EOK;

    __exit:
    rt_kprintf("put in a number for the ELEC work.\n");
    return -RT_ERROR;
}
MSH_CMD_EXPORT(operate_lock , open the lock);



void rfid_entry(void *parameter)
{
    int i = 0;
    while (1){
        /*RFID码值获取*/
        for (i = 0; i < BOARD_NUM; ++i) {
            scan_rfid(board[i].M606);
        }
        /*RFID码值打印*/


    }
}

void flash_entry(void* parameter)
{
    rt_hw_spi_device_attach("spi2", "spi20", GPIOA, GPIO_PIN_12);
    rt_sfud_flash_probe("GD25Q127", "spi20");
}


