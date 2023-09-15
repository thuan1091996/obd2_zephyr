#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pwm.h>
#include "common.h"
#include "zephyr/drivers/gpio.h"
#include "zephyr/logging/log.h"
#include "../hal.h"
#define MODULE_NAME			        hal_pwm
#define MODULE_LOG_LEVEL	        LOG_LEVEL_ERR
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

#define PWM_DT_SPEC(node_id, prop, idx) PWM_DT_SPEC_GET_BY_IDX(node_id, idx),

static struct pwm_dt_spec pwm0_device[] = {
        DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), pwms, PWM_DT_SPEC)
    } ;



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

static const struct device *gpio0_device = DEVICE_DT_GET(DT_NODELABEL(gpio0));
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

#include <zephyr/drivers/adc.h>

#define ADC_TOTAL_CHANNELS              ARRAY_SIZE(adc_channels)
#define ADC_DT_SPEC(node_id, prop, idx) ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

static const struct adc_dt_spec adc_channels[] = {
        DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, ADC_DT_SPEC) 
    };



int __InitADC(void)
{
	/* Configure channels individually prior to sampling. */
    for(uint8_t idx=0; idx < ARRAY_SIZE(adc_channels); idx++)
    {
        if (!device_is_ready(adc_channels[idx].dev))
        {
            LOG_ERR("ADC channel %d is not ready", idx);
            return FAILURE;
        }
    }
    LOG_INF("Total ADC channels: %d", ADC_TOTAL_CHANNELS);
    return SUCCESS;
}

int hal__ADCRead(uint8_t channel_num)
{
    param_check( (channel_num >= 0) && (channel_num <ADC_TOTAL_CHANNELS));
    if( adc_channel_setup_dt(&adc_channels[channel_num]) != 0)
    {
        LOG_ERR("Failed to setup ADC channel %d", channel_num);
        return FAILURE;
    }
    int16_t raw_adc = 0;
    struct adc_sequence sequence = {
        .buffer      = &raw_adc,
        .buffer_size = sizeof(raw_adc),
        .channels = BIT(channel_num),
        .resolution = adc_channels[channel_num].resolution,
        .oversampling = adc_channels[channel_num].oversampling,
    };
    int err = adc_read(adc_channels[channel_num].dev, &sequence);
    if (err != 0) {
        LOG_ERR("ADC channel %d read failed with error %d", channel_num, err);
        return FAILURE;
    }
    LOG_INF("ADC channel %d read success with value %d", channel_num, raw_adc);
    return raw_adc;
}

int hal__ADCReadMV(uint8_t channel_num)
{
    int adc_mv = hal__ADCRead(channel_num);
    if(adc_mv < 0)
    {
        LOG_ERR("Read ADC channel %d, reading value %d", channel_num, adc_mv);
    }
    int status = adc_raw_to_millivolts_dt(&adc_channels[channel_num], &adc_mv);
    if(status != 0)
    {
        LOG_ERR("Failed to convert ADC channel %d raw value to mV", channel_num);
        return FAILURE;
    }
    LOG_INF("ADC channel %d: %d (mV)", channel_num, adc_mv);
    return adc_mv;
}

