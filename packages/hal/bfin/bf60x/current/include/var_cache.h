#ifndef CYGONCE_IMP_CACHE_H
#define CYGONCE_IMP_CACHE_H

//=============================================================================
//
//      imp_cache.h
//
//      HAL cache control API
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
// Date:        2013-09-23
// Purpose:     Cache control API
// Description: The macros defined here provide the HAL APIs for handling
//              cache control operations.
// Usage:
//              #include <cyg/hal/var_cache.h>
//              ...
//              
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/hal.h>
#include <pkgconf/hal_bfin_bf60x.h>
#include <cyg/infra/cyg_type.h>

#include <cyg/hal/plf_cache.h>


#ifndef HAL_DCACHE_SIZE


#ifdef CYGPKG_HAL_BFIN_BF533
// Data cache
#define HAL_DCACHE_SIZE                 32768   // Size of data cache in bytes
#define HAL_DCACHE_LINE_SIZE            32      // Size of a data cache line
#define HAL_DCACHE_WAYS                 2       // Associativity of the cache

#define HAL_DSRAM_A_SIZE                16384   // Size of data sram in bytes
#define HAL_DSRAM_B_SIZE                16384   // Size of data sram in bytes
#define HAL_DSRAM_A_START               0xFF800000
#define HAL_DSRAM_B_START               0xFF900000


#define HAL_SCRATCHPAD_SIZE             4096    // Size of scratchpad

// Instruction cache
#define HAL_ICACHE_SIZE                 16384    // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_ICACHE_WAYS                 2       // Associativity of the cache
#define HAL_ICACHE_START                0xFFA10000

#define HAL_ISRAM_SIZE                  65536   // Size of data sram in bytes
#define HAL_ISRAM_START                 0xFFA00000

#elif defined(CYGPKG_HAL_BFIN_BF537)

// Data cache
#define HAL_DCACHE_SIZE                 32768   // Size of data cache in bytes
#define HAL_DCACHE_LINE_SIZE            32      // Size of a data cache line
#define HAL_DCACHE_WAYS                 4       // Associativity of the cache

#define HAL_DSRAM_A_SIZE                16384   // Size of data sram in bytes
#define HAL_DSRAM_B_SIZE                16384   // Size of data sram in bytes
#define HAL_DSRAM_A_START               0xFF800000
#define HAL_DSRAM_B_START               0xFF900000


#define HAL_SCRATCHPAD_SIZE             4096    // Size of scratchpad

// Instruction cache
#define HAL_ICACHE_SIZE                 16384    // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_ICACHE_WAYS                 2       // Associativity of the cache
#define HAL_ICACHE_START                0xFFA10000

#define HAL_ISRAM_SIZE                  49152   // Size of data sram in bytes
#define HAL_ISRAM_START                 0xFFA00000

#endif

#define HAL_DCACHE_SETS (HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))
#define HAL_ICACHE_SETS (HAL_ICACHE_SIZE/(HAL_ICACHE_LINE_SIZE*HAL_ICACHE_WAYS))

#endif



//-----------------------------------------------------------------------------
#endif // ifndef CYGONCE_IMP_CACHE_H
// End of imp_cache.h
