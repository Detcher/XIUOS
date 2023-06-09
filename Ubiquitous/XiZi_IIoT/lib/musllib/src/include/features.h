#ifndef FEATURES_H
#define FEATURES_H

#include "../../include/features.h"

#define hidden __attribute__((__visibility__("hidden")))
// #define weak __attribute__((__weak__))
#ifndef weak_alias
#define weak_alias(old, new) \
    extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))
#endif
#ifndef strong_alias
#define strong_alias(old, new) \
    extern __typeof(old) new __attribute__((__alias__(#old)))
#endif
#endif
