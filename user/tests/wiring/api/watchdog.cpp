
#include "testapi.h"

#if Wiring_Watchdog == 1

using namespace std::placeholders;

class Handlers {
public:
    void onWatchdogExpired() {
    }
};

Handlers watchdogHandler;

void onWatchdogExpiredHandlerFunc(void* context) {
}

test(watchdog_caps) {
    API_COMPILE({
        EnumFlags<WatchdogCaps> caps = WatchdogCaps::NONE |
                                       WatchdogCaps::SOFT_RESET |
                                       WatchdogCaps::INT |
                                       WatchdogCaps::RECONFIGURABLE |
                                       WatchdogCaps::STOPPABLE |
                                       WatchdogCaps::ALL;
        (void)caps;
    });
}

test(watchdog_class) {
    WatchdogConfiguration config = {};
    WatchdogInfo info = {};

    API_COMPILE({ int ret = Watchdog.init(config); (void)ret; });
    API_COMPILE({ int ret = Watchdog.setTimeout(1234); (void)ret; });
    API_COMPILE({ int ret = Watchdog.start(); (void)ret; });
    API_COMPILE({ int ret = Watchdog.stop(); (void)ret; });
    API_COMPILE({ int ret = Watchdog.refresh(); (void)ret; });
    API_COMPILE({ int ret = Watchdog.getInfo(info); (void)ret; });

    API_COMPILE({ int ret = Watchdog.onExpired(onWatchdogExpiredHandlerFunc); (void)ret; });
    API_COMPILE({ int ret = Watchdog.onExpired(onWatchdogExpiredHandlerFunc, nullptr); (void)ret; });
    API_COMPILE({ int ret = Watchdog.onExpired([]() {}); (void)ret; });
    API_COMPILE({ int ret = Watchdog.onExpired(&Handlers::onWatchdogExpired, &watchdogHandler); (void)ret; });
    API_COMPILE({ int ret = Watchdog.onExpired(std::bind(&Handlers::onWatchdogExpired, &watchdogHandler)); (void)ret; });
}

#endif
