#include <string.h>
#include <stddef.h>
#include <zephyr/types.h>


#include "hal.h"
#include "logging/log.h"
#define MODULE_NAME			        main
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);


#define APP_TEST_BLE            (0)
#define APP_TEST_PWM            (0)
#define APP_TEST_ADC            (0)
#define APP_TEST_GPIO           (0)
#define APP_TEST_UART           (1)


#if (APP_TEST_BLE != 0)
#include "ble\ble_gatt.h"
#include "ble\bluetoothle.h"

void* char1_read_cb(void* p_data, void* p_len)
{
    static char default_char_value[]="CHAR1_PAYLOAD";
    *(int*)p_len = strlen(default_char_value);
    return default_char_value;
}

void* char2_read_cb(void* p_data, void* p_len)
{
    static char default_char_value[]="CHAR2_PAYLOAD";
    *(int*)p_len = strlen(default_char_value);
    return default_char_value;
}

void* char3_read_cb(void* p_data, void* p_len)
{
    static char default_char_value[]="CHAR3_PAYLOAD";
    *(int*)p_len = strlen(default_char_value);
    return default_char_value;
}

void* char4_read_cb(void* p_data, void* p_len)
{
    static char default_char_value[]="CHAR4_PAYLOAD";
    *(int*)p_len = strlen(default_char_value);
    return default_char_value;
}

int char5_write_cb(void* p_data, void* p_len)
{
    // Dump receive data
    LOG_INF("char5_write_cb, recv %dB", *(int*)p_len);
    LOG_HEXDUMP_INF(p_data, *(int*)p_len, "raw_payload: ");
    return 0;
}

ble_custom_gatt_cb_t custom_service_app =
{
    .custom_char1_read_cb = &char1_read_cb,
    .custom_char2_read_cb = &char2_read_cb,
    .custom_char3_read_cb = &char3_read_cb,
    .custom_char4_read_cb = &char4_read_cb,
    .custom_char5_write_cb = &char5_write_cb,
};

void ble_test(void)
{
    LOG_INF("============== BLE testing =====================");
    ble_init(NULL);
    ble_custom_service_init(&custom_service_app);

    ble_set_adv_name("ITACHI-BLE");
    ble_adv_start();
    while(1)
    {
        k_msleep(500);
        char3_send_notify("HELLO THERE, IM 3", strlen("HELLO THERE, IM 3"));
        k_msleep(500);
        char4_send_notify("HELLO THERE, IM 4", strlen("HELLO THERE, IM 4"));
    }
    LOG_INF("============== BLE testing =====================");
}
#endif /* End of (APP_TEST_BLE != 0) */

#if (APP_TEST_PWM != 0)
int __InitPWM();
int hal__setDutyCycle(uint8_t channel_num, uint16_t dutyCycle_tenth);

void pwm_custom_task(void *pvParameters)
{
    __InitPWM();
    while(1)
    {
        // Test PWM APIS with max duty = 1000%
        // Increase the duty cycle of each pin 50% every 300ms
        for(uint16_t duty = 0; duty <= 1000; duty += 100)
        {
            hal__setDutyCycle(0, duty);
            hal__setDutyCycle(1, duty);
            hal__setDutyCycle(2, duty);
            hal__setDutyCycle(3, duty);
            k_msleep(500);
            if(duty == 1000)
            {
                k_msleep(2000);
                duty = 0;
            }
            else if(duty == 0)
                k_msleep(2000);
        }
    }
}
#endif /* End of (APP_TEST_PWM != 0) */

#if (APP_TEST_ADC != 0)
void adc_custom_task(void *pvParameters)
{
    // Read all ADC channels
    if(__InitADC() != 0)
    {
        LOG_ERR("Failed to init ADC");
    }

    while(1)
    {
        int adc_raw[8]={0};
        int adc_voltage[8]={0};
        for(uint8_t idx=0; idx < 8; idx++)
        {
            adc_voltage[idx] = hal__ADCReadMV(idx);
            if(adc_voltage[idx] == FAILURE)
            {
                LOG_ERR("Failed to read ADC channel %d", idx);
                continue;
            }
            else if(adc_voltage[idx] < 0)
            {
                LOG_WRN("ADC channel %d read value %d", idx, adc_voltage[idx]);
            }
            else
            {
                // Normal
            }
        }
        // RAW: [%d %d %d %d %d %d %d %d]\n", adc_raw[0], adc_raw[1], adc_raw[2], adc_raw[3], adc_raw[4], adc_raw[5], adc_raw[6], adc_raw[7]
        LOG_INF("ADC: [%d %d %d %d %d %d %d %d]\n", adc_voltage[0], adc_voltage[1], adc_voltage[2], adc_voltage[3], adc_voltage[4], adc_voltage[5], adc_voltage[6], adc_voltage[7]);
        // mV: [%d %d %d %d %d %d %d %d]\n", adc_voltage[0], adc_voltage[1], adc_voltage[2], adc_voltage[3], adc_voltage[4], adc_voltage[5], adc_voltage[6], adc_voltage[7]
        k_msleep(1000);
    }
}
#endif /* End of (APP_TEST_ADC != 0) */

