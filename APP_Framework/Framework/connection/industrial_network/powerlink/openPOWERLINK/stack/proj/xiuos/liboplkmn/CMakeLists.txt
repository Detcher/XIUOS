################################################################################
#
# CMake file for openPOWERLINK MN library
#
# Copyright (c) 2017, B&R Industrial Automation GmbH
# Copyright (c) 2020, AIIT XUOS Lab
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the copyright holders nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
################################################################################

# Set library name
SET(LIB_NAME "oplkmn")
MESSAGE(STATUS "Configuring ${LIB_NAME}")

# Set type of library
IF(CFG_COMPILE_SHARED_LIBRARY)
    SET(LIB_TYPE "SHARED")
ELSE()
    SET(LIB_TYPE "STATIC")
ENDIF()

# set general sources of POWERLINK library
SET (LIB_SOURCES
     ${USER_SOURCES}
     ${USER_MN_SOURCES}
     ${CTRL_UCAL_DIRECT_SOURCES}
     ${DLL_UCAL_CIRCBUF_SOURCES}
     ${ERRHND_UCAL_LOCAL_SOURCES}
     ${EVENT_UCAL_NOOSKERNEL_SOURCES}
     ${PDO_UCAL_LOCAL_SOURCES}
     ${USER_TIMER_XIUOS_SOURCES}
     ${KERNEL_SOURCES}
     ${CTRL_KCAL_DIRECT_SOURCES}
     ${DLL_KCAL_CIRCBUF_SOURCES}
     ${ERRHND_KCAL_LOCAL_SOURCES}
     ${EVENT_KCAL_NOOSKERNEL_SOURCES}
     ${PDO_KCAL_LOCAL_SOURCES}
     ${COMMON_SOURCES}
     ${COMMON_LINUXUSER_SOURCES}
     ${TARGET_XIUOS_SOURCES}
     ${CIRCBUF_NOOS_SOURCES}
     ${MEMMAP_NULL_SOURCES}
     ${HARDWARE_DRIVER_XIUOS_SOURCES}
     )

# Configure compile definitions
IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86(_64)?)$")
    SET(LIB_SOURCES ${LIB_SOURCES} ${ARCH_X86_SOURCES})
ELSEIF(CMAKE_SYSTEM_PROCESSOR MATCHES arm*)
    SET(LIB_SOURCES ${LIB_SOURCES} ${ARCH_LE_SOURCES})
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR}")
ENDIF()

# Configure compile definitions
IF(CFG_INCLUDE_MN_REDUNDANCY)
    ADD_DEFINITIONS(-DCONFIG_INCLUDE_NMT_RMN)
ENDIF()
ADD_DEFINITIONS(-D__XIUOS__)
ADD_DEFINITIONS(-DCONFIG_FIND_LOCAL_INTERFACES)
ADD_DEFINITIONS(-DCONFIG_MN -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=c99 -fno-strict-aliasing")

# Additional include directories
INCLUDE_DIRECTORIES(
    .
    )

# Define library and installation rules
ADD_LIBRARY(${LIB_NAME} ${LIB_TYPE} ${LIB_SOURCES})
TARGET_LINK_LIBRARIES(${LIB_NAME} ${ARCH_LIBRARIES})
SET_PROPERTY(TARGET ${LIB_NAME} PROPERTY COMPILE_DEFINITIONS_DEBUG DEBUG;DEF_DEBUG_LVL=${CFG_DEBUG_LVL})
SET_PROPERTY(TARGET ${LIB_NAME} PROPERTY DEBUG_POSTFIX "_d")
INSTALL(TARGETS ${LIB_NAME} ARCHIVE DESTINATION . LIBRARY DESTINATION .)
