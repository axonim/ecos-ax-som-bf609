//========================================================================
//
//      bfin-stub.h
//
//      Helper functions for stub, generic to all BFIN processors
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
// Contributors:  Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:          2013-09-21
// Purpose:       
// Description:   Helper functions for stub, generic to all BLACKfin on STAMP
// Usage:         
//
//####DESCRIPTIONEND####
//
//========================================================================

#include <stddef.h>

#include <pkgconf/hal.h>

#ifdef CYGPKG_REDBOOT
#include <pkgconf/redboot.h>
#endif

#ifdef CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS

#include <cyg/hal/hal_stub.h>

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#include <cyg/hal/bfin-regs.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_intr.h>
//#include <cyg/hal/mips_opcode.h>

typedef unsigned long t_inst;

/*----------------------------------------------------------------------
 * Asynchronous interrupt support
 */

static struct
{
  t_inst *targetAddr;
  t_inst savedInstr;
} asyncBuffer;

/* Called to asynchronously interrupt a running program.
   Must be passed address of instruction interrupted.
   This is typically called in response to a debug port
   receive interrupt.
*/

void
install_async_breakpoint(void *epc)
{
}

/*--------------------------------------------------------------------*/
/* Given a trap value TRAP, return the corresponding signal. */

int __computeSignal (unsigned int trap_number)
{
    return SIGTRAP;
}

/* Return the trap number corresponding to the last-taken trap. */

int __get_trap_number (void)
{
  return (get_register (REG_SEQSTAT) & SEQSTAT_EXCAUSE);
}

#if defined(CYGSEM_REDBOOT_BSP_SYSCALLS)
int __is_bsp_syscall(void) 
{
    return __get_trap_number() == EXC_SYS;
}
#endif

/* Set the currently-saved pc register value to PC. This also updates NPC
   as needed. */

void set_pc (target_register_t pc)
{
  put_register (REG_PC, pc);
}


/*----------------------------------------------------------------------
 * Single-step support
 */

/* Saved instruction data for single step support.  */

static struct
{
  t_inst *targetAddr;
  t_inst savedInstr;
} instrBuffer;


/* Set things up so that the next user resume will execute one instruction.
   This may be done by setting breakpoints or setting a single step flag
   in the saved user registers, for example. */

void __single_step (void)
{
    unsigned long syscfg;
    
    syscfg = get_register(REG_SYSCFG);
    syscfg = syscfg | (0x1);
    put_register(REG_SYSCFG, syscfg);
}


/* Clear the single-step state. */

void __clear_single_step (void)
{
    unsigned long syscfg;

    syscfg = get_register(REG_SYSCFG);
    syscfg = syscfg & (~0x1);
    put_register(REG_SYSCFG, syscfg);
}


void __install_breakpoints ()
{
//diag_printf("install_bpt ta %08x\n",instrBuffer.targetAddr);
  if (instrBuffer.targetAddr != NULL)
    {
      instrBuffer.savedInstr = *instrBuffer.targetAddr;
      *instrBuffer.targetAddr = __break_opcode ();
//diag_printf("ta %08x si %08x *ta %08x\n",
//            instrBuffer.targetAddr,instrBuffer.savedInstr,*instrBuffer.targetAddr);

      /* Ensure that the planted breakpoint makes it out to memory and
         is subsequently loaded into the instruction cache.
      */
      __data_cache (CACHE_FLUSH) ;
      __instruction_cache (CACHE_FLUSH) ;
    }

  /* Install the breakpoints in the breakpoint list */
  __install_breakpoint_list();
}

void __clear_breakpoints (void)
{
  __clear_breakpoint_list();
}


/* If the breakpoint we hit is in the breakpoint() instruction, return a
   non-zero value. */

int
__is_breakpoint_function ()
{
  //  return get_register (REG_PC) == (target_register_t)(unsigned long)&_breakinst;
    return 1;
}


/* Skip the current instruction.  Since this is only called by the
   stub when the PC points to a breakpoint or trap instruction,
   we can safely just skip 2. */

void __skipinst (void)
{
    put_register (REG_PC, get_register (REG_PC) + 2);
}

#endif // CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS
