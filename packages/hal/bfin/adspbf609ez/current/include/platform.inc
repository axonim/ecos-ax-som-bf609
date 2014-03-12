#ifndef CYGONCE_HAL_PLATFORM_INC
#define CYGONCE_HAL_PLATFORM_INC
##=============================================================================
##
##	platform.inc
##
##	ADSP-BF609-EZ board assembler header file
##
##=============================================================================
#####ECOSGPLCOPYRIGHTBEGIN####
## -------------------------------------------
## This file is part of eCos, the Embedded Configurable Operating System.
## Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
##
## eCos is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 or (at your option) any later version.
##
## eCos is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with eCos; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
##
## As a special exception, if other files instantiate templates or use macros
## or inline functions from this file, or you compile this file and link it
## with other works to produce a work based on this file, this file does not
## by itself cause the resulting work to be covered by the GNU General Public
## License. However the source code for this file must still be made available
## in accordance with section (3) of the GNU General Public License.
##
## This exception does not invalidate any other reasons why a work based on
## this file might be covered by the GNU General Public License.
##
## Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
## at http://sources.redhat.com/ecos/ecos-license/
## -------------------------------------------
#####ECOSGPLCOPYRIGHTEND####
##=============================================================================
#######DESCRIPTIONBEGIN####
##
## Author(s): 	Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
## Contributors:Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
## Date:	    2013-11-06
## Purpose:	    ADSP-BF609-EZ board definitions.
## Description:	This file contains various definitions and macros that are
##              useful for writing assembly code for the ADSP-BF609-EZ board.
## Usage:
##		#include <cyg/hal/platform.inc>
##		...
##		
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <cyg/hal/bfin.inc>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/plf_io.h>
#include <cyg/hal/bfin-regs.h>
#include <pkgconf/hal_bfin_bf60x.h>
#include <pkgconf/hal_bfin.h>

## /* Equation from section 17 (p17-46) of BF533 HRM */

## original source is uboot, thanks

#ifdef CYGPKG_HAL_BFIN_BF533

