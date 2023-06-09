/****************************************************************************
 * libs/libc-musl/locale/lib_setlocale.c
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
* @file lib_setlocale.c
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

#include <sys/types.h>
#include <string.h>
#include <locale.h>

#ifdef CONFIG_LIBC_LOCALE

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: setlocale
 *
 * Description:
 *   locales are not supported by NuttX
 *
 * Input Parameters:
 *   category and locale - Select the appropriate piece of the program's
 *     locale.
 *
 ****************************************************************************/

FAR char *setlocale(int category, FAR const char *locale)
{
  return ((locale == NULL || strcmp(locale, "POSIX") == 0 ||
           strcmp(locale, "C") == 0 || strcmp(locale, "") ==
            0) ? "C" :  NULL);
}
#endif
