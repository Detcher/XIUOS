/**
 * @file integer.h
 * @brief integer
 * @version 0.1
 * @author SYS Lab
 * @date 2022.11.01
 */

#ifndef _FF_INTEGER
#define _FF_INTEGER

#ifdef _WIN32 /* FatFs development platform */

#include <tchar.h>
#include <windows.h>
typedef unsigned __int64 QWORD;

#else /* Embedded platform */

/* These types MUST be 16-bit or 32-bit */
typedef int INT;
typedef unsigned int UINT;

/* This type MUST be 8-bit */
typedef unsigned char BYTE;

/* These types MUST be 16-bit */
typedef short SHORT;
typedef unsigned short WORD;
typedef unsigned short WCHAR;

/* These types MUST be 32-bit */
typedef long LONG;
typedef unsigned long DWORD;

/* This type MUST be 64-bit (Remove this for C89 compatibility) */
typedef unsigned long long QWORD;

#endif

#endif
