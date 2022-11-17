/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-18     army       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_i2c_nca9555.h"

//#ifdef BSP_USING_I2C_NCA955

#define DBG_TAG "nca9555"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define RT_I2C_NCA9555_NAME "i2c1"
#define RT_I2C_NCA9555_ADDR 0x21

/* nca9555 register */
#define NCA9555_REG_INPORT0      0x00
#define NCA9555_REG_INPORT1      0x01
#define NCA9555_REG_OUTPORT0     0x02
#define NCA9555_REG_OUTPORT1     0x03
#define NCA9555_REG_PI0          0x04
#define NCA9555_REG_PI1          0x05
#define NCA9555_REG_CFG0         0x06
#define NCA9555_REG_CFG1         0x07

/* nca9555 register value */
#define NCA9555_MODE_IN         0x01    /* input mode */
#define NCA9555_MODE_OUT        0x00    /* output mode */
#define NCA9555_PI_NOR          0x00    /* normal polarity */
#define NCA9555_PI_NOT          0x01    /* inversion  polarity */
#define NCA9555_PIN_LOW         0x00    /* pin low */
#define NCA9555_PIN_HIGH        0x01    /* pin high */

/* nca9555 device */
typedef struct nca9555_dev
{
    struct rt_i2c_bus_device *i2c_bus;
    rt_uint8_t slave_addr;
}nca9555_dev_t;