#define CONFIG_SCLK_HZ (CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000*1000)
    #if ( CONFIG_SCLK_HZ > 119402985 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_7
        #define SDRAM_tRAS_num  7
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ > 104477612 ) && ( CONFIG_SCLK_HZ <= 119402985 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_6
        #define SDRAM_tRAS_num  6
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  89552239 ) && ( CONFIG_SCLK_HZ <= 104477612 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_5
        #define SDRAM_tRAS_num  5
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  74626866 ) && ( CONFIG_SCLK_HZ <=  89552239 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_4
        #define SDRAM_tRAS_num  4
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  66666667 ) && ( CONFIG_SCLK_HZ <= 74626866 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_3
        #define SDRAM_tRAS_num  3
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  59701493 ) && ( CONFIG_SCLK_HZ <= 66666667 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_4
        #define SDRAM_tRAS_num  3
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  44776119 ) && ( CONFIG_SCLK_HZ <=  59701493 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_3
        #define SDRAM_tRAS_num  3
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  29850746 ) && ( CONFIG_SCLK_HZ <=  44776119 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_2
        #define SDRAM_tRAS_num  2
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ <=  29850746 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_1
        #define SDRAM_tRAS_num  1
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif

#define SDRAM_Tref  64       /* Refresh period in milliseconds   */
#define SDRAM_NRA   8192     /* Number of row addresses in SDRAM */
#define SDRAM_CL    CL_2

#define mem_SDRRC   ((( CYGNUM_HAL_BFIN_SYSTEM_CLOCK * 1000) * SDRAM_Tref)  / SDRAM_NRA) - (SDRAM_tRAS_num + SDRAM_tRP_num)
#define mem_SDGCTL  ( SCTLE | SDRAM_CL | SDRAM_tRAS | SDRAM_tRP | SDRAM_tRCD | SDRAM_tWR | PSS )
#define mem_SDBCTL  (EBCAW_11 | EBSZ_128 | EBE)
#endif

#ifdef CYGPKG_HAL_BFIN_BF537

#define CONFIG_SCLK_HZ (CYGNUM_HAL_BFIN_SYSTEM_CLOCK*1000*1000)
    #if ( CONFIG_SCLK_HZ > 119402985 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_7
        #define SDRAM_tRAS_num  7
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ > 104477612 ) && ( CONFIG_SCLK_HZ <= 119402985 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_6
        #define SDRAM_tRAS_num  6
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  89552239 ) && ( CONFIG_SCLK_HZ <= 104477612 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_5
        #define SDRAM_tRAS_num  5
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  74626866 ) && ( CONFIG_SCLK_HZ <=  89552239 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_4
        #define SDRAM_tRAS_num  4
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  66666667 ) && ( CONFIG_SCLK_HZ <= 74626866 )
        #define SDRAM_tRP   TRP_2
        #define SDRAM_tRP_num   2
        #define SDRAM_tRAS  TRAS_3
        #define SDRAM_tRAS_num  3
        #define SDRAM_tRCD  TRCD_2
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  59701493 ) && ( CONFIG_SCLK_HZ <= 66666667 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_4
        #define SDRAM_tRAS_num  3
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  44776119 ) && ( CONFIG_SCLK_HZ <=  59701493 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_3
        #define SDRAM_tRAS_num  3
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ >  29850746 ) && ( CONFIG_SCLK_HZ <=  44776119 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_2
        #define SDRAM_tRAS_num  2
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif
    #if ( CONFIG_SCLK_HZ <=  29850746 )
        #define SDRAM_tRP   TRP_1
        #define SDRAM_tRP_num   1
        #define SDRAM_tRAS  TRAS_1
        #define SDRAM_tRAS_num  1
        #define SDRAM_tRCD  TRCD_1
        #define SDRAM_tWR   TWR_2
    #endif

#define SDRAM_Tref  64       /* Refresh period in milliseconds   */
#define SDRAM_NRA   4096     /* Number of row addresses in SDRAM */
#define SDRAM_CL    CL_3     /* 2 */

#define mem_SDRRC   ((( CYGNUM_HAL_BFIN_SYSTEM_CLOCK * 1000) * SDRAM_Tref)  / SDRAM_NRA) - (SDRAM_tRAS_num + SDRAM_tRP_num)
#define mem_SDGCTL  ( SCTLE | SDRAM_CL | SDRAM_tRAS | SDRAM_tRP | SDRAM_tRCD | SDRAM_tWR | PSS )
#define mem_SDBCTL  (EBCAW_10 | EBSZ_64 | EBE)
#endif

#ifndef HI
#define HI(X) (X >> 16)
#endif
#ifndef LO
#define LO(X) (X &0xFFFF)
#endif

#ifndef CYGPKG_HAL_BFIN_CACHE_DEFINED

.macro  hal_led_init

#ifdef CYGPKG_HAL_BFIN_BF533
    ##set up LED control
    P0.H = 0xFFC0
    P0.L = 0x0730
    R0.H = 0x0000
    R0.L = 0x1D
    W[P0] = R0
    SSYNC
#endif
#ifdef CYGPKG_HAL_BFIN_BF537
    ##set up LED control
##    P0.H = 0xFFC0
##    P0.L = 0x3200
##    R0.L = W[P0]
##    R1.L = 0xF03F
##    R0   = R0 & R1
##    W[P0] = R0
##    P0.H = 0xFFC0
##    P0.L = 0x0730
##    R0.L = W[P0]
##    R1.L = 0x0FC0
##    R0   = R0 | R1
##    W[P0] = R0
##    P0.H = 0xFFC0
##    P0.L = 0x0740
##    R0.L = W[P0]
##    R1.L = 0xF03F
##    R0   = R0 & R1
##    W[P0] = R0
##    P0.H = 0xFFC0
##    P0.L = 0x0700
##    R0.L = W[P0]
##    R1.L = 0xF03F
##    R0   = R0 & R1
##    W[P0] = R0
##    SSYNC
#endif
    .endm


    .macro  hal_led_support idx
#ifdef CYGPKG_HAL_BFIN_BF53x
        P0.H = 0xFFC0
        P0.L = 0x0700
        R1   = W[P0]
        R0.L = \idx
#ifdef CYGPKG_HAL_BFIN_BF533
        R0  <<= 2
        R0   = ~R0
        R1   = R0 & R1
#endif
#ifdef CYGPKG_HAL_BFIN_BF537
##        R0  <<= 6
##        R1   = R0 | R1
#endif
        W[P0] = R1
#endif
    .endm

    .macro  hal_led_stack_support idx
#ifdef CYGPKG_HAL_BFIN_BF53x
        [--SP] = P0
        [--SP] = R0
        [--SP] = R1
        [--SP] = R2
        P0.H = 0xFFC0
        P0.L = 0x0700
        R1   = W[P0]
        R0.L = \idx
#ifdef CYGPKG_HAL_BFIN_BF533
        R0  <<= 2
        R0   = ~R0
        R2.L = 0x001C
        R1   = R1 | R2
        R1   = R0 & R1
#endif
#ifdef CYGPKG_HAL_BFIN_BF537
##        R0  <<= 6
##        R2.L = 0xF03F
##        R1   = R1 & R2
##        R1   = R0 | R1
#endif
        W[P0] = R1
        R2    = [SP++]
        R1    = [SP++]
        R0    = [SP++]
        P0    = [SP++]
#endif
    .endm

#endif 

#ifndef CYGPKG_HAL_BFIN_INIT_ASYNC
.macro  hal_async_init
        #based on uboot
        
        // EBIU_AMBCTL0
        P0.H = 0xFFC0
        P0.L = 0x0A04
        R0.H = 0xBBC3
        R0.L = 0xBBC3
        [P0] = R0
        SSYNC
        
        // EBIU_AMBCTL1
        P0.H = 0xFFC0
        P0.L = 0x0A08
        R0.H = 0x99B3
        R0.L = 0x9983
        [P0] = R0
        SSYNC

        
        // EBIU_AMGCTL
        P0.H  = 0xFFC0
        P0.L  = 0x0A00
        R0    = 0xFF
        W[P0] = R0
        SSYNC
    .endm
#endif

#ifndef CYGPKG_HAL_BFIN_INIT_SDRAM
.macro  hal_sdram_init
    ## based on uboot

    // SDRAM Global Control Register - global programmable parameters
    // Disable self-refresh
    P2.H = 0xFFC0
    P2.L = 0x0A10
    // Write in the new value in the register
    R0.L = LO(mem_SDGCTL)
    R0.H = HI(mem_SDGCTL)
///  R0.H = 0x8011
///  R0.L = 0x1109
    [P2] = R0
    SSYNC

    // SDRAM Memory Bank Control Register - bank specific parameters
    P0.H = 0xFFC0
    P0.L = 0x0A14
    R0.H = HI(mem_SDBCTL)
    R0.L = LO(mem_SDBCTL)
/// R0.L = 0x3F
    W[P0] = R0.L
    SSYNC

    // EBIU_SDRRC
    P0.H = 0xFFC0
    P0.L = 0x0A18
//  R0.L = mem_SDRRC
    R0.L  = 0x100
    W[P0] = R0.L
    SSYNC

    .endm
#endif

#------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_PLATFORM_INC
# end of platform.inc
