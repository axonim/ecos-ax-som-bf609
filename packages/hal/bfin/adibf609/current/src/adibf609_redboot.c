//*==========================================================================
//
//      adibf609_redboot.c
//
//      Redboot support code for BlackFin ADIBF609 board
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2006 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later
// version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License
// along with eCos; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// As a special exception, if other files instantiate templates or use
// macros or inline functions from this file, or you compile this file
// and link it with other works to produce a work based on this file,
// this file does not by itself cause the resulting work to be covered by
// the GNU General Public License. However the source code for this file
// must still be made available in accordance with section (3) of the GNU
// General Public License v2.
//
// This exception does not invalidate any other reasons why a work based
// on this file might be covered by the GNU General Public License.
// -------------------------------------------
// ####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    Aleksandr Loiko
// Date:         2013-09-20
// Purpose:      Redboot support code
//
//####DESCRIPTIONEND####
//
//========================================================================*/
#include <pkgconf/system.h>
#ifdef CYGPKG_REDBOOT

#include <pkgconf/hal.h>
#include <cyg/hal/hal_io.h>             // IO macros
#include <redboot.h>
#include CYGHWR_MEMORY_LAYOUT_H

//--------------------------------------
// Memory section for loading applications
//--------------------------------------
EXTERN void
cyg_plf_memory_segment(int seg, unsigned char **start, unsigned char **end)
{
	if (seg == 1) 
	{
		*start = CYGMEM_REGION_sdram;
		*end   = CYGMEM_REGION_sdram + CYGMEM_REGION_sdram_SIZE - 1;
	} else {
		diag_printf("Wrong segment, seg: %d\n", seg);
	}
}


//--------------------------------------
// Watchdog
//--------------------------------------
/* Kick the watchdog. */
// static void
// adibf609_kick_watchdog(bool is_idle)
// {
    // /* Write magic code to reset the watchdog. */
    // HAL_WRITE_UINT32(AT91_WDTC + AT91_WDTC_WDCR,
                     // AT91_WDTC_WDCR_RELOAD | AT91_WDTC_WDCR_KEY);
// }

// // Add an idle function to be run by RedBoot when idle
// RedBoot_idle(at91sam9g20ek_kick_watchdog, RedBoot_IDLE_BEFORE_NETIO);

// /* Watchdog command for Redboot. */
// static void
// adibf609_cmd_watchdog(int argc, char *argv[])
// {
    // cyg_uint32 reg_val;

    // if (argc == 2 && strcasecmp(argv[1], "off") == 0) {
        // HAL_WRITE_UINT32(AT91_WDTC + AT91_WDTC_WDMR, AT91_WDTC_WDMR_DIS);
        // argc--;
    // }

    // if (argc == 1) {
        // HAL_READ_UINT32(AT91_WDTC + AT91_WDTC_WDMR, reg_val);
        // diag_printf("Watchdog is: %s\n",
                    // (reg_val & AT91_WDTC_WDMR_DIS) ? "off" : "on");
    // }
    // else
        // diag_printf("Invalid watchdog command option\n");
// }

// // Add CLI command 'watchdog' as defined in this file
// RedBoot_cmd("watchdog",
            // "Display watchdog status or disable watchdog",
            // "[off]",
            // adibf609_cmd_watchdog
    // );


//--------------------------------------
// Dipswitches
//--------------------------------------
// #ifdef CYGFUN_REDBOOT_BOOT_SCRIPT
// /* Check dipswitches and invalidate script if both switches are on. */
// static void
// adibf609_check_dipswitches(void)
// {
    // cyg_uint32 dipswitch;

// //    dipswitch = hal_at91sam9g20_pushbutton();
// //    if (!(dipswitch & 3))
// //        script = NULL;
// }

// // Add function to check dipswitches after fconfig loads script
// RedBoot_init(adibf609_check_dipswitches,  RedBoot_INIT_PRIO(500));
// #endif // ifdef CYGFUN_REDBOOT_BOOT_SCRIPT

#endif // ifdef CYGPKG_REDBOOT

//--------------------------------------------------------------------------
// EOF adibf609_redboot.c