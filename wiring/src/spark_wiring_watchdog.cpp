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

#include "spark_wiring_watchdog.h"

#if Wiring_Watchdog

#include "check.h"
#include "logging.h"
LOG_SOURCE_CATEGORY("wiring.watchdog")

namespace particle {

int WatchdogClass::init(const WatchdogConfiguration& config) {
    instance_ = config.watchdogInstance();
    WatchdogInfo info;
    CHECK(hal_watchdog_get_info(instance_, &info, nullptr));
    if (static_cast<WatchdogCaps>(info.capabilities) & WatchdogCaps::INT) {
        CHECK(hal_watchdog_on_expired_callback(instance_, onWatchdogExpiredCallback, this, nullptr));
    }
    return hal_watchdog_set_config(instance_, config.halConfig(), nullptr);
}

int WatchdogClass::start() {
    return hal_watchdog_start(instance_, nullptr);
}

bool WatchdogClass::started() {
    WatchdogInfo info;
    CHECK_RETURN(hal_watchdog_get_info(instance_, &info, nullptr), false);
    return info.running;
}

int WatchdogClass::stop() {
    return hal_watchdog_stop(instance_, nullptr);
}

int WatchdogClass::refresh() {
    return hal_watchdog_refresh(instance_, nullptr);
}

int WatchdogClass::getInfo(WatchdogInfo& info) {
    return hal_watchdog_get_info(instance_, &info, nullptr);
}

int WatchdogClass::onExpired(WatchdogOnExpiredCallback callback, void* context) {
    WatchdogInfo info;
    CHECK(hal_watchdog_get_info(instance_, &info, nullptr));
    if (static_cast<WatchdogCaps>(info.capabilities) & WatchdogCaps::INT) {
        callback_ = callback ? std::bind(callback, context) : (WatchdogOnExpiredStdFunction)nullptr;
    }
    return SYSTEM_ERROR_NOT_SUPPORTED;
}

int WatchdogClass::onExpired(const WatchdogOnExpiredStdFunction& callback) {
    WatchdogInfo info;
    CHECK(hal_watchdog_get_info(instance_, &info, nullptr));
    if (static_cast<WatchdogCaps>(info.capabilities) & WatchdogCaps::INT) {
        callback_ = callback;
    }
    return SYSTEM_ERROR_NOT_SUPPORTED;
}

} /* namespace particle */

#endif /* Wiring_Watchdog */
