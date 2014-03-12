#ifndef CYGONCE_ADIBF609_H
#define CYGONCE_ADIBF609_H

/*=============================================================================
//
//      adibf609.h
//
//      Platform specific support (register layout, etc)
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
// Author(s):    Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Contributors: Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:         2013-08-26
// Purpose:      AX-SOM-BF609 platform specific support routines
// Description: 
// Usage:        #include <cyg/hal/adibf609.h>
//
//####DESCRIPTIONEND####
//
//===========================================================================*/

/*
 * Clock Settings
 *	CCLK = (CLKIN * VCO_MULT) / CCLK_DIV
 *	SCLK = (CLKIN * VCO_MULT) / SYSCLK_DIV
 *	SCLK0 = SCLK / SCLK0_DIV
 *	SCLK1 = SCLK / SCLK1_DIV
 */
/* CONFIG_CLKIN_HZ is any value in Hz					   */
#define CONFIG_CLKIN_HZ			(25000000)
/* CLKIN_HALF controls the DF bit in PLL_CTL      0 = CLKIN		    */
/*                                                1 = CLKIN / 2		*/
#define CONFIG_CLKIN_HALF		(0)

/* VCO_MULT controls the MSEL (multiplier) bits in PLL_CTL	*/
/* Values can range from 1-127 			                    */
#define CONFIG_VCO_MULT			(CYGNUM_HAL_BFIN_PLL_MULTIPLIER)

/* CCLK_DIV controls the core clock divider				    */
/* Values can range from 0-31 		                    	*/
#define CONFIG_CCLK_DIV			(CYGNUM_HAL_BFIN_CORE_CLOCK_DIVIDER)
/* SCLK_DIV controls the system clock divider				*/
/* Values can range from 1-31 		                    	*/
#define CONFIG_SCLK_DIV			(CYGNUM_HAL_BFIN_SYSTEM_CLOCK_DIVIDER)
/* Values can range from 1-7 		                		*/
#define CONFIG_SCLK0_DIV		(CYGNUM_HAL_BFIN_S0SEL_DIVIDER)
#define CONFIG_SCLK1_DIV		(CYGNUM_HAL_BFIN_S1SEL_DIVIDER)
/* DCLK_DIV controls the DDR clock divider				    */
/* Values can range from 1-31 		                    	*/
#define CONFIG_DCLK_DIV			(CYGNUM_HAL_BFIN_DSEL_DIVIDER)
/* OCLK_DIV controls the output clock divider				*/
/* Values can range from 1-127                  			*/
#define CONFIG_OCLK_DIV			(CYGNUM_HAL_BFIN_OSEL_DIVIDER)

#define CONFIG_BFIN_GET_DCLK 		((CONFIG_CLKIN_HZ*CONFIG_VCO_MULT)/CONFIG_DCLK_DIV)
/* On-Board Ethernet (physical addrs) */
#define STAMP_GPIO_ETH_IRQ         (9)  /* IRQ from Ethernet controller */
#define STAMP_FHH_ETH_IOBASE       0x20300300  /* I/O base */
#define STAMP_FHH_ETH_MMBASE       0x20300300  /* Attribute-memory base */


#define STAMP_SYSTEM_CLOCK_SPEED   80000000 /* 80MHz system clock */

#define STAMP_WATCHDOG_WDOGCTL      0xFFC00200
#define STAMP_WATCHDOG_WDOGSTAT     0xFFC00208
#define STAMP_WATCHDOG_WDOGCNT      0xFFC00204

#define SWITCH_TO_ETH() \
        __asm __volatile (  "P0.H = 0xFFC0;\n"              \
                            "P0.L = 0x0730;\n"              \
                            "SSYNC;\n"                      \
                            "R0.L = W[P0];\n"               \
                            "BITSET(R0,0);\n"               \
                            "W[P0] = R0;\n"                 \
                            "SSYNC;\n"                      \
                            "P0.L = 0x0708;\n"              \
                            "R0.L = 0x01;\n"                \
                            "W[P0] = R0;\n"                 \
                            "SSYNC;\n" : : : "P0","R0")

#define SWITCH_TO_FLASH() \
        __asm __volatile (  "P0.H = 0xFFC0;\n"              \
                            "P0.L = 0x0730;\n"              \
                            "SSYNC;\n"                      \
                            "R0.L = W[P0];\n"               \
                            "BITSET(R0,0);\n"               \
                            "W[P0] = R0;\n"                 \
                            "SSYNC;\n"                      \
                            "P0.L = 0x0704;\n"              \
                            "R0.L = 0x01;\n"                \
                            "W[P0] = R0;\n"                 \
                            "SSYNC;\n" : : : "P0","R0")
/*---------------------------------------------------------------------------*/
/* end of adibf609.h                                                          */
#endif /* CYGONCE_ADIBF609_H */
