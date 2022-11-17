/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-03     liwentai       the first version
 */
#include "elec.h"

void elec_open(rt_device_t elec,int i){
    char open[6],close[6];
    rt_sprintf(open, "%s%d%s%d","IO", 0, "_", i);
    rt_sprintf(close, "%s%d%s%d","IO", 1, "_", i);

    rt_device_control(elec, OUTPUT, close);
    rt_device_write(elec, PIN_LOW, close, 0);

    rt_device_control(elec, OUTPUT, open);
    rt_device_write(elec, PIN_HIGH, open, 0);
    rt_thread_mdelay(50);
    rt_device_write(elec, PIN_LOW, open, 0);
}

void elec_close(rt_device_t elec,int i){
    char open[6],close[6];
    rt_sprintf(open, "%s%d%s%d","IO", 0, "_", i);
    rt_sprintf(close, "%s%d%s%d","IO", 1, "_", i);

    rt_device_control(elec, OUTPUT, open);
    rt_device_write(elec, PIN_LOW, open, 0);

    rt_device_control(elec, OUTPUT, close);
    rt_device_write(elec, PIN_HIGH, close, 0);
    rt_thread_mdelay(50);
    rt_device_write(elec, PIN_LOW, close, 0);

}

rt_device_t elec_init(){
    rt_device_t elec = RT_NULL;

    elec = rt_device_find("ELEC");
    rt_device_open(elec, RT_DEVICE_OFLAG_RDWR);

    for(int i=0; i<APP_NUM; i++){
        elec_open(elec, i);
    }
//    for(int i=0; i<APP_NUM; i++){
//        elec_close(elec, i);
//    }
    return elec;
}

static int operate_lock(int argc, char* argv[]){
    int i = 0;
    if(argc == 2){
        i = atoi(argv[1]);
        if ((i>=0)&&(i<APP_NUM)){
            rt_device_t dev = rt_device_find("ELEC");
            rt_kprintf("the NO.%d lock will be open...\n",i);
            rt_thread_mdelay(500);
            elec_open(dev, i);
//            rt_thread_mdelay(500);
//            elec_close(dev, i);
        }else{
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


