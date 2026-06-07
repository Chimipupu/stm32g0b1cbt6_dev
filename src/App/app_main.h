/**
 * @file app_main.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメイン (for STM32G0B1CBT6)
 * @version 0.1
 * @date 2026-06-07
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */

#ifndef APP_MAIN_H
#define APP_MAIN_H

// C Std Lib
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
// #include <math.h>

// ST Lib
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "usart.h"
#include "rtc.h"
#include "spi.h"
#include "gpio.h"

// MyApp Lib

// --------------------------------------------------------------------------
void app_main_init(void);
void app_main(void);

// --------------------------------------------------------------------------

#endif // APP_MAIN_H