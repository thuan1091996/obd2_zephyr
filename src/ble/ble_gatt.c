/*******************************************************************************
* Title                 :    
* Filename              :   gatt_sensor.c
* Author                :   thuantm5
* Origin Date           :   2023/03/17
* Version               :   0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nrf52
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/03/17       0.0.0	         thuantm5      Module Created.
*
*******************************************************************************/

/** \file gatt_sensor.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "bluetoothle.h"
#include <zephyr/logging/log.h>
#include "ble_gatt.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MODULE_NAME			        gatt_sensor
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/



/******************************************************************************
* Module Typedefs
*******************************************************************************/
static ble_custom_gatt_cb_t ble_custom_gatt_cb = 
{
    .custom_char1_read_cb = NULL,
    .custom_char2_read_cb = NULL,
    .custom_char3_read_cb = NULL,
    .custom_char4_read_cb = NULL,
    .custom_char5_write_cb = NULL,
};

/**
 * @brief Assigned application callbacks for custom char 1 - > custom char 5
 * @param ble_gatt_cb Application callbacks 
 */
void ble_custom_service_init(ble_custom_gatt_cb_t* ble_gatt_cb)
{
    if(ble_gatt_cb != NULL)
    {
        ble_custom_gatt_cb.custom_char1_read_cb = ble_gatt_cb->custom_char1_read_cb;
        ble_custom_gatt_cb.custom_char2_read_cb = ble_gatt_cb->custom_char2_read_cb;
        ble_custom_gatt_cb.custom_char3_read_cb = ble_gatt_cb->custom_char3_read_cb;
        ble_custom_gatt_cb.custom_char4_read_cb = ble_gatt_cb->custom_char4_read_cb;
        ble_custom_gatt_cb.custom_char5_write_cb = ble_gatt_cb->custom_char5_write_cb;
    }
}


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static bool g_char3_notify_en=false, g_char4_notify_en=false;
uint16_t char123_value=0;
/******************************************************************************
* Static Function Prototypes
*******************************************************************************/
static ssize_t custom_char1_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
static ssize_t custom_char2_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
static ssize_t custom_char3_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
static ssize_t custom_char4_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
static void custom_char5_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

static void custom_char3_notify_changed(const struct bt_gatt_attr *attr, uint16_t value);
static void custom_char4_notify_changed(const struct bt_gatt_attr *attr, uint16_t value);


/******************************************************************************
* Function Definitions
*******************************************************************************/
static ssize_t custom_char1_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    LOG_INF("CUSTOM_CHAR1 - ReadCB");
    void* p_char1_data = NULL;      // Pointer to char1 data    
    uint16_t char1_data_len = 0;
    do
    {
        if(ble_custom_gatt_cb.custom_char1_read_cb == NULL)
            break;
        // Call application callback to update p_char1_data point to the new data
        p_char1_data = ble_custom_gatt_cb.custom_char1_read_cb(NULL, &char1_data_len);
        
        if(p_char1_data == NULL)
        {
            LOG_ERR("Invalid data pointer");
            break;
        }

        if(char1_data_len > len)
        {
            LOG_ERR("%d is an invalid data length", char1_data_len);
            break;
        }

        return bt_gatt_attr_read(conn, attr, buf, len, offset, p_char1_data, char1_data_len);
    }while(0);

    LOG_ERR("Invalid char1 attribute payload");
    return bt_gatt_attr_read(conn, attr, buf, len, offset, "WARN: INVALID DATA", strlen("WARN: INVALID DATA"));
}

static ssize_t custom_char2_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    LOG_INF("CUSTOM_CHAR2 - ReadCB");
    void* p_char2_data = NULL;      // Pointer to char2 data    
    uint16_t char2_data_len = 0;
    do
    {
        if(ble_custom_gatt_cb.custom_char2_read_cb == NULL)
            break;
        // Call application callback to update p_char2_data point to the new data
        p_char2_data = ble_custom_gatt_cb.custom_char2_read_cb(NULL, &char2_data_len);
        
        if(p_char2_data == NULL)
        {
            LOG_ERR("Invalid data pointer");
            break;
        }

        if(char2_data_len > len)
        {
            LOG_ERR("%d is an invalid data length", char2_data_len);
            break;
        }

        return bt_gatt_attr_read(conn, attr, buf, len, offset, p_char2_data, char2_data_len);
    }while(0);

    LOG_ERR("Invalid char2 attribute payload");
    return bt_gatt_attr_read(conn, attr, buf, len, offset, "WARN: INVALID DATA", strlen("WARN: INVALID DATA"));
}

