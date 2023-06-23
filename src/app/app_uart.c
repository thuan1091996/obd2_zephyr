/****************************************************************************
* Title                 :    header file
* Filename              :   app_uart.h
* Author                :   thuantm5
* Origin Date           :   2023/06/23
* Version               :   v0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nRF52
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2023/06/23    v0.0.0         	thuantm5      Interface Created.
*
*****************************************************************************/

/** \file app_uart.h
 *  \brief This module contains .
 *
 *  This is the header file for 
 */
#ifndef APP_APP_UART_C_
#define APP_APP_UART_C_

/******************************************************************************
* Includes
*******************************************************************************/
/* Zephyr include */
#include <assert.h>
#include <zephyr/logging/log.h>

/* Applications include */
#include "common.h"

/* Low level driver relatives */
#include "uart_handler.h"
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/
/* Logging relative */
#define MODULE_NAME			        uart_app
#define MODULE_LOG_LEVEL	        LOG_LEVEL_INF
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

Q_DEFINE_THIS_FILE

/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
 * @brief Async UART event handler
 */
static void on_app_uart_event(struct app_uart_evt_t *evt)
{
    int status;
	switch(evt->type) {
		case APP_UART_EVT_RX:
			// Print the incoming data to the console
			// printk("RX ");
			// for(int i = 0; i < evt->data.rx.length; i++) printk("%.2x ", evt->data.rx.bytes[i]);
			// printk("\n");
            status = bt_nus_send(NULL, evt->data.rx.bytes, evt->data.rx.length);
            if (status != 0) 
            {
                LOG_ERR("Failed to notify client (err %d)\n", status);
            }
			break;

		// A UART error ocurred, such as a break or frame error condition
		case APP_UART_EVT_ERROR:
			printk("UART error: Reason %i\n", evt->data.error.reason);
			break;

		// The UART event queue has overflowed. If this happens consider increasing the UART_EVENT_QUEUE_SIZE (will increase RAM usage),
		// or increase the UART_RX_TIMEOUT_US parameter to avoid a lot of small RX packets filling up the event queue
		case APP_UART_EVT_QUEUE_OVERFLOW:
			printk("UART library error: Event queue overflow!\n");
			break;
	}
}

int uart_init(void)
{
    int err = app_uart_init(on_app_uart_event);
    if(err != 0) {
        LOG_ERR("app_uart_init failed: %i\n", err);
    }
}

#endif // APP_APP_UART_C_

/*** End of File **************************************************************/