
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
// File:        dave_memory.c
// Description: This file defines the D/AVE driver memory management functions.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <string.h>
#include "RTE_Components.h"
#include CMSIS_device_header

#include "dave_cfg.h"
#include "dave_base.h"
#include "global_map.h"
#if (D1_MEM_ALLOC == D1_MALLOC_D0LIB)
#include "dave_d0lib.h"
#elif (D1_MEM_ALLOC == D1_MALLOC_STDLIB)
#include <malloc.h>
#endif
#include "dave_base_intern.h"
#include "dave_registermap.h"
#include "dave_driver.h"

//--------------------------------------------------------------------------
//
void * d1_allocvidmem( d1_device *handle, int memtype, unsigned int size )
{
    (void) handle;
    (void) memtype;

#if (D1_MEM_ALLOC == D1_MALLOC_D0LIB)
    return d0_allocvidmem(size);
#elif (D1_MEM_ALLOC == D1_MALLOC_STDLIB)
    return malloc(size);
#else
    return d1_malloc(size);
#endif
}

//--------------------------------------------------------------------------
//
void d1_freevidmem( d1_device *handle, int memtype, void *ptr )
{
    (void) handle;
    (void) memtype;

#if (D1_MEM_ALLOC == D1_MALLOC_D0LIB)
    return d0_freevidmem(ptr);
#elif (D1_MEM_ALLOC == D1_MALLOC_STDLIB)
    return free(ptr);
#else
    return d1_free(ptr);
#endif
}

//--------------------------------------------------------------------------
//
int d1_queryarchitecture( d1_device *handle )
{
    (void) handle;

    return d1_ma_unified;
}

static inline void d1_setcachectl(long flags, long bits)
{
    long cachectl = bits | D2C_CACHECTL_ENABLE_FB | D2C_CACHECTL_ENABLE_TX;

    if (flags & d2_df_no_fbcache)
    {
        cachectl &= ~D2C_CACHECTL_ENABLE_FB;
    }

    if (flags & d2_df_no_texcache)
    {
        cachectl &= ~D2C_CACHECTL_ENABLE_TX;
    }

    D1_REG(D2_CACHECTL) = cachectl;
}

//--------------------------------------------------------------------------
//
int d1_cacheblockflush( d1_device *handle, int memtype, const void *ptr, unsigned int size )
{
    (void) handle;
    (void) memtype;

    SCB_CleanInvalidateDCache_by_Addr((void*)ptr, size);
    return 1;
}

//--------------------------------------------------------------------------
//
int d1_cacheflush( d1_device *handle, int memtype )
{
    if (memtype & d1_mem_any)
    {
        memtype |= d1_mem_display | d1_mem_texture | d1_mem_dlist;
    }

    if (memtype & d1_mem_display)
    {
        d1_setcachectl(D1_DEV(handle)->flags, D2C_CACHECTL_FLUSH_FB);
    }

    if (memtype & d1_mem_texture)
    {
        d1_setcachectl(D1_DEV(handle)->flags, D2C_CACHECTL_FLUSH_TX);
    }

    if (memtype & d1_mem_dlist)
    {
        SCB_CleanInvalidateDCache();
    }
}

//--------------------------------------------------------------------------
//
void * d1_maptovidmem( d1_device *handle, void *ptr )
{
    (void) handle;
    (void) ptr;
}

//--------------------------------------------------------------------------
//
void * d1_mapfromvidmem( d1_device *handle, void *ptr )
{
    (void) handle;
    (void) ptr;
}

//--------------------------------------------------------------------------
//
int d1_copytovidmem( d1_device *handle, void *dst, const void *src, unsigned int size, int flags )
{
    (void) handle;
    (void) flags;

    memcpy(dst, src, size);

    return 1;
}

//--------------------------------------------------------------------------
//
void * d1_allocmem( unsigned int size )
{
#if (D1_MEM_ALLOC == D1_MALLOC_D0LIB)
    return d0_allocmem(size);
#elif (D1_MEM_ALLOC == D1_MALLOC_STDLIB)
    return malloc(size);
#else
    return d1_malloc(size);
#endif
}

//--------------------------------------------------------------------------
//
void d1_freemem( void *ptr )
{
#if (D1_MEM_ALLOC == D1_MALLOC_D0LIB)
    d0_freemem(ptr);
#elif (D1_MEM_ALLOC == D1_MALLOC_STDLIB)
    free(ptr);
#else
    d1_free(ptr);
#endif
}

//--------------------------------------------------------------------------
//
unsigned int d1_memsize( void * ptr)
{
#if (D1_MEM_ALLOC == D1_MALLOC_D0LIB)
    return d0_memsize(ptr);
#else
    // TOOD: figure out how to get memsize using default malloc
    return 1;
#endif
}