static ssize_t custom_char3_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    LOG_INF("CUSTOM_CHAR3 - ReadCB");
    void* p_char3_data = NULL;      // Pointer to char3 data    
    uint16_t char3_data_len = 0;
    do
    {
        if(ble_custom_gatt_cb.custom_char3_read_cb == NULL)
            break;
        // Call application callback to update p_char3_data point to the new data
        p_char3_data = ble_custom_gatt_cb.custom_char3_read_cb(NULL, &char3_data_len);
        
        if(p_char3_data == NULL)
        {
            LOG_ERR("Invalid data pointer");
            break;
        }

        if(char3_data_len > len)
        {
            LOG_ERR("%d is an invalid data length", char3_data_len);
            break;
        }

        return bt_gatt_attr_read(conn, attr, buf, len, offset, p_char3_data, char3_data_len);
    }while(0);

    LOG_ERR("Invalid char3 attribute payload");
    return bt_gatt_attr_read(conn, attr, buf, len, offset, "WARN: INVALID DATA", strlen("WARN: INVALID DATA"));
}

static ssize_t custom_char4_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    LOG_INF("CUSTOM_CHAR4 - ReadCB");
    void* p_char4_data = NULL;      // Pointer to char4 data    
    uint16_t char4_data_len = 0;
    do
    {
        if(ble_custom_gatt_cb.custom_char4_read_cb == NULL)
            break;
        // Call application callback to update p_char4_data point to the new data
        p_char4_data = ble_custom_gatt_cb.custom_char4_read_cb(NULL, &char4_data_len);
        
        if(p_char4_data == NULL)
        {
            LOG_ERR("Invalid data pointer");
            break;
        }

        if(char4_data_len > len)
        {
            LOG_ERR("%d is an invalid data length", char4_data_len);
            break;
        }

        return bt_gatt_attr_read(conn, attr, buf, len, offset, p_char4_data, char4_data_len);
    }while(0);

    LOG_ERR("Invalid char4 attribute payload");
    return bt_gatt_attr_read(conn, attr, buf, len, offset, "WARN: INVALID DATA", strlen("WARN: INVALID DATA"));
}


static void custom_char5_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    ARG_UNUSED(conn);
    ARG_UNUSED(offset);
    ARG_UNUSED(flags);
    LOG_INF("CUSTOM_CHAR5 - WriteCB");
    if(ble_custom_gatt_cb.custom_char5_write_cb != NULL)
    {
        int recv_len = len;
        ble_custom_gatt_cb.custom_char5_write_cb((void*)buf, &recv_len);
    }
}

void custom_char3_notify_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
    switch(value)
    {
        case BT_GATT_CCC_NOTIFY:

            g_char3_notify_en = true;
            break;

        case 0: 
            g_char3_notify_en = false;
            break;
        
        default: 
            LOG_ERR("Error, CCCD has been set to an invalid value");     
    } 
    LOG_INF("CUSTOM_CHAR3 notification %s.", g_char3_notify_en ? "enabled" : "disabled");
}

void custom_char4_notify_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
    switch(value)
    {
        case BT_GATT_CCC_NOTIFY:

            g_char4_notify_en = true;
            break;

        case 0: 
            g_char4_notify_en = false;
            break;
        
        default: 
            LOG_ERR("Error, CCCD has been set to an invalid value");     
    } 
    LOG_INF("CUSTOM_CHAR4 notification %s.", g_char4_notify_en ? "enabled" : "disabled");
}


/*

#define BT_UUID_CUSTOM_SERV1    BT_UUID_DECLARE_128(BT_CUSTOM_SERV1_UUID)
#define BT_UUID_CUSTOM_CHAR1    BT_UUID_DECLARE_128(BT_CUSTOM_CHAR1_UUID)
#define BT_UUID_CUSTOM_CHAR2    BT_UUID_DECLARE_128(BT_CUSTOM_CHAR2_UUID)
#define BT_UUID_CUSTOM_CHAR3    BT_UUID_DECLARE_128(BT_CUSTOM_CHAR3_UUID)
#define BT_UUID_CUSTOM_CHAR4    BT_UUID_DECLARE_128(BT_CUSTOM_CHAR4_UUID)
#define BT_UUID_CUSTOM_CHAR5    BT_UUID_DECLARE_128(BT_CUSTOM_CHAR5_UUID)


BLE custom characteristic attribute table (Characteristic UUID  |  Properties | Permission)
Custom char1: read / permit read/write
Custom char2: read / permit read/write
Custom char3: read + notify | permit read/write
Custom char4: read + notify | permit read/write
Custom char5: write | permit read/write

*/
BT_GATT_SERVICE_DEFINE(CUSTOM_SERVICE1_NAME,
BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERV1),
    // Custom char1
    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHAR1,
                    BT_GATT_CHRC_READ,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, /* No security enable */
                    custom_char1_read_cb, NULL, NULL),
    // Custom char2
    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHAR2,
                    BT_GATT_CHRC_READ,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, /* No security enable */
                    custom_char2_read_cb, NULL, NULL),
    // Custom char3
    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHAR3,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, /* No security enable */
                    custom_char3_read_cb, NULL, NULL),          
    BT_GATT_CCC(custom_char3_notify_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    // Custom char4
    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHAR4,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, /* No security enable */
                    custom_char4_read_cb, NULL, NULL),
    BT_GATT_CCC(custom_char4_notify_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    // // Custom char5
    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHAR5,
                    BT_GATT_CHRC_WRITE,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, /* No security enable */
                    NULL, custom_char5_write_cb, NULL),
);