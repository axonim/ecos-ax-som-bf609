#ifndef CYGONCE_HAL_VARIANT_INC
#define CYGONCE_HAL_VARIANT_INC
##=============================================================================
##
##	variant.inc
##
##	BFIN BF60x family assembler header file
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
## Date:	    2013-09-10
## Purpose:	    BF60x family definitions.
## Description:	This file contains various definitions and macros
## Usage:
##		#include <cyg/hal/variant.inc>
##		...
##		
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <pkgconf/hal.h>
#include <pkgconf/hal_bfin_bf60x.h>
	
#include <cyg/hal/bfin.inc>

#include <cyg/hal/platform.inc>

#ifndef CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP7
#define CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP7 0
#endif

#ifndef CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP6
#define CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP6 0
#endif

#ifndef CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP5
#define CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP5 0
#endif

#ifndef CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP4
#define CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP4 0
#endif

#ifndef CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP3
#define CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP3 0
#endif

#ifndef CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP2
#define CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP2 0
#endif

#ifndef CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP1
#define CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP1 0
#endif

#ifndef CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP0
#define CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP0 0
#endif

#define HAL_BFIN_SIC_MASK_HI    ((CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP7<<12) | \
                                 (CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP6<<8)  | \
                                 (CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP5<<4)  | \
                                 (CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP4) )

#define HAL_BFIN_SIC_MASK_LO    ((CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP3<<12) | \
                                 (CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP2<<8)  | \
                                 (CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP1<<4)  | \
                                 (CYGNUM_HAL_BFIN_SIC_UNMASK_GROUP0) )


#define HAL_BFIN_CPU_ID_MASK 0x0FFFFFFF
#define HAL_BFIN_REV_ID_MASK 0xF0000000



#ifdef CYGPKG_HAL_BFIN_BF537
    #define HAL_BFIN_CPU_ID  0x027c80cb
#endif
#ifdef CYGPKG_HAL_BFIN_BF533
    #define HAL_BFIN_CPU_ID  0x027a50cb
#endif
#ifdef CYGPKG_HAL_BFIN_BF609
    #define HAL_BFIN_CPU_ID  0x00000000 //FIXME temporary fake value
#endif
#ifndef HAL_BFIN_CPU_ID
#error Not a valid CPU variant for this variant HAL
#endif

#ifdef CYGPKG_HAL_BFIN_BF53x
.macro  hal_bfin_init_sic_mask
    R0.H = HAL_BFIN_SIC_MASK_HI
    R0.L = HAL_BFIN_SIC_MASK_LO
    P0.H = 0xFFC0
    P0.L = 0x010C
    [P0] = R0
    SSYNC
    
.endm

.macro  hal_bfin_init_sic_assign
    P0.H = hal_interrupt_sic_assign_mask
    P0.L = hal_interrupt_sic_assign_mask

#ifdef CYGPKG_HAL_BFIN_BF537
    R0     = R0 ^ R0
    R0.L   = 0x7
    [P0++] = R0

    R0     = R0 ^ R0
    R0.L   = 0x18
    [P0++] = R0

    R0     = R0 ^ R0
    R0.L   = 0x1E0
    [P0++] = R0

    R0     = R0 ^ R0
    R0.L   = 0x7E00
    [P0++] = R0

    R0.H   = 0x7
    R0.L   = 0x8000
    [P0++] = R0

    R0.H   = 0x1ff8
    R0.L   = 0x0
    [P0++] = R0

    R0.H   = 0xE000
    R0.L   = 0x0
    [P0]   = R0
#endif

#ifdef CYGPKG_HAL_BFIN_BF533
    R0     = R0 ^ R0
    R0.L   = 0x7F
    [P0++] = R0

    R0     = R0 ^ R0
    R0.L   = 0x180
    [P0++] = R0

    R0     = R0 ^ R0
    R0.L   = 0x1E00
    [P0++] = R0

    R0     = R0 ^ R0
    R0.L   = 0xE000
    [P0++] = R0

    R0.H   = 0x7
    R0.L   = 0x0000
    [P0++] = R0

    R0.H   = 0x18
    R0.L   = 0x0
    [P0++] = R0

    R0.H   = 0xE0
    R0.L   = 0x0
    [P0]   = R0
#endif

.endm
#endif //CYGPKG_HAL_BFIN_BF53x

.macro  hal_bfin_init_sec
    P0.H = 0xFFCA   // SEC0_GCTL
    P0.L = 0x4000
    R0.H = 0x0000   // Reset SEC
    R0.L = 0x0002
    [P0] = R0

    R0.H = 0x0000   // Enable SEC
    R0.L = 0x0001
    [P0] = R0

    P0.H = 0xFFCA   // SEC0_CCTL0
    P0.L = 0x4400
    R0.H = 0x0000   // Enable SCI
    R0.L = 0x0001
    [P0] = R0
.endm

#------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_VARIANT_INC
# end of variant.inc
