/**
 * @file iswalpha.c
 * @brief musl source code
 *        https://github.com/bminor/musl.git
 * @version 1.0
 * @author AIIT XUOS Lab
 * @date 2022-08-04
 */

#include <wctype.h>

static const unsigned char table[] = {
#include "alpha.h"
};

int iswalpha(wint_t wc)
{
	if (wc<0x20000U)
		return (table[table[wc>>8]*32+((wc&255)>>3)]>>(wc&7))&1;
	if (wc<0x2fffeU)
		return 1;
	return 0;
}
