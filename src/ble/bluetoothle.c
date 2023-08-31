/*******************************************************************************
* Title                 :   Bluetooth LE 
* Filename              :   bluetoothle.c
* Author                :   Itachi
* Origin Date           :   2022/09/04
* Version               :   0.0.0
* Compiler              :   NCS toolchain v2.0.0
* Target                :   nRF52840dk
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2022/09/04       0.0.0	         Itachi      Module Created.
*
*******************************************************************************/

/** \file bluetoothle.c
 * \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include <zephyr/logging/log.h>
#include "bluetoothle.h"

/******************************************************************************
*******************************************************************************/
#define DEVICE_NAME		            CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN		        (sizeof(DEVICE_NAME) - 1) // Ignore null terminated
#define BT_KEYBOARD_APPEARANCE       0x03C1  /* More on: https://specificationrefs.bluetooth.com/assigned-values/Appearance%20Values.pdf */

/* Logging relative */
#define MODULE_NAME			        bluetoothle
#define MODULE_LOG_LEVEL	        LOG_LEVEL_INF
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

#define BLE_SCAN_RESP_SUPPORT       (0) // 1: Support scan response
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
//TODO: Update the custom_adv_data
static uint8_t custom_adv_data[] = {/* 0x1E, 0xFF, */0x4C, 0x00, 0x12, 0x19, 0x10, 0x12, 0x12, 0x34, 0x56,
0x78, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x12,  0x34, 0x56, 0x12, 0x45, 00};

static const struct bt_data ADV_DATA[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, custom_adv_data, sizeof(custom_adv_data))
};

static const struct bt_data SCAN_RESP_DATA[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    BT_DATA(BT_DATA_GAP_APPEARANCE, ((uint16_t []) {BT_KEYBOARD_APPEARANCE}), sizeof(uint16_t))
};

/* Bluetooth applicatiton callbacks */
static ble_callback_t ble_cb_app = {
    .ble_connected_cb = NULL,
    .ble_disconnected_cb = NULL,
    .ble_adv_started_cb = NULL,
    .ble_adv_stopped_cb = NULL,
};

/******************************************************************************
* Static Function Definitions
*******************************************************************************/
static void on_ble_connect(struct bt_conn *conn, uint8_t err)
{
	if(err) 
    {
		LOG_ERR("BLE connection err: %d, re-advertising \n", err);
		return;
	}
    LOG_INF("BLE Connected.");

    if(ble_cb_app.ble_connected_cb != NULL)
    {
        ble_cb_app.ble_connected_cb();
    }
}

static void on_ble_disconnect(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("BLE Disconnected (reason: %d)", reason);
    if(ble_cb_app.ble_disconnected_cb != NULL)
    {
        ble_cb_app.ble_disconnected_cb();
    }
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
int ble_adv_start(void)
{
    // Get the BLE address
    bt_addr_le_t addr = {0};
    char addr_s[BT_ADDR_LE_STR_LEN] = {0};
    size_t count = 1;
	bt_id_get(&addr, &count);
    bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

    LOG_INF("Advertising address: %s \n", addr_s);

#if (BLE_SCAN_RESP_SUPPORT != 0)
    int errorcode = bt_le_adv_start(BT_LE_ADV_CONN, ADV_DATA, ARRAY_SIZE(ADV_DATA), SCAN_RESP_DATA, ARRAY_SIZE(SCAN_RESP_DATA));
#else /* !(BLE_SCAN_RESP_SUPPORT != 0) */
    int errorcode = bt_le_adv_start(BT_LE_ADV_CONN, ADV_DATA, ARRAY_SIZE(ADV_DATA), NULL, 0);
#endif /* End of (BLE_SCAN_RESP_SUPPORT != 0) */
    if (errorcode) {
        LOG_ERR("Couldn't start advertising (err = %d)", errorcode);
        return errorcode;
    }
    LOG_INF("Advertising successfully started\n");
    if(ble_cb_app.ble_adv_started_cb != NULL)
    {
        ble_cb_app.ble_adv_started_cb();
    }
    return 0;
}

int ble_adv_stop(void)
{
    int errorcode = bt_le_adv_stop();
    if (errorcode) {
        LOG_ERR("Couldn't stop advertising (err = %d)", errorcode);
        return errorcode;
    }
    LOG_INF("Advertising successfully stopped\n");
    if(ble_cb_app.ble_adv_stopped_cb != NULL)
    {
        ble_cb_app.ble_adv_stopped_cb();
    }
    return 0;
}

int ble_init(ble_callback_t* p_app_cb)
{
    int errorcode= 0;

    /* Bluetooth zephyr internal callbacks */
    static struct bt_conn_cb ble_cb = {
        .connected 		= &on_ble_connect,
        .disconnected 	= &on_ble_disconnect,
        
    };

    LOG_INF("BLE initializing \n\r");

    /* Initialize the application callbacks */
    if(p_app_cb != NULL)
    {
        ble_cb_app = *p_app_cb;
    }
    /* Assign callbacks for connection events */
    bt_conn_cb_register(&ble_cb);

    /* BLE initialization */
    errorcode = bt_enable(NULL);
    if(errorcode)
    {
        LOG_ERR("bt_enable return err %d \r\n", errorcode);
        return errorcode;
    }
    LOG_INF("BLE init succesfully");
    return 0;
}