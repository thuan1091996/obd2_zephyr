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
#define CPS_NODE DT_ALIAS(led2)
#define CHL_NODE DT_ALIAS(led3)

static const struct gpio_dt_spec cps_pin = GPIO_DT_SPEC_GET(CPS_NODE, gpios);
static const struct gpio_dt_spec chl_pin = GPIO_DT_SPEC_GET(CHL_NODE, gpios);

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
	LOG_INF("-------------> System started <-------------");

	int ret;

	if (!gpio_is_ready_dt(&cps_pin)) {
		LOG_ERR	("CPS gpio_is_ready_dt() error");
	}

	if (!gpio_is_ready_dt(&chl_pin)) {
		LOG_ERR	("CHL gpio_is_ready_dt() error");
	}

	ret = gpio_pin_configure_dt(&cps_pin, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		LOG_ERR	("CPS gpio_pin_configure_dt() error");
	}

	ret = gpio_pin_configure_dt(&chl_pin, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		LOG_ERR	("CHL gpio_pin_configure_dt() error");
	}

	// cps-gpios = <&gpio0 6 GPIO_ACTIVE_HIGH>; => MUST BE LOW
	// chl-gpios = <&gpio0 8 GPIO_ACTIVE_HIGH>; => MUST BE HIGH
	gpio_pin_set_dt(&cps_pin, 0);
	gpio_pin_set_dt(&chl_pin, 1);

	framework_init();
	ble_actor_ctor();
    ble_actor_start();
    QF_run(); /* run the QF application */
}

