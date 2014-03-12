#ifndef CYGONCE_HAL_VAR_ARCH_H
#define CYGONCE_HAL_VAR_ARCH_H

//==========================================================================
//
//      var_arch.h
//
//      Architecture specific abstractions
//
//==========================================================================
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
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Contributors: Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:         2013-09-02
// Purpose:      Define architecture abstractions
// Description:  This file contains any extra or modified definitions for
//               this variant of the architecture.
// Usage:        #include <cyg/hal/var_arch.h>
//              
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>

#ifdef CYGSEM_HAL_USE_ROM_MONITOR_CygMon
externC int
hal_diag_irq_check(int vector);

#define HAL_DIAG_IRQ_CHECK(_vector_, _ret_) \
CYG_MACRO_START                             \
_ret_ =  hal_diag_irq_check((_vector_));    \
CYG_MACRO_END
#endif


// get the value of the dspid register
#define HAL_GET_CPUID(__dspid__) \
        __asm __volatile(   "P0.H = 0xFFE0;" \
                            "P0.L = 0x5000;" \
                            "R0   = [P0];"   \
                            "%0   = R0;" : "=r"(__dspid__) : : "P0", "R0")

// get the value of the revision register this identifies the processor
// type out of the family as well?
#define HAL_GET_REVID(__revid__) \
        __asm __volatile(   "P0.H = 0xFFC0;" \
                            "P0.L = 0x0014;" \
                            "R0   = [P0];"   \
                            "%0   = R0;" : "=r"(__revid__) : : "P0", "R0")

/// Read the state of the respective GPIO pins (port = PORTA, PORTB etc.)
#define HAL_PORT_READ(port, pins) HAL_READ_UINT32(port##_DATA, pins)

/// Set or clear respective GPIO pins (1 - set, 0 - clear; port = PORTA, PORTB etc.)
#define HAL_PORT_WRITE(port, pins)  HAL_WRITE_UINT32(port##_DATA, pins)
/// Set specific pins without impacting other pins of the port (write 1 to set; port = PORTA, PORTB etc.)
#define HAL_PORT_WRITE_SET(port, pins) HAL_WRITE_UINT32(port##_DATA_SET, pins)
/// Clear specific pins without impacting other pins of the port (write 1 to clear; port = PORTA, PORTB etc.)
#define HAL_PORT_WRITE_CLR(port, pins) HAL_WRITE_UINT32(port##_DATA_CLR, pins)

/// Enable output mode and enables output drivers for GPIO pins (port = PORTA, PORTB etc.)
#define HAL_PORT_DIR_OUTPUT(port, pins) HAL_WRITE_UINT32(port##_DIR_SET, pins)

/// Enable input mode and enables input drivers for GPIO pins
#define HAL_PORT_DIR_INPUT(port, pins) \
    HAL_WRITE_UINT32(port##_DIR_CLR, pins); \
    HAL_WRITE_UINT32(port##_INEN_SET, pins)
    
/// Read writing arbitration settings (scb = SCB0, SCB1 etc.)
#define HAL_SCB_ARBW_READ(scb, mi, slot, res)   \
    CYG_MACRO_START \
    cyg_uint32 val = (slot & 0xFF) | 0xFF000000; \
    HAL_WRITE_UINT32(REG_##scb##_ARBW##mi, val); \
    HAL_READ_UINT32(REG_##scb##_ARBW##mi, val); \
    res = val & 0x000000FF; \
    CYG_MACRO_END

/// Write writing arbitration settings (scb = SCB0, SCB1 etc.)
#define HAL_SCB_ARBW_WRITE(scb, mi, slot, si) \
    CYG_MACRO_START \
    cyg_uint32 val = (slot << 24) | (si & 0xFF); \
    HAL_WRITE_UINT32(REG_##scb##_ARBW##mi, val); \
    CYG_MACRO_END

/// Read reading arbitration settings (scb = SCB0, SCB1 etc.)
#define HAL_SCB_ARBR_READ(scb, mi, slot, res)    \
    CYG_MACRO_START \
    cyg_uint32 val = (slot & 0xFF) | 0xFF000000; \
    HAL_WRITE_UINT32(REG_##scb##_ARBR##mi, val); \
    HAL_READ_UINT32(REG_##scb##_ARBR##mi, val); \
    res = val & 0x000000FF; \
    CYG_MACRO_END

/// Write reading arbitration settings (scb = SCB0, SCB1 etc.)
#define HAL_SCB_ARBR_WRITE(scb, mi, slot, si) \
    CYG_MACRO_START \
    cyg_uint32 val = (slot << 24) | (si & 0xFF); \
    HAL_WRITE_UINT32(REG_##scb##_ARBR##mi, val); \
    CYG_MACRO_END

/// Read the number of masters interfaces (MI) connected to SCB (scb = SCB0, SCB1 etc.)
#define HAL_SCB_MASTERS_READ(scb, val) \
    HAL_READ_UINT32(REG_##scb##_MASTERS, val)

/// Read the number of slaves interfaces (SI) connected to SCB (scb = SCB0, SCB1 etc.)   
#define HAL_SCB_SLAVES_READ(scb, val) \
    HAL_READ_UINT32(REG_##scb##_SLAVES, val)                         
                            
//--------------------------------------------------------------------------
#endif // CYGONCE_HAL_VAR_ARCH_H
// End of var_arch.h
