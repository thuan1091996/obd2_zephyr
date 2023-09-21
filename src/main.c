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
#define APP_TEST_UART           (0)
#define APP_TEST_I2C            (1)


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

#if (APP_TEST_I2C != 0)
#define DS3231_I2C_ADDR 	0x68
#define DS3231_REG_CONTROL 	0x0e
#define DS3231_REG_SECOND 	0x00
#define DS3231_REG_MINUTE 	0x01
#define DS3231_REG_HOUR  	0x02
#define DS3231_REG_DOW    	0x03


uint8_t DS3231_DecodeBCD(uint8_t bin) {
	return (((bin & 0xf0) >> 4) * 10) + (bin & 0x0f);
}

/**
 * @brief Encodes a decimal number to binaty-coded decimal for storage in registers.
 * @param dec Decimal number to encode.
 * @return Encoded binary-coded decimal value.
 */
uint8_t DS3231_EncodeBCD(uint8_t dec) {
	return (dec % 10 + ((dec / 10) << 4));
}


int8_t DS3231_GetRegByte(uint8_t regAddr) {
	uint8_t val;
    if (0 != hal__I2CREAD_uint8(0, DS3231_I2C_ADDR, regAddr, &val))
    {
        LOG_ERR("I2C Read address 0x%02x failed", regAddr);
        return -1;
    }
    else
    {
        LOG_INF("I2C Read address 0x%02x: %d", regAddr, val);
    }
	return val;
}

int8_t DS3231_SetRegByte(uint8_t regAddr, uint8_t val) {
    if (0 != hal__I2CWRITE_uint8(0, DS3231_I2C_ADDR, regAddr, val))
    {
       LOG_ERR("I2C Write address 0x%02x failed", regAddr);
        return -1;
    }
    else
    {
        LOG_INF("I2C Write address 0x%02x: %d", regAddr, val);
    }
    return 0;
}

/**
 * @brief Gets the current hour in 24h format.
 * @return Hour in 24h format, 0 to 23.
 */
uint8_t DS3231_GetHour(void) {
	return DS3231_DecodeBCD(DS3231_GetRegByte(DS3231_REG_HOUR));
}

void DS3231_GetFullTime(uint8_t* hour, uint8_t* minute, uint8_t* second) {
    // Using hal__I2CREAD() to read 3 time bytes (SS:MM:HH)
    uint8_t data[3] = {0};
    if(hal__I2CREAD(0, DS3231_I2C_ADDR, DS3231_REG_SECOND, data, 3))
    // Decode the data
    *hour = DS3231_DecodeBCD(data[2]);
    *minute = DS3231_DecodeBCD(data[1]);
    *second = DS3231_DecodeBCD(data[0]);
    LOG_INF("Current time: %02d:%02d:%02d", *hour, *minute, *second);
}

void DS3231_SetFullTime(uint8_t hour, uint8_t min, uint8_t sec)
{
    // Encode the data
    uint8_t data[3] = {0};
    data[0] = DS3231_EncodeBCD(sec);
    data[1] = DS3231_EncodeBCD(min);
    data[2] = DS3231_EncodeBCD(hour);
    // Using hal__I2CWRITE() to write 3 time bytes (SS:MM:HH)
    if(hal__I2CWRITE(0, DS3231_I2C_ADDR, DS3231_REG_SECOND, data, 3) != 3)
    {
        LOG_ERR("Failed to set time");
    }
    else
    {
        LOG_INF("Set time: %02d:%02d:%02d", hour, min, sec);
    }
}

/**
 * @brief Set the current hour, in 24h format.
 * @param hour_24mode Hour in 24h format, 0 to 23.
 */
void DS3231_SetHour(uint8_t hour_24mode) {
	DS3231_SetRegByte(DS3231_REG_HOUR, DS3231_EncodeBCD(hour_24mode & 0x3f));
}

void i2c_custom_task(void *pvParameters)
{
    uint8_t data = 0;
    uint8_t i2c_num = 0;
    uint8_t addr = 0x68;
    uint8_t reg = 0x00;
    int test_hour = 1;

    while(1)
    {

        //Test I2C APIs with RTC RS3231
        if( true != hal__I2CEXISTS(0, 0x68))
        {
            LOG_ERR("I2C device does not exist");
        }
        else
        {
            LOG_INF("I2C device exists");
        }
        k_msleep(1000);

        // Get/Set/Get hour
        uint8_t cur_hour = DS3231_GetHour();
        LOG_INF("Current hour: %d", cur_hour);
        k_msleep(1000);

        DS3231_SetHour(test_hour++ + 1);
        k_msleep(1000);

        cur_hour = DS3231_GetHour();
        LOG_INF("Current hour: %d", cur_hour);
        k_msleep(1000);

        // Set full time use
        uint8_t hour = 10;
        uint8_t minute = 11;
        uint8_t second = 12;
        DS3231_SetFullTime(hour, minute, second);
        for(uint8_t count=0; count<20; count++)
        {
            DS3231_GetFullTime(&hour, &minute, &second);
            k_msleep(1000);
        }
    }
}
#endif /* End of (APP_TEST_I2C == 1) */


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

#if (APP_TEST_I2C != 0)
    i2c_custom_task(NULL);
#endif /* End of (APP_TEST_I2C != 0) */

    while (1)
    {
        k_msleep(300);
    }
}