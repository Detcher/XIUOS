/****************************************************************************
 * boards/arm/stm32/aiit-arm32-board/src/stm32_pm.c
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

/**
* @file stm32_pm.c
* @brief nuttx source code
*                https://github.com/apache/incubator-nuttx.git
* @version 10.3.0 
* @author AIIT XUOS Lab
* @date 2022-03-17
*/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "arm_internal.h"
#include "stm32_pm.h"
#include "aiit-arm32-board.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_pminitialize
 *
 * Description:
 *   This function is called by MCU-specific logic at power-on reset in
 *   order to provide one-time initialization the power management subsystem.
 *   This function must be called *very* early in the initialization sequence
 *   *before* any other device drivers are initialized (since they may
 *   attempt to register with the power management subsystem).
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *    None.
 *
 ****************************************************************************/

void arm_pminitialize(void)
{
#if defined(CONFIG_ARCH_IDLE_CUSTOM) && defined(CONFIG_PM_BUTTONS)
  /* Initialize the buttons to wake up the system from low power modes */

  stm32_pm_buttons();
#endif

  /* Initialize the LED PM */

  stm32_led_pminitialize();
}

#endif /* CONFIG_PM */