static rt_err_t nca9555_read_reg(rt_device_t pdev, rt_uint8_t reg, rt_uint8_t *pdata)
{
    struct rt_i2c_msg msg[2] = {0};
    nca9555_dev_t *pnca_dev = RT_NULL;

    RT_ASSERT(pdev != RT_NULL);

    pnca_dev = (nca9555_dev_t*)pdev->user_data;
    msg[0].addr  = pnca_dev->slave_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = 1;
    msg[0].buf   = &reg;
    msg[1].addr  = pnca_dev->slave_addr;
    msg[1].flags = RT_I2C_RD;
    msg[1].len   = 2;
    msg[1].buf   = pdata;
    if(rt_i2c_transfer(pnca_dev->i2c_bus, msg, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        LOG_E("i2c bus read failed!\r\n");
        return -RT_ERROR;
    }
}

rt_err_t nca9555_write_reg(rt_device_t pdev, rt_uint8_t reg, rt_uint8_t data)
{
    struct rt_i2c_msg msg[2] = {0};
    struct nca9555_dev *pnca_dev = RT_NULL;

    RT_ASSERT(pdev != RT_NULL);

    pnca_dev = (nca9555_dev_t*)pdev->user_data;
    msg[0].addr     = pnca_dev->slave_addr;
    msg[0].flags    = RT_I2C_WR;
    msg[0].len      = 1;
    msg[0].buf      = &reg;
    msg[1].addr     = pnca_dev->slave_addr;
    msg[1].flags    = RT_I2C_WR | RT_I2C_NO_START;
    msg[1].len      = 1;
    msg[1].buf      = &data;
    if(rt_i2c_transfer(pnca_dev->i2c_bus, msg, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        LOG_E("i2c bus write failed!\r\n");
        return -RT_ERROR;
    }
}

static rt_err_t rt_nca9555_open(rt_device_t dev, rt_uint16_t flag)
{
    if (dev->rx_indicate != RT_NULL)
    {
        /* open interrupt */
    }

    return RT_EOK;
}

rt_size_t rt_nca9555_write(rt_device_t dev, rt_off_t pos, void const *buffer, rt_size_t size)
{
    rt_uint8_t temp[2] = {0};

    rt_uint8_t a = 0x66;

    nca9555_write_reg(dev, NCA9555_REG_CFG0, 0x00);

    nca9555_read_reg(dev, NCA9555_REG_CFG0, &a);

    LOG_E("NCA9555_REG_CFG0 is %#x!\r\n", a);


    if (PIN_LOW == *(rt_uint8_t*)buffer)
    {
        if(pos < 8)
        {
            temp[0] &= ~(NCA9555_PIN_HIGH << pos);
            nca9555_write_reg(dev, NCA9555_REG_OUTPORT0, temp[0]);
        }
        else
        {
            temp[1] &= ~(NCA9555_PIN_HIGH << (pos - 8));
            nca9555_write_reg(dev, NCA9555_REG_OUTPORT1, temp[1]);
        }
    }
    else
    {
        if(pos < 8)
        {
            temp[0] |= NCA9555_PIN_HIGH << pos;
            nca9555_write_reg(dev, NCA9555_REG_OUTPORT0, temp[0]);
        }
        else {
            temp[1] |= NCA9555_PIN_HIGH << (pos - 8);
            nca9555_write_reg(dev, NCA9555_REG_OUTPORT1, temp[1]);
        }

    }
    return size;
}

rt_size_t rt_nca9555_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_uint8_t temp[2] = {0};

    nca9555_read_reg(dev, NCA9555_REG_INPORT0, temp);
//    if(pos < 8)
//        *(rt_uint8_t*)buffer =  = (temp[0]>>pos)&0x01;
//    else
//        *(rt_uint8_t*)buffer = (temp[1]>>(pos-8))&0x01;
		rt_memcpy(buffer,temp,size);
    return size;
}

rt_err_t rt_nca9555_init(const char *dev_name, const char *i2c_name, rt_uint8_t i2c_addr)
{
    rt_err_t ret = RT_EOK;
    nca9555_dev_t *pnca_dev = RT_NULL;
    rt_device_t prt_dev = RT_NULL;

    pnca_dev = rt_calloc(1, sizeof(nca9555_dev_t));

    if (pnca_dev == RT_NULL)
    {
        LOG_E("can't allocate memory for tca9534 device on '%s' ", i2c_name);
        goto __exit;
    }

    pnca_dev->i2c_bus = rt_i2c_bus_device_find(i2c_name);
    if(pnca_dev->i2c_bus == RT_NULL)
    {
        LOG_E("i2c bus device %s not found!", i2c_name);
        ret = -RT_ERROR;
        goto __exit;
    }
    pnca_dev->slave_addr = i2c_addr;

    prt_dev = rt_calloc(1, sizeof(struct rt_device));
    if (prt_dev == RT_NULL)
    {
        LOG_E("can't allocate memory for tca9534 device");
        goto __exit;
    }

    /* register device */
    prt_dev->type   = RT_Device_Class_Miscellaneous;
#ifdef RT_USING_DEVICE_OPS
    prt_dev->ops = &tca9534_dev_ops;
#else
    prt_dev->init   = RT_NULL;
    prt_dev->open   = rt_nca9555_open;
    prt_dev->close  = RT_NULL;
    prt_dev->read   = rt_nca9555_read;
    prt_dev->write  = rt_nca9555_write;
    prt_dev->control= RT_NULL;
#endif
    prt_dev->user_data  = (void*)pnca_dev;  /* device */;
    rt_device_register(prt_dev, dev_name, RT_DEVICE_FLAG_RDWR);

    return ret;

__exit:
    if (pnca_dev != RT_NULL)
    {
        rt_free(pnca_dev);
    }

    if (prt_dev != RT_NULL)
    {
        rt_free(prt_dev);
    }

    return RT_ERROR;
}

static int rt_hw_nca9555_port(void)
{
    return rt_nca9555_init("nca9555", RT_I2C_NCA9555_NAME, RT_I2C_NCA9555_ADDR);
}
INIT_COMPONENT_EXPORT(rt_hw_nca9555_port);

static rt_uint8_t InvertUint8(rt_uint8_t in)
{
	rt_uint32_t i;
	rt_uint8_t tmp = 0;
 
	for(i=0; i< 8; i++)
	{
		if(in & (1 << i))
			tmp |= 1 << (7 - i);
	}
	return tmp;
}



void nca9555_read_test()
{
	rt_device_t nca9555_dev;
	nca9555_dev = rt_device_find("nca9555");
	rt_uint16_t comm_addr = 0;
	if (nca9555_dev == RT_NULL)
	{
		rt_kprintf("can not find nca9555\r\n");
	}
	rt_uint8_t value[2] = {0};
	rt_device_open(nca9555_dev,RT_DEVICE_FLAG_RDWR);
	rt_device_read(nca9555_dev,0,value,2);
	rt_device_close(nca9555_dev);
	comm_addr = ((value[0]>>6)&0x01) | ((((value[0]>>7)&0x01)) << 1) | (rt_uint16_t)((InvertUint8(value[1])<<2));
	rt_kprintf("comm_addr = %08x \r\n",comm_addr);
}

MSH_CMD_EXPORT(nca9555_read_test, nca9555 tets)

