/****************************************************************************
* Title                 :    header file
* Filename              :   common.h
* Author                :   ItachiVN
* Origin Date           :   2023/03/19
* Version               :   v0.0.0
* Compiler              :   
* Target                :   
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2023/03/19    v0.0.0         	ItachiVN      Interface Created.
*
*****************************************************************************/

/** \file common.h
 *  \brief This module contains .
 *
 *  This is the header file for 
 */
#ifndef COMMON_H_
#define COMMON_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include "qpc.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define APP_ACTOR_QUEUE_SIZE		        (20)
#define APP_ACTOR_STACK_SIZE		        (1024)
#define APP_ACTOR_PRIO                      (CONFIG_NUM_PREEMPT_PRIORITIES - 11)

#define BLE_ACTOR_QUEUE_SIZE		        (20)
#define BLE_ACTOR_STACK_SIZE		        (1024)
#define BLE_ACTOR_PRIO                      (CONFIG_NUM_PREEMPT_PRIORITIES - 10)

#define OBD_ACTOR_QUEUE_SIZE		        (30)
#define OBD_ACTOR_STACK_SIZE		        (1024)
#define OBD_ACTOR_PRIO                      (CONFIG_NUM_PREEMPT_PRIORITIES - 9)



#define BLE_ADV_TIMEOUT                     (30000U)

#define SUCCESS                 0
#define FAILURE                 -1

#define param_check(param)	    if ( !(param) ) return FAILURE
#define error_check(con, error) if ( con ) return error


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/
enum ApplicationEvt {
    TIMEOUT_SIG = Q_USER_SIG,       /* Timer timeout */
    TIMEOUT_BLINK_SIG,

    BLE_EVT_SIG,
    BLE_RDY_SIG,                    /* BLE ready event */
    BLE_CONNECTED_SIG,              /* BLE connected event */
    BLE_DISCONNECTED_SIG,           /* BLE disconnected event */
    BLE_ADV_STARTED_SIG,            /* BLE advertising started event */
    BLE_ADV_STOPPED_SIG,            /* BLE advertising stopped event */
    BLE_ADV_TIMEOUT_SIG,            /* BLE advertising timeout without connection */
    BLE_TRIGGER_ADV_START_SIG,      /* Trigger BLE actor to advertising */
    BLE_TRIGGER_TERMINATE_CONN_SIG, /* Trigger BLE actor to terminate connection */
    UART_RX_SIG,                    /* UART RX event */
};

/******************************************************************************
* Variables
*******************************************************************************/
extern QActive * const p_app_actor;

/******************************************************************************
* Function Prototypes
*******************************************************************************/

#endif // COMMON_H_

/*** End of File **************************************************************/