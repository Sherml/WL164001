///*
// * Copyright (c) 2006-2021, RT-Thread Development Team
// *
// * SPDX-License-Identifier: Apache-2.0
// *
// * Change Logs:
// * Date           Author       Notes
// * 2022-10-12     liwentai       the first version
// */

#include    "i2c_module.h"
#include    <rtthread.h>

#define    printfERROR




rt_uint8_t timeCount = 0;
rt_uint8_t setBuff[4]={0x00,0x00,0xff,0xff};

rt_base_t  gpio_list[2][8][2]={
        {
                //LED_RED       [red][0x00]
                {0x01,0x00},//PA0
                {0x02,0x00},//PA1
                {0x04,0x00},//PA2
                {0x08,0x00},//PA3
                {0x10,0x00},//PA4
                {0x20,0x00},//PA5
                {0x40,0x00},//PA6
                {0x80,0x00},//PA7
        },

        {
                //LED_GREEN     [0x00][green]
                {0x00,0x01}, //PB0
                {0x00,0x02}, //PB1
                {0x00,0x04}, //PB2
                {0x00,0x08}, //PB3
                {0x00,0x10}, //PB4
                {0x00,0x20}, //PB5
                {0x00,0x40}, //PB6
                {0x00,0x80}, //PB7
        },
};

//#define     USING_I2C_BIT_OPS
#ifdef      USING_I2C_BIT_OPS

#include    "i2c_bit_tool.h"


nca_device_t nca9555_init(const char *i2c_bus_name)
{
    nca_device_t dev;

    RT_ASSERT(i2c_bus_name);

    dev = rt_calloc(1, sizeof(struct nca9555_device));
    if (dev == RT_NULL)
    {
        LOG_E("Can't allocate memory for NCA9555 device on '%s' ", i2c_bus_name);
        rt_free(dev);
        return RT_NULL;
    }

    dev->i2c_bus = rt_i2c_bus_device_find(i2c_bus_name);

    if(dev->i2c_bus == RT_NULL)
    {
        LOG_E("Can't find NCA9555 device on '%s' ", i2c_bus_name);
                rt_free(dev);
                return RT_NULL;
    }

    dev->lock = rt_mutex_create("mutex_NCA9555", RT_IPC_FLAG_FIFO);
    if (dev->lock == RT_NULL)
    {
        LOG_E("Can't create mutex for NCA9555 device on '%s' ", i2c_bus_name);
        rt_free(dev);

        return RT_NULL;
    }

    //使用i2c-bit-ops进行调试
    //rt_i2c_bit_add_bus(dev->i2c_bus, i2c_bus_name);
    //dev->i2c_bus->ops= &i2c_bit_bus_ops;

    return dev;

}
MSH_CMD_EXPORT(nca9555_init, int the nca9555);

static rt_err_t nca9555_write(struct rt_i2c_bus_device *i2c_bus, rt_uint8_t reg, rt_uint16_t len, rt_uint8_t *data)
{
    rt_uint8_t ret = 0;
    rt_uint8_t i_temp = 0;

    struct rt_i2c_bit_ops *ops = (struct rt_i2c_bit_ops *)i2c_bus->priv;

    ret = iic_start(ops);
    if(ret)
    {
        ret = i2c_writeb(i2c_bus, NCA_ADDR_WR);
    }
    if (ret)
    {
        ret = i2c_writeb(i2c_bus, reg);
    }
    for(i_temp=0;i_temp<(len);i_temp++)
    {
        if (ret)
        {
            i2c_writeb(i2c_bus, data[i_temp]);
        }
    }


    if (ret)
    {
        i2c_stop(ops);
    }

    return RT_EOK;
}

static rt_err_t nca9555_read(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *data)
{
    rt_uint8_t ret = 0;
    rt_uint8_t i_temp = 0;
    rt_uint8_t DataTemp = 0;

    struct rt_i2c_bit_ops *ops = (struct rt_i2c_bit_ops *)bus->priv;

    ret = i2c_start(ops);
    if(ret)
    {
        ret = i2c_writeb(bus, NCA_ADDR_WR);
    }
    if(ret)
    {
        ret = i2c_writeb(bus, reg);
    }

    if(ret)
    {
        ret = i2c_start(ops);
    }
    if(ret)
    {
        ret = i2c_writeb(bus, NCA_ADDR_RD);
    }
    for(i_temp=0;i_temp<len;i_temp++)
    {
        DataTemp = i2c_readb(bus);
        *data++ = DataTemp;
    }

    if (ret)
    {
        i2c_stop(ops);
    }

}

void nca9555_write_pin(nca_device_t dev, int index, int type, rt_uint8_t flag)
{
//    rt_uint8_t pin_status[2];
    rt_uint8_t writeData[2] = {0};

    if(flag = PIN_LOW)
    {
        //定位引脚
        writeData[0] |= gpio_list[type][index][0];
        writeData[1] |= gpio_list[type][index][1];

        nca9555_write(dev->i2c_bus, REGISTER_0_CONFIGURATION_PORT, 2, &setBuff[0]);
        nca9555_write(dev->i2c_bus, REGISTER_0_OUTPUT_PORT, 2, &writeData);
    }
    if(flag = PIN_HIGH)
    {
        writeData[0] &= ~(gpio_list[type][index][0]);
        writeData[1] &= ~(gpio_list[type][index][1]);

        nca9555_write(dev->i2c_bus, REGISTER_0_CONFIGURATION_PORT, 2, &setBuff[0]);
        nca9555_write(dev->i2c_bus, REGISTER_0_OUTPUT_PORT, 2, &writeData);
    }

}

