/**
 ******************************************************************************
  Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
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

namespace particle {

enum class WatchdogCaps : uint32_t {
    NONE            = WATCHDOG_CAPS_NONE,
    RESET           = WATCHDOG_CAPS_RESET,
    INT             = WATCHDOG_CAPS_INT,
    RECONFIGURABLE  = WATCHDOG_CAPS_RECONFIGURABLE,
    STOPPABLE       = WATCHDOG_CAPS_STOPPABLE,
    ALL             = WATCHDOG_CAPS_ALL
};

typedef std::function<void(void)> WatchdogOnExpiredStdFunction;
typedef void (*WatchdogOnExpiredCallback)(void);

class WatchdogConfiguration : public hal_watchdog_config_t {
};

class WatchdogInfo : public hal_watchdog_info_t {
};

class WatchdogClass {
public:
    int init(const WatchdogConfiguration& config);
    int setTimeout(system_tick_t timeout);
    int start();
    int stop();
    int kick();
    int getInfo(WatchdogInfo& info);

    int onExpired(WatchdogOnExpiredCallback callback, void* context = nullptr);
    int onExpired(const WatchdogOnExpiredStdFunction& callback);
    template<typename T>
    void onExpired(void(T::*callback)(void), T* instance) const {
        return onExpired((callback && instance) ? std::bind(callback, instance) : (WatchdogOnExpiredStdFunction)nullptr);
    }

    WatchdogClass& getInstance() {
        static WatchdogClass watchdog;
        return watchdog;
    }

private:
    WatchdogClass() = default;
    ~WatchdogClass() = default;

    static void onWatchdogExpiredCallback(void);

    WatchdogOnExpiredStdFunction callback_;
};

} // namespace particle

#endif // Wiring_Watchdog

