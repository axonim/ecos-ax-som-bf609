#ifndef CYGONCE_HAL_CACHE_H
#define CYGONCE_HAL_CACHE_H

//=============================================================================
//
//      hal_cache.h
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
// Author(s):   andre, Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:        2006-05-22
// Purpose:     Cache control API
// Description: The macros defined here provide the HAL APIs for handling
//              cache control operations.
// Usage:
//              #include <cyg/hal/hal_cache.h>
//              ...
//              
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>

#include <cyg/hal/var_cache.h>

#ifndef HAL_ICACHE
#define HAL_ICACHE
// CACHE CONTROL definitions

// instruction cache
#define HAL_ICACHE_IMEM_CONTROL    0xFFE01004
#define HAL_ICACHE_ENICPLD         0x2
#define HAL_ICACHE_IMC             0x4
#define HAL_ICACHE_ILOCKMASK       0x78
#define HAL_ICACHE_LRUPRIORST      0x2000



// instruction cacheline options
#define HAL_ICACHE_DATA_BASE       0xFFE01200
#define HAL_ICACHE_ADDR_BASE       0xFFE01100

#define HAL_ICACHE_MMU_ENTRIES     0xF
#define HAL_ICACHE_MMU_ENTRY_MASK  ~HAL_ICACHE_MMU_ENTRIES


#define HAL_ICACHE_PAGE_SIZE_1K    0x00000000
#define HAL_ICACHE_PAGE_SIZE_4K    0x00010000
#define HAL_ICACHE_PAGE_SIZE_1M    0x00020000
#define HAL_ICACHE_PAGE_SIZE_4M    0x00030000
#define HAL_ICACHE_PAGE_SIZE_MASK  0x00030000

#define HAL_ICACHE_SET_CACHABLE        0x00001000
#define HAL_ICACHE_SET_HIGH_LRUPRIO    0x00000100
#define HAL_ICACHE_SET_ALLOW_USER_RD   0x00000004
#define HAL_ICACHE_SET_LOCK            0x00000002
#define HAL_ICACHE_SET_VALID           0x00000001
#define HAL_ICACHE_SET_UNAVAILABLE     ( HAL_ICACHE_SET_LOCK | HAL_ICACHE_SET_VALID )

#define HAL_ICACHE_SET_OPTION_MASK     (HAL_ICACHE_PAGE_SIZE_MASK \
                                    | HAL_ICACHE_SET_CACHABLE | HAL_ICACHE_SET_HIGH_LRUPRIO \
                                    | HAL_ICACHE_SET_ALLOW_USER_RD | HAL_ICACHE_SET_LOCK \
                                    | HAL_ICACHE_SET_VALID)

#ifdef CYGNUM_HAL_BFIN_USE_ICACHE_PAGE_SIZE_1
#define HAL_ICACHE_DEFAULT_PAGE_SIZE HAL_ICACHE_PAGE_SIZE_1K
#define HAL_ICACHE_DEFAULT_MULTIPLIER 1
#endif

#ifdef CYGNUM_HAL_BFIN_USE_ICACHE_PAGE_SIZE_4
#define HAL_ICACHE_DEFAULT_PAGE_SIZE HAL_ICACHE_PAGE_SIZE_4K
#define HAL_ICACHE_DEFAULT_MULTIPLIER 4
#endif

#ifdef CYGNUM_HAL_BFIN_USE_ICACHE_PAGE_SIZE_1024
#define HAL_ICACHE_DEFAULT_PAGE_SIZE HAL_ICACHE_PAGE_SIZE_1M
#define HAL_ICACHE_DEFAULT_MULTIPLIER 1024
#endif

#ifdef CYGNUM_HAL_BFIN_USE_ICACHE_PAGE_SIZE_4096
#define HAL_ICACHE_DEFAULT_PAGE_SIZE HAL_ICACHE_PAGE_SIZE_4M
#define HAL_ICACHE_DEFAULT_MULTIPLIER 4096
#endif

