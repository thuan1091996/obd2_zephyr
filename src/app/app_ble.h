/****************************************************************************
* Title                 :   header file
* Filename              :   app_ble.h
* Author                :   
* Origin Date           :   2023/06/16
* Version               :   v0.0.0
* Compiler              :   
* Target                :   
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2023/06/16    v0.0.0         	ItachiVN      Interface Created.
*
*****************************************************************************/

/** \file app_ble.h
 *  \brief This module contains .
 *
 *  This is the header file for 
 */
#ifndef APP_APP_BLE_H_
#define APP_APP_BLE_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include "qpc.h"


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


/******************************************************************************
* Extern Variables
*******************************************************************************/
extern QActive * const p_ble_actor;


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void ble_actor_ctor();
void ble_actor_start();


#endif // APP_APP_BLE_H_

/*** End of File **************************************************************/