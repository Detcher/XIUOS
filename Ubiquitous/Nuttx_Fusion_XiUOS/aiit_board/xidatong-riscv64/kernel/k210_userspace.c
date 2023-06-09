/*
* Copyright (c) 2020 AIIT XUOS Lab
* XiUOS is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*        http://license.coscl.org.cn/MulanPSL2
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
* See the Mulan PSL v2 for more details.
*/

/**
 * @file k210_userspace.c
 * @brief xidatong-riscv64 k210_userspace.c
 * @version 1.0
 * @author AIIT XUOS Lab
 * @date 2022.03.17
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdlib.h>

#include <nuttx/arch.h>
#include <nuttx/mm/mm.h>
#include <nuttx/wqueue.h>
#include <nuttx/userspace.h>

#if defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

#ifndef CONFIG_NUTTX_USERSPACE
#  error "CONFIG_NUTTX_USERSPACE not defined"
#endif

#if CONFIG_NUTTX_USERSPACE != 0x80100000
#  error "CONFIG_NUTTX_USERSPACE must match the value in memory.ld"
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* These 'addresses' of these values are setup by the linker script.
 * They are not actual uint32_t storage locations!
 * They are only used meaningfully in the following way:
 *
 *  - The linker script defines, for example, the symbol_sdata.
 *  - The declaration extern uint32_t _sdata; makes C happy.  C will believe
 *    that the value _sdata is the address of a uint32_t variable _data
 *    (it is not!).
 *  - We can recover the linker value then by simply taking the address of
 *    of _data.  like:  uint32_t *pdata = &_sdata;
 */

extern uint32_t _stext;           /* Start of .text */
extern uint32_t _etext;           /* End_1 of .text + .rodata */
extern const uint32_t _eronly;    /* End+1 of read only section */
extern uint32_t _sdata;           /* Start of .data */
extern uint32_t _edata;           /* End+1 of .data */
extern uint32_t _sbss;            /* Start of .bss */
extern uint32_t _ebss;            /* End+1 of .bss */

/* This is the user space entry point */

int CONFIG_INIT_ENTRYPOINT(int argc, char *argv[]);

const struct userspace_s userspace locate_data(".userspace") =
{
  /* General memory map */

  .us_entrypoint    = (main_t)CONFIG_INIT_ENTRYPOINT,
  .us_textstart     = (uintptr_t)&_stext,
  .us_textend       = (uintptr_t)&_etext,
  .us_datasource    = (uintptr_t)&_eronly,
  .us_datastart     = (uintptr_t)&_sdata,
  .us_dataend       = (uintptr_t)&_edata,
  .us_bssstart      = (uintptr_t)&_sbss,
  .us_bssend        = (uintptr_t)&_ebss,

  /* Memory manager heap structure */

  .us_heap          = &g_mmheap,

  /* Task/thread startup routines */

  .task_startup     = nxtask_startup,

  /* Signal handler trampoline */

  .signal_handler   = up_signal_handler,

  /* User-space work queue support (declared in include/nuttx/wqueue.h) */

#ifdef CONFIG_LIBC_USRWORK
  .work_usrstart    = work_usrstart,
#endif
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#endif /* CONFIG_BUILD_PROTECTED && !__KERNEL__ */
