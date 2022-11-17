/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-18     liwentai       the first version
 */


//#include "drv_config.h"
#include    <rtthread.h>
#include    <rtdevice.h>


#include    <rtdef.h>
#include    <drivers/pin.h>
#include    "i2c_ops.h"

#define     DBG_TAG "nca9555"
#define     DBG_LVL DBG_INFO
#include    <rtdbg.h>

#define     SCL    GET_PIN(B, 6)
#define     SDA    GET_PIN(B, 7)


//#define     SCL    GET_PIN(C, 5)
//#define     SDA    GET_PIN(C, 7)




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

/**
 * This function will delay for some us.
 *
 * @param us the delay time of us
 */

//#define printfERROR
static void i2c_udelay(rt_uint32_t us)
{
    rt_uint32_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t reload = SysTick->LOAD;

    ticks = us * reload / (10000000 / RT_TICK_PER_SECOND);
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

#define     delay()   i2c_udelay(5)

rt_uint8_t  I2C_Start(void)
{
//    rt_uint8_t i = 0;
    rt_pin_mode(SCL, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(SDA, PIN_MODE_OUTPUT_OD);

    //SDA 高
    rt_pin_write(SDA, PIN_HIGH);

    //随后 SCL 高
    rt_pin_write(SCL, PIN_HIGH);
    delay();

    //SCL 高时拉低 SDA， 开始
    rt_pin_write(SDA, PIN_LOW);
    delay();

    //拉低 SCL 准备发
    rt_pin_write(SCL, PIN_LOW);
//    rt_pin_mode(SCL, PIN_MODE_INPUT);


//    if(PIN_HIGH == rt_pin_read(SDA))
//    {
//        rt_pin_mode(SDA, PIN_MODE_OUTPUT_OD);
//        delay();
//
//        rt_pin_write(SDA, PIN_LOW);
//        delay();
//
//        rt_pin_write(SCL, PIN_LOW);
////        delay();
//
//        return 1;
//    }
//    else
//    {
//        rt_pin_mode(SDA, PIN_MODE_OUTPUT_OD);
//        delay();
//
//        rt_pin_write(SCL, PIN_LOW);
//        delay();
//
//        for(i=0; i<9; i++)
//        {
//            rt_pin_write(SCL, PIN_HIGH);
//            delay();
//            rt_pin_write(SDA, PIN_LOW);
//        }
//        rt_kprintf("[ERROR] i2c start failure.\n");
//        return 0;
//    }

    return 0;

}

void        I2C_Stop(void)
{
    rt_pin_mode(SCL, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(SDA, PIN_MODE_OUTPUT_OD);

    //SCL SDA 低
    rt_pin_write(SDA, PIN_LOW);
    delay();

    //SCL SDA 高
    rt_pin_write(SCL, PIN_HIGH);
    delay();

    rt_pin_write(SDA, PIN_HIGH);
    delay();

//    rt_pin_mode(SCL, PIN_MODE_INPUT);
//    rt_pin_mode(SDA, PIN_MODE_INPUT);
}

rt_uint8_t  I2C_Send_Byte(rt_uint8_t i2c_data)
{
    rt_uint8_t i = 0;

    rt_pin_mode(SCL, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(SDA, PIN_MODE_OUTPUT_OD);
    //在每次收发（开始、停止）期间需要一个bus free time>4.7us


    //自带一个i2c_start操作
//    rt_pin_write(SDA, PIN_HIGH);
//    delay();
//    rt_pin_write(SCL, PIN_HIGH);
//    delay();
//
//    rt_pin_write(SDA, PIN_LOW);
//    delay();
//    rt_pin_write(SCL, PIN_LOW);
//    delay();

    //只有 SCL = 0 时，才能调整 SDA 否则会视为停止信号
    for(i=0;i<8;i++)
    {
        if(i2c_data & 0x80)
        {
            rt_pin_write(SDA, PIN_HIGH);
        }
        else
        {
            rt_pin_write(SDA, PIN_LOW);
        }
        i2c_data <<=1;
        //SDA 已稳定 ， 跳 SCL 一个脉冲发数据
        rt_pin_write(SCL, PIN_HIGH);
        delay();
        rt_pin_write(SCL, PIN_LOW);
        delay();
    }

    /*
     * SDA 在 SCL 第 9 脉冲前拉高，准备读从机 ACK
     * ACK 为低有效，需要在 SCL 第 9 脉冲期间持续至少 4us
     */

    rt_pin_mode(SDA, PIN_MODE_INPUT);


    rt_pin_write(SCL, PIN_HIGH);
    rt_uint8_t current_status = rt_pin_read(SDA);
    i2c_udelay(4);
    rt_uint8_t last_status = rt_pin_read(SDA);
//    delay();
    rt_pin_write(SCL, PIN_LOW);

    if(current_status == last_status)
    {
//        rt_pin_mode(SDA, PIN_MODE_OUTPUT_OD);
//        rt_pin_write(SCL, PIN_LOW);
        delay();

        return 0;
    }
    else
    {
//        rt_pin_mode(SDA, PIN_MODE_OUTPUT_OD);
//        rt_pin_write(SCL, PIN_LOW);
        delay();
        LOG_E("[error] lost the ack.\n");

        return 1;
    }
}

rt_uint8_t  I2C_Read_Byte(void)
{
    rt_uint8_t i = 0;
    rt_uint8_t result = 0;

    rt_pin_mode(SCL, PIN_MODE_OUTPUT_OD);
    delay();
    rt_pin_mode(SDA, PIN_MODE_INPUT);
    delay();
    rt_pin_write(SCL, PIN_LOW);

    for(i=0; i<8; i++)
    {
        result <<=1;
        delay();
        rt_pin_write(SCL, PIN_HIGH);
        delay();

        if(PIN_HIGH == rt_pin_read(SDA))
            result |= 0x01;

        delay();
        rt_pin_write(SCL, PIN_LOW);
    }

    rt_pin_mode(SDA, PIN_MODE_OUTPUT_OD);
    delay();
    rt_pin_write(SDA, PIN_LOW);
    delay();
    rt_pin_write(SCL, PIN_HIGH);
    delay();
    rt_pin_write(SCL, PIN_LOW);
    delay();
    rt_pin_write(SDA, PIN_HIGH);

    return result;
}


rt_uint8_t  I2C_Read_Last_Byte(void)
{
    rt_uint8_t i = 0;
    rt_uint8_t result = 0;

    rt_pin_mode(SCL, PIN_MODE_OUTPUT_OD);
    delay();
    rt_pin_mode(SDA, PIN_MODE_INPUT);
    delay();
    rt_pin_write(SCL, PIN_LOW);

    for(i=0; i<8; i++)
    {
        result <<=1;
        delay();
        rt_pin_write(SCL, PIN_HIGH);
        delay();

        if(PIN_HIGH == rt_pin_read(SDA))
            result |= 0x01;

        delay();
        rt_pin_write(SCL, PIN_LOW);
    }

    rt_pin_mode(SDA, PIN_MODE_OUTPUT_OD);
    delay();
    rt_pin_write(SDA, PIN_HIGH);
    delay();
    rt_pin_write(SCL, PIN_HIGH);
    delay();
    rt_pin_write(SCL, PIN_LOW);
    delay();

    return result;
}

rt_uint8_t nca9555_write(rt_uint8_t address,
                         rt_uint8_t command,
                         rt_uint16_t length,
                         rt_uint8_t *data)
{
    rt_uint8_t ret = 1;
    rt_uint8_t i_temp = 0;

    ret = I2C_Start();

    //INPUT
    if (!ret)
    {

        ret = I2C_Send_Byte(address);

    }
    //send address successfully
    if (!ret)
    {
        ret = I2C_Send_Byte(command);
    }
//    if (ret)
//    {
//        I2C_Send_Byte(data);
//    }

    for(i_temp=0;i_temp<(length);i_temp++)
    {
        //send register data successfully
        if (!ret)
        {
            I2C_Send_Byte(data[i_temp]);
        }
    }
#ifdef  printfERROR
    rt_kprintf("send over,ret = %d\n",ret);
#endif
//    all byte has been send
    if (!ret)
    {
        I2C_Stop();
    }

    return 0;
}

void nca9555_read(rt_uint8_t address,
                       rt_uint8_t command,
                       rt_uint16_t length,
                       rt_uint8_t *data)
{
    rt_uint8_t ret = 0;
    rt_uint8_t i_temp = 0;
    rt_uint8_t DataTemp = 0;

    ret = I2C_Start();
    if (ret)
    {
        ret = I2C_Send_Byte(address);
    }
    if (ret)
    {
        ret = I2C_Send_Byte(command);
    }

    if (ret)
    {
        ret = I2C_Start();
    }
    if (ret)
    {
        ret = I2C_Send_Byte((rt_uint8_t)(address|0x01));
    }
    for(i_temp=0;i_temp<(length-1);i_temp++)
    {
        DataTemp = I2C_Read_Byte();
        *data++ = DataTemp;
    }
    DataTemp = I2C_Read_Last_Byte();
    *data = DataTemp;
    if (ret)
    {
        I2C_Stop();
    }
}




rt_uint8_t setBuff[4]={0x00,0x00,0xff,0xff};

void nca9555_write_pin(int index, int type, rt_uint8_t flag)
{
//    rt_uint8_t pin_status[2];
    rt_uint8_t writeData[2] = {0};
    rt_uint8_t a = 0xff;
    rt_uint8_t b = 0x00;

    if(flag == PIN_LOW)
    {
        //定位引脚
        writeData[0] |= gpio_list[type][index][0];
        writeData[1] |= gpio_list[type][index][1];

        nca9555_write(NCA_ADDR, REGISTER_0_CONFIGURATION_PORT, 2, &setBuff[0]);
        // nca9555_write(NCA_ADDR, REGISTER_1_CONFIGURATION_PORT, 1, &setBuff[0]);
        nca9555_write(NCA_ADDR, REGISTER_0_OUTPUT_PORT, 2, &writeData[0]);
        // nca9555_write(NCA_ADDR, REGISTER_1_OUTPUT_PORT, 1, &b);

    }
    if(flag == PIN_HIGH)
    {
        writeData[0] &= ~(gpio_list[type][index][0]);
        writeData[1] &= ~(gpio_list[type][index][1]);

        nca9555_write(NCA_ADDR, REGISTER_0_CONFIGURATION_PORT, 2, &setBuff[0]);
        // nca9555_write(NCA_ADDR, REGISTER_1_CONFIGURATION_PORT, 1, &setBuff[0]);
        nca9555_write(NCA_ADDR, REGISTER_0_OUTPUT_PORT, 2, &writeData[0]);
        // nca9555_write(NCA_ADDR, REGISTER_1_OUTPUT_PORT, 1, &b);

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
        nca9555_read(0x22, REGISTER_0_INPUT_PORT, 1, &register_data);
        pin_status = (register_data >> index)&0x01 ;
        break;
    case GREEN_LED:
        nca9555_read(0x22, REGISTER_1_INPUT_PORT, 1, &register_data);
        pin_status = (register_data >> index)&0x01 ;
        break;
    default:
        rt_kprintf("[ERROR] nca999_gpio error!\r\n");
    }

    return pin_status;
}
