##=============================================================================
##
##	variant.S
##
##	BFIN BF60x variant code
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
## Contributors:	Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
## Date:	2013-08-23
## Purpose:	BFIN BF60x code
##              
##		
##		
##
######DESCRIPTIONEND####
##
##=============================================================================
#include <pkgconf/hal_bfin.h>
#include <pkgconf/hal_bfin_bf60x.h>


#define CYGNUM_HAL_ISR_COUNT                   156
#define CYGNUM_CALL_IF_TABLE_SIZE              64

    .section ".interrupt_vectors","ax"

## entry
## R0 base address
## R1 size
    .globl _hal_bfin_dcache_flush
_hal_bfin_dcache_flush:
    LINK 0x0
    [--SP] = LC0
    [--SP] = LB0
    [--SP] = LT0

# divide the size of the page by 4
    R1 >>= 2
    P1 = R1
    P0 = R0

    LOOP _hal_bfin_dcache_flush_page LC0 = P1
    LOOP_BEGIN _hal_bfin_dcache_flush_page;      // # start of loop
        FLUSHINV[P0++]
    LOOP_END _hal_bfin_dcache_flush_page 
    SSYNC
    

    UNLINK
    RTS



    .section ".system_data","aw"

//#ifdef CYGSEM_HAL_VIRTUAL_VECTOR_SUPPORT
    // Vectors used to communicate between eCos and ROM environments
        .balign 32

hal_virtual_vector_table:
        .globl  hal_virtual_vector_table
_hal_virtual_vector_table:
        .globl  _hal_virtual_vector_table
        .rept   CYGNUM_CALL_IF_TABLE_SIZE
        .long   0
        .endr
//#endif

    .globl  hal_vsr_table

hal_vsr_table:
    .globl  _hal_vsr_table

_hal_vsr_table: 
    .rept  CYGNUM_HAL_ISR_COUNT
    .long   __default_interrupt_vsr 
    .endr
    .rept   48
    .long   __default_exception_vsr 
    .endr

    .extern _hal_default_isr
    .section ".system_data","aw"

    .globl  hal_interrupt_handlers
hal_interrupt_handlers:
    .globl  _hal_interrupt_handlers
_hal_interrupt_handlers:
    .rept   CYGNUM_HAL_ISR_COUNT
    .long   _hal_default_isr
    .endr

    .globl  hal_interrupt_data
hal_interrupt_data:
    .globl  _hal_interrupt_data
_hal_interrupt_data:
    .rept   CYGNUM_HAL_ISR_COUNT
    .long   0
    .endr

    .globl  hal_interrupt_objects
hal_interrupt_objects:
    .globl  _hal_interrupt_objects
_hal_interrupt_objects:
    .rept   CYGNUM_HAL_ISR_COUNT
    .long   0
    .endr

    .align  4
    .globl  hal_interrupt_sic_assign_mask
hal_interrupt_sic_assign_mask:
    .globl  _hal_interrupt_sic_assign_mask
_hal_interrupt_sic_assign_mask:
    .rept   7
    .long   0
    .endr

##-----------------------------------------------------------------------------
## end of variant.S
	
