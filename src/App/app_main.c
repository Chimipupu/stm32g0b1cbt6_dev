/**
 * @file app_main.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメイン (for STM32G0B1CBT6)
 * @version 0.1
 * @date 2026-06-07
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */

#include "app_main.h"
#include "app_uart_cmd.h"

// --------------------------------------------------------------------------
static app_uart_cmd_config_t s_cmd_config;
static uint8_t s_led_state = 0;
static void _rtc_update(void);

E_APP_UART_CMD_RESULT _cmd_dbg(void *p_args);

// 拡張コマンド
static const app_uart_cmd_tbl_t s_ext_cmd_tbl[] = {
    {"debug", "dbg", _cmd_dbg},
};
static const uint8_t EXT_CMD_NUM = sizeof(s_ext_cmd_tbl) / sizeof(s_ext_cmd_tbl[0]);

#ifdef DBG_APP
void _debug_pcb_check(void);
#endif // DBG_APP
// --------------------------------------------------------------------------
// [Static]

// printf()をUARTにポーティング
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

E_APP_UART_CMD_RESULT _cmd_dbg(void *p_args)
{
    printf("Debug CMD Exec\r\n");

    return CMD_RESULT_EXEC_OK;
}

static void _rtc_update(void)
{
    RTC_DateTypeDef sdatestructureget;
    RTC_TimeTypeDef stimestructureget;

    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);

#if 0
    static uint8_t s_prev_seconds;

    if(s_prev_seconds != stimestructureget.Seconds) {
        s_prev_seconds  = stimestructureget.Seconds;
        printf("RTC: 20%02d/%02d/%02d %02d:%02d:%02d\r\n",
                sdatestructureget.Year,sdatestructureget.Month,sdatestructureget.Date, \
                stimestructureget.Hours,stimestructureget.Minutes,stimestructureget.Seconds);
    }
#endif
}

#ifdef DBG_APP
void _debug_pcb_check(void)
{
    // 基板動作確認: Lチカ！ LEDチカチカ！
    HAL_GPIO_WritePin(PCB_LED_GPIO_Port, PCB_LED_Pin, s_led_state);
    s_led_state = !s_led_state;
    // printf("LED: %s\r\n", s_led_state ? "ON" : "OFF");
}
#endif // DBG_APP
// --------------------------------------------------------------------------
// [APP]

void app_main_init(void)
{
    // printf()をUARTにポーティング
    setbuf(stdout, NULL);
    printf("STM32G0B1CBT6 Develop by Chimipupu\r\n");

    // UARTコマンドの初期化
    s_cmd_config.p_ext_cmd_tbl = (app_uart_cmd_tbl_t *)s_ext_cmd_tbl;
    s_cmd_config.ext_cmd_num = EXT_CMD_NUM;
    app_uart_cmd_init(&s_cmd_config);
    app_uart_cmd_exec(CMD_TYPE_BASIC, 0, NULL);
    app_uart_cmd_exec(CMD_TYPE_EXT, 0, NULL);
}

void app_main(void)
{
#ifdef DBG_APP
    _debug_pcb_check(); // [DEBUG] 基板チェック
#endif // DBG_APP

    _rtc_update(); // RTC更新
}
// --------------------------------------------------------------------------