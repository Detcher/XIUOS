/****************************************************************************
 * libs/libc-musl/string/lib_strncasecmp.c
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
* @file lib_strncasecmp.c
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
#include <strings.h>
#include <ctype.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifndef CONFIG_ARCH_STRNCASECMP
#undef strncasecmp /* See mm/README.txt */
int strncasecmp(const char *cs, const char *ct, size_t nb)
{
	const unsigned char *l = (void *)cs, *r = (void *)ct;
	if (!nb--) return 0;
	for (; *l && *r && nb && (*l == *r || (tolower(*l) == tolower(*r))); l++, r++, nb--);
	return tolower(*l) - tolower(*r);
}
#endif