#endif // HAL_ICACHE

#ifndef HAL_DCACHE
#define HAL_DCACHE

// data cache
#define HAL_DCACHE_DMEM_CONTROL    0xFFE00004
#define HAL_DCACHE_ENDCPLD         0x2
#define HAL_DCACHE_DMC             0xC
#define HAL_DCACHE_DCBS            0x10
#define HAL_DCACHE_PORT_PREF0      0x1000
#define HAL_DCACHE_PORT_PREF1      0x2000

// instruction cacheline options
#define HAL_DCACHE_DATA_BASE       0xFFE00200
#define HAL_DCACHE_ADDR_BASE       0xFFE00100

#define HAL_DCACHE_MMU_ENTRIES     0xF
#define HAL_DCACHE_MMU_ENTRY_MASK  ~HAL_DCACHE_MMU_ENTRIES

#ifdef CYGPKG_HAL_BFIN_BF53x
#define ANOMALY_05000158        0x200
#else
#define ANOMALY_05000158        0x00
#endif

#define HAL_DCACHE_PAGE_SIZE_1K    0x00000000
#define HAL_DCACHE_PAGE_SIZE_4K    0x00010000
#define HAL_DCACHE_PAGE_SIZE_1M    0x00020000
#define HAL_DCACHE_PAGE_SIZE_4M    0x00030000
#define HAL_DCACHE_PAGE_SIZE_16K   0x00040000
#define HAL_DCACHE_PAGE_SIZE_64K   0x00050000
#define HAL_DCACHE_PAGE_SIZE_16M   0x00060000
#define HAL_DCACHE_PAGE_SIZE_64M   0x00070000
#define HAL_DCACHE_PAGE_SIZE_MASK  0x00070000

#define HAL_DCACHE_SET_ALLOCATEWRITE   0x00008000
#define HAL_DCACHE_SET_WRITETHROUGH    0x00004000
#define HAL_DCACHE_SET_CACHABLE        0x00001000
#define HAL_DCACHE_SET_DIRTY           0x00000080
#define HAL_DCACHE_SET_ALLOW_SUPER_WR  0x00000010
#define HAL_DCACHE_SET_ALLOW_USER_WR   0x00000008
#define HAL_DCACHE_SET_ALLOW_USER_RD   0x00000004
#define HAL_DCACHE_SET_ALLOW_MASK      0x0000001C
#define HAL_DCACHE_SET_LOCK            0x00000002
#define HAL_DCACHE_SET_VALID           0x00000001
#define HAL_DCACHE_SET_UNAVAILABLE     ( HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID )


#define HAL_DCACHE_SET_OPTION_MASK     (HAL_ICACHE_PAGE_SIZE_MASK \
                                    | HAL_DCACHE_SET_ALLOCATEWRITE \
                                    | HAL_DCACHE_SET_WRITETHROUGH \
                                    | HAL_DCACHE_SET_CACHABLE | HAL_DCACHE_SET_DIRTY \
                                    | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK \
                                    | HAL_DCACHE_SET_VALID)


#define HAL_DCACHE_DEFAULT_MULTIPLIER CYGNUM_HAL_BFIN_USE_DCACHE_PAGE_SIZE

#ifdef CYGNUM_HAL_BFIN_USE_DCACHE_PAGE_SIZE_1
#define HAL_DCACHE_DEFAULT_PAGE_SIZE HAL_DCACHE_PAGE_SIZE_1K
#endif

#ifdef CYGNUM_HAL_BFIN_USE_DCACHE_PAGE_SIZE_4
#define HAL_DCACHE_DEFAULT_PAGE_SIZE HAL_DCACHE_PAGE_SIZE_4K
#endif

#ifdef CYGNUM_HAL_BFIN_USE_DCACHE_PAGE_SIZE_1024
#define HAL_DCACHE_DEFAULT_PAGE_SIZE HAL_DCACHE_PAGE_SIZE_1M
#endif

