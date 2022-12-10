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

#include "watchdog_hal.h"

#if HAL_PLATFORM_HW_WATCHDOG
#include "check.h"
#include "logging.h"

int hal_watchdog_set_config(hal_watchdog_instance_t instance, const hal_watchdog_config_t* config, void* reserved) {
    return SYSTEM_ERROR_NONE;
}

int hal_watchdog_on_expired_callback(hal_watchdog_instance_t instance, hal_watchdog_on_expired_callback_t callback, void* context, void* reserved) {
    return SYSTEM_ERROR_NONE;
}

int hal_watchdog_start(hal_watchdog_instance_t instance, void* reserved) {
    return SYSTEM_ERROR_NONE;
}

int hal_watchdog_stop(hal_watchdog_instance_t instance, void* reserved) {
    return SYSTEM_ERROR_NONE;
}

int hal_watchdog_refresh(hal_watchdog_instance_t instance, void* reserved) {
    return SYSTEM_ERROR_NONE;
}

int hal_watchdog_get_info(hal_watchdog_instance_t instance, hal_watchdog_info_t* info, void* reserved) {
    return SYSTEM_ERROR_NONE;
}

// backward compatibility for nRF52
bool hal_watchdog_reset_flagged_deprecated(void) {
    return false;
}

void hal_watchdog_refresh_deprecated() {
}

#endif // HAL_PLATFORM_HW_WATCHDOG
