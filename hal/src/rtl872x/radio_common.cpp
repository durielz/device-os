/*
 * Copyright (c) 2019 Particle Industries, Inc.  All rights reserved.
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

#include "radio_common.h"

#include "dct.h"
#include "gpio_hal.h"
#include "system_error.h"
#include "check.h"

namespace particle {

namespace {

#if HAL_PLATFORM_RADIO_ANTENNA_INTERNAL
const auto DEFAULT_ANTENNA = RADIO_ANT_INTERNAL;
#elif HAL_PLATFORM_RADIO_ANTENNA_EXTERNAL
const auto DEFAULT_ANTENNA = RADIO_ANT_EXTERNAL;
#else
// #error "Unsupported platform"
const auto DEFAULT_ANTENNA = RADIO_ANT_INTERNAL;
#endif

radio_antenna_type currAntenna = DEFAULT_ANTENNA;

#if HAL_PLATFORM_RADIO_ANTENNA_INTERNAL

int selectInternalAntenna() {
    hal_gpio_mode(ANTSW, OUTPUT);
    hal_gpio_write(ANTSW, 0);
    currAntenna = RADIO_ANT_INTERNAL;
    return SYSTEM_ERROR_NONE;
}

#endif // HAL_PLATFORM_RADIO_ANTENNA_INTERNAL

#if HAL_PLATFORM_RADIO_ANTENNA_EXTERNAL

int selectExtenalAntenna() {
    hal_gpio_mode(ANTSW, OUTPUT);
    hal_gpio_write(ANTSW, 1);
    currAntenna = RADIO_ANT_EXTERNAL;
    return SYSTEM_ERROR_NONE;
}

#endif // HAL_PLATFORM_RADIO_ANTENNA_EXTERNAL

int selectAntenna(radio_antenna_type antenna) {
    if (antenna == RADIO_ANT_DEFAULT) {
        antenna = DEFAULT_ANTENNA;
    }
    switch (antenna) {
#if HAL_PLATFORM_RADIO_ANTENNA_INTERNAL
    case RADIO_ANT_INTERNAL: {
        CHECK(selectInternalAntenna());
        LOG(TRACE, "Using internal BLE antenna");
        break;
    }
#endif
#if HAL_PLATFORM_RADIO_ANTENNA_EXTERNAL
    case RADIO_ANT_EXTERNAL: {
        CHECK(selectExtenalAntenna());
        LOG(TRACE, "Using external BLE antenna");
        break;
    }
#endif
    default:
        return SYSTEM_ERROR_NOT_SUPPORTED;
    }
    return SYSTEM_ERROR_NONE;
}

} // namespace

int initRadioAntenna() {
    auto antenna = RADIO_ANT_DEFAULT;
    uint8_t dctAntenna = 0xff;
    if (dct_read_app_data_copy(DCT_RADIO_ANTENNA_OFFSET, &dctAntenna, DCT_RADIO_ANTENNA_SIZE) < 0) {
        LOG(ERROR, "Unable to load antenna settings");
        // Use the default antenna
    } else if (dctAntenna == RADIO_ANT_INTERNAL || dctAntenna == RADIO_ANT_EXTERNAL) {
        antenna = (radio_antenna_type)dctAntenna;
    }
    CHECK(selectAntenna(antenna));
    return SYSTEM_ERROR_NONE;
}

int disableRadioAntenna() {
    hal_gpio_mode(ANTSW, PIN_MODE_NONE);
    return SYSTEM_ERROR_NONE;
}

int enableRadioAntenna() {
    CHECK(selectAntenna(currAntenna));
    return SYSTEM_ERROR_NONE;
}

int selectRadioAntenna(radio_antenna_type antenna) {
    CHECK(selectAntenna(antenna));
    uint8_t dctAntenna = 0xff;
    if (antenna != RADIO_ANT_DEFAULT) {
        dctAntenna = antenna;
    }
    if (dct_write_app_data(&dctAntenna, DCT_RADIO_ANTENNA_OFFSET, DCT_RADIO_ANTENNA_SIZE) < 0) {
        LOG(ERROR, "Unable to save antenna settings");
        return SYSTEM_ERROR_UNKNOWN;
    }
    return SYSTEM_ERROR_NONE;
}

} // namespace particle
