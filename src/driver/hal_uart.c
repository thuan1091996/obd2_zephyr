/*******************************************************************************
* Title                 :   HAL UART for nRF52
* Filename              :   hal_uart.c
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

/** \file hal_uart.c
 *  \brief This module contains the source code for UART HAL driver
 */
/******************************************************************************
* Includes
*******************************************************************************/

#include "hal.h"
#include <devicetree.h>
#include <drivers/uart.h>
#include "sys/ring_buffer.h"

#include "logging/log.h"
#define MODULE_NAME			        hal_uart
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define UART_MAX_INSTANCE               2
#define UART_DEFAULT_RINGBUFFER_SIZE    64
#define UART_DEFAULT_CONF_BAUDRATE      115200
#define UART_DEFAULT_CONF_PARITY        UART_CFG_PARITY_NONE
#define UART_DEAFULT_CONF_STOP_BITS     UART_CFG_STOP_BITS_1
#define UART_DEAFULT_CONF_DATA_BITS     UART_CFG_DATA_BITS_8
#define UART_DEAFULT_CONF_FLOW_CTRL     UART_CFG_FLOW_CTRL_NONE

#define UART_CONF_DUMP_RX_DATA          (1) // Set to 1 to dump UART RX data when received
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define UART0_NODE DT_NODELABEL(uart0)
#define UART1_NODE DT_NODELABEL(uart1)

/******************************************************************************
* Module Typedefs
*******************************************************************************/

typedef struct {
    struct ring_buf ringbuf;
    uint32_t buffer[UART_DEFAULT_RINGBUFFER_SIZE];
}uart_ringbuffer_t;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static const struct device *uart_device[UART_MAX_INSTANCE] = {
	DEVICE_DT_GET(UART0_NODE),
	DEVICE_DT_GET(UART1_NODE),
};

uart_ringbuffer_t uart_ringbuffer[UART_MAX_INSTANCE];
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
// Using LOG_HEXDUMP_DBG() to dump current ring buffer data base on head and tail member of ring buffer
void ring_buf_dump_data(struct ring_buf *p_ringbuf)
{
    uint8_t* p_data = NULL;
    // Get ring buffer current size
    uint32_t size = ring_buf_size_get(p_ringbuf);
    if(size == 0)
    {
        LOG_INF("Ring buffer is empty");
        return;
    }
    // Allocate memory for storing ring buffer data, get data and put it back to ring buffer
    p_data = k_malloc(size);
    if(p_data == NULL)
    {
        LOG_ERR("Failed to allocate memory for ring buffer data");
        return;
    }
    uint32_t ret = ring_buf_get(p_ringbuf, p_data, size);
    if(ret != size)
    {
        LOG_ERR("Failed to get ring buffer data, err code %d", ret);
        k_free(p_data);
        return;
    }
    ret = ring_buf_put(p_ringbuf, p_data, size);
    if(ret != size)
    {
        LOG_ERR("Failed to put ring buffer data, err code %d", ret);
        k_free(p_data);
        return;
    }
    // Dump ring buffer data
    LOG_HEXDUMP_DBG(p_data, size, "Ring buffer data");
    k_free(p_data);
}

void uart_irq_callback(const struct device *dev, void *user_data)
{
    // user_data is a pointer to uart_ringbuffer_t
    // if (!uart_irq_update(dev)) 
    // {
    //     LOG_ERR("Failed to update UART IRQ, device %s", dev->name);
    //     return;
    // }

    #if 0
    // Claim ring buffer memory
    uart_ringbuffer_t* p_uart_ringbuffer = (uart_ringbuffer_t*)user_data;
    uint8_t* p_ringbuf_rx = NULL;
    uint32_t ring_buf_size = ring_buf_put_claim(&p_uart_ringbuffer->ringbuf, &p_ringbuf_rx, UART_DEFAULT_RINGBUFFER_SIZE);
    if(ring_buf_size == 0 || p_ringbuf_rx == NULL)
    {
        LOG_ERR("Failed to claim ring buffer memory");
        return;
    }
    // Put data into ring buffer
    while (uart_irq_rx_ready(dev)) 
    {

	}
    #endif
    // Claim ring buffer memory
    uart_ringbuffer_t* p_uart_ringbuffer = (uart_ringbuffer_t*)user_data;
    int rx = 0;
    uint8_t *dst;
    uint32_t partial_size = 0;
    uint32_t total_size = 0;

    while (uart_irq_update(dev) && uart_irq_rx_ready(dev)) 
    {
        if (!partial_size) 
        {
            partial_size = ring_buf_put_claim(&p_uart_ringbuffer->ringbuf, &dst, UART_DEFAULT_RINGBUFFER_SIZE);
        }
        if (!partial_size) 
        {
            LOG_ERR("Ring buffer of UART device: %s doesn't have enough space", dev->name);
            break;
        }
        rx = uart_fifo_read(dev, dst, partial_size);
        if (rx <= 0) 
        {
            continue;
        }
        dst += rx;
        total_size += rx;
        partial_size -= rx;
    }
    ring_buf_put_finish(&p_uart_ringbuffer->ringbuf, total_size);
}

