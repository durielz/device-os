/*
 * Copyright (c) 2022 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WATCHDOG_HAL_H
#define	WATCHDOG_HAL_H

#include <stdint.h>
#include <stdbool.h>
#include "hal_platform.h"

#ifdef	__cplusplus
extern "C" {
#endif

#if HAL_PLATFORM_HW_WATCHDOG

#define HAL_WATCHDOG_VERSION    1

typedef void (*hal_watchdog_on_expired_callback_t)(void);

typedef enum hal_watchdog_capability_t {
    WATCHDOG_CAPS_NONE              = 0x00,
    WATCHDOG_CAPS_RESET             = 0x01,
    WATCHDOG_CAPS_INT               = 0x02,
    WATCHDOG_CAPS_RECONFIGURABLE    = 0x04,
    WATCHDOG_CAPS_STOPPABLE         = 0x08,
    WATCHDOG_CAPS_ALL               = 0xFFFFFFFF
} hal_watchdog_capability_t;

typedef struct hal_watchdog_config_t {
    uint16_t                            size;
    uint16_t                            version;
    uint32_t                            period_ms;
    hal_watchdog_on_expired_callback_t  callback;
    void*                               context;
} hal_watchdog_config_t;

typedef struct hal_watchdog_info_t {
    uint16_t                    size;
    uint16_t                    version;
    hal_watchdog_capability_t   capabilities;
    uint32_t                    period_ms;
    uint32_t                    min_period_ms;
    uint32_t                    max_period_ms;
    uint8_t                     running;
    uint8_t                     reserved[3];
} hal_watchdog_info_t;

int hal_watchdog_set_config(const hal_watchdog_config_t* config, void* reserved);
int hal_watchdog_set_timeout(uint32_t timeout, void* reserved);
int hal_watchdog_on_expired_callback(hal_watchdog_on_expired_callback_t callback, void* context, void* reserved);
int hal_watchdog_start(void* reserved);
int hal_watchdog_stop(void* reserved);
void hal_watchdog_kick(void);
int hal_watchdog_get_info(hal_watchdog_info_t* info, void* reserved);

// Dynalib backwards compatible
bool hal_watchdog_reset_flagged_deprecated();

#endif // HAL_PLATFORM_HW_WATCHDOG

#ifdef	__cplusplus
}
#endif

#endif	/* WATCHDOG_HAL_H */

