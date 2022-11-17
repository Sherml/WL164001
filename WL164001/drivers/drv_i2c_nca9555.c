// * Copyright (c) 2006-2021, RT-Thread Development Team
// *
// * SPDX-License-Identifier: Apache-2.0
// *
// * Change Logs:
// * Date           Author       Notes
// * 2022-10-12     liwentai       the first version
// *

#include    "drv_i2c_nca9555.h"

#define     DBG_TAG "nca9555"
#define     DBG_LVL DBG_INFO
#include    <rtdbg.h>

static struct rt_nca9555 nca9555_obj[] =
{
        NCA9555_config_0,
        NCA9555_config_1,
        NCA9555_config_2,
        NCA9555_config_3,
        NCA9555_config_4,
        NCA9555_config_5,
};

#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])
static const struct pin_list list[] =
{
        IO0_0,
        IO0_1,
        IO0_2,
        IO0_3,
        IO0_4,
        IO0_5,
        IO0_6,
        IO0_7,

        IO1_0,
        IO1_1,
        IO1_2,
        IO1_3,
        IO1_4,
        IO1_5,
        IO1_6,
        IO1_7,
};

rt_uint8_t set_pin[4] = { 0x00, 0x00, 0xff, 0xff };

/**
 * This function can use to read nca9555 register, only-reading 1 and continuation-reading 2 register both are feasible
 *
 * @param dev               the name of nca9555 device
 * @param reg               the register which would be read
 * @param len               the length of the data which would be read
 * @param *buf              the pointer of the buffer
 *
 * @return                  the status of reading.
 */
rt_err_t nca9555_read_data(nca_device_t dev, rt_uint8_t reg, rt_uint8_t len, rt_uint8_t* buf)
{
    struct rt_i2c_msg msgs[2] = { 0 };


    msgs[0].addr = dev->addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = &reg;
    msgs[0].len = 1;

    msgs[1].addr = dev->addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf = buf;
    msgs[1].len = len;

    if (rt_i2c_transfer(dev->i2c_bus, msgs, 2) == 2)
    {
        return RT_EOK;
        LOG_D("Read successfully\n");
    }
    else
    {
        LOG_E("NCA9555 on %s 0x%02X read failed!\n",dev->i2c_bus->parent.parent.name,dev->addr);
        return -RT_ERROR;
    }
}

/**
 * This function can use to write nca9555 register, only-writing 1 and continuation-writing 2 register both are feasible
 *
 * @param dev               the name of nca9555 device
 * @param reg               the register which would be read
 * @param len               the length of the data which would be read
 * @param *buf              the pointer of the buffer
 *
 * @return                  the status of writing.
 */
rt_err_t nca9555_write_data(nca_device_t dev, rt_uint8_t reg, rt_uint8_t len, rt_uint8_t* buf)
{
    struct rt_i2c_msg msgs[2] = { 0 };

    msgs[0].addr = dev->addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = &reg;
    msgs[0].len = 1;

    msgs[1].addr = dev->addr;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf = buf;
    msgs[1].len = len;

    if (rt_i2c_transfer(dev->i2c_bus, msgs, 2) == 2)
    {
        return RT_EOK;
        LOG_D("Read successfully\n");
    }
    else
    {
        LOG_E("NCA9555 read failed!\n");
        return -RT_ERROR;
    }
}

/**
 * This function is used to get offset position of I/O of nca9555
 *
 * @param   pin  a pointer to char represent of I/O
 * @return       the offset position of the pin put in.
 */
static const struct pin_list* get_list(char* pin)
{
    const struct pin_list* index;
    index = RT_NULL;

    for (rt_uint8_t i = 0; i < ITEM_NUM(list); i++)
    {
        if (!rt_strncmp(list[i].name, pin, sizeof(list[i].name) + 1))
        {
            index = &list[i];
            LOG_D("%s has been chosen.\n", index->name);
            goto __exit;
        }
    }

    __exit:
    if (index == RT_NULL)
    {
        LOG_E("%s is not a usable pin, check the char whether conform to format like IO1_1 ", pin);
    }

    return index;
}

