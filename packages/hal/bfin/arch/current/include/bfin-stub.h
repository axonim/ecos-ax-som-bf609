#ifndef CYGONCE_HAL_MIPS_STUB_H
#define CYGONCE_HAL_MIPS_STUB_H
//========================================================================
//
//      bfin-stub.h
//
//      BFIN-specific definitions for generic stub
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
// Author(s):     andre, Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:          2006-08-21
// Purpose:       
// Description:   BFIN-specific definitions for generic stub
// Usage:         
//
//####DESCRIPTIONEND####
//
//========================================================================


#include <pkgconf/system.h>
#include <pkgconf/hal_bfin.h>

#include <cyg/hal/hal_io.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NUMREGS    56

// The simplest case of 32-bit regs represented to GDB as 32-bit regs.
#define REGSIZE(X) 4
typedef unsigned long target_register_t;

enum regnames {
        REG_SYSCFG, REG_R0,     REG_R1,     REG_R2,     REG_R3,     REG_R4,     REG_R5,     REG_R6,
        REG_R7,     REG_P0,     REG_P1,     REG_P2,     REG_P3,     REG_P4,     REG_P5,     REG_SP,
        REG_FP,     REG_I0,     REG_I1,     REG_I2,     REG_I3,     REG_M0,     REG_M1,     REG_M2,
        REG_M3,     REG_L0,     REG_L1,     REG_L2,     REG_L3,     REG_B0,     REG_B1,     REG_B2,
        REG_B3,     REG_A0X,    REG_A0W,    REG_A1X,    REG_A1W,    REG_LC0,    REG_LC1,    REG_LT0,
        REG_LT1,    REG_LB0,    REG_LB1,    REG_ASTAT,  REG_RES,    REG_RETS,   REG_PC,     REG_RETX,
        REG_RETN,   REG_RETE,   REG_SEQSTAT, REG_IPEND, REG_ORIGPC, REG_EXTRA1, REG_EXTRA2, REG_EXTRA3
};
#define USE_LONG_NAMES_FOR_ENUM_REGNAMES

typedef enum regnames regnames_t;

/* Given a trap value TRAP, return the corresponding signal. */
extern int __computeSignal (unsigned int trap_number);

/* Return the SPARC trap number corresponding to the last-taken trap. */
extern int __get_trap_number (void);

/* Return the currently-saved value corresponding to register REG. */
extern target_register_t get_register (regnames_t reg);

/* Store VALUE in the register corresponding to WHICH. */
extern void put_register (regnames_t which, target_register_t value);

/* Set the currently-saved pc register value to PC. This also updates NPC
   as needed. */
#if !defined(SET_PC_PROTOTYPE_EXISTS) && !defined(set_pc)
#define SET_PC_PROTOTYPE_EXISTS
extern void set_pc (target_register_t pc);
#endif

/* Set things up so that the next user resume will execute one instruction.
   This may be done by setting breakpoints or setting a single step flag
   in the saved user registers, for example. */
#ifndef __single_step
void __single_step (void);
#endif

/* Clear the single-step state. */
void __clear_single_step (void);

extern int __is_bsp_syscall(void);

extern int hal_syscall_handler(void);
    
/* If the breakpoint we hit is in the breakpoint() instruction, return a
   non-zero value. */
#ifndef __is_breakpoint_function
extern int __is_breakpoint_function (void);
#endif

/* Skip the current instruction. */
extern void __skipinst (void);

extern void __install_breakpoints (void);

extern void __clear_breakpoints (void);

extern void __install_breakpoint_list (void);

extern void __clear_breakpoint_list (void);

#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif // ifndef CYGONCE_HAL_MIPS_STUB_H
