/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-03     liwentai       the first version
 */
#ifndef APPLICATIONS_HW_MODULE_INCLUDE_LED_H_
#define APPLICATIONS_HW_MODULE_INCLUDE_LED_H_

#include "tool.h"
#include <drv_i2c_nca9555.h>

rt_device_t led_init(const char* dev_name);

#endif /* APPLICATIONS_HW_MODULE_INCLUDE_LED_H_ */
