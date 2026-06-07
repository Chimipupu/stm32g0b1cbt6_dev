/**
 * @file app_uart_cmd.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリ: UARTコマンド
 * @version 0.1
 * @date 2026-06-07
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */

#include "app_uart_cmd.h"

// --------------------------------------------------------------------------
E_APP_UART_CMD_RESULT _cmd_help(void *p_args);

// 基本コマンド
static const app_uart_cmd_tbl_t s_basic_cmd_tbl[] = {
    {"help", "?", _cmd_help},
};
static const uint8_t BASIC_CMD_NUM = sizeof(s_basic_cmd_tbl) / sizeof(s_basic_cmd_tbl[0]);

static app_uart_cmd_config_t s_cmd_config;
// --------------------------------------------------------------------------
// [Static]
E_APP_UART_CMD_RESULT _cmd_help(void *p_args)
{
    uint8_t i;

    printf("Help CMD Exec\r\n");

    // 基本コマンドを表示
    printf("Basic CMD List: NO, Cmd, Short Cmd\r\n");
    for(i = 0; i < BASIC_CMD_NUM; i++)
    {
        printf("%d, %s, %s\r\n", i, s_basic_cmd_tbl[i].p_cmd_str, s_basic_cmd_tbl[i].p_cmd_str_short);
    }

    // 拡張コマンドを表示
    if(s_cmd_config.p_ext_cmd_tbl != NULL) {
        printf("Ext CMD List: NO, Cmd, Short Cmd\r\n");
        for(i = 0; i < s_cmd_config.ext_cmd_num; i++)
        {
            printf("%d, %s, %s\r\n", i, s_cmd_config.p_ext_cmd_tbl[i].p_cmd_str, s_cmd_config.p_ext_cmd_tbl[i].p_cmd_str_short);
        }
    }

    return CMD_RESULT_EXEC_OK;
}

// --------------------------------------------------------------------------
// [APP]

void app_uart_cmd_init(app_uart_cmd_config_t *p_cmd_config)
{
    // フェールセーフ #1: 引数のヌルポをチェック)
    if(p_cmd_config == NULL) {
        return;
    }

    // フェールセーフ #2: 引数のコマンドテーブルのチェック
    if((p_cmd_config->p_ext_cmd_tbl == NULL) || (p_cmd_config->ext_cmd_num == 0)) {
        return;
    }

    s_cmd_config = *p_cmd_config;
}

E_APP_UART_CMD_RESULT app_uart_cmd_exec(E_APP_UART_CMD_TYPE cmd_type, uint8_t cmd_no, void *p_cmd_args)
{
    E_APP_UART_CMD_RESULT ret = CMD_RESULT_NONE;

    // 基本コマンドの実行
    if(cmd_type == CMD_TYPE_BASIC) {
        if(cmd_no >= BASIC_CMD_NUM) {
            return CMD_RESULT_ARGS_ERROR;
        }

        if(s_basic_cmd_tbl[cmd_no].pfunc == NULL) {
            return CMD_RESULT_UNKNOWN_ERROR;
        }

        ret = s_basic_cmd_tbl[cmd_no].pfunc(p_cmd_args);
    }
    // 拡張コマンドの実行
    else if(cmd_type == CMD_TYPE_EXT) {
        // 引数のヌルポをチェック
        if(s_cmd_config.p_ext_cmd_tbl == NULL) {
            return CMD_RESULT_ARGS_ERROR;
        }

        // 拡張コマンド番号の範囲チェック
        if(cmd_no >= s_cmd_config.ext_cmd_num) {
            return CMD_RESULT_ARGS_ERROR;
        }

        // 拡張コマンドテーブルからコマンド情報を取得
        app_uart_cmd_tbl_t *p_cmd_info = &s_cmd_config.p_ext_cmd_tbl[cmd_no];

        // 拡張コマンドのコールバック関数がNULLでないかチェック
        if(p_cmd_info->pfunc == NULL) {
            return CMD_RESULT_UNKNOWN_ERROR;
        }

        // 拡張コマンドのコールバック関数を呼び出す
        ret = p_cmd_info->pfunc(p_cmd_args);
    }

    return ret;
}
// --------------------------------------------------------------------------