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

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static bool g_char1_notify_en=false, g_char2_notify_en=false, g_char3_notify_en=false;
uint16_t char123_value=0;
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

ssize_t custom_char1_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    ARG_UNUSED(conn);
    LOG_INF("CUSTOM_CHAR1 - ReadCB, current CHAR1 value: %d", char123_value);
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &char123_value, sizeof(char123_value));
}

ssize_t custom_char2_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    ARG_UNUSED(conn);
    LOG_INF("CUSTOM_CHAR2 - ReadCB, current CHAR2 value: %d", char123_value);
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &char123_value, sizeof(char123_value));
}

ssize_t custom_char3_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    ARG_UNUSED(conn);
    LOG_INF("CUSTOM_CHAR3 - ReadCB, current CHAR3 value: %d", char123_value);
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &char123_value, sizeof(char123_value));
}

ssize_t	custom_char1_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    ARG_UNUSED(conn);
    ARG_UNUSED(offset);
    ARG_UNUSED(flags);
    LOG_INF("CUSTOM_CHAR1 - Write request");
    if(len != sizeof(char123_value))
    {
        LOG_ERR("Error, invalid data length");
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
    }
    memcpy(&char123_value, buf, sizeof(char123_value));
    LOG_INF("CUSTOM_CHAR1 - Value: %d", char123_value);
    return len;
}

ssize_t	custom_char2_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    ARG_UNUSED(conn);
    ARG_UNUSED(offset);
    ARG_UNUSED(flags);
    LOG_INF("CUSTOM_CHAR2 - Write request");
    if(len != sizeof(char123_value))
    {
        LOG_ERR("Error, invalid data length");
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
    }
    memcpy(&char123_value, buf, sizeof(char123_value));
    LOG_INF("CUSTOM_CHAR2 - Value: %d", char123_value);
    return len;
}

ssize_t	custom_char3_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    ARG_UNUSED(conn);
    ARG_UNUSED(offset);
    ARG_UNUSED(flags);
    LOG_INF("CUSTOM_CHAR3 - Write request");
    if(len != sizeof(char123_value))
    {
        LOG_ERR("Error, invalid data length");
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
    }
    memcpy(&char123_value, buf, sizeof(char123_value));
    LOG_INF("CUSTOM_CHAR3 - Value: %d", char123_value);
    return len;
}


void custom_char1_notify_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
    switch(value)
    {
        case BT_GATT_CCC_NOTIFY:

            g_char1_notify_en = true;
            break;

        case 0: 
            g_char1_notify_en = false;
            break;
        
        default: 
            LOG_ERR("Error, CCCD has been set to an invalid value");     
    }
    LOG_INF("CUSTOM_CHAR1 notification %s.", g_char1_notify_en ? "enabled" : "disabled");
}

void custom_char2_notify_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
    switch(value)
    {
        case BT_GATT_CCC_NOTIFY:

            g_char2_notify_en = true;
            break;

        case 0: 
            g_char2_notify_en = false;
            break;
        
        default: 
            LOG_ERR("Error, CCCD has been set to an invalid value");     
    }
    LOG_INF("CUSTOM_CHAR2 notification %s.", g_char2_notify_en ? "enabled" : "disabled");
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

BT_GATT_SERVICE_DEFINE(CUSTOM_SERVICE1_NAME,
BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERV1),
    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHAR1,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                    custom_char1_read_cb, custom_char1_write_cb, NULL),
    BT_GATT_CCC(custom_char1_notify_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

BT_GATT_SERVICE_DEFINE(CUSTOM_SERVICE2_NAME,
BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERV2),
    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHAR2,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                    custom_char2_read_cb, custom_char2_write_cb, NULL),
    BT_GATT_CCC(custom_char2_notify_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

BT_GATT_SERVICE_DEFINE(CUSTOM_SERVICE3_NAME,
BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERV3),
    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHAR3,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                    custom_char3_read_cb, custom_char3_write_cb, NULL),
    BT_GATT_CCC(custom_char3_notify_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);