rt_uint8_t nca9555_read_pin(nca_device_t dev, int index, int type)
{
    rt_uint8_t pin_status = 0;
    rt_uint8_t register_data = 0;
//    if((type >= TYPE_MAX && type < 0) || (index >= 8 && index < 0 ))
//        return -RT_ERROR ;

    switch(type)
    {
    case RED_LED:
        nca9555_read(dev->i2c_bus, REGISTER_0_INPUT_PORT, 1, &register_data);
        pin_status = (register_data >> index)&0x01 ;
        break;
    case GREEN_LED:
        nca9555_read(dev->i2c_bus, REGISTER_1_INPUT_PORT, 1, &register_data);
        pin_status = (register_data >> index)&0x01 ;
        break;
    default:
        rt_kprintf("[ERROR] nca999_gpio error!\r\n");
    }

    return pin_status;
}

#else

static struct rt_i2c_bus_device *i2c_bus;
static rt_mutex_t i2c_lock;
void nca9555_init(const char *i2c_bus_name)
{
//    if(i2c_bus_name<2)
//    {
//        rt_kprintf("please input a char that repersent the I2C BUS\n");
//        return;
//    }


    i2c_bus=(struct rt_i2c_bus_device*)rt_device_find(i2c_bus_name);
    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("can't find %s device!\n", i2c_bus_name);
    }
    i2c_lock=rt_mutex_create("i2c_lock",RT_IPC_FLAG_FIFO);
    if(i2c_lock==RT_NULL)
    {
        rt_kprintf("i2c create mutex fail\r\n");
    }
    rt_kprintf("init OK\n");
    rt_thread_delay(100);

}
MSH_CMD_EXPORT(nca9555_init, init the nca9555);

rt_err_t nca9555_write(rt_uint8_t reg, rt_uint8_t byte)
{
    rt_uint8_t buf[2];

    buf[0]=reg;
    buf[1]=byte;

    rt_size_t ret;
    ret = rt_i2c_master_send(i2c_bus,NCA_ADDR,0,buf,2);
    if(ret ==2)
    {
        rt_kprintf("the data has been written successfully\n");
        return RT_EOK;
    }
    else
    {
        rt_kprintf("ret = %s",ret);
#ifdef  printfERROR
        rt_kprintf("write data error,check i2c gpio init\r\n");
#endif
        return RT_ERROR;
    }
}


rt_err_t nca9555_read(rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *buf)
{

    rt_i2c_master_send(i2c_bus,NCA_ADDR,0,&reg,1);


    struct rt_i2c_msg msgs;

    msgs.addr = NCA_ADDR;
    msgs.flags = RT_I2C_RD;
    msgs.buf = buf;
    msgs.len = len;

    if (rt_i2c_transfer(i2c_bus, &msgs, 1) == 1)
    {
        return RT_EOK;
        rt_kprintf("the data has been read successfully\n");
    }
    else
    {
        return -RT_ERROR;
    }
}



void nca9555_write_pin(int index, int type, rt_uint8_t flag)
{
//    rt_uint8_t pin_status[2];
    rt_uint8_t writeData[2] = {0};

    if(flag == PIN_LOW)
    {
        //定位引脚
        writeData[0] |= gpio_list[type][index][0];
        writeData[1] |= gpio_list[type][index][1];

        nca9555_write(REGISTER_0_CONFIGURATION_PORT,    0x00);
        nca9555_write(REGISTER_1_CONFIGURATION_PORT,    0x00);
        nca9555_write(REGISTER_0_OUTPUT_PORT,   writeData[0]);
        nca9555_write(REGISTER_1_OUTPUT_PORT,   writeData[1]);
#ifdef  printfERROR
        rt_kprintf("the PIN_LOW command has been written successfully\n");
#endif
    }
    if(flag == PIN_HIGH)
    {
        writeData[0] &= ~(gpio_list[type][index][0]);
        writeData[1] &= ~(gpio_list[type][index][1]);

        nca9555_write(REGISTER_0_CONFIGURATION_PORT,    0x00);
        nca9555_write(REGISTER_1_CONFIGURATION_PORT,    0x00);
        nca9555_write(REGISTER_0_OUTPUT_PORT,   writeData[0]);
        nca9555_write(REGISTER_1_OUTPUT_PORT,   writeData[1]);
#ifdef  printfERROR
        rt_kprintf("the PIN_HIGH command has been written successfully\n");
#endif
    }

}

rt_uint8_t nca9555_read_pin(int index, int type)
{
    rt_uint8_t pin_status = 0;
    rt_uint8_t register_data = 0;
//    if((type >= TYPE_MAX && type < 0) || (index >= 8 && index < 0 ))
//        return -RT_ERROR ;

    switch(type)
    {
    case RED_LED:
        nca9555_read(REGISTER_0_INPUT_PORT, 1, &register_data);
        pin_status = (register_data >> index)&0x01 ;
        break;
    case GREEN_LED:
        nca9555_read(REGISTER_1_INPUT_PORT, 1, &register_data);
        pin_status = (register_data >> index)&0x01 ;
        break;
    default:
        rt_kprintf("[ERROR] nca999_gpio error!\r\n");
    }

    return pin_status;
}

//const struct rt_nca9555_ops ops =
//{
//        .init    = nca9555_init,
//        .write   = nca9555_write_pin,
//        .read    = nca9555_read_pin,
//
//};
#endif