/*******************************************************************************
* Title                 :    
* Filename              :   framework-app.c
* Author                :   ItachiVN
* Origin Date           :   2023/03/19
* Version               :   0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nrf52
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/03/19       0.0.0	         ItachiVN      Module Created.
*
*******************************************************************************/

/** \file framework-app.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "framework-app.h"
#include <zephyr/logging/log.h>
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MEMPOOL_SIZE            (200)
#define MEMPOOL_BLOCKSIZE       (10)

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define MODULE_NAME			framework_module
#define MODULE_LOG_LEVEL	LOG_LEVEL_DBG

LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct k_timer QF_tick_timer;

static uint8_t mempool_small[MEMPOOL_SIZE];
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

/*..........................................................................*/
static void QF_tick_function(struct k_timer *tid) {
    (void)tid; /* unused parameter */
    QTIMEEVT_TICK_X(0U, (void *)0);
}
void framework_init()
{
    printk("Framework init\n");
    QF_init();  /* initialize the framework and the underlying RT kernel */
    k_timer_init(&QF_tick_timer, &QF_tick_function, NULL);

    QF_poolInit(mempool_small, sizeof(mempool_small), MEMPOOL_BLOCKSIZE);

}

/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    k_timer_start(&QF_tick_timer, K_MSEC(1), K_MSEC(1));
    printk("QF_onStartup\n");
}
/*..........................................................................*/
void QF_onCleanup(void) {
    printk("QF_onCleanup\n");
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    printk("\nASSERTION in %s:%d\n", module, loc);
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */
#ifndef NDEBUG
    k_panic(); /* debug build: halt the system for error search... */
#else
    sys_reboot(SYS_REBOOT_COLD); /* release build: reboot the system */
#endif
}