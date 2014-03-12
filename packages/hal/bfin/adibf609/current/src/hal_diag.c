/*=============================================================================
//
//      hal_diag.c
//
//      HAL diagnostic output code
//
//=============================================================================
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Contributors:Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:        2013-06-10
// Purpose:     HAL diagnostic output
// Description: Implementations of HAL diagnostic output support.
//
//####DESCRIPTIONEND####
//
//===========================================================================*/

#include <pkgconf/hal.h>

#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_diag.h>

#include <cyg/hal/hal_intr.h>

#include <cyg/hal/hal_io.h>

//-----------------------------------------------------------------------------
// Select which diag channels to use

#define CYG_KERNEL_DIAG_SERIAL

/*---------------------------------------------------------------------------*/

void hal_diag_led(int x)
{
 // HAL_WRITE_UINT32(HAL_DISPLAY_LEDBAR, x);
#if !defined(CYG_KERNEL_DIAG_LCD)
 // HAL_WRITE_UINT32(HAL_DISPLAY_ASCIIWORD, x);
#endif
}

externC void diag_write_string (const char*);

extern void cyg_hal_plf_comms_init(void);
extern void cyg_hal_plf_serial_putc(void*, cyg_uint8);
extern cyg_uint8 cyg_hal_plf_serial_getc(void*);

void hal_diag_print_reg_dump(HAL_SavedRegisters *regs)
{
    diag_write_string("Register dump: \n");
    
    diag_write_string("Core registers: \n");
    diag_printf("R0 = %8X, R1 = %8X, R2 = %8X, R3 = %8X\n", regs->r[7], regs->r[6], regs->r[5], regs->r[4]);
    diag_printf("R4 = %8X, R5 = %8X, R6 = %8X, R7 = %8X\n", regs->r[3], regs->r[2], regs->r[1], regs->r[0]);
    diag_printf("P0 = %8X, P1 = %8X, P2 = %8X\n", regs->r[5], regs->r[4], regs->r[3]);
    diag_printf("P3 = %8X, P4 = %8X, P5 = %8X\n", regs->r[2], regs->r[1], regs->r[0]);
    diag_printf("A0 = %4X%8X,        A1 = %4X%8X\n", regs->ax[0], regs->aw[0], regs->ax[1], regs->aw[1]);
    diag_printf("ASTAT = %8X,        SEQSTAT = %8X\n", regs->astat, regs->seqstat);
    diag_printf("IPEND = %8X,        IMASK = %8X\n", regs->ipend, regs->imask);
    diag_write_string("System registers: \n");
    diag_printf("RETS  = %8X, RETI  = %8X, RETX  = %8X, RETN  = %8X\n", regs->rets, regs->reti, regs->retx, regs->retn);
    diag_printf("RETE  = %8X, PC    = %8X, USP   = %8X, FP    = %8X\n", regs->rete, regs->pc, regs->usp, regs->fp);
    diag_printf("SYSC  = %8X, CYC   = %8X, CYC2  = %8X\n", regs->syscfg, regs->cycles, regs->cycles2);
    diag_printf("ICPLD = %8X, DCPLD = %8X\n", regs->icpld_fault, regs->dcpld_fault);
    diag_write_string("INDEX registers: \n");
    int i = 0;
    for (i=0; i<4; i++)
        diag_printf("%x : I = %8X, L = %8X, B = %8X, M = %8X\n",i, regs->i[i], regs->l[i], regs->b[i], regs->m[i]);
    diag_write_string("LOOP registers: \n");
    for (i=0; i<2; i++)
        diag_printf("%x : Top = %8X, Bottom = %8X, Count = %8X\n",i, regs->lt[i], regs->lb[i], regs->lc[i]);
    diag_write_string("------------ END register dump-------------------\n");


}

void hal_diag_init(void)
{
#if defined(CYGSEM_HAL_ROM_MONITOR) && !defined(CYG_KERNEL_DIAG_SERIAL)
  // It's handy to have the LCD initialized at reset when using it
  // for debugging output.
  // The serial port likely doesn't work yet.  Let's wait.
  diag_write_string ("eCos ROM   " __TIME__ "\n");
  diag_write_string (__DATE__ "\n");
#endif
  cyg_hal_plf_comms_init();
}

void hal_diag_write_char(char c)
{
  unsigned long __state;

  HAL_DISABLE_INTERRUPTS(__state);

  if(c == '\n')
    {
      cyg_hal_plf_serial_putc(NULL, '\r');
      cyg_hal_plf_serial_putc(NULL, '\n');
    }
  else if (c == '\r')
    {
      // Ignore '\r'
    }
  else
    {
      cyg_hal_plf_serial_putc(NULL, c);
    }

   HAL_RESTORE_INTERRUPTS(__state);
}

void hal_diag_read_char(char* c)
{
  *c = cyg_hal_plf_serial_getc(NULL);
}

/*---------------------------------------------------------------------------*/
/* End of hal_diag.c */
