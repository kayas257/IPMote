/*
Copyright (c) 2015, Dust Networks. All rights reserved.

Port of the uart module to the MSP430FR4133.

On the MSP430FR4133 LaunchPad, we connect the SmartMesh IP device's serial
API to UCA0 using:
- P1.0 (UCA0TXD)
- P1.1 (UCA0RXD)

\license See attached DN_LICENSE.txt.
*/


#include "dn_uart.h"

//=========================== variables =======================================

typedef struct {
   dn_uart_rxByte_cbt   ipmt_uart_rxByte_cb;
} dn_uart_vars_t;

dn_uart_vars_t dn_uart_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================
extern UART_HandleTypeDef huart4;
void dn_uart_init(dn_uart_rxByte_cbt rxByte_cb){
	   dn_uart_vars.ipmt_uart_rxByte_cb = rxByte_cb;
	   huart4.Instance = UART4;
	     huart4.Init.BaudRate = 115200;
	     huart4.Init.WordLength = UART_WORDLENGTH_8B;
	     huart4.Init.StopBits = UART_STOPBITS_1;
	     huart4.Init.Parity = UART_PARITY_NONE;
	     huart4.Init.Mode = UART_MODE_TX_RX;
	     huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	     huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	     huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	     huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	     if (HAL_UART_Init(&huart4) != HAL_OK)
	     {
	       _Error_Handler(__FILE__, __LINE__);
	     }
	     __HAL_UART_ENABLE_IT(&huart4,UART_IT_RXNE);
            // enable USCI_A0 RX interrupt
}

void dn_uart_txByte(uint8_t byte){
	 HAL_UART_Transmit(&huart4, (uint8_t *)&byte, 1, 100);                       // send byte
}

void dn_uart_txFlush(){
   // nothing to do since MSP430 driver is byte-oriented
}

//=========================== interrupt handlers ==============================
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */
	uint8_t UART3_Data;
  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */
  HAL_UART_Receive_IT(&huart4, (uint8_t*) &UART3_Data, 1);
  dn_uart_vars.ipmt_uart_rxByte_cb(UART3_Data);
  /* USER CODE END UART4_IRQn 1 */
}
/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uint8_t UART3_Data;
	  if(huart->Instance==USART3)
	  {


		  dn_uart_vars.ipmt_uart_rxByte_cb(UART3_Data);

	  }
}

*/