/**
 * This is an api for RT-Thread I/O Organizer function rt_device_write to write pin status
 * @param dev       a device can be using as nca9555
 * @param pos       using as the logic level that user want, format like PIN_LOW / PIN_HIGH
 * @param buffer    using as a char represent of I/O of nca9555, format like "IO1_1"
 * @param size      invalid item
 * @return          invalid item
 */
static rt_size_t rt_nca9555_write_pin(rt_device_t dev, rt_off_t pos, void const* buffer, rt_size_t size)
{
    rt_uint8_t temp[2] = { 0 };
    nca_device_t nca9555 = RT_NULL;
    rt_off_t offset = 0;
    char* str = (char*) buffer;
    const struct pin_list* index;

    nca9555 = (nca_device_t) (dev->user_data);
    index = get_list(str);
    offset = index->offset;

    nca9555_read_data(nca9555, REGISTER_0_OUTPUT_PORT, 2, &temp[0]);

    if (0 == pos)
    {
        if (offset < 8)
        {
            temp[0] &= ~(0x01 << offset);
        }
        else
        {
            temp[1] &= ~(0x01 << (offset - 8));
        }
    }
    else
    {
        if (offset < 8)
        {
            temp[0] |= 0x01 << offset;
        }
        else
        {
            temp[1] |= 0x01 << (offset - 8);
        }

    }
    nca9555_write_data(nca9555, REGISTER_0_OUTPUT_PORT, 2, &temp[0]);
    return size;
}

/**
 * This is an api for RT-Thread I/O Organizer function rt_device_read to get pin status
 * @param dev       a device can be using as nca9555
 * @param pos       invalid item
 * @param buffer    using as a char represent of I/O of nca9555, format like "IO1_1"
 * @param size      invalid item
 * @return          a uint8_t description of logical level of chosen pin
 */
static rt_size_t rt_nca9555_read_pin(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    rt_uint8_t temp[2] = { 0 };
    rt_size_t pin_status = 0;
    nca_device_t nca9555 = RT_NULL;
    char* str = (char*) buffer;
    const struct pin_list *index;
    rt_off_t offset = 0;

    nca9555 = (nca_device_t) (dev->user_data);
    index = get_list(str);
    offset = index->offset;

    nca9555_read_data(nca9555, REGISTER_0_INPUT_PORT, 2, &temp[0]);

    if (offset < 8)
    {
        pin_status = (temp[0] >> offset) & 0x01;
    }
    else
    {
        pin_status = (temp[1] >> (offset - 8)) & 0x01;
    }

    return pin_status;
}


/**
 * This is an api for RT-Thread I/O Organizer function rt_device_control to control pin mode
 * @param dev       a device can be using as nca9555
 * @param cmd       the mode user want,choose between OUTPUT / INPUT
 * @param args      using as a char represent of I/O of nca9555, format like "IO1_1"
 * @return          invalid item.
 */
static rt_err_t rt_nca9555_control_pin(rt_device_t dev, int cmd, void* args)
{
    nca_device_t nca9555 = RT_NULL;
    nca9555 = (nca_device_t) (dev->user_data);

    rt_uint8_t buf[2] = { 0 };

    char* str = (char*) args;

    const struct pin_list* index;
    index = get_list(str);
    LOG_D("%s ",index->name);

    rt_off_t pos = index->offset;

    nca9555_read_data(nca9555, REGISTER_0_CONFIGURATION_PORT, 2, &buf[0]);
    switch (cmd)
    {
    case OUTPUT:
        LOG_D("OUTPUT mode\n");
        if (pos < 8)
        {
            buf[0] &= ~(0x01 << pos);
        }
        else
        {
            buf[1] &= ~(0x01 << (pos - 8));
        }
        nca9555_write_data(nca9555, REGISTER_0_CONFIGURATION_PORT, 2, &buf[0]);
        break;

    case INPUT:
        LOG_D("INPUT mode\n");
        if (pos < 8)
        {
            buf[0] |= 0x01 << pos;
        }
        else
        {
            buf[1] |= 0x01 << (pos - 8);
        }
        nca9555_write_data(nca9555, REGISTER_0_CONFIGURATION_PORT, 2, &buf[0]);
        break;

    case INVERT:
        LOG_D("INVERT!\n");
        if (pos < 8)
        {
            buf[0] |= 0x01 << pos;
        }
        else
        {
            buf[1] |= 0x01 << (pos - 8);
        }
        nca9555_write_data(nca9555, REGISTER_0_POLARITY_INVERSION_PORT, 2, &buf[0]);
        break;

    default:
        break;
    }


    return RT_EOK;
}

