/*******************************************************************************
* Title                 :   
* Filename              :   app_ble.c
* Author                :   
* Origin Date           :   2023/06/16
* Version               :   0.0.0
* Compiler              :   
* Target                :   
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/06/16       0.0.0	                  Module Created.
*
*******************************************************************************/

/** \file app_ble.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
/* Zephyr include */
#include <zephyr/logging/log.h>
#include <assert.h>

/* Applications include */
#include "app_ble.h"
#include "common.h"

/* Low level driver relatives */
#include "bluetoothle.h"
#include <bluetooth/services/nus.h>

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define BLE_CONTINOUS_ADV  1


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* Logging relative */
#define MODULE_NAME			        ble_app
#define MODULE_LOG_LEVEL	        LOG_LEVEL_INF
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

Q_DEFINE_THIS_FILE

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef struct {      /* the ble_actor_t active object */
    QActive super;    /* inherit QActive */
    QTimeEvt timeEvt; /* private time event generator */
}ble_actor_t;


/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* QPC relative functionality*/
static QState ble_initial(ble_actor_t * const me, QEvt const * const e);
static QState ble_ready(ble_actor_t * const me, QEvt const * const e);
static QState ble_advertising(ble_actor_t * const me, QEvt const * const e);
static QState ble_connected(ble_actor_t * const me, QEvt const * const e);

void app_ble_connected_cb(void);
void app_ble_disconnected_cb(void);
void app_ble_adv_started_cb(void);
void app_ble_adv_stopped_cb(void);

void app_ble_nus_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len);
void app_ble_nus_sent(struct bt_conn *conn);
void app_ble_nus_send_enabled(enum bt_nus_send_status status);

void ble_actor_ctor(void);
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
K_THREAD_STACK_DEFINE(ble_stack, BLE_ACTOR_STACK_SIZE); /* stack storage for BLE_AO */
static QEvt const *ble_queue[BLE_ACTOR_QUEUE_SIZE]; 	/* event queue storage for BLE_AO */

static ble_callback_t app_ble_cb = {
    .ble_connected_cb = &app_ble_connected_cb,
    .ble_disconnected_cb = &app_ble_disconnected_cb,
    .ble_adv_started_cb = &app_ble_adv_started_cb,
    .ble_adv_stopped_cb = &app_ble_adv_stopped_cb
};

static struct bt_nus_cb ble_nus_cb = {
    .received = &app_ble_nus_received,
    .sent = &app_ble_nus_sent,
    .send_enabled = app_ble_nus_send_enabled,
};

static QEvt ble_evt = 
{
    .sig = BLE_EVT_SIG,
    .poolId_ = 0, /* Immutable event */
};

static ble_actor_t ao_ble; /* the ble_actor_t active object */
QActive * const p_ble_actor = &ao_ble.super;

/******************************************************************************
* Function Definitions
*******************************************************************************/
void ble_actor_ctor() 
{
    ble_actor_t * const me = &ao_ble;
    QActive_ctor(&me->super, Q_STATE_CAST(&ble_initial));
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

void ble_actor_start()
{
    QActive_setAttr(p_ble_actor,
                    0,            		/* thread options */
                    "BLE_AO");    		/* thread name */

    QACTIVE_START(p_ble_actor,      	/* AO pointer to start */
                  BLE_ACTOR_PRIO,       /* unique QP priority of the AO */
                  ble_queue,   		    /* storage for the AO's queue */
                  Q_DIM(ble_queue),     /* length of the queue [entries] */
                  (void *)ble_stack,    /* stack storage */
                  K_THREAD_STACK_SIZEOF(ble_stack), /* stack size [bytes] */
                  (void *)0);           /* initial event (or 0) */
}

/* App BLE callbacks */
void app_ble_connected_cb(void)
{
    LOG_INF("on app_ble_connected_cb");
    ble_evt.sig = BLE_CONNECTED_SIG;
    QACTIVE_POST(p_ble_actor, (QEvt *)&ble_evt, 0U);
}

void app_ble_disconnected_cb(void)
{
    LOG_INF("on app_ble_disconnected_cb");
    ble_evt.sig = BLE_DISCONNECTED_SIG;
    QACTIVE_POST(p_ble_actor, (QEvt *)&ble_evt, 0U);
}

void app_ble_adv_started_cb(void)
{
    LOG_INF("on app_ble_adv_started_cb");
    ble_evt.sig = BLE_ADV_STARTED_SIG;
    QACTIVE_POST(p_ble_actor, (QEvt *)&ble_evt, 0U);

}

void app_ble_adv_stopped_cb(void)
{
    LOG_INF("on app_ble_adv_stopped_cb");
    ble_evt.sig = BLE_ADV_STOPPED_SIG;
    QACTIVE_POST(p_ble_actor, (QEvt *)&ble_evt, 0U);
}

/* BLE nus callback */
void app_ble_nus_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len)
{
    LOG_INF("on app_ble_nus_received()");
}

