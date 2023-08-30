/*******************************************************************************
* Title                 :    
* Filename              :   
* Author                :   ItachiVN
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
#include <zephyr/drivers/gpio.h>
/* Applications include */
#include "framework/framework-app.h"
#include "app/common.h"
#include "app/app_ble.h"
#include "app/app_uart.h"




/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define SLEEP_TIME_MS   1000

Q_DEFINE_THIS_FILE

#define MODULE_NAME			main_module
#define MODULE_LOG_LEVEL	LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define LED3_NODE DT_ALIAS(led3)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

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

	int ret;

	if (!gpio_is_ready_dt(&led)) {
		LOG_ERR	("gpio_is_ready_dt error");
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		LOG_ERR	("gpio_pin_configure_dt error");
	}

	// uart_init();
	framework_init();
	ble_actor_ctor();
    ble_actor_start();
    QF_run(); /* run the QF application */

	while (1) 
	{
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) 
		{
			LOG_ERR	("gpio_pin_toggle_dt error");
		}
		k_msleep(SLEEP_TIME_MS);
		LOG_INF("Sleep time: %d", SLEEP_TIME_MS);
	}
}

