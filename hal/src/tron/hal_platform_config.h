#pragma once

#include "hal_platform_rtl8721x_config.h"
#include "platforms.h"

#define HAL_PLATFORM_NCP (1)
#define HAL_PLATFORM_USART2 (1)
#define HAL_PLATFORM_NCP_AT (0)
#define HAL_PLATFORM_CELLULAR (0)
// #define HAL_PLATFORM_CELLULAR_SERIAL (HAL_USART_SERIAL2)
#define HAL_PLATFORM_SETUP_BUTTON_UX (1)
// #define HAL_PLATFORM_MUXER_MAY_NEED_DELAY_IN_TX (1)
#define HAL_PLATFORM_SPI_NUM (2)
#define HAL_PLATFORM_I2C_NUM (2)
#define HAL_PLATFORM_USART_NUM (3)
#define HAL_PLATFORM_NCP_COUNT (1)
#define HAL_PLATFORM_BROKEN_MTU (1)
#define HAL_PLATFORM_WIFI (1)
#define HAL_PLATFORM_WIFI_COMPAT (1)

#define HAL_PLATFORM_RADIO_ANTENNA_INTERNAL (1)
#define HAL_PLATFORM_RADIO_ANTENNA_EXTERNAL (1)

// #define HAL_PLATFORM_POWER_MANAGEMENT (1)
// #define HAL_PLATFORM_PMIC_BQ24195 (1)
// #define HAL_PLATFORM_PMIC_BQ24195_FAULT_COUNT_THRESHOLD (4)
// #define HAL_PLATFORM_FUELGAUGE_MAX17043 (1)
// #define HAL_PLATFORM_PMIC_BQ24195_I2C (HAL_I2C_INTERFACE2)
// #define HAL_PLATFORM_FUELGAUGE_MAX17043_I2C (HAL_I2C_INTERFACE2)
#define HAL_PLATFORM_FLASH_MX25R6435FZNIL0 (1)

#if defined(MODULE_FUNCTION) && MODULE_FUNCTION != 2 // MOD_FUNC_BOOTLOADER
#define HAL_PLATFORM_USB_PRODUCT_STRING "P2"
#else
#define HAL_PLATFORM_USB_PRODUCT_STRING "P2 DFU Mode"
#endif // defined(MODULE_FUNCTION) && MODULE_FUNCTION != 2 // MOD_FUNC_BOOTLOADER

#define PRODUCT_SERIES "Pseries"
