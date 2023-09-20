/*******************************************************************************
* Title                 :   HAL for PWM
* Filename              :   hal_pwm.c
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

/** \file hal_pwm.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/

#include <device.h>
#include <devicetree.h>
#include <drivers/pwm.h>
#include <zephyr.h>


#include "hal.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#include "logging/log.h"
#define MODULE_NAME			        hal_pwm
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

#define PWM0_DEFAULT_FLAGS          0
#define PWM0_NUMB_CHANNELS	        4	
#define PWM0_DEFAULT_PERIOD_USEC	1000 // 1KHz
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


static const struct device *pwm0_device[] = {
	DEVICE_DT_GET(DT_PWMS_CTLR_BY_IDX(DT_NODELABEL(pwm_led0), 0)),
	DEVICE_DT_GET(DT_PWMS_CTLR_BY_IDX(DT_NODELABEL(pwm_led0), 1)),
	DEVICE_DT_GET(DT_PWMS_CTLR_BY_IDX(DT_NODELABEL(pwm_led0), 2)),
	DEVICE_DT_GET(DT_PWMS_CTLR_BY_IDX(DT_NODELABEL(pwm_led0), 3)),
};
uint8_t pwm0_channel[] = {
	DT_PWMS_CHANNEL_BY_IDX(DT_NODELABEL(pwm_led0), 0),
	DT_PWMS_CHANNEL_BY_IDX(DT_NODELABEL(pwm_led0), 1),
	DT_PWMS_CHANNEL_BY_IDX(DT_NODELABEL(pwm_led0), 2),
	DT_PWMS_CHANNEL_BY_IDX(DT_NODELABEL(pwm_led0), 3),
};

uint16_t pwm0_period[PWM0_NUMB_CHANNELS] = {0};

/******************************************************************************
* Functions definitions
*******************************************************************************/
/**
 * @brief Set the period of a PWM channel
 * @param channel_num The channel number [0, 3]
 * @param period_usec The period in microseconds
 * @return 0 if successful, otherwise a (negative) error code.
 */
int __hal__setPeriod(uint8_t channel_num, uint16_t period_usec)
{
	param_check((channel_num >= 0) && (channel_num < 4));
	int status = pwm_pin_set_usec(pwm0_device[channel_num], pwm0_channel[channel_num], period_usec, 0, PWM0_DEFAULT_FLAGS);
	if (status != SUCCESS)
	{
		LOG_ERR("Error: Failed to set period to %d (us) for channel %d\n", period_usec, channel_num);
		return status;
	}
	pwm0_period[channel_num] = period_usec;
	return SUCCESS;
}

int __InitPWM()
{
	for (uint8_t idx = 0; idx < ARRAY_SIZE(pwm0_device); idx++)
	{
		if (!device_is_ready(pwm0_device[idx]))
		{
			LOG_ERR("Error: PWM device %s is not ready\n", pwm0_device[idx]->name);
			return FAILURE;
		}
	}
	LOG_INF("Init PWM0 success");
	return SUCCESS;
}



int hal__setDutyCycle(uint8_t channel_num, uint16_t dutyCycle_tenth)
{
	param_check((channel_num >= 0) && (channel_num < 4));
	param_check(dutyCycle_tenth <= 1000 && dutyCycle_tenth >= 0);

	//Check if period is set (non-zero)
	if(pwm0_period[channel_num] == 0)
	{
		int status = __hal__setPeriod(channel_num, PWM0_DEFAULT_PERIOD_USEC);
		if (status != SUCCESS)
		{
			LOG_ERR("Error: Failed to set period to %d (us) for channel %d\n", PWM0_DEFAULT_PERIOD_USEC, channel_num);
			return FAILURE;
		}
	}
	return pwm_pin_set_usec(pwm0_device[channel_num], pwm0_channel[channel_num], pwm0_period[channel_num], pwm0_period[channel_num] * dutyCycle_tenth / 1000, PWM0_DEFAULT_FLAGS);
}