#if (APP_TEST_GPIO!= 0)
uint8_t io_test_pins[]={13, 11, 2, 15};
void gpio_custom__task(void *pvParameters)
{
    // Test pin as outputs
    for(uint8_t idx=0; idx < ARRAY_SIZE(io_test_pins); idx++)
    {
        hal__setState(io_test_pins[idx], 1);
    }

    for(uint8_t idx=0; idx < ARRAY_SIZE(io_test_pins); idx++)
    {
        hal__setLow(io_test_pins[idx]);
        k_msleep(1000);
    }

    // Turn on each pin for 1 second and then of 1 every 1 sec
    for(uint8_t idx=0; idx < ARRAY_SIZE(io_test_pins); idx++)
    {
        hal__setHigh(io_test_pins[idx]);
        k_msleep(1000);
    }

    // Set test pins as input
    for(uint8_t idx=0; idx < ARRAY_SIZE(io_test_pins) - 1; idx++)
    {
        hal__setState(io_test_pins[idx], 0);
    }
    hal__setState(io_test_pins[ARRAY_SIZE(io_test_pins)-1], 2);

    while(1)
    {
        LOG_INF("============== GPIO input =====================");
        // Read pin state
        for(uint8_t idx=0; idx < ARRAY_SIZE(io_test_pins); idx++)
        {
            LOG_INF("Pin %d state: %d", io_test_pins[idx], hal__read(io_test_pins[idx]));
        }
        k_msleep(1000);
        LOG_INF("============== GPIO input =====================\r\n ");
    }
}
#endif /* End of (APP_TEST_GPIO!= 0) */

#if (APP_TEST_UART != 0)
void uart_custom_task(void* p_param)
{
    int status = __InitUART();
    if(status != 0)
    {
        LOG_ERR("Failed to init UART with error code %d", status);
        return;
    }
    while(1)
    {
        int size = hal__UARTAvailable(0);
        if(size > 0)
        {
            uint8_t data[128]={0};
            hal__UARTRead(0, data, size);
            LOG_INF("UART0 recv %dB", size);
            // Echo back
            hal__UARTWrite(0, data, size);
            LOG_HEXDUMP_INF(data, size, "UART0 recv: ");
        }

        size = hal__UARTAvailable(1);
        if(size > 0)
        {
            uint8_t data[128]={0};
            hal__UARTRead(1, data, size);
            LOG_INF("UART1 recv %dB", size);
            // Echo back
            hal__UARTWrite(1, data, size);
            LOG_HEXDUMP_INF(data, size, "UART1 recv: ");
        }
        k_msleep(1000);
    }
}
#endif /* End of (APP_TEST_UART != 0) */

void main(void)
{
    if (SUCCESS != hal__init())
    {
        LOG_ERR("Failed to init HAL");
        return;
    }
#if (APP_TEST_BLE != 0)
    ble_test();
#endif /* End of (APP_TEST_BLE != 0) */

#if (APP_TEST_GPIO != 0)
    gpio_custom__task(NULL);
#endif /* End of (APP_TEST_GPIO != 0) */

#if (APP_TEST_PWM != 0)
    pwm_custom_task(NULL);
#endif /* End of (APP_TEST_PWM != 0) */

#if (APP_TEST_ADC != 0 )
    adc_custom_task(NULL);
#endif /* End of (APP_TEST_ADC != 0 ) */

#if (APP_TEST_UART != 0)
    uart_custom_task(NULL);
#endif /* End of (APP_TEST_UART != 0) */

    while (1)
    {
        k_msleep(300);
    }
}