/*******************************************************************************
* Title                 :    
* Filename              :   
* Author                :   thuantm5
* Origin Date           :   2023/03/24
* Version               :   0.0.0
* Compiler              :   nRF Connect SDK v2.3.0
* Target                :   nRF52
* Notes                 :   None
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/

/* Zephyr include */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/logging/log.h>

/* Applications include */
#include "framework/framework-app.h"
#include "app/common.h"
#include "app/app_ble.h"
#include "app/app_uart.h"




/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
Q_DEFINE_THIS_FILE

#define MODULE_NAME			main_module
#define MODULE_LOG_LEVEL	LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void main(void)
{
	LOG_INF("BLE sample demo using %s", CONFIG_BOARD);
	uart_init();
	framework_init();
	ble_actor_ctor();
    ble_actor_start();
    QF_run(); /* run the QF application */
}

