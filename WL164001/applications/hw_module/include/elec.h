/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-03     liwentai       the first version
 */
#ifndef APPLICATIONS_HW_MODULE_INCLUDE_ELEC_H_
#define APPLICATIONS_HW_MODULE_INCLUDE_ELEC_H_

#include "tool.h"
#include <drv_i2c_nca9555.h>

rt_err_t elec_init(rt_device_t elec);
rt_err_t elec_open(int i, WL164001_t board);
rt_err_t elec_close(int i, WL164001_t board);

#endif /* APPLICATIONS_HW_MODULE_INCLUDE_ELEC_H_ */
