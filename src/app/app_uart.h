/****************************************************************************
* Title                 :   header file
* Filename              :   app_uart.h
* Author                :   ItachiVN
* Origin Date           :   2023/06/23
* Version               :   v0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nRF52
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2023/06/23    v0.0.0         	ItachiVN      Interface Created.
*
*****************************************************************************/

/** \file app_uart.h
 *  \brief This module contains .
 *
 *  This is the header file for 
 */
#ifndef APP_APP_UART_H_
#define APP_APP_UART_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <zephyr/kernel.h>
#include "common.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
int uart_init(void);
int uart_send(const uint8_t* p_data, uint16_t len, uint32_t timeout);

#endif // APP_APP_UART_H_

/*** End of File **************************************************************/