#ifdef CYGNUM_HAL_BFIN_USE_DCACHE_PAGE_SIZE_4096
#define HAL_DCACHE_DEFAULT_PAGE_SIZE HAL_DCACHE_PAGE_SIZE_4M
#endif

#ifdef CYGSEM_HAL_DCACHE_STARTUP_MODE_COPYBACK
#define HAL_DCACHE_SET_CACHING_MODE 0x0
#endif

#ifdef CYGSEM_HAL_DCACHE_STARTUP_MODE_WRITETHRU
#ifdef CYGSEM_HAL_BFIN_DCACHE_ALLOCATE_WRITE
    #define HAL_DCACHE_SET_CACHING_MODE (HAL_DCACHE_SET_WRITETHROUGH | HAL_DCACHE_SET_ALLOCATEWRITE)
#else
    #define HAL_DCACHE_SET_CACHING_MODE HAL_DCACHE_SET_WRITETHROUGH
#endif
#endif

#ifndef HAL_DCACHE_SET_CACHING_MODE
#define HAL_DCACHE_SET_CACHING_MODE HAL_DCACHE_SET_WRITETHROUGH
#endif


#endif // HAL_DCACHE


#define HAL_UCACHE_INVALIDATE_ALL()

#define HAL_UCACHE_DISABLE()

#ifndef HAL_ICACHE_DISABLE
#define HAL_ICACHE_DISABLE() \
    __asm __volatile( "P0.H = 0xFFE0;" \
                      "P0.L = 0x1004;"  \
                      "R0   = R0 ^ R0;"  \
                      "R1   = [P0];"  \
                      "R0   = 0x6;"  \
                      "R0   = ~R0;"  \
                      "R1   = R1 & R0;"  \
                      "CLI R2;" \
                      "SSYNC;" \
                      ".align 8;" \
                      "[P0] = R1;" \
                      "SSYNC;" \
                      "STI R2;" : : : "P0", "R0", "R1", "R2")
#endif

#ifndef HAL_DCACHE_DISABLE
#define HAL_DCACHE_DISABLE() \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                      "P0.L = 0x0004;"  \
                      "R0   = R0 ^ R0;"  \
                      "R1   = [P0];"  \
                      "R0   = 0xE;"  \
                      "R0   = ~R0;"  \
                      "R1   = R1 & R0;"  \
                      "CLI R2;" \
                      "SSYNC;" \
                      ".align 8;" \
                      "[P0] = R1;" \
                      "SSYNC;"\
                      "STI R2;" : : : "P0", "R0", "R1", "R2")
#endif

#ifndef HAL_ICACHE_ENABLE
externC void hal_bfin_prepare_icache();
#define HAL_ICACHE_ENABLE() \
    hal_bfin_prepare_icache(); \
    HAL_ICACHE_REENABLE()

#define HAL_ICACHE_REENABLE() \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                      "P0.L = 0x1004;"  \
                      "R0   = 0x6;"  \
                      "CLI R2;" \
                      "SSYNC;" \
                      ".align 8;" \
                      "[P0] = R0;" \
                      /*".align 8;"*/ \
                      "SSYNC;" \
                      "STI R2;"  : : : "P0", "R0", "R1", "R2")
#endif

#ifndef HAL_DCACHE_ENABLE
externC void hal_bfin_prepare_dcache();
#define HAL_DCACHE_ENABLE() \
    hal_bfin_prepare_dcache(); \
    HAL_DCACHE_REENABLE()

#define HAL_DCACHE_REENABLE() \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                          "P0.L = 0x0004;"  \
                          "R0   = 0x101E;" \
                          "SSYNC;" \
                          "R1   = [P0];"\
                          "R0   = R0 | R1; " \
                          "CLI R2;" \
                          "SSYNC;" \
                          ".align 8;" \
                          "[P0] = R0;"  \
                          "SSYNC;"  \
                          "STI R2;"  :  :  : "P0", "R0", "R1", "R2")
