/****************************************************************************
* Title                 :   bluetooth le interface
* Filename              :   bluetoothle.h
* Author                :   Itachi
* Origin Date           :   2022/09/04
* Version               :   v0.0.0
* Compiler              :   NCS toolchain v2.0.0
* Target                :   nRF52840dk
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2022/09/04    v0.0.0         	Itachi      Interface Created.
*
*****************************************************************************/

/** \file bluetoothle.h
 *  \brief This module contains interface for ble with Zephyr RTOS using nRF52840dk
 *
 *  This is the header file for 
 */
#ifndef BLE_BLUETOOTHLE_H_
#define BLE_BLUETOOTHLE_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/
typedef struct
{
    void (*ble_connected_cb)(void);
    void (*ble_disconnected_cb)(void);
    void (*ble_adv_started_cb)(void);
    void (*ble_adv_stopped_cb)(void);
}ble_callback_t;

/******************************************************************************
* Variables
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
int ble_init(ble_callback_t *cb);
int ble_adv_start(void);
int ble_adv_stop(void);
int ble_set_adv_name(char* p_name);
#endif // BLE_BLUETOOTHLE_H_

/*** End of File **************************************************************/