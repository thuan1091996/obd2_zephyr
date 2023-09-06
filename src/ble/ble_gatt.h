/****************************************************************************
* Title                 :    header file
* Filename              :   gatt_sensor.h
* Author                :   thuantm5
* Origin Date           :   2023/03/17
* Version               :   v0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nrf52
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2023/03/17    v0.0.0         	thuantm5      Interface Created.
*
*****************************************************************************/

/** \file gatt_sensor.h
 *  \brief This module contains .
 *
 *  This is the header file for 
 */
#ifndef BLE_GATT_SENSOR_H_
#define BLE_GATT_SENSOR_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
/** @brief UUID of the SERVICE1/ CHAR1 **/
#define CUSTOM_SERVICE1_NAME    CUSTOM_SERVICE1
#define BT_CUSTOM_SERV1_UUID    BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc11)
#define BT_CUSTOM_CHAR1_UUID    BT_UUID_128_ENCODE(0xe9ea0002, 0xe19b, 0x482d, 0x9293, 0xc7907585fc11)

#define BT_UUID_CUSTOM_SERV1    BT_UUID_DECLARE_128(BT_CUSTOM_SERV1_UUID)
#define BT_UUID_CUSTOM_CHAR1    BT_UUID_DECLARE_128(BT_CUSTOM_CHAR1_UUID)

/** @brief UUID of the SERVICE2/ CHAR2 **/
#define CUSTOM_SERVICE2_NAME    CUSTOM_SERVICE2
#define BT_CUSTOM_SERV2_UUID    BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc22)
#define BT_CUSTOM_CHAR2_UUID    BT_UUID_128_ENCODE(0xe9ea0002, 0xe19b, 0x482d, 0x9293, 0xc7907585fc22)

#define BT_UUID_CUSTOM_SERV2    BT_UUID_DECLARE_128(BT_CUSTOM_SERV2_UUID)
#define BT_UUID_CUSTOM_CHAR2    BT_UUID_DECLARE_128(BT_CUSTOM_CHAR2_UUID)

/** @brief UUID of the SERVICE2/ CHAR3 **/
#define CUSTOM_SERVICE3_NAME    CUSTOM_SERVICE3
#define BT_CUSTOM_SERV3_UUID    BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc33)
#define BT_CUSTOM_CHAR3_UUID    BT_UUID_128_ENCODE(0xe9ea0002, 0xe19b, 0x482d, 0x9293, 0xc7907585fc33)

#define BT_UUID_CUSTOM_SERV3    BT_UUID_DECLARE_128(BT_CUSTOM_SERV3_UUID)
#define BT_UUID_CUSTOM_CHAR3    BT_UUID_DECLARE_128(BT_CUSTOM_CHAR3_UUID)

/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @brief SERVICE1/ CHAR1 **/
ssize_t custom_char1_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t	custom_char1_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);
void custom_char1_notify_changed(const struct bt_gatt_attr *attr, uint16_t value);


/** @brief SERVICE2/ CHAR2 **/
ssize_t custom_char2_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t	custom_char2_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);
void custom_char2_notify_changed(const struct bt_gatt_attr *attr, uint16_t value);

/** @brief SERVICE3/ CHAR3 **/
ssize_t custom_char3_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t	custom_char3_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);
void custom_char3_notify_changed(const struct bt_gatt_attr *attr, uint16_t value);



#endif // BLE_GATT_SENSOR_H_

/*** End of File **************************************************************/