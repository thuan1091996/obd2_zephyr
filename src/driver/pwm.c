#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pwm.h>
#include "zephyr/drivers/gpio.h"
#include "zephyr/logging/log.h"
#include "common.h"

#define MODULE_NAME			        hal_pwm
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

#define PWM_DT_SPEC(n, p, i) PWM_DT_SPEC_GET_BY_IDX(n, i),

static struct pwm_dt_spec pwm0_device[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(pwm0_pins), pwms, PWM_DT_SPEC)} ;


int __InitPWM()
{
    for(uint8_t idx=0; idx < ARRAY_SIZE(pwm0_device); idx++)
    {
        if (device_is_ready(pwm0_device[idx].dev))
        {
            LOG_INF("PWM0 channel %d is ready", idx);
        }
        else
        {
            LOG_ERR("PWM0 channel %d is not ready", idx);
            return FAILURE;
        }
    }
    LOG_INF("Init PWM0 success");
    return SUCCESS;
}

int hal__setDutyCycle(uint8_t channel_num, uint16_t dutyCycle_tenth)
{
    param_check( (channel_num >= 0) && (channel_num <4));
    param_check(dutyCycle_tenth <= 1000 && dutyCycle_tenth >= 0);
    uint64_t pulse_dt = (uint64_t)pwm0_device[channel_num].period * dutyCycle_tenth / 1000;
    pwm_set_pulse_dt(&pwm0_device[channel_num], pulse_dt);
    return 0;
}

