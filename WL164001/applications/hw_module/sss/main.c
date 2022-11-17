/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-12     RT-Thread    first version
 */

#include <rtthread.h>
#include <board.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <drivers/pin.h>
#include <drv_i2c_nca9555.h>


#define    RUN_LED              GET_PIN(A, 0)

static rt_thread_t led;
static rt_thread_t rfid;
static void led_entry(void *parameter);
static void rfid_entry(void *parameter);


int main(void)
{

    rt_pin_mode(RUN_LED, PIN_MODE_OUTPUT);

    led = rt_thread_create("led", led_entry, RT_NULL, 2048, 20, 5);
    rt_thread_startup(led);
    rfid = rt_thread_create("rfid", rfid_entry, RT_NULL, 2048, 20, 10);
    rt_thread_startup(rfid);
    while (1)
    {
        rt_pin_write(RUN_LED, PIN_LOW);
        rt_thread_mdelay(1000);
        rt_pin_write(RUN_LED, PIN_HIGH);
        rt_thread_mdelay(1000);


    }

    return RT_EOK;
}


static void led_entry(void *parameter)
{
    rt_device_t dev = rt_device_find("nca9555");
    rt_device_open(dev, RT_DEVICE_OFLAG_RDWR);

//    nca_device_t nca = nca9555_init("i2c1", 0x21);

    while(1)
    {
//        rt_device_control(dev, OUTPUT, "IO2_3");
        rt_device_control(dev, 0, "IO0_0");
//        rt_device_control(dev, 0, "IO0_1");
        rt_device_control(dev, 0, "IO0_2");


        rt_device_control(dev, 0, "IO1_5");

        rt_device_write(dev, PIN_HIGH, "IO0_0", 0);
        rt_device_write(dev, PIN_HIGH, "IO0_2", 0);

        rt_device_write(dev, PIN_HIGH, "IO1_5", 0);

    }
}


#define RFID_UART3_NAME "uart3"
#define MAXLEN  88
#define DATA_CMD_END '\r'
#define RFID_SERIAL_CONFIG  \
{                           \
    BAUD_RATE_38400,        \
    DATA_BITS_8,            \
    STOP_BITS_1,            \
    PARITY_NONE,            \
    BIT_ORDER_LSB,          \
    NRZ_NORMAL,             \
    1024,                    \
    0                       \
}

#include <string.h>




static void rfid_entry(void *parameter)
{
    static rt_device_t  M606;
    M606 = rt_device_find(RFID_UART3_NAME);

    if(!M606)
    {
        rt_kprintf("find %s failed!\n", RFID_UART3_NAME);

    }

    //修改设置
    struct serial_configure config = RFID_SERIAL_CONFIG;
    rt_device_control(M606, RT_DEVICE_CTRL_CONFIG, &config);

    //读写及中断接收
    rt_device_open(M606, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

    //M606 指令
    const char *str = "?";
    char ch[MAXLEN];
    char last_code[MAXLEN], current_code[MAXLEN];

    while(1)
    {

        rt_device_write(M606, 0, str, (sizeof(str)-1));

        //等待 M606 相应，否则会丢失前几位
        rt_thread_mdelay(50);
        rt_device_read(M606, 0, &ch, MAXLEN);

        memcpy(last_code,current_code, MAXLEN);
        memcpy(current_code,ch, MAXLEN);


        if(memcmp(last_code,current_code, MAXLEN) == 0)
        {
            rt_thread_mdelay(20);
        }
        else
        {
            int i = 0;
            while((i<2))
            {
                rt_device_write(M606, 0, str, (sizeof(str)-1));
                rt_thread_mdelay(50);
                rt_device_read(M606, 0, &ch, MAXLEN);
                memcpy(last_code,current_code, MAXLEN);
                memcpy(current_code,ch, MAXLEN);


                if(memcmp(last_code,current_code, MAXLEN) == 0)
                    i++;
                else
                    break;
            }
            if(i == 2)
                rt_kprintf("rfid is %s\n", current_code);
//            rt_thread_mdelay(150);
        }
    }
}


