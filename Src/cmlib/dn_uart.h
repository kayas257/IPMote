/*
Copyright (c) 2014, Dust Networks. All rights reserved.

\license See attached DN_LICENSE.txt.
*/

#ifndef DN_UART_H
#define DN_UART_H

#include "dn_common.h"
#include "main.h"
#include "stm32f3xx_hal.h"

//=========================== defined =========================================

//=========================== typedef =========================================

typedef void (*dn_uart_rxByte_cbt)(uint8_t byte);

//=========================== variables =======================================

//=========================== prototypes ======================================

#ifdef __cplusplus
 extern "C" {
#endif

void dn_uart_init(dn_uart_rxByte_cbt rxByte_cb);
void dn_uart_txByte(uint8_t byte);
void dn_uart_txFlush();
UART_HandleTypeDef huart4;
#ifdef __cplusplus
}
#endif

#endif
