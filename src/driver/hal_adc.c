/*******************************************************************************
* Title                 :   ADC HAL for ZephyrRTOS
* Filename              :   hal_adc.c
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

/** \file hal_adc.c
 *  \brief This module contains the hardware abstraction layer for ADC.
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "hal.h"
#include <devicetree.h>
#include <drivers/adc.h>
#include "logging/log.h"

#define MODULE_NAME			        hal_adc
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define ADC_TOTAL_CHANNELS              ARRAY_SIZE(adc_channels)
#define ADC_DT_SPEC(node_id, prop, idx) ADC_DT_SPEC_GET_BY_IDX(node_id, idx),


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
// static const struct adc_dt_spec adc_channels[] = {
//         DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, ADC_DT_SPEC) 
//     };
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

// int __InitADC(void)
// {
// 	/* Configure channels individually prior to sampling. */
//     for(uint8_t idx=0; idx < ARRAY_SIZE(adc_channels); idx++)
//     {
//         if (!device_is_ready(adc_channels[idx].dev))
//         {
//             LOG_ERR("ADC channel %d is not ready", idx);
//             return FAILURE;
//         }
//     }
//     LOG_INF("Total ADC channels: %d", ADC_TOTAL_CHANNELS);
//     return SUCCESS;
// }

// int hal__ADCRead(uint8_t channel_num)
// {
//     param_check( (channel_num >= 0) && (channel_num <ADC_TOTAL_CHANNELS));
//     if( adc_channel_setup_dt(&adc_channels[channel_num]) != 0)
//     {
//         LOG_ERR("Failed to setup ADC channel %d", channel_num);
//         return FAILURE;
//     }
//     int16_t raw_adc = 0;
//     struct adc_sequence sequence = {
//         .buffer      = &raw_adc,
//         .buffer_size = sizeof(raw_adc),
//         .channels = BIT(channel_num),
//         .resolution = adc_channels[channel_num].resolution,
//         .oversampling = adc_channels[channel_num].oversampling,
//     };
//     int err = adc_read(adc_channels[channel_num].dev, &sequence);
//     if (err != 0) {
//         LOG_ERR("ADC channel %d read failed with error %d", channel_num, err);
//         return FAILURE;
//     }
//     LOG_INF("ADC channel %d read success with value %d", channel_num, raw_adc);
//     return raw_adc;
// }

// int hal__ADCReadMV(uint8_t channel_num)
// {
//     int adc_mv = hal__ADCRead(channel_num);
//     if(adc_mv < 0)
//     {
//         LOG_ERR("Read ADC channel %d, reading value %d", channel_num, adc_mv);
//     }
//     int status = adc_raw_to_millivolts_dt(&adc_channels[channel_num], &adc_mv);
//     if(status != 0)
//     {
//         LOG_ERR("Failed to convert ADC channel %d raw value to mV", channel_num);
//         return FAILURE;
//     }
//     LOG_INF("ADC channel %d: %d (mV)", channel_num, adc_mv);
//     return adc_mv;
// }