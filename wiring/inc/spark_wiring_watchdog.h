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

#pragma once

#include <chrono>

#include "spark_wiring_thread.h"
#include "spark_wiring_platform.h"
#include "delay_hal.h"
#include "timer_hal.h"

#if PLATFORM_THREADING

class ApplicationWatchdog
{
	volatile system_tick_t timeout;
	static volatile system_tick_t last_checkin;

	std::function<void(void)> timeout_fn;

	Thread* thread;

	static void start(void* pointer);

	void loop();

public:
    static const unsigned DEFAULT_STACK_SIZE = 512;

	ApplicationWatchdog(unsigned timeout_ms, std::function<void(void)> fn, unsigned stack_size=DEFAULT_STACK_SIZE) :
		timeout(timeout_ms),
		timeout_fn(fn)
	{
		checkin();
		thread = new Thread("appwdt", start, this, OS_THREAD_PRIORITY_CRITICAL, stack_size);
	}
	ApplicationWatchdog(std::chrono::milliseconds ms, std::function<void(void)> fn, unsigned stack_size=DEFAULT_STACK_SIZE) : ApplicationWatchdog(ms.count(), fn, stack_size) {}

    // This constuctor helps to resolve overloaded function types, such as System.reset(), which is not always
    // possible in case of std::function
    ApplicationWatchdog(unsigned timeout_ms, void (*fn)(), unsigned stack_size=DEFAULT_STACK_SIZE) :
        ApplicationWatchdog(timeout_ms, std::function<void()>(fn), stack_size)
    {
    }
    ApplicationWatchdog(std::chrono::milliseconds ms, void (*fn)(), unsigned stack_size=DEFAULT_STACK_SIZE) : ApplicationWatchdog(ms.count(), fn, stack_size) {}

	~ApplicationWatchdog() {
		dispose();
		if (thread) {
			// NOTE: this may have to wait up to original timeout for the thread to exit
			delete thread;
		}
	}

	bool isComplete()
	{
		return !thread || !thread->isRunning();
	}

	static inline system_tick_t current_time()
	{
		return HAL_Timer_Get_Milli_Seconds();
	}

	bool has_timedout()
	{
		return (current_time()-last_checkin)>=timeout;
	}

	/**
	 * Dispose of this thread next time it wakes up.
	 */
	void dispose()
	{
		timeout = 0;
	}

	/**
	 * Lifesign that the application is still working normally.
	 */
	static void checkin()
	{
		last_checkin = current_time();
	}

};

inline void application_checkin() { ApplicationWatchdog::checkin(); }

#else

inline void application_checkin() {  }

#endif // PLATFORM_THREADING


#if Wiring_Watchdog

#include "watchdog_hal.h"
#include "enumflags.h"

namespace particle {

enum class WatchdogCaps : uint32_t {
    NONE            = WATCHDOG_CAPS_NONE,
    SOFT_RESET      = WATCHDOG_CAPS_SOFT_RESET,         /** The watchdog can be configured to not hard resetting device on expired. */
    INT             = WATCHDOG_CAPS_INT,                /** The watchdog can generate an interrupt on expired. */
    RECONFIGURABLE  = WATCHDOG_CAPS_RECONFIGURABLE,     /** The watchdog can be re-configured after started. */
    STOPPABLE       = WATCHDOG_CAPS_STOPPABLE,          /** The watchdog can be stopped after started. */
    ALL             = WATCHDOG_CAPS_ALL
};
ENABLE_ENUM_CLASS_BITWISE(WatchdogCaps);

using WatchdogOnExpiredStdFunction = std::function<void(void)>;
typedef void (*WatchdogOnExpiredCallback)(void* context);

class WatchdogInfo : public hal_watchdog_info_t {
public:
    WatchdogInfo() {
        this->size = sizeof(hal_watchdog_info_t);
        this->version = HAL_WATCHDOG_VERSION;
    }
};

class WatchdogConfiguration {
public:
    WatchdogConfiguration()
            : config_() {
        config_.size = sizeof(hal_watchdog_config_t);
        config_.version = HAL_WATCHDOG_VERSION;
        config_.timeout_ms = WATCHDOG_DEFAULT_TIMEOUT_MS;
        config_.hard_reset = true;
    }

    ~WatchdogConfiguration() = default;

    WatchdogConfiguration& timeout(system_tick_t ms) {
        config_.timeout_ms = ms;
        return *this;
    }

