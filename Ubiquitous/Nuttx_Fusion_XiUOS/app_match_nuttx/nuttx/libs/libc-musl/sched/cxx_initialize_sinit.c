/****************************************************************************
 * libs/libc-musl/sched/cxx_initialize_sinit.c
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
* @file cxx_initialize_sinit.c
* @brief nuttx source code
*        https://github.com/apache/incubator-nuttx.git
* @version 10.3.0 
* @author AIIT XUOS Lab
* @date 2022-08-04
*/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <assert.h>
#include <debug.h>
#include <sched.h>
#include <stdlib.h>

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This type defines one entry in initialization array */

typedef CODE void (*initializer_t)(void);

/****************************************************************************
 * External References
 ****************************************************************************/

/* _sinit and _einit are symbols exported by the linker script that mark the
 * beginning and the end of the C++ initialization section.
 */

extern initializer_t _sinit;
extern initializer_t _einit;

/* _stext and _etext are symbols exported by the linker script that mark the
 * beginning and the end of text.
 */

extern uintptr_t _stext;
extern uintptr_t _etext;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cxx_initialize
 *
 * Description:
 *   If C++ and C++ static constructors are supported, then this function
 *   must be provided by board-specific logic in order to perform
 *   initialization of the static C++ class instances.
 *
 *   This function should then be called in the application-specific
 *   user_start logic in order to perform the C++ initialization.  NOTE
 *   that no component of the core NuttX RTOS logic is involved; this
 *   function definition only provides the 'contract' between application
 *   specific C++ code and platform-specific toolchain support.
 *
 ****************************************************************************/

void cxx_initialize(void)
{
#ifdef CONFIG_HAVE_CXXINITIALIZE
  static int inited = 0;

  if (inited == 0)
    {
      initializer_t *initp;

      sinfo("_sinit: %p _einit: %p _stext: %p _etext: %p\n",
            &_sinit, &_einit, &_stext, &_etext);

      /* Visit each entry in the initialization table */

      for (initp = &_sinit; initp != &_einit; initp++)
        {
          initializer_t initializer = *initp;
          sinfo("initp: %p initializer: %p\n", initp, initializer);

          /* Make sure that the address is non-NULL and lies in the text
           * region defined by the linker script.  Some toolchains may put
           * NULL values or counts in the initialization table.
           */

          if ((FAR void *)initializer >= (FAR void *)&_stext &&
              (FAR void *)initializer < (FAR void *)&_etext)
            {
              sinfo("Calling %p\n", initializer);
              initializer();
            }
        }

      inited = 1;
    }
#endif
}