/**
 * This function initializes nca9555 registered device driver
 *
 * @param i2c_bus_name      the using i2c bus
 * @param addr              the address of nca9555 device
 *
 * @return                  the nca9555 device.
 */
nca_device_t nca9555_init(const char* i2c_bus_name, rt_uint8_t addr)
{
    //create a nca_deivce for writing various functions
    nca_device_t dev;

    RT_ASSERT(i2c_bus_name);

    dev = rt_calloc(1, sizeof(struct nca9555_device));

    if (dev == RT_NULL)
    {
        LOG_E("Can't allocate memory for NCA9555 device on '%s' ", i2c_bus_name);
        rt_free(dev);
        return RT_NULL;
    }

    /*
     * connecting the NCA9555 with the i2c_bus, and action here will make
     * the nca9555 device have overt representation of the name of i2c bus.
     */
    dev->i2c_bus = rt_i2c_bus_device_find(i2c_bus_name);

    if (dev == RT_NULL)
    {
        LOG_E("Can't find NCA9555 device on '%s' ", i2c_bus_name);
        rt_free(dev);
        return RT_NULL;
    }

    dev->addr = addr;

    nca9555_write_data(dev, REGISTER_0_CONFIGURATION_PORT, 2, &set_pin[2]);
    nca9555_write_data(dev, REGISTER_0_OUTPUT_PORT, 2, &set_pin[0]);

    return dev;
}

/**
 * This function is using for registering nca9555 as a device to RT-Thread
 * @param dev               a device using for nca9555
 * @param nca9555_name      a char using as the name of nca9555 just register
 @verbatim


 @endverbatim
 * @return                  the status of registering.
  */
rt_err_t rt_device_nca9555_register(nca_device_t dev, const char* nca9555_name)
{
    rt_device_t device = RT_NULL;
    device = rt_calloc(1, sizeof(struct rt_device));
    if (device == RT_NULL)
    {
        LOG_E("can't allocate memory for NCA9555 device");
        free(device);
    }

    /* register device */
    device->type = RT_Device_Class_Miscellaneous;

    device->init = RT_NULL;
    device->open = RT_NULL;
    device->close = RT_NULL;
    device->read = rt_nca9555_read_pin;
    device->write = rt_nca9555_write_pin;
    device->control = rt_nca9555_control_pin;

    device->user_data = (void*) dev;

    return rt_device_register(device, nca9555_name, RT_DEVICE_FLAG_RDWR);
}

/**
 * This function is using for hardware initialization at system startup
 * @return  the status of initialization.
 */
static int rt_hw_nca9555_port(void)
{
    for (int i = 0; i < ITEM_NUM(nca9555_obj); i++)
    {
        nca9555_obj[i].dev = nca9555_init(nca9555_obj[i].bus_name, nca9555_obj[i].addr);
        if (rt_device_nca9555_register(nca9555_obj[i].dev, nca9555_obj[i].name) != RT_EOK)
        {
            LOG_E("NCA9555 device %s register failed.", nca9555_obj[i].name);
            return -RT_ERROR;
        }
    }
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_nca9555_port);

