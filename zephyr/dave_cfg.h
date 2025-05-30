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
// File:        dave_cfg.h
//
// Description: This file contains the D/AVE D1 configuration definitions.
//-----------------------------------------------------------------------------

#ifndef __1_dave_cfg_h_H
#define __1_dave_cfg_h_H

#ifdef CONFIG_D1_MALLOC_D0LIB
#define D1_MALLOC_D0LIB     0
#endif

#ifdef CONFIG_D1_MALLOC_STDLIB
#define D1_MALLOC_STDLIB    1
#endif

#ifdef CONFIG_D1_MALLOC_CUSTOM
#define D1_MALLOC_CUSTOM    2
#endif

#ifdef CONFIG_DLIST_INDIRECT
#define D1_DLIST_INDIRECT   1
#else
#define D1_DLIST_INDIRECT   0
#endif

#ifdef CONFIG_WITH_MM_FIXED_RANGE_FIXED_BLKCNT
#define WITH_MM_FIXED_RANGE_FIXED_BLKCNT
#endif

#ifdef CONFIG_WITH_MM_FIXED_RANGE
#define WITH_MM_FIXED_RANGE
#endif

#ifdef CONFIG_WITH_MM_DYNAMIC
#define WITH_MM_DYNAMIC
#endif

#endif