void app_ble_nus_sent(struct bt_conn *conn)
{
    LOG_INF("on app_ble_nus_sent()");
}

void app_ble_nus_send_enabled(enum bt_nus_send_status status)
{
    LOG_INF("on app_ble_nus_send_enabled()");
    LOG_INF("NUS notification %s", status == (BT_NUS_SEND_STATUS_ENABLED) ? "enabled" : "disabled");
}

/* HSM definition ----------------------------------------------------------*/
QState ble_initial(ble_actor_t * const me, QEvt const * const e) {
    (void)e;
    LOG_DBG("BLE state INITIAL, event %d", e->sig);
    for(uint8_t retry_count=0; retry_count < 5; retry_count++)
    {
        if(!ble_init(&app_ble_cb) )
        {
            if (IS_ENABLED(CONFIG_BT_NUS))
            {
                /* Initialize the NUS service */
                if (!bt_nus_init(&ble_nus_cb))
                {
                    LOG_INF("NUS service initialized");
                }   
                else
                {
                    LOG_ERR("NUS service initialization failed");
                }
            }
            return Q_TRAN(&ble_ready);
        }
    }
    LOG_ERR("BLE failed to initialize");
    //TODO: Add error handling
    return Q_UNHANDLED();
}


QState ble_ready(ble_actor_t * const me, QEvt const * const e) {
    QState status;
    LOG_DBG("BLE state READY, event %d", e->sig);
    switch (e->sig) 
    {
        #if BLE_CONTINOUS_ADV
        case Q_ENTRY_SIG:
        {
            ble_evt.sig = BLE_TRIGGER_ADV_START_SIG;
            QACTIVE_POST(p_ble_actor, (QEvt *)&ble_evt, 0U);
            status = Q_HANDLED();
            break;
        }
        #endif /* End of BLE_CONTINOUS_ADV */

        case BLE_TRIGGER_ADV_START_SIG:
        {
            for(uint8_t retry_count = 0; retry_count < 5; retry_count++)
            {
                if ( !ble_adv_start())
                {
                    break;
                }
                LOG_WRN("BLE adv start failed, retrying %d", retry_count);
            }
            status = Q_HANDLED();
            break;
        }

        case BLE_ADV_STARTED_SIG:
        {
            status = Q_TRAN(&ble_advertising);
            break;
        }

        default:
        {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}

QState ble_advertising(ble_actor_t * const me, QEvt const * const e) {
    QState status;  
    LOG_DBG("BLE state ADVERTISING, event %d", e->sig);
    switch (e->sig) {

        case Q_ENTRY_SIG:
        {
            /* Arm the time event to expire in BLE_ADV_TIMEOUT ms */
            QTimeEvt_armX(&me->timeEvt, BLE_ADV_TIMEOUT, 0);
            status = Q_HANDLED();
            break;
        }

        case TIMEOUT_SIG:
        {
            status = Q_TRAN(&ble_ready);
            break;
        }

        case BLE_ADV_STOPPED_SIG:
        {
            status = Q_TRAN(&ble_ready);
            break;
        }

        case BLE_CONNECTED_SIG:
        {
            status = Q_TRAN(&ble_connected);
            break;
        }

        case Q_EXIT_SIG:
        {
            for(uint8_t retry_count = 0; retry_count < 5; retry_count++)
            {
                if ( !ble_adv_stop())
                {
                    break;
                }
                LOG_WRN("BLE adv stop failed, retrying %d", retry_count);
            }
            QTimeEvt_disarm(&me->timeEvt);
            status = Q_HANDLED();
            break;
        }


        default: 
        {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}

static QState ble_connected(ble_actor_t * const me, QEvt const * const e)
{
    QState status;
    LOG_DBG("BLE state CONNECTED, event %d", e->sig);
    switch (e->sig) 
    {
        case BLE_DISCONNECTED_SIG:
        {
            status = Q_TRAN(&ble_ready);
            break;
        }

        default: 
        {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}