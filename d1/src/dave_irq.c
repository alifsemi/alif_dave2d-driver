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
// File:        dave_irq.c
// Description: This file defines the D/AVE driver IRQ setting functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stddef.h>
#include "RTE_Components.h"
#include CMSIS_device_header

#include "dave_cfg.h"
#include "dave_base_intern.h"
#include "dave_base.h"
#include "dave_registermap.h"

#if defined(LV_USE_OS) && (LV_USE_OS == LV_OS_FREERTOS)
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#if configUSE_COUNTING_SEMAPHORES
#include "semphr.h"
#define USE_SEMAPHORE
#endif // USE_SEMAPHORE
#endif // LV_OS_FREERTOS

#define D1_IRQCTL_ENABLE    (D2IRQCTL_CLR_FINISH_DLIST | D2IRQCTL_CLR_FINISH_ENUM\
                            | D2IRQCTL_ENABLE_FINISH_DLIST)
#define D1_IRQCTL_CLEAR     D1_IRQCTL_ENABLE
#define D1_IRQCTL_DISABLE   (D2IRQCTL_CLR_FINISH_DLIST | D2IRQCTL_CLR_FINISH_ENUM)

static d1_device_intern *s_isr_context;

#if defined(USE_SEMAPHORE)
static SemaphoreHandle_t irqSemaphoreHandle = NULL;
#else
static volatile unsigned int s_dlists_done = 0;
#endif


//--------------------------------------------------------------------------
//
void d1_irq_enable()
{
    // Clear all interrupts and enable DLIST IRQ
    D1_REG(D2_IRQCTL) = D1_IRQCTL_ENABLE;

#if defined(LV_USE_OS) && (LV_USE_OS == LV_OS_FREERTOS)
    // Set DAVE 2D interrupt priority to FreeRTOS kernel level
    NVIC_SetPriority(GPU2D_IRQ_IRQn, configKERNEL_INTERRUPT_PRIORITY-1);
#endif

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(GPU2D_IRQ_IRQn);
}

//--------------------------------------------------------------------------
//
void d1_irq_disable()
{
    // Clear all interrupts and disable DLIST IRQ
    D1_REG(D2_IRQCTL) = D1_IRQCTL_DISABLE;

    // Disable interrupt in NVIC
    NVIC_DisableIRQ(GPU2D_IRQ_IRQn);
}

//--------------------------------------------------------------------------
//
int d1_queryirq( d1_device *handle, int irqmask, int timeout )
{
    (void) handle;
    (void) irqmask;

    if (timeout == d1_to_no_wait)
    {
        return 1;
    }

#if defined(USE_SEMAPHORE)
    if (irqSemaphoreHandle != NULL)
    {
        if (xSemaphoreTake(irqSemaphoreHandle,
                          (timeout/10)/portTICK_PERIOD_MS) == pdTRUE)
        {
            return GPU2D_IRQ_IRQn;
        }
    }
#else
    for (; timeout > 0; --timeout)
    {
        if (s_dlists_done)
        {
            --s_dlists_done;
            return GPU2D_IRQ_IRQn;
        }
    }
#endif

    return 0;
}

//--------------------------------------------------------------------------
//
void d1_set_isr_context( void *context )
{
    s_isr_context = (d1_device_intern *) context;

#if defined(USE_SEMAPHORE)
    if (irqSemaphoreHandle != NULL)
    {
        vSemaphoreDelete(irqSemaphoreHandle);
    }
    if (context != NULL)
    {
        irqSemaphoreHandle = xSemaphoreCreateCounting(10, 0);
    }
#endif
}

//--------------------------------------------------------------------------
//
void GPU2D_IRQHandler( void )
{
    // Get D/AVE2D interrupt status
    long dave_status = D1_REG(D2_STATUS);

    // Clear all interrupts triggered
    D1_REG(D2_IRQCTL) = D1_IRQCTL_CLEAR;

    // Check if DLIST IRQ triggered
    if (dave_status & D2C_IRQ_DLIST)
    {
#if (D1_DLIST_INDIRECT == 1)
        if (s_isr_context->dlist_indirect
            && ((void *) *s_isr_context->dlist_start) != NULL)
        {
            D1_REG(D2_DLISTSTART) = *s_isr_context->dlist_start;

            ++s_isr_context->dlist_start;
        }
#endif

#if defined(USE_SEMAPHORE)
        BaseType_t context_switch = pdFALSE;
        xSemaphoreGiveFromISR(irqSemaphoreHandle, &context_switch);
        portYIELD_FROM_ISR(context_switch);
#else
        ++s_dlists_done;
#endif
    }
}