    WatchdogConfiguration& timeout(std::chrono::milliseconds ms) {
        return timeout(ms.count());
    }

    WatchdogConfiguration& hardReset(bool val = true) {
        config_.hard_reset = val;
        return *this;
    }

    const hal_watchdog_config_t* halConfig() const {
        return &config_;
    }

    hal_watchdog_instance_t watchdogInstance() const {
        // TODO: Adjust the instance according to the watchdog configuration.
        return HAL_WATCHDOG_INSTANCE1;
    }

private:
    hal_watchdog_config_t config_;

    static constexpr uint32_t WATCHDOG_DEFAULT_TIMEOUT_MS = 5000;
};

/**
 * @note For nRF52-based platforms,
 *  - the watchdog will be paused during sleep mode and debug mode.
 *  - When device wakes up from the hibernate mode, the watchdog won't restart automatically.
 *  - When device wakes up from the ultra-low power mode or stop mode, or restarts after System.reset(),
 *    the watchdog will continue running if the watchdog has started previously. Use Watchdog.started()
 *    to check the status of watchdog.
 */
class WatchdogClass {
public:
    /**
     * @brief Initialize the hardware watchdog so that it is ready to be started.
     * 
     * @note Some hardware watchdog don't support re-configuring once started. @ref WatchdogCaps::RECONFIGURABLE
     * 
     * @param[in] config The watchdog configuration @ref WatchdogConfiguration
     * @return SYSTEM_ERROR_NONE if succeeded, otherwise failed.
     */
    int init(const WatchdogConfiguration& config);

    /**
     * @brief Start the hardware watchdog. Only if the watchdog is initialized, it can be started. When the
     * watchdog is running, calling this method makes no effect, i.e. it won't refresh the watchdog.
     * 
     * @return SYSTEM_ERROR_NONE if succeeded, otherwise failed.
     */
    int start();

    /**
     * @brief Check if the watchdog is started.
     * 
     * @return true if started, otherwise false.
     */
    bool started();

    /**
     * @brief Stop the hardware watchdog.
     * 
     * @note Some hardware watchdog don't support stopping once started. @ref WatchdogCaps::WATCHDOG_CAPS_STOPPABLE
     * 
     * @return SYSTEM_ERROR_NONE if succeeded, otherwise failed.
     */
    int stop();

    /**
     * @brief Refresh the hardware watchdog counter, as like it is re-started. The watchdog should be refreshed
     * before it expires, otherwise, a callback is invoked (if supported) followed by hard resetting the device.
     * 
     * @return SYSTEM_ERROR_NONE if succeeded, otherwise failed.
     */
    int refresh();

    /**
     * @brief Get the hardware watchdog infomations. @ref WatchdogInfo
     * 
     * @param[in,out] info A left-value reference for storing the retrieved watchdog informations.
     * @return SYSTEM_ERROR_NONE if succeeded, otherwise failed.
     */
    int getInfo(WatchdogInfo& info);

    /**
     * @brief Set the callback to be invoked when watchdog expires.
     * 
     * @note Some hardware watchdog don't support callback when expired. @ref WatchdogCaps::INT
     * 
     * @param[in] callback The callback function to be set.
     * @param[in] context The context to be propagated when the callback is invoked.
     * @return SYSTEM_ERROR_NONE if succeeded, otherwise failed.
     */
    int onExpired(WatchdogOnExpiredCallback callback, void* context = nullptr);

    int onExpired(const WatchdogOnExpiredStdFunction& callback);
    
    template<typename T>
    int onExpired(void(T::*callback)(void), T* instance) {
        return onExpired((callback && instance) ? std::bind(callback, instance) : (WatchdogOnExpiredStdFunction)nullptr);
    }

    /**
     * @brief Get the hardware watchdog instance.
     * 
     * @return The hardware watchdog singlton.
     */
    static WatchdogClass& getInstance() {
        static WatchdogClass watchdog;
        return watchdog;
    }

private:
    WatchdogClass()
            : callback_(nullptr),
              instance_(HAL_WATCHDOG_INSTANCE1) {
    }
    ~WatchdogClass() = default;

    static void onWatchdogExpiredCallback(void* context) {
        auto instance = reinterpret_cast<WatchdogClass*>(context);
        if (instance->callback_) {
            instance->callback_();
        }
    }

    WatchdogOnExpiredStdFunction callback_;
    hal_watchdog_instance_t instance_;
};

#define Watchdog WatchdogClass::getInstance()

} // namespace particle

#endif // Wiring_Watchdog

