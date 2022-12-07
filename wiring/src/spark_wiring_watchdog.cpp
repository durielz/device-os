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
    return SYSTEM_ERROR_NONE;
}

int WatchdogClass::setTimeout(system_tick_t timeout) {
    return SYSTEM_ERROR_NONE;
}

int WatchdogClass::start() {
    return SYSTEM_ERROR_NONE;
}

int WatchdogClass::stop() {
    return SYSTEM_ERROR_NONE;
}

int WatchdogClass::refresh() {
    return SYSTEM_ERROR_NONE;
}

int WatchdogClass::getInfo(WatchdogInfo& info) {
    return SYSTEM_ERROR_NONE;
}

int WatchdogClass::onExpired(WatchdogOnExpiredCallback callback, void* context) {
    callback_ = callback ? std::bind(callback, context) : (WatchdogOnExpiredStdFunction)nullptr;
    return SYSTEM_ERROR_NONE;
}

int WatchdogClass::onExpired(const WatchdogOnExpiredStdFunction& callback) {
    callback_ = callback;
    return SYSTEM_ERROR_NONE;
}

void WatchdogClass::onWatchdogExpiredCallback(void* context) {
    auto instance = reinterpret_cast<WatchdogClass*>(context);
    if (instance->callback_) {
        instance->callback_();
    }
}

} /* namespace particle */

#endif /* Wiring_Watchdog */
