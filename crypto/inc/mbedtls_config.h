/**
 * \file config.h
 *
 * \brief Configuration options (set of defines)
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

#include "platforms.h"

/* For new platforms the configuration file resides in hal */
#if PLATFORM_ID == PLATFORM_GCC || PLATFORM_ID == PLATFORM_NEWHAL
#include "mbedtls_config_default.h"
#else
#include "mbedtls_config_platform.h"
#endif /* PLATFORM_ID == PLATFORM_GCC || PLATFORM_ID == PLATFORM_NEWHAL */

#if defined(MODULAR_FIRMWARE) && MODULAR_FIRMWARE
#endif /* defined(MODULAR_FIRMWARE) && MODULAR_FIRMWARE */

#endif /* MBEDTLS_CONFIG_H */
