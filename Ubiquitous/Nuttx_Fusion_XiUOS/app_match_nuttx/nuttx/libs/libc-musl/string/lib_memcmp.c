/****************************************************************************
 * libs/libc-musl/string/lib_memcmp.c
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
* @file lib_memcmp.c
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

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifndef CONFIG_LIBC_ARCH_MEMCMP
#undef memcmp /* See mm/README.txt */
int memcmp(FAR const void *s1, FAR const void *s2, size_t n)
{
  const unsigned char *l = (unsigned char *)s1;
  const unsigned char *r = (unsigned char *)s2;

	for (; n && *l == *r; n--, l++, r++);
	return n ? *l-*r : 0;
}
#endif