#endif



#ifndef HAL_ICACHE_IS_ENABLED
#define HAL_ICACHE_IS_ENABLED(__enabled__) \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                      "P0.L = 0x1004;"  \
                      "SSYNC;" \
                      "R1   = [P0];"  \
                      "R0   = 0x4;"  \
                      "R1   = R1 & R0;"  \
                      "%0   = R1;" : "=r"(__enabled__) : : "P0", "R0", "R1")

#endif

#ifndef HAL_ICACHE_PROT_DISABLE
#define HAL_ICACHE_PROT_DISABLE() \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                      "P0.L = 0x1004;"  \
                      "R0   = R0 ^ R0;"  \
                      "R1   = [P0];"  \
                      "R0   = 0x2;"  \
                      "R0   = ~R0;"  \
                      "R1   = R1 & R0;"  \
                      "CLI R2;" \
                      "SSYNC;" \
                      ".align 8;" \
                      "[P0] = R1;" \
                      "SSYNC;"\
                      "STI R2;" : : : "P0", "R0", "R1", "R2")

#define HAL_ICACHE_PROT_REENABLE() \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                      "P0.L = 0x1004;"  \
                      "R0   = 0x2;"  \
                      "CLI R2;" \
                      "SSYNC;" \
                      "R1   = [P0];" \
                      "R0   = R0 | R1;" \
                      "SSYNC;" \
                      ".align 8;" \
                      "[P0] = R0;" \
                      /*".align 8;"*/ \
                      "SSYNC;" \
                      "STI R2;"  : : : "P0", "R0", "R1", "R2")

#endif


#ifndef HAL_DCACHE_IS_ENABLED
#define HAL_DCACHE_IS_ENABLED(__enabled__) \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                      "P0.L = 0x0004;"  \
                      "SSYNC;" \
                      "R0   = [P0];" \
                      "R1   = 0xC;" \
                      "R0   = R0 & R1;" \
                      "%0   = R0;"  : "=r"(__enabled__) : : "P0", "R0", "R1");
#endif

#ifndef HAL_DCACHE_PROT_DISABLE
#define HAL_DCACHE_PROT_DISABLE() \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                      "P0.L = 0x0004;"  \
                      "R0   = R0 ^ R0;"  \
                      "SSYNC;" \
                      "R1   = [P0];"  \
                      "R0   = 0x2;"  \
                      "R0   = ~R0;"  \
                      "R1   = R1 & R0;"  \
                      "CLI R2;" \
                      "SSYNC;" \
                      ".align 8;" \
                      "[P0] = R1;" \
                      "SSYNC;"\
                      "STI R2;" : : : "P0", "R0", "R1", "R2")

#define HAL_DCACHE_PROT_REENABLE() \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                      "P0.L = 0x0004;"  \
                      "R0   = 0x2;"  \
                      "CLI R2;" \
                      "SSYNC;" \
                      "R1   = [P0];" \
                      "R0   = R0 | R1;" \
                      "SSYNC;" \
                      ".align 8;" \
                      "[P0] = R0;" \
                      /*".align 8;"*/ \
                      "SSYNC;" \
                      "STI R2;"  : : : "P0", "R0", "R1", "R2")

#endif

#define HAL_DCACHE_FLUSH(__address__) \
    __asm__ __volatile( "SSYNC;" \
                        "FLUSH [%0];" \
                        "SSYNC;" : : "p"(__address__));
                        
#define HAL_DCACHE_INVALIDATE(__address__) \
    __asm__ __volatile( "SSYNC;"         \
                        "FLUSHINV [%0];" \
                        "SSYNC;" : : "p"(__address__));

void hal_flushinv_area(void *start, cyg_uint32 bytes);
void hal_flush_area(void *start, cyg_uint32 bytes);

