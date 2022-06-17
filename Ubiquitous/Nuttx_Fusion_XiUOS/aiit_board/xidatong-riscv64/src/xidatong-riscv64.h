/****************************************************************************
 * boards/risc-v/k210/xidatong-riscv64/src/xidatong-riscv64.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __BOARDS_RISCV_K210_XIDATONG_RISCV64_SRC_XIDATONG_RISCV64_H
#define __BOARDS_RISCV_K210_XIDATONG_RISCV64_SRC_XIDATONG_RISCV64_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

int k210_bringup(void);

#ifdef CONFIG_DEV_GPIO
int k210_gpio_init(void);
#endif

#endif /* __BOARDS_RISCV_K210_XIDATONG_RISCV64_SRC_XIDATONG_RISCV64_H */
