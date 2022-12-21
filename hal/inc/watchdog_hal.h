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
#include "system_tick_hal.h"

#if HAL_PLATFORM_HW_WATCHDOG

#define HAL_WATCHDOG_VERSION    1

typedef void (*hal_watchdog_on_expired_callback_t)(void* context);

typedef enum hal_watchdog_instance_t {
    HAL_WATCHDOG_INSTANCE1 = 0,
    HAL_WATCHDOG_INSTANCE2 = 1,
} hal_watchdog_instance_t;

typedef enum hal_watchdog_capability_t {
    WATCHDOG_CAPS_NONE              = 0x00,
    WATCHDOG_CAPS_SOFT_RESET        = 0x01,
    WATCHDOG_CAPS_INT               = 0x02,
    WATCHDOG_CAPS_RECONFIGURABLE    = 0x04,
    WATCHDOG_CAPS_STOPPABLE         = 0x08,
    WATCHDOG_CAPS_ALL               = 0xFFFFFFFF
    // halt in sleep/debug mode?
} hal_watchdog_capability_t;

typedef struct hal_watchdog_config_t {
    uint16_t                            size;
    uint16_t                            version;
    system_tick_t                       timeout_ms;
    uint8_t                             hard_reset;
    uint8_t                             reserved[3];
} hal_watchdog_config_t;

typedef struct hal_watchdog_info_t {
    uint16_t                    size;
    uint16_t                    version;
    uint32_t                    capabilities;
    system_tick_t               timeout_ms;
    system_tick_t               min_timeout_ms;
    system_tick_t               max_timeout_ms;
    uint8_t                     running;
    uint8_t                     reserved[3];
} hal_watchdog_info_t;

#ifdef __cplusplus
#include <algorithm>
#include "check.h"
class Watchdog {
public:
    Watchdog(uint32_t capabilities, uint32_t minTimeout, uint32_t maxTimeout)
            : config_{},
              info_{} {
        config_.size = sizeof(hal_watchdog_config_t);
        config_.version = HAL_WATCHDOG_VERSION;

        info_.size = sizeof(hal_watchdog_info_t);
        info_.version = HAL_WATCHDOG_VERSION;
        info_.capabilities = capabilities;
        info_.min_timeout_ms = minTimeout;
        info_.max_timeout_ms = maxTimeout;
        info_.running = false;
    }

    Watchdog() = default;

    void notify() {
        if (callback_) {
            callback_(context_);
        }
    }

    virtual bool started() { 
        return info_.running;
    }

    virtual int stop() {
        return SYSTEM_ERROR_NOT_SUPPORTED;
    }
    
    virtual int getInfo(hal_watchdog_info_t* info) {
        CHECK_TRUE(info && (info->size > 0), SYSTEM_ERROR_INVALID_ARGUMENT);
        memcpy(info, &info_, std::min(info->size, info_.size));
        return SYSTEM_ERROR_NONE;
    }

    virtual int setOnExpiredCallback(hal_watchdog_on_expired_callback_t callback, void* context) {
        return SYSTEM_ERROR_NOT_SUPPORTED;
    }

    virtual int init(const hal_watchdog_config_t* config) = 0;
    virtual int start() = 0;
    virtual int refresh() = 0;

    hal_watchdog_config_t config_;
    hal_watchdog_info_t info_;
    hal_watchdog_on_expired_callback_t callback_;
    void* context_;

};
#endif // __cplusplus

#ifdef	__cplusplus
extern "C" {
#endif

int hal_watchdog_set_config(hal_watchdog_instance_t instance, const hal_watchdog_config_t* config, void* reserved);
int hal_watchdog_on_expired_callback(hal_watchdog_instance_t instance, hal_watchdog_on_expired_callback_t callback, void* context, void* reserved);
int hal_watchdog_start(hal_watchdog_instance_t instance, void* reserved);
int hal_watchdog_stop(hal_watchdog_instance_t instance, void* reserved);
int hal_watchdog_refresh(hal_watchdog_instance_t instance, void* reserved);
int hal_watchdog_get_info(hal_watchdog_instance_t instance, hal_watchdog_info_t* info, void* reserved);

// Dynalib backwards compatible
bool hal_watchdog_reset_flagged_deprecated();
void hal_watchdog_refresh_deprecated(void);

#ifdef	__cplusplus
}
#endif

#endif // HAL_PLATFORM_HW_WATCHDOG

#endif	/* WATCHDOG_HAL_H */