externC void flush_data_cache();

#ifdef CYGSEM_HAL_DCACHE_STARTUP_MODE_WRITETHRU
    #define HAL_DCACHE_SYNC()
#else
    #define HAL_DCACHE_SYNC() flush_data_cache()
                                                
#endif

//#define HAL_DCACHE_PURGE_ALL()
//#define HAL_ICACHE_INVALIDATE_ALL()


#define HAL_ICACHE_INVALIDATE_ALL() \
    { \
      int state = 0; \
      int ints  = 0; \
      HAL_DISABLE_INTERRUPTS(ints); \
      HAL_ICACHE_IS_ENABLED(state); \
      if (state) \
      {  \
        HAL_ICACHE_DISABLE(); \
        HAL_ICACHE_REENABLE(); \
      } \
      HAL_RESTORE_INTERRUPTS(ints); \
    }
//*/

#define HAL_ICACHE_SYNC() HAL_ICACHE_INVALIDATE_ALL()


#define HAL_DCACHE_INVALIDATE_ALL() \
    { \
      int state = 0; \
      int ints  = 0; \
      HAL_DISABLE_INTERRUPTS(ints); \
      HAL_DCACHE_IS_ENABLED(state); \
      if (state) \
      {  \
        HAL_DCACHE_DISABLE(); \
        HAL_DCACHE_REENABLE(); \
      } \
      HAL_RESTORE_INTERRUPTS(ints); \
    }


// HAL_ICACHE_SET_CPLD
//
// macro for setting a new CPLD at index i into the INSTRUCTION mmu table.
//
// __index__      ( IN) index in the mmu table
//      cond:   [0..15]
// __base__       ( IN) base address of page
//      cond:   __base__ must be aligned to page size boundary specified in
//              __options__ 
// __options__    ( IN) options for the page that are needed for the mmu
//      cond:   HAL_ICACHE_OPTION_MASK is applied to the options
//              (all undefined fields must be 0)
//
//  postcon:    given value is stored in MMR register if valid
//              previous value is discarded
//
#ifndef HAL_ICACHE_SET_CPLD
#define HAL_ICACHE_SET_CPLD(__index__ , __base__, __options__) \
    { \
      if ((__index__ & HAL_ICACHE_MMU_ENTRY_MASK ) == 0) \
      {  \
        unsigned long mmr_data = HAL_ICACHE_DATA_BASE; \
        unsigned long index    = __index__ << 2;  \
        unsigned long mmr_addr = HAL_ICACHE_ADDR_BASE;  \
        mmr_data += index; \
        mmr_addr += index; \
        unsigned long base     = __base__; \
        unsigned long data     = __options__ & (HAL_ICACHE_SET_OPTION_MASK); \
        __asm __volatile(   "P0   = %0; "\
                            "R0   = %2; " \
                            "SSYNC; " \
                            "[P0] = R0; " \
                            "SSYNC; " \
                            "P0   = %1; "\
                            "R0   = %3; " \
                            "SSYNC; " \
                            "[P0] = R0; " \
                            "SSYNC; " \
                        : : "p"(mmr_addr), "p"(mmr_data), "r"(base), "r"(data) : "P0", "R0"); \
       } \
    }

#endif // HAL_ICACHE_SET_CPLD

