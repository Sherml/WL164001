/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-10     liwentai     first version.
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <spi_flash_sfud.h>
#include <drv_spi.h>
 
sfud_flash_t sfud_dev;
rt_uint8_t w25x_read_id = 0x9f;
char wData[] = "test";

rt_mutex_t flash_lock = RT_NULL;

static void write_entry(void *parameter)
{    
    while (1) {
        rt_mutex_take(flash_lock, RT_WAITING_FOREVER);
        sfud_erase_write(sfud_dev, w25x_read_id, sizeof(wData), (uint8_t*)wData);
        rt_thread_mdelay(500);
        rt_mutex_release(flash_lock);
    }
}

static void read_entry(void *parameter)
{
    char str[30];
    struct rt_spi_device *spi_dev_w25q;
    spi_dev_w25q = (struct rt_spi_device *)rt_device_find("spi10");
    while (1) {
        rt_mutex_take(flash_lock, RT_WAITING_FOREVER);
        sfud_read(sfud_dev, w25x_read_id, 5, (uint8_t*)str);
                
        rt_kprintf("%s\n", str);
        rt_thread_mdelay(500);
        rt_mutex_release(flash_lock);   
    }
}

static void sfud_sample()
{
    rt_thread_t write = RT_NULL;
    rt_thread_t read = RT_NULL;
    rt_hw_spi_device_attach("spi2", "spi20", GPIOA, GPIO_PIN_12);
    rt_sfud_flash_probe("GD25Q127", "spi20");
    sfud_dev = rt_sfud_flash_find_by_dev_name("GD25Q127");
    
    flash_lock = rt_mutex_create("flash_lock", RT_IPC_FLAG_FIFO);
    
    write = rt_thread_create("flash_write", write_entry, RT_NULL, 1024, 24, 15);
    rt_thread_startup(write);

    read = rt_thread_create("flash_read", read_entry, RT_NULL, 1024, 24, 15);
    rt_thread_startup(read);
}
MSH_CMD_EXPORT(sfud_sample, sfud for w25q sample);  

