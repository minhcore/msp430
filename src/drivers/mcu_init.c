#include "mcu_init.h"
#include "io.h"
#include <msp430.h>

static void watchdog_stop(void)
{
    WDTCTL = WDTPW + WDTHOLD;
}

void mcu_init(void)
{
    watchdog_stop();
}