// HAL_DCACHE_SET_CPLD
//
// macro for setting a new CPLD at index i into the DATA mmu table.
//
// __index__      ( IN) index in the mmu table
//      cond:   [0..15]
// __base__       ( IN) base address of page
//      cond:   __base__ must be aligned to page size boundary specified in
//              __options__ 
// __options__    ( IN) options for the page that are needed for the mmu
//      cond:   HAL_DCACHE_OPTION_MASK is applied to the options
//              (all undefined fields must be 0)
//
//  postcon:    given value is stored in MMR register if valid
//              previous value is discarded
//
#ifndef HAL_DCACHE_SET_CPLD
#define HAL_DCACHE_SET_CPLD(__index__ , __base__, __options__) \
    { \
      if ((__index__ & HAL_DCACHE_MMU_ENTRY_MASK ) == 0) \
      {  \
        unsigned long mmr_data = HAL_DCACHE_DATA_BASE; \
        unsigned long index    = __index__ << 2;  \
        unsigned long mmr_addr = HAL_DCACHE_ADDR_BASE;  \
        mmr_data += index; \
        mmr_addr += index; \
        unsigned long base     = __base__; \
        unsigned long data     = __options__ & (HAL_DCACHE_SET_OPTION_MASK); \
        __asm __volatile(   "P0   = %0; "\
                            "R0   = %2; " \
                            "SSYNC; " \
                            "[P0] = R0; " \
                            "SSYNC; " \
                            "P0   = %1; "\
                            "R0   = %3; " \
                            "SSYNC; " \
                            "[P0] = R0; " \
                            "SSYNC; " \
                        : : "p"(mmr_addr), "p"(mmr_data), "r"(base), "r"(data) : "P0", "R0"); \
       } \
    }

#endif // HAL_DCACHE_SET_CPLD

// HAL_ICACHE_IS_CPLD_AVAILABLE
//
// macro is testing if the CPLD is available for replacement
//
// __index__      ( IN) index in the mmu table
//      cond:   [0..15]
// __available__  (OUT) boolean (return)
//      cond:   [0..1]
//
//  postcon:    __available__ will be
//              0   if page is VALID  AND  LOCKED
//              1   otherwise
//
#ifndef HAL_ICACHE_IS_CPLD_AVAILABLE
#define HAL_ICACHE_IS_CPLD_AVAILABLE(__index__, __available__) \
    { \
      unsigned long options  = 0; \
      if ((__index__ & HAL_ICACHE_MMU_ENTRY_MASK ) == 0) \
      {  \
        unsigned long mmr_data = HAL_ICACHE_DATA_BASE;  \
        unsigned long index    = __index__ << 2;  \
        mmr_data += index; \
        __asm __volatile(   "P0   = %1; "\
                            "R0   = [P0]; " \
                            "%0   = R0; "\
                        : "=r"(options) : "p"(mmr_data) : "P0", "R0"); \
        if ((options & HAL_ICACHE_SET_UNAVAILABLE)==HAL_ICACHE_SET_UNAVAILABLE) \
             __available__=0; \
        else __available__=1; \
      } else __available__=0; \
    }
#endif // HAL_ICACHE_IS_CPLD_AVAILABLE



// HAL_DCACHE_IS_CPLD_AVAILABLE
//
// macro is testing if the CPLD is available for replacement
//
// __index__      ( IN) index in the mmu table
//      cond:   [0..15]
// __available__  (OUT) boolean (return)
//      cond:   [0..1]
//
//  postcon:    __available__ will be
//              0   if page is VALID  AND  LOCKED
//              1   otherwise
//
#ifndef HAL_DCACHE_IS_CPLD_AVAILABLE
#define HAL_DCACHE_IS_CPLD_AVAILABLE(__index__, __available__) \
    { \
      unsigned long options  = 0; \
      if ((__index__ & HAL_DCACHE_MMU_ENTRY_MASK ) == 0) \
      {  \
        unsigned long mmr_data = HAL_DCACHE_DATA_BASE + (__index__ << 2);  \
        __asm __volatile(   "P0   = %1; "\
                            "SSYNC; "\
                            "R0   = [P0]; " \
                            "%0   = R0; "\
                        : "=r"(options) : "p"(mmr_data) : "P0", "R0"); \
        if ((options & HAL_DCACHE_SET_UNAVAILABLE)==HAL_DCACHE_SET_UNAVAILABLE) \
             __available__=0; \
        else __available__=1; \
      } else __available__=0; \
    }
