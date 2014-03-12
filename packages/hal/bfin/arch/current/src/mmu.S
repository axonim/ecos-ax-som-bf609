##=============================================================================
##
##	mmu.S
##
##	MMU handling code
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
## Author(s): 	andre
## Date:	    2006-09-16
## Purpose:	    BFIN exception vectors
## Description:	This file defines the code placed into the exception
##              vectors. It also contains the first level default VSRs
##		        that save and restore state for both exceptions and
##		        interrupts.
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <pkgconf/system.h>	
#include <pkgconf/hal.h>

#ifdef CYGPKG_KERNEL
#include <pkgconf/kernel.h>	
#endif

#define HAL_BFIN_IMPL_CAUSE
			
#include <cyg/hal/arch.inc>	
#include <cyg/hal/hal_arch.h>	
#include <cyg/hal/bfin-regs.h>

	
#ifdef at
#undef at
#endif

    .section ".interrupt_vectors","ax"




.globl _hal_bfin_dcpld_handle_protection;
_hal_bfin_dcpld_handle_protection:

#define DCPLD_STATUS 0xFFE00008
#define DCPLD_BASE   0xFFE00200

    P0.H = (DCPLD_STATUS >> 16)
    P0.L = (DCPLD_STATUS & 0xFFFF)

    R1   = [P0]
    R2   = R2 ^ R2
    R2.H = 0x3
    R3   = R2 & R1
    
    CC   = R2 == R3
    IF !CC JUMP hal_bfin_cpld_handle_protection

    # we do have a supervisor writing access to a page

    R0   = R1
    R0.H = 0x0

    R1.L = SIGNBITS R0
    R1   = R1.L (Z)
    R0   =  30
    R0   = R0 - R1

    R1.H = (DCPLD_BASE >> 16)
    R0   = R0 << 2
    R1.L = (DCPLD_BASE & 0xFFFF)

    R1   = R0 + R1
    P0   = R1

    R0   = [P0]

    R1.H = 0x0
    R1.L = 0x5090
    R2.H = 0x0
    R2.L = 0x1010

    R1   = R0 & R1
    CC   = R2 == R1
    IF !CC JUMP hal_bfin_cpld_handle_protection

    BITSET(R0,7)
    [P0] = R0
    R0 = 0x0
    RTS

hal_bfin_cpld_handle_protection:
    R0 = 0x1
    RTS
