/****************************************************************************
 * libs/libc-musl/string/lib_flsll.c
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
* @file lib_flsll.c
* @brief nuttx source code
*        https://github.com/apache/incubator-nuttx.git
* @version 10.3.0 
* @author AIIT XUOS Lab
* @date 2022-08-04
*/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/compiler.h>
#include <strings.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NBITS (8 * sizeof(unsigned long long))

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_HAVE_LONG_LONG

/****************************************************************************
 * Name: flsll
 *
 * Description:
 *   The flsll() function will find the last bit set in value and return
 *   the index of that bit. Bits are numbered starting at one (the least
 *   significant bit).
 *
 * Returned Value:
 *   The flsll() function will return the index of the last bit set. If j is
 *   0, then flsll() will return 0.
 *
 ****************************************************************************/

int flsll(long long j)
{
  int ret = 0;

  if (j != 0)
    {
#ifdef CONFIG_HAVE_BUILTIN_CLZ
      /* Count leading zeros function can be used to implement fls. */

      ret = NBITS - __builtin_clzll(j);
#else
      unsigned long long value = (unsigned long long)j;
      int bitno;

      for (bitno = 1; bitno <= NBITS; bitno++, value >>= 1)
        {
          if (value == 1)
            {
              ret = bitno;
              break;
            }
        }
#endif
    }

  return ret;
}

#endif
