/*******************************************************************************
* Title                 :   App OBD
* Filename              :   app_obd.c
* Author                :   ItachiVN
* Origin Date           :   2023/06/28
* Version               :   0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nRF52
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/06/28       0.0.0	         ItachiVN      Module Created.
*
*******************************************************************************/

/** \file app_obd.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
/* Zephyr include */
#include <zephyr/logging/log.h>
#include <assert.h>

/* Applications include */
#include "common.h"
#include "app_ble.h"
#include "app_uart.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* Logging relative */
#define MODULE_NAME			        obd_app
#define MODULE_LOG_LEVEL	        LOG_LEVEL_INF
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

Q_DEFINE_THIS_FILE
/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef struct {      /* the obd_actor_t active object */
    QActive super;    /* inherit QActive */
    QTimeEvt timeEvt; /* private time event generator */
}obd_actor_t;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
K_THREAD_STACK_DEFINE(obd_actor_stack, OBD_ACTOR_STACK_SIZE);   /* stack storage for OBD_AO */
static QEvt const *obd_actor_queue[OBD_ACTOR_QUEUE_SIZE]; 	    /* event queue storage for OBD_AO */


static obd_actor_t ao_obd; /* the obd_actor_t active object */

QActive * const p_obd_actor = &ao_obd.super;
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void obd_actor_start()
{
    QActive_setAttr((QActive *)&ao_obd,
                    0,            		    /* thread options */
                    "OBD_AO");    		    /* thread name */

    QACTIVE_START(&ao_obd,      	        /* AO pointer to start */
                  OBD_ACTOR_PRIO,           /* unique QP priority of the AO */
                  obd_actor_queue,          /* storage for the AO's queue */
                  Q_DIM(obd_actor_queue),   /* length of the queue [entries] */
                  (void *)obd_actor_stack,  /* stack storage */
                  K_THREAD_STACK_SIZEOF(obd_actor_stack), /* stack size [bytes] */
                  (void *)0);               /* initial event (or 0) */
}