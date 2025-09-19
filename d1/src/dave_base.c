//-----------------------------------------------------------------------------
// Copyright (C) 2023 Alif Semiconductor - All Rights Reserved.
// Use, distribution and modification of this code is permitted under the
// terms stated in the Alif Semiconductor Software License Agreement
//
// You should have received a copy of the Alif Semiconductor Software
// License Agreement with this file. If not, please write to:
// contact@alifsemi.com, or visit: https://alifsemi.com/license
//
//-----------------------------------------------------------------------------
// Project:     D/AVE
// File:        dave_base.c
// Description: This file defines the D/AVE low-level driver basic functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#ifdef __ZEPHYR__
#include <zephyr/drivers/clock_control.h>
#include <zephyr/dt-bindings/clock/alif_ensemble_clocks.h>
#else
#include "RTE_Components.h"
#include CMSIS_device_header
#ifndef SYSTEM_UTILS_H
#include "sys_utils.h"
#endif
#endif

#include "dave_cfg.h"
#include "dave_base.h"
#include "dave_base_intern.h"
#include "dave_d0lib.h"
#include "dave_registermap.h"
#include "dave_irq.h"

//---------------------------------------------------------------------------
// These helper macros are used to stringify a given macro
#define D1_STR(s)           # s
#define D1_XSTR(s)          D1_STR(s)

// These helper macros are used to concat two strings with a dot.
#ifdef __CA850__
#define D1_DOT(a,b)         a.b
#else
#define D1_DOT(a,b)         a ## . ## b
#endif
#define D1_XDOT(a,b)        D1_DOT(a,b)

//---------------------------------------------------------------------------
// Define the D1_VERSION and D1_VERSION_STRING macros

// Build up the D1_VERSION macro
#define D1_VERSION ((D1_VERSION_MAJOR << 16) | D1_VERSION_MINOR )

// Create the D1_VERSION_STRING macro
#define D1_VERSION_STRING  D1_XSTR( D1_XDOT( D1_VERSION_MAJOR, D1_VERSION_MINOR) )

//--------------------------------------------------------------------------

static const char g_versionid[] = "V" D1_VERSION_STRING ;

#ifdef __ZEPHYR__
#define DAVE2D_CLK DT_CLOCKS_CELL(DAVE2D_DEV, clkid)
static const struct device *const clock_ctrl = DEVICE_DT_GET(DT_NODELABEL(clock));
#endif

//--------------------------------------------------------------------------
//
const char * d1_getversionstring()
{
    return g_versionid;
}

//--------------------------------------------------------------------------
//
int d1_getversion()
{
    return D1_VERSION;
}

//--------------------------------------------------------------------------
//
static void d1_clock_enable()
{
#ifdef __ZEPHYR__
    if (!device_is_ready(clock_ctrl)) {
        return;
    }

    clock_control_on(clock_ctrl, (clock_control_subsys_t)DAVE2D_CLK);
#else
    REG32_SET_ONE_BIT(CLKCTL_PER_MST->PERIPH_CLK_ENA, 8);
#endif
}

//--------------------------------------------------------------------------
//
static void d1_clock_disable()
{
#ifdef __ZEPHYR__
    if (!device_is_ready(clock_ctrl)) {
        return;
    }

    clock_control_off(clock_ctrl, (clock_control_subsys_t)DAVE2D_CLK);
#else
    REG32_CLR_ONE_BIT(CLKCTL_PER_MST->PERIPH_CLK_ENA, 8);
#endif
}


//--------------------------------------------------------------------------
//
d1_device * d1_opendevice( long flags )
{
    d1_device * handle = d1_allocmem(sizeof(d1_device_intern));

    D1_DEV(handle)->flags = flags;

#if (D1_DLIST_INDIRECT == 1)
    D1_DEV(handle)->dlist_indirect = 1;
#endif

    // Enable D/AVE2D clock
    d1_clock_enable();

    // Enable interrupts
    d1_irq_enable();
    d1_set_isr_context(handle);

    return handle;
}

//--------------------------------------------------------------------------
//
int d1_closedevice( d1_device *handle )
{
    // Disable interrupts
    d1_irq_disable();
    d1_set_isr_context(NULL);

    // Disable clock
    d1_clock_disable();

    d1_freemem(handle);

    return 1;
}

//--------------------------------------------------------------------------
//
int d1_devicesupported( d1_device *handle, int deviceid )
{
    switch (deviceid)
    {
        case D1_DAVE2D:
#if (D1_DLIST_INDIRECT == 1)
        case D1_DLISTINDIRECT:
#endif
            return 1;

        default:
            return 0;
    }
}

//--------------------------------------------------------------------------
//
long d1_getregister( d1_device *handle, int deviceid, int index )
{
    switch (deviceid)
    {
        case D1_DAVE2D:
            return D1_REG(index);

#if (D1_DLIST_INDIRECT == 1)
        case D1_DLISTINDIRECT:
            return D1_DEV(handle)->dlist_indirect;
#endif

        default:
            return 0;
    }
}

//--------------------------------------------------------------------------
//
void d1_setregister( d1_device *handle, int deviceid, int index, long value )
{
    switch (deviceid)
    {
        case D1_DAVE2D:
#if (D1_DLIST_INDIRECT == 1)
            if (index == D2_DLISTSTART && D1_DEV(handle)->dlist_indirect)
            {
                long *dlist_ptr = (long *) value;
                D1_DEV(handle)->dlist_start = dlist_ptr + 1;
                D1_REG(index) = *dlist_ptr;
            }
            else
            {
                D1_REG(index) = value;
            }
#else
            D1_REG(index) = value;
#endif

            break;

#if (D1_DLIST_INDIRECT == 1)
        case D1_DLISTINDIRECT:
            D1_DEV(handle)->dlist_indirect = value;
#endif

        default:
            break;
    }
}

