/****************************************************************************
 * libs/libc-musl/queue/dq_addfirst.c
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
* @file dq_addfirst.c
* @brief nuttx source code
*        https://github.com/apache/incubator-nuttx.git
* @version 10.3.0 
* @author AIIT XUOS Lab
* @date 2022-08-04
*/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <queue.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dq_addfirst
 *
 * Description:
 *  dq_addfirst affs 'node' at the beginning of 'queue'
 *
 ****************************************************************************/

void dq_addfirst(FAR dq_entry_t *node, dq_queue_t *queue)
{
  node->blink = NULL;
  node->flink = queue->head;

  if (!queue->head)
    {
      queue->head = node;
      queue->tail = node;
    }
  else
    {
      queue->head->blink = node;
      queue->head = node;
    }
}