#endif // HAL_DCACHE_IS_CPLD_AVAILABLE



// HAL_ICACHE_CPLD_GET_OPTIONS
//
// macro for retrieving the CPLD at index in the MMU INSTRUCTION table
//
// __index__      ( IN) index in the mmu table
//      cond:   [0..15]
// __base__       (OUT) base address of page 
// __options__    (OUT) options of the page
//
//  postcon:    given value is stored in MMR register if valid
//              previous value is discarded
//
#ifndef HAL_ICACHE_CPLD_GET_OPTIONS
#define HAL_ICACHE_CPLD_GET_OPTIONS(__index__, __address__, __options__) \
    { \
      if ((__index__ & HAL_ICACHE_MMU_ENTRY_MASK ) == 0) \
      {  \
        unsigned long mmr_data = HAL_ICACHE_DATA_BASE; \
        unsigned long index    = __index__ << 2;  \
        unsigned long mmr_addr = HAL_ICACHE_ADDR_BASE;  \
        mmr_data += index; \
        mmr_addr += index; \
        __asm __volatile(   "P0   = %2; "\
                            "R0   = [P0]; " \
                            "%0   = R0; "\
                            "P0   = %3; "\
                            "R0   = [P0]; " \
                            "%1   = R0; "\
                        : "=r"(__options__), "=r"(__address__) : "p"(mmr_data), "p"(mmr_addr) : "P0", "R0"); \
        } \
    }
#endif // HAL_ICACHE_CPLD_GET_OPTIONS

// HAL_DCACHE_CPLD_GET_OPTIONS
//
// macro for retrieving the CPLD at index in the MMU INSTRUCTION table
//
// __index__      ( IN) index in the mmu table
//      cond:   [0..15]
// __base__       (OUT) base address of page 
// __options__    (OUT) options of the page
//
//  postcon:    given value is stored in MMR register if valid
//              previous value is discarded
//
#ifndef HAL_DCACHE_CPLD_GET_OPTIONS
#define HAL_DCACHE_CPLD_GET_OPTIONS(__index__, __address__, __options__) \
    { \
      if ((__index__ & HAL_DCACHE_MMU_ENTRY_MASK ) == 0) \
      {  \
        unsigned long mmr_data = HAL_DCACHE_DATA_BASE; \
        unsigned long index    = __index__ << 2;  \
        unsigned long mmr_addr = HAL_DCACHE_ADDR_BASE;  \
        mmr_data += index; \
        mmr_addr += index; \
        __asm __volatile(   "P0   = %2; "\
                            "R0   = [P0]; " \
                            "%0   = R0; "\
                            "P0   = %3; "\
                            "R0   = [P0]; " \
                            "%1   = R0; "\
                        : "=r"(__options__), "=r"(__address__) : "p"(mmr_data), "p"(mmr_addr) : "P0", "R0"); \
        } \
    }
#endif // HAL_DCACHE_CPLD_GET_OPTIONS



#define HAL_ICACHE_GET_FAULTING_ADDRESS(__address__) \
    { \
      __asm __volatile(   "P0.H   = 0xFFE0; "\
                            "P0.L = 0x100C; " \
                            "R0   = [P0];" \
                            "%0   = R0; "\
                        : "=r"(__address__) : : "P0", "R0"); \
    }


#define HAL_DCACHE_GET_FAULTING_ADDRESS(__address__) \
    { \
      __asm __volatile(   "P0.H   = 0xFFE0; "\
                            "P0.L = 0x000C; " \
                            "R0   = [P0];" \
                            "%0   = R0; "\
                        : "=r"(__address__) : : "P0", "R0"); \
    }
// no cache instructions defined as yet, no caches are used for initial port, due
// to mmu use for caching
#define HAL_FLASH_CACHES_OFF(_d_, _i_)
    

#define HAL_FLASH_CACHES_ON(_d_, _i_)

//-----------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_CACHE_H
// End of hal_cache.h
