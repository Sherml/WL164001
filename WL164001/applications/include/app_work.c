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
#define     DBG_LVL DBG_LOG
#include    <rtdbg.h>



void led_entry(void* parameter)
{
    int i = 0;
    char red[6], green[6];
    rt_device_t led = led_init("LED1");
#ifdef USING_SLAVE_BOARD
    rt_device_t led_slave = led_init("LED2");
#endif
    while (1) {
        rt_sem_take(app_flag, RT_WAITING_FOREVER);
        for (i = 0; i < APP_NUM; i++) {
            rt_sprintf(red, "%s%d%s%d", "IO", 0, "_", i);
            rt_sprintf(green, "%s%d%s%d", "IO", 1, "_", i);

            if (key[i].key_status == key_in && key[i].last_key_status != key_in) {
                rt_device_write(led, PIN_HIGH, red, 0); //在位亮红灯
                rt_device_write(led, PIN_LOW, green, 0);
                key[i].last_key_status = key[i].key_status;
            }

            if (key[i].key_status == key_out && key[i].last_key_status != key_out) {
                rt_device_write(led, PIN_HIGH, green, 0); //不在位亮绿灯
                rt_device_write(led, PIN_LOW, red, 0);
                key[i].last_key_status = key[i].key_status;
            }
        }
    }
}

void amr_entry(void* parameter)
{
    rt_device_t amr = amr_init("AMR1");
    rt_device_t amr2 = amr_init("AMR2");
    while (1) {
        /*钥匙在位检测*/
        check_key(amr);
        check_key(amr2);
        /*电磁铁闭合检测*/
        check_lock(amr);
        check_lock(amr2);
        rt_sem_release(app_flag);
    }
}

void elec_entry(void* parameter)
{
    elec_init();
}



void rfid_entry(void *parameter)
{
    int i = 0;
    while (1){
        for (i = 0; i < 1; ++i) {
            /*RFID码值获取*/
            scan_rfid(M606_dev[i]);
        }
        /*RFID码值打印*/
        for (i = 0; i < APP_NUM; ++i) {
            rfid_printf(i);
        }

    }
}

void flash_entry(void* parameter)
{
    rt_hw_spi_device_attach("spi2", "spi20", GPIOA, GPIO_PIN_12);
    rt_sfud_flash_probe("GD25Q127", "spi20");
}


