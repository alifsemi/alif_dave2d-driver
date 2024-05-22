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
// File:        dave_cfg.c
// Description:
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <RTE_Device.h>
#include "dave_cfg.h"
#include "hal_data.h"

//-----------------------------------------------------------------------------
// Extern variables
//-----------------------------------------------------------------------------
// Display configuration structure
// NOTE: Referred in unused function lv_draw_dave2d_cf_fb_get()
//       Should be initialized with RTE_CDC200_PIXEL_FORMAT */
const display0_cfg g_display0_cfg = {{RTE_CDC200_PIXEL_FORMAT}};

//-----------------------------------------------------------------------------
