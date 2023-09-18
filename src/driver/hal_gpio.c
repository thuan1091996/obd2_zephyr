/*******************************************************************************
* Title                 :   HAL GPIO ZephyrRTOS
* Filename              :   hal_gpio.c
* Origin Date           :   2023/09/15
* Version               :   0.0.0
* Compiler              :   Zephyr 2.7
* Target                :   nRF52 
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "hal.h"

#include <devicetree.h>
#include <drivers/gpio.h>
#include "logging/log.h"

#define MODULE_NAME			        hal_gpio
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static const struct device *gpio0_device = DEVICE_DT_GET(DT_NODELABEL(gpio0));

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
int __InitGPIO(void)
{
    if (device_is_ready(gpio0_device))
    {
        LOG_INF("GPIO0 is ready");
    }
    else
    {
        LOG_ERR("GPIO0 is not ready");
        return FAILURE;
    }
    LOG_INF("Init GPIO0 success");
    return SUCCESS;
}

// (0,0) sets pin 0 as input, 
// (0,1) sets pin 0 as output, 
// (0,2) sets pin as high impedance. 
// Returns 0 on success, -1 on failure.
int hal__setState(uint8_t pinNum, uint8_t state)
{
    param_check( (pinNum >= 0) && (pinNum <32));
    param_check( (state >= 0) && (state <3));
    gpio_flags_t pin_flags;
    if(state == 0)
        pin_flags = GPIO_INPUT;
    else if(state == 1)
        pin_flags = GPIO_OUTPUT;
    else if(state == 2)
        pin_flags = GPIO_DISCONNECTED;
    else
    {
        LOG_ERR("%d is an invalid pin state", state);
        return FAILURE;
    }
    if(gpio_pin_configure(gpio0_device, pinNum, pin_flags) != 0)
    {
        LOG_ERR("Failed to set pin %d to state %d", pinNum, state);
        return FAILURE;
    }
    return SUCCESS;
}

int hal__setHigh(uint8_t pinNum)
{
    param_check( (pinNum >= 0) && (pinNum <32));
    int status = gpio_pin_set_raw(gpio0_device, pinNum, 1);
    if(status != 0)
    {
        LOG_ERR("Failed to set pin %d to high with error %d", pinNum, status);
        return FAILURE;
    }
    return SUCCESS;
}

int hal__setLow(uint8_t pinNum)
{
    param_check( (pinNum >= 0) && (pinNum <32));
    int status = gpio_pin_set_raw(gpio0_device, pinNum, 0);
    if(status != 0)
    {
        LOG_ERR("Failed to set pin %d to low with error %d", pinNum, status);
        return FAILURE;
    }
    return SUCCESS;
}

int hal__read(uint8_t pinNum)
{
    param_check( (pinNum >= 0) && (pinNum <32));
    int status = gpio_pin_get_raw(gpio0_device, pinNum);
    if(status < 0)
    {
        LOG_ERR("Failed to read pin %d with error %d", pinNum, status);
        return FAILURE;
    }
    return status;
}