int __UARTInit(uint8_t uart_num)
{
    param_check((uart_num >= 0) && (uart_num <= 1));
    if( !device_is_ready(uart_device[uart_num]) )
    {
        LOG_ERR("UART %d device is not ready!", uart_num);
        return FAILURE;
    }
    const struct uart_config uart_cfg = {
		.baudrate = UART_DEFAULT_CONF_BAUDRATE,
		.parity = UART_DEFAULT_CONF_PARITY,
		.stop_bits = UART_DEAFULT_CONF_STOP_BITS,
		.data_bits = UART_DEAFULT_CONF_DATA_BITS,
		.flow_ctrl = UART_DEAFULT_CONF_FLOW_CTRL
	};
    int status = uart_configure(uart_device[uart_num], &uart_cfg);
    if(status != 0)
    {
        LOG_ERR("Failed to configure UART %d with error code %d", uart_num, status);
        return status;
    }
    LOG_INF("Init UART %d success", uart_num);
    // UART interrupt config
    ring_buf_init(&uart_ringbuffer[uart_num].ringbuf, UART_DEFAULT_RINGBUFFER_SIZE, uart_ringbuffer[uart_num].buffer);
    uart_irq_callback_user_data_set(uart_device[uart_num], &uart_irq_callback, &uart_ringbuffer[uart_num]);
    uart_irq_rx_enable(uart_device[uart_num]);
    return SUCCESS;
}

int __InitUART(void)
{
    int ret = __UARTInit(0);
    if(ret != SUCCESS)
    {
        LOG_ERR("Failed to init UART 0");
        return ret;
    }
    ret = __UARTInit(1);
    if(ret != SUCCESS)
    {
        LOG_ERR("Failed to init UART 1");
        return ret;
    }
    return SUCCESS;
}

//NOTE: if API limitations only allow for knowing IF data is available, return 1 if data is available.
//Returns number of bytes available to read from UART. Returns 0 on success, -1 on failure.  
int hal__UARTAvailable(uint8_t uartNum)
{
    param_check((uartNum >= 0) && (uartNum <= 1));
    int cur_size = ring_buf_size_get(&uart_ringbuffer[uartNum].ringbuf);
    if(cur_size > 0)
    {
        LOG_DBG("UART %d has %d bytes available", uartNum, cur_size);
        #if ( UART_CONF_DUMP_RX_DATA != 0)
        ring_buf_dump_data(&uart_ringbuffer[uartNum].ringbuf);
        #endif /* End of ( UART_CONF_DUMP_RX_DATA != 0) */    
    }
    return cur_size;
}

//Write data to UART. Returns 0 on success, -1 on failure.
int hal__UARTWrite_uint8(uint8_t uartNum, uint8_t data)
{
    param_check((uartNum >= 0) && (uartNum <= 1));
    uart_poll_out(uart_device[uartNum], data);
    return SUCCESS;
}

//Write data to UART. Returns number of bytes written on success, -1 on failure.
int hal__UARTWrite(uint8_t uartNum, uint8_t *data, uint16_t len)
{
    param_check((uartNum >= 0) && (uartNum <= 1) && (data != NULL));
    for(uint16_t i = 0; i < len; i++)
    {
        uart_poll_out(uart_device[uartNum], data[i]);
    }
    return SUCCESS;
} 

 //Read data from UART. Returns 0 on success, -1 on failure.
int hal__UARTRead_uint8(uint8_t uartNum, uint8_t *data)
{
    param_check((uartNum >= 0) && (uartNum <= 1) && (data != NULL));
    int ret = ring_buf_get(&uart_ringbuffer[uartNum].ringbuf, data, 1);
    if(ret != 1)
    {
        LOG_ERR("hal__UARTRead_uint8() read failed on UART %d, err code %d", uartNum, ret);
        return FAILURE;
    }
    return SUCCESS;
}

 //Read data from UART. Returns number of bytes read on success, -1 on failure.
int hal__UARTRead(uint8_t uartNum, uint8_t *data, uint16_t len)
{
    param_check((uartNum >= 0) && (uartNum <= 1) && (data != NULL));
    int ret = ring_buf_get(&uart_ringbuffer[uartNum].ringbuf, data, len);
    if(ret != len)
    {
        LOG_ERR("hal__UARTRead() read failed on UART %d, err code %d", uartNum, ret);
        return FAILURE;
    }
    return SUCCESS;
}
