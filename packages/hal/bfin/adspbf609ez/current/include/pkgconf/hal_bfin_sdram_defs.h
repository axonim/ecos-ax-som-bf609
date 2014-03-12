#ifndef CYGONCE_HAL_BFIN_STAMP_DEFS_H
#define CYGONCE_HAL_BFIN_STAMP_DEFS_H
//========================================================================
//
//      hal_bfin_stamp_defs.h
//
//      Register defines for BFIN processors
//
//========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:          2013-10-19
// Purpose:       
// Description:   Register defines for BFIN processors
// Usage:         
//
//####DESCRIPTIONEND####
//
//========================================================================

#include <pkgconf/hal.h>
#include <pkgconf/hal_bfin.h>

#define CONFIG_MEM_MT48LC64M4A2FB_7E


// values based on uboot

#if (CONFIG_MEM_MT48LC64M4A2FB_7E)
    /*SDRAM INFORMATION: */
    #define SDRAM_Tref  64       /* Refresh period in milliseconds   */
    #define SDRAM_NRA   8192     /* Number of row addresses in SDRAM */
    #define SDRAM_CL    CL_2
#endif

#if ( CONFIG_MEM_MT48LC16M16A2TG_75  ||  CONFIG_MEM_MT48LC64M4A2FB_7E || CONFIG_MEM_MT48LC16M8A2TG_75)
    #if ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 > 119402985 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_7
        #define SDRAM_tRAS_num  7
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 > 104477612 ) && ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 <= 119402985 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_6
        #define SDRAM_tRAS_num  6
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 >  89552239 ) && ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 <= 104477612 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_5
        #define SDRAM_tRAS_num  5
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 >  74626866 ) && ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 <=  89552239 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_4
        #define SDRAM_tRAS_num  4
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 >  66666667 ) && ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 <= 74626866 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_3
        #define SDRAM_tRAS_num  3
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 >  59701493 ) && ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 <= 66666667 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_4
        #define SDRAM_tRAS_num  3
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 >  44776119 ) && ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 <=  59701493 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_3
        #define SDRAM_tRAS_num  3
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 >  29850746 ) && ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 <=  44776119 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_2
        #define SDRAM_tRAS_num  2
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000000 <=  29850746 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_1
        #define SDRAM_tRAS_num  1
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
#endif

#endif // ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#endif // ifndef CYGONCE_HAL_BFIN_REGS_H
