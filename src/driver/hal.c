/*******************************************************************************
* Title                 :   HAL for nRF52
* Filename              :   hal.c
* Origin Date           :   2023/09/20
* Version               :   0.0.0
* Compiler              :   Zephyr 2.7
* Target                :   nRF52832
* Notes                 :   None
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "hal.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#include "logging/log.h"
#define MODULE_NAME			        hal
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
uint8_t io_input_pins[] = {3, 4, 5, 12, 13, 16};

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
int __InitUART()
{
    // Init UART0
    if(hal__UARTInit(0) != SUCCESS)
    {
        LOG_ERR("UART0 init failed");
        return FAILURE;
    }
    return SUCCESS;
}

int __InitI2C()
{
    // Init I2C0
    if(hal__I2CINIT(0) != SUCCESS)
    {
        LOG_ERR("I2C0 init failed");
        return FAILURE;
    }
    return SUCCESS;
}

int hal__init(void)
{
    // Init input pins
    if (__InitGPIO() != SUCCESS)
    {
        LOG_ERR("GPIO init failed");
        return FAILURE;
    }
    for (int i = 0; i < ARRAY_SIZE(io_input_pins); i++)
    {
        if ( hal__setState(io_input_pins[i], 0) != SUCCESS)
        {
            LOG_ERR("Setting pin %d as input failed", io_input_pins[i]);
            return FAILURE;
        }      
    }

    // Init ADC
    if (__InitADC() != SUCCESS)
    {
        LOG_ERR("ADC init failed");
        return FAILURE;
    }

    // Init UART
    if (__InitUART() != SUCCESS)
    {
        LOG_ERR("UART init failed");
        return FAILURE;
    }

    // Init I2C
    if (__InitI2C() != SUCCESS)
    {
        LOG_ERR("I2C init failed");
        return FAILURE;
    }

    // Init PWM
    if (__InitPWM() != SUCCESS)
    {
        LOG_ERR("PWM init failed");
        return FAILURE;
    }

}