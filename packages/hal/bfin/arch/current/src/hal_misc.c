//==========================================================================
//
//      hal_misc.c
//
//      HAL miscellaneous functions
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
// Author(s):    andre, Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:         2013-08-23
// Purpose:      HAL miscellaneous functions
// Description:  This file contains miscellaneous functions provided by the
//               HAL.
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <pkgconf/hal.h>

#include <cyg/infra/cyg_type.h>         // Base types
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros

//#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#include <cyg/hal/hal_arch.h>           // architectural definitions

#include <cyg/hal/hal_intr.h>           // Interrupt handling

#include <cyg/hal/hal_cache.h>          // Cache handling
#include <cyg/hal/hal_if.h>             // hal_ctrlc_isr()
//#include <cyg/hal/bfin-regs.h>          // FPU cause register definitions

#include CYGHWR_MEMORY_LAYOUT_H

/*------------------------------------------------------------------------*/
/* If required, define a variable to store the clock period.              */

#ifdef CYGHWR_HAL_CLOCK_PERIOD_DEFINED

CYG_WORD32 cyg_hal_clock_period;

#endif

extern unsigned long _sint_hal;
extern unsigned long _eint_hal;
extern unsigned long _START_OF_HAL;
extern unsigned long _END_OF_HAL;
extern unsigned long _START_OF_SYSTEM_DATA;
extern unsigned long _END_OF_SYSTEM_DATA;
extern unsigned long stext;
extern unsigned long etext;
extern unsigned long _ram_data_start;
extern unsigned long _ram_data_end;
extern unsigned long _rom_data_start;
extern unsigned long _rom_system_data_start;

extern unsigned long booted;
extern unsigned long _interrupt_stack;

static      int     d_cpld_min_next_replacement    = 0;

externC void dcplb_flush(unsigned long address, unsigned long data);

#define HAL_MISC_DEBUG 6

#if HAL_MISC_DEBUG & 1
    #define HAL_CACHE_STATUS
#endif

#if HAL_MISC_DEBUG & 2
    #define HAL_EXCEPTION_DEBUG
#endif

#if HAL_MISC_DEBUG & 4
    #define HAL_INTERRUPT_DEBUG
#endif

#if HAL_MISC_DEBUG & 8
    #define HAL_DCACHE_DEBUG
#endif

#if HAL_MISC_DEBUG & 16
    #define HAL_ICACHE_DEBUG
#endif



externC cyg_uint32 cyg_hal_exception_handler(HAL_SavedRegisters *regs);


void cyg_hal_diag_write_hex(unsigned long hex, int leading_zeros)
{
    const char numbers[] = "0123456789ABCDEF";
    char number[8] = "00000000";
    int index = 7;
    int numberedindex = 0;
    int first;
    
    while ( (index>-1) && (hex>0))
    {
        numberedindex = hex & 0xF;
        hex = hex >> 4;
        
        number[index] = numbers[numberedindex];
        
        index--;
    }

    index=0;

    diag_write_char('0');
    diag_write_char('x');
    first = 1;
    while(index<8)
    {
        if (leading_zeros==8-index)
            first=0;
        if ((!first) || (index==7) || (number[index]!='0') )
        {
            diag_write_char(number[index]);
            if (number[index]!='0')
                first=0;
        }
        index++;
    }
}

void cyg_hal_diag_write_string(char * string)
{
    if (string==0)
        return;

    unsigned int i = 0;
    while (string[i]!=0)
    {
        diag_write_char(string[i]);
        i++;
    }
}

void panic(char * string)
{
    diag_write_string("HAL PANIC : ");
    if (string!=0)
        diag_write_string(string);
    else
        diag_write_string("NMI");
    
    diag_write_char('\n');

    while (1)
        __asm __volatile("IDLE;");
}

void display_icache_cpld(int i)
{
    unsigned long address;
    unsigned long options;

    HAL_ICACHE_CPLD_GET_OPTIONS(i, address, options);
    
    cyg_hal_diag_write_hex(i,2);
    cyg_hal_diag_write_string(" | ");
    cyg_hal_diag_write_hex(address,8);
    cyg_hal_diag_write_string(" | ");
    switch(options & HAL_ICACHE_PAGE_SIZE_MASK)
    {
        case(HAL_ICACHE_PAGE_SIZE_1K) : cyg_hal_diag_write_string(" 1K ");
                                        break;
        case(HAL_ICACHE_PAGE_SIZE_4K) : cyg_hal_diag_write_string(" 4K ");
                                        break;
        case(HAL_ICACHE_PAGE_SIZE_1M) : cyg_hal_diag_write_string(" 1M ");
                                        break;
        case(HAL_ICACHE_PAGE_SIZE_4M) : cyg_hal_diag_write_string(" 4M ");
                                        break;
        default : cyg_hal_diag_write_string(" IN ");
                                        break;
    }
    cyg_hal_diag_write_string(" | ");
    if (options & HAL_ICACHE_SET_VALID)
        diag_write_char('X');
    else
        diag_write_char(' ');
    cyg_hal_diag_write_string(" | ");
    if (options & HAL_ICACHE_SET_LOCK)
        diag_write_char('X');
    else
        diag_write_char(' ');
    cyg_hal_diag_write_string(" | ");
    if (options & HAL_ICACHE_SET_CACHABLE)
        diag_write_char('X');
    else
        diag_write_char(' ');
    cyg_hal_diag_write_string(" | ");
    if (options & HAL_ICACHE_SET_HIGH_LRUPRIO)
        diag_write_char('X');
    else
        diag_write_char(' ');
    cyg_hal_diag_write_string(" | ");
    if (options & HAL_ICACHE_SET_ALLOW_USER_RD)
        diag_write_char('X');
    else
        diag_write_char(' ');

    diag_write_char('\n');
}

inline void display_icache()
{
    cyg_hal_diag_write_string("-------------------ICACHE--MMU---------------\n");
    cyg_hal_diag_write_string(" NR  |  ADDRESS   | SIZE | V | L | C | P | UR\n");
    cyg_hal_diag_write_string("---------------------------------------------\n");
    
    int a=0;
    while (a<16)
    {
        display_icache_cpld(a);

        a++;
    }
}

#define HAL_DCACHE_PROTECTION_ENABLED(__enabled__) \
    __asm __volatile( "P0.H = 0xFFE0;"  \
                      "P0.L = 0x0004;"  \
                      "SSYNC;" \
                      "R0   = [P0];" \
                      "%0   = R0;"  : "=r"(__enabled__) : : "P0", "R0");

void display_dcache_cpld(int i)
{
    unsigned long address;
    unsigned long options;

    HAL_DCACHE_CPLD_GET_OPTIONS(i, address, options);
    
    cyg_hal_diag_write_hex(i,2);
    diag_write_string(" | ");
    cyg_hal_diag_write_hex(address,8);
    diag_write_string(" | ");
    switch(options & HAL_DCACHE_PAGE_SIZE_MASK)
    {
        case(HAL_DCACHE_PAGE_SIZE_1K) : diag_write_string(" 1K ");
                                        break;
        case(HAL_DCACHE_PAGE_SIZE_4K) : diag_write_string(" 4K ");
                                        break;
        case(HAL_DCACHE_PAGE_SIZE_1M) : diag_write_string(" 1M ");
                                        break;
        case(HAL_DCACHE_PAGE_SIZE_4M) : diag_write_string(" 4M ");
                                        break;
        default : diag_write_string(" IN ");
                                        break;
    }
    diag_write_string(" | ");
    if (options & HAL_DCACHE_SET_VALID)
        diag_write_char('X');
    else
        diag_write_char(' ');
    diag_write_string(" | ");
    if (options & HAL_DCACHE_SET_LOCK)
        diag_write_char('X');
    else
        diag_write_char(' ');
    diag_write_string(" | ");
    if (options & HAL_DCACHE_SET_CACHABLE)
        diag_write_char('X');
    else
        diag_write_char(' ');
    diag_write_string(" | ");
    if (options & HAL_DCACHE_SET_DIRTY)
        diag_write_char('X');
    else
        diag_write_char(' ');
    diag_write_string(" | ");
    if (options & HAL_DCACHE_SET_WRITETHROUGH)
        diag_write_string("X ");
    else
        diag_write_string("  ");
    diag_write_string(" | ");
    if (options & HAL_DCACHE_SET_ALLOCATEWRITE)
        diag_write_string("X ");
    else
        diag_write_string("  ");
    diag_write_string(" | ");

    if (options & HAL_DCACHE_SET_ALLOW_SUPER_WR)
        diag_write_char('S');
    else
        diag_write_char('-');
    if (options & HAL_DCACHE_SET_ALLOW_USER_RD)
        diag_write_char('R');
    else
        diag_write_char('-');
    if (options & HAL_DCACHE_SET_ALLOW_USER_RD)
        diag_write_char('W');
    else
        diag_write_char('-');

    diag_write_char('\n');
}

inline void display_dcache()
{
    diag_write_string("-------------------DCACHE--MMU-----------------------------\n");
    diag_write_string(" NR  |  ADDRESS   | SIZE | V | L | C | D | WT | AW | RIGHTS\n");
    diag_write_string("-----------------------------------------------------------\n");
    int a = 0;
    while (a<16)
    {
        display_dcache_cpld(a);

        a++;
    }
}

/*
    hal_bfin_prepare_icache

    This function disables the instruction cache in order to be able to change the MMU MMR's.
    It will replace any current set of CPLD's in the 16 entry table without saving the current set.
    This function is intended to be used BEFORE enabling the CACHE.
    In order to use this function while the cache is in use the system must have copies of the
    CPLD's of the current set or must regenerate them when needed.

    INPUT PARAMS :  -
    OUTPUT PARAMS : -
    SIDEEFFECTS :   ICACHE is disabled after this function returns
                    only the newly entered CPLD's are valid.
*/
void hal_bfin_prepare_icache()
{
    int i = 0;
    unsigned long temp = 0x07000000;

    // disable Instruction cache
    HAL_ICACHE_DISABLE();

    unsigned long run = &_END_OF_HAL-&_START_OF_HAL;
    
    temp = (unsigned long)&_START_OF_HAL;
    temp = temp & ~0x0FFFFF;
    run = run * 4;

    if (run % (HAL_ICACHE_DEFAULT_MULTIPLIER*1024) != 0)
        run = (run / (HAL_ICACHE_DEFAULT_MULTIPLIER*1024))+1;
    else
        run = (run / (HAL_ICACHE_DEFAULT_MULTIPLIER*1024));

    
    while (   (i<16)
           && (i<run) )
    {
        
        HAL_ICACHE_SET_CPLD(i, temp, HAL_ICACHE_SET_CACHABLE | HAL_ICACHE_DEFAULT_PAGE_SIZE | HAL_ICACHE_SET_HIGH_LRUPRIO | HAL_ICACHE_SET_LOCK | HAL_ICACHE_SET_VALID);

        i++;
        temp+=(HAL_ICACHE_DEFAULT_MULTIPLIER*1024);
    }

    HAL_ICACHE_SET_CPLD(i, 0x20000000, HAL_ICACHE_PAGE_SIZE_4M | HAL_ICACHE_SET_HIGH_LRUPRIO | HAL_ICACHE_SET_LOCK | HAL_ICACHE_SET_VALID);

    i++;

    // invalidate the remaining CPLD's entries
    while (i<16)
    {
        // create a page table at the current location of i
        // the page entry is invalid
        HAL_ICACHE_SET_CPLD(i, 0xA0000000, HAL_ICACHE_DEFAULT_PAGE_SIZE );
        i++;
    }

#ifdef HAL_CACHE_STATUS
    if (booted)
        display_icache();
#endif
}


void hal_bfin_prepare_dcache_entries()
{
    int i = 0;
    unsigned long temp = 0x07000000;
//  unsigned long vector_table = (unsigned long)(&hal_virtual_vector_table);
    unsigned long run = &_END_OF_SYSTEM_DATA-&_START_OF_SYSTEM_DATA;
 
    HAL_DCACHE_PROT_DISABLE();
   
    temp = &_START_OF_SYSTEM_DATA;
    temp = temp & ~0x0FFF;
    run = run * 4;

    if (run % (4*1024) != 0)
        run = (run / (4*1024))+1;
    else
        run = (run / (4*1024));

    HAL_DCACHE_SET_CPLD(i, 0x00000000, HAL_DCACHE_PAGE_SIZE_64M | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | HAL_DCACHE_SET_CACHABLE | HAL_DCACHE_SET_ALLOCATEWRITE | HAL_DCACHE_SET_DIRTY | ANOMALY_05000158);
    i++;

    HAL_DCACHE_SET_CPLD(i, 0x04000000, HAL_DCACHE_PAGE_SIZE_64M | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | HAL_DCACHE_SET_CACHABLE | HAL_DCACHE_SET_ALLOCATEWRITE | HAL_DCACHE_SET_DIRTY | ANOMALY_05000158);
    i++;

    HAL_DCACHE_SET_CPLD(i, 0x08000000, HAL_DCACHE_PAGE_SIZE_64M | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | HAL_DCACHE_SET_CACHABLE | HAL_DCACHE_SET_ALLOCATEWRITE | HAL_DCACHE_SET_DIRTY | ANOMALY_05000158);
    i++;

    HAL_DCACHE_SET_CPLD(i, 0x0C000000, HAL_DCACHE_PAGE_SIZE_64M | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | HAL_DCACHE_SET_CACHABLE | HAL_DCACHE_SET_ALLOCATEWRITE | HAL_DCACHE_SET_DIRTY | ANOMALY_05000158);
    i++;

    HAL_DCACHE_SET_CPLD(i, 0xFF800000, HAL_DCACHE_PAGE_SIZE_4K | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | ANOMALY_05000158);
    i++;

    HAL_DCACHE_SET_CPLD(i, 0xFF801000, HAL_DCACHE_PAGE_SIZE_4K | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | ANOMALY_05000158);
    i++;

    HAL_DCACHE_SET_CPLD(i, 0xFF802000, HAL_DCACHE_PAGE_SIZE_4K | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | ANOMALY_05000158);
    i++;

    HAL_DCACHE_SET_CPLD(i, 0xFF803000, HAL_DCACHE_PAGE_SIZE_4K | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | ANOMALY_05000158);
    i++;

    HAL_DCACHE_SET_CPLD(i, 0xFFC00000, HAL_DCACHE_PAGE_SIZE_4M | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | ANOMALY_05000158);
    i++;
    
    HAL_DCACHE_SET_CPLD(i, 0xC8080000, HAL_DCACHE_PAGE_SIZE_64K | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | ANOMALY_05000158);
    i++;
    
    HAL_DCACHE_SET_CPLD(i, 0xC8090000, HAL_DCACHE_PAGE_SIZE_64K | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | ANOMALY_05000158);
    i++;
    
    HAL_DCACHE_SET_CPLD(i, 0xC80A0000, HAL_DCACHE_PAGE_SIZE_64K | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | ANOMALY_05000158);
    i++;
    
    HAL_DCACHE_SET_CPLD(i, 0xC80B0000, HAL_DCACHE_PAGE_SIZE_64K | HAL_DCACHE_SET_ALLOW_MASK | HAL_DCACHE_SET_LOCK | HAL_DCACHE_SET_VALID  | HAL_DCACHE_SET_WRITETHROUGH | ANOMALY_05000158);
    i++;

    d_cpld_min_next_replacement = i;

    // invalidate the remaining CPLD's entries
    while (i<16)
    {
        // create a page table at the current location of i
        // the page entry is invalid
        HAL_DCACHE_SET_CPLD(i, 0xA0000000, HAL_DCACHE_DEFAULT_PAGE_SIZE );
        i++;
    }

#ifdef HAL_CACHE_STATUS
    if (booted)
        display_dcache();
#endif
}
/*
    hal_bfin_prepare_dcache

    This function disables the data cache in order to be able to change the MMU MMR's.
    It will replace any current set of CPLD's in the 16 entry table without saving the current set.
    This function is intended to be used BEFORE enabling the CACHE.
    In order to use this function while the cache is in use the system must have copies of the
    CPLD's of the current set or must regenerate them when needed.

    INPUT PARAMS :  -
    OUTPUT PARAMS : -
    SIDEEFFECTS :   DCACHE is disabled after this function returns
                    only the newly entered CPLD's are valid.
*/
void hal_bfin_prepare_dcache()
{
    // disable Instruction cache
    HAL_DCACHE_DISABLE();

    hal_bfin_prepare_dcache_entries();
}

/*------------------------------------------------------------------------*/
/* First level C exception handler.                                       */

externC void __handle_exception (void);

externC HAL_SavedRegisters *_hal_registers;

externC void* volatile __mem_fault_handler;



inline int get_next_cache_location(int should, int data)
{
    int avail = 0;
    int tries = 0;


    while ((!avail) && tries<16)
    {
        if (data) {
            if (should<d_cpld_min_next_replacement)
                should = d_cpld_min_next_replacement;
            HAL_DCACHE_IS_CPLD_AVAILABLE(should, avail);
        } else {
            HAL_ICACHE_IS_CPLD_AVAILABLE(should, avail);
        }
        tries++;
        if (!avail)
            should = (should+1) % 16;
    }

    if (!avail)
    {
        return -1;
    }

    return should;

}

inline int get_next_icache_location(int should)
{
    return get_next_cache_location(should, 0);
}

inline int get_next_dcache_location(int should)
{
    return get_next_cache_location(should, 1);
}

cyg_uint32 cyg_hal_icache_exists(unsigned long page)
{
    int i = 0;
    int found = 0;
    unsigned long page_start = 0;

    unsigned long address, options;

    while ((i<16) && (!found))
    {
        HAL_ICACHE_CPLD_GET_OPTIONS(i , address, options);

        switch (options & HAL_ICACHE_PAGE_SIZE_MASK)
        {
            case(HAL_ICACHE_PAGE_SIZE_1K) :
                            page_start = page & ~0x3FF;
                            break;
            case(HAL_ICACHE_PAGE_SIZE_4K) :
                            page_start = page & ~0x0FFF;
                            break;
            case(HAL_ICACHE_PAGE_SIZE_1M) :
                            page_start = page & ~0x0FFFFF;
                            break;
            case(HAL_ICACHE_PAGE_SIZE_4M) :
            default:
                            page_start = page & ~0x3FFFFF;
                            break;
        }

        if (address == page_start)
            found = options & HAL_ICACHE_SET_VALID;

        i++;
    }

    return found;
    
}



inline cyg_uint32 cyg_hal_dcache_exists(unsigned long page)
{
    int i = 0;
    int found = 0;
    unsigned long page_start = 0;

    unsigned long address, options;

    while ((i<16) && (!found))
    {
        HAL_DCACHE_CPLD_GET_OPTIONS(i , address, options);

        switch (options & HAL_DCACHE_PAGE_SIZE_MASK)
        {
            case(HAL_DCACHE_PAGE_SIZE_1K) :
                            page_start = page & ~0x3FF;
                            break;
            case(HAL_DCACHE_PAGE_SIZE_4K) :
                            page_start = page & ~0x0FFF;
                            break;
            case(HAL_DCACHE_PAGE_SIZE_1M) :
                            page_start = page & ~0x0FFFFF;
                            break;
            case(HAL_DCACHE_PAGE_SIZE_4M) :
            default:
                            page_start = page & ~0x3FFFFF;
                            break;
        }

        if (address == page_start)
            found = options & HAL_DCACHE_SET_VALID;

        i++;
    }

    return found;
    
}

inline cyg_uint32 get_dcpld_for(unsigned long * address, unsigned long * options)
{

    unsigned    long    cacheable           = HAL_DCACHE_SET_CACHABLE;
    unsigned    long    page_size           = HAL_DCACHE_DEFAULT_PAGE_SIZE;
    // check cachability
    if (*address>=0x20000000)
        cacheable = 0;

    // check page size to be applied

    switch (page_size)
    {
            case(HAL_DCACHE_PAGE_SIZE_1K) :
                            *address = *address & ~0x3FF;
                            break;
            case(HAL_DCACHE_PAGE_SIZE_4K) :
                            *address = *address & ~0x0FFF;
                            break;
            case(HAL_DCACHE_PAGE_SIZE_1M) :
                            *address = *address & ~0x0FFFFF;
                            break;
            case(HAL_DCACHE_PAGE_SIZE_4M) :
            default:
                            *address = *address & ~0x3FFFFF;
                            break;
    }


    *options =     cacheable
                | HAL_DCACHE_SET_ALLOW_SUPER_WR
                | HAL_DCACHE_DEFAULT_PAGE_SIZE
                | HAL_DCACHE_SET_VALID
                | HAL_DCACHE_SET_CACHING_MODE
                | ANOMALY_05000158;

    return 1;
}

inline cyg_uint32 add_dcpld_for(unsigned long address, unsigned long request)
{
    unsigned    long    page_start          = address;
    unsigned    long    options             = 0;
    static      int     next_replacement    = 0;

    

        

    HAL_DCACHE_PROT_DISABLE();

    register unsigned long old_address;
    register unsigned long old_options;
    HAL_DCACHE_CPLD_GET_OPTIONS(next_replacement,old_address,old_options);
    if ( (old_options & HAL_DCACHE_SET_VALID)
        == HAL_DCACHE_SET_VALID)
            dcplb_flush(old_address, old_options);

    
    if (get_dcpld_for(&page_start,&options)) {
        if (next_replacement<d_cpld_min_next_replacement)
            next_replacement=d_cpld_min_next_replacement;
        // specific checks

    

        // if the instruction is located in a valid ram then
        // get the new page
        // get a spot that is available and not locked
        next_replacement = get_next_dcache_location(next_replacement+1);

        if (next_replacement==-1)
            panic("No DCACHE CPLD entry available for replacement!");

        HAL_DCACHE_SET_CPLD(next_replacement, page_start , options );
    }

#ifdef HAL_DCACHE_DEBUG
        diag_write_string("set DCPLD (");
        cyg_hal_diag_write_hex(next_replacement,0);
        diag_write_string(") for page ");
        cyg_hal_diag_write_hex(page_start,0);
        diag_write_string(") requested by ");
        cyg_hal_diag_write_hex(request,8);
        diag_write_string(" with options ");
        cyg_hal_diag_write_hex(options,0);
        diag_write_char('\n');
#endif

    HAL_DCACHE_PROT_REENABLE(); 
}

externC unsigned long hal_bfin_dcpld_handle_protection(void);

externC cyg_uint32 cyg_hal_dcache_miss(HAL_SavedRegisters *regs)
{
    int                 valid_ram           = 1;
    unsigned    long    page_start          = regs->dcpld_fault;
 
    if ((regs->seqstat&0x3f)==0x23)
    {
      // we have an access which could be a write to a clean data page
        HAL_DCACHE_PROT_DISABLE();
        
        page_start = hal_bfin_dcpld_handle_protection();
        
        HAL_DCACHE_PROT_REENABLE(); 
    } else {

        // do specific checks for code and text segments

        if (cyg_hal_dcache_exists(page_start))
        {
            page_start = page_start + (HAL_DCACHE_DEFAULT_MULTIPLIER*1024);
        }

#ifndef CYGNUM_HAL_BFIN_USE_MMU_MAP_CODE_SECTION
//        if ( page_start< &_ram_data_start)
//            valid_ram=0;

#endif
    
        

        if (    valid_ram
             && (!cyg_hal_dcache_exists(page_start)))
            
        {
            add_dcpld_for(page_start, regs->retx);
        }
    }

#ifdef HAL_DCACHE_STATUS
    if (booted)
    {
        display_dcache();
    }
#endif

    return 0;
}



externC cyg_uint32 cyg_hal_icache_miss(HAL_SavedRegisters *regs)
{
    int                 valid_ram           = 0;
    unsigned    long    page_start          = regs->icpld_fault;
    unsigned    long    page_size           = HAL_ICACHE_DEFAULT_PAGE_SIZE;
    static      int     next_replacement    = 0;

    // general RAM boundary check

    // check for instruction being in SDRAM
    if (  (page_start>=DRAM_START) 
        &&(page_start<DRAM_END))
        valid_ram=1;

    // check for instruction being in asynchronous RAM
    if (  (page_start>=ARAM_START) 
        &&(page_start<ARAM_END))
        valid_ram=1;

    // specific checks
#ifdef CYGNUM_HAL_BFIN_EXECUTE_TEXT_ONLY
    // do not add page descriptors for addresses
    // that are out of the text segment
    // this does not affect the HAL code segments.
    // the HAL code segments are accessible via
    // locked pages
    if ((page_start>=&etext))
        valid_ram=0;
#endif

    // if the instruction is located in a valid ram then
    // get the new page
    if (valid_ram)
    {
        // get a spot that is available and not locked
        next_replacement = get_next_icache_location(next_replacement+1);

        if (next_replacement==-1)
            panic("No ICACHE CPLD entry available for replacement!");

        switch (page_size)
        {
            case(HAL_ICACHE_PAGE_SIZE_1K) :
                            page_start = page_start & ~0x3FF;
                            break;
            case(HAL_ICACHE_PAGE_SIZE_4K) :
                            page_start = page_start & ~0x0FFF;
                            break;
            case(HAL_ICACHE_PAGE_SIZE_1M) :
                            page_start = page_start & ~0x0FFFFF;
                            break;
            case(HAL_ICACHE_PAGE_SIZE_4M) :
            default:
                            page_start = page_start & ~0x3FFFFF;
                            break;
        }

        // disable the cache temporarily
        HAL_ICACHE_PROT_DISABLE();

        if (cyg_hal_icache_exists(page_start))
        {
            page_start = page_start + (HAL_ICACHE_DEFAULT_MULTIPLIER*1024);
        }

    
        if (!cyg_hal_icache_exists(page_start))
        {
    
#ifdef HAL_ICACHE_DEBUG
  //  if (booted)
    {
        diag_write_string("set ICPLD (");
        cyg_hal_diag_write_hex(next_replacement,0);
        diag_write_string(") for page ");
        cyg_hal_diag_write_hex(page_start,0);
        diag_write_string(" with options ");
        cyg_hal_diag_write_hex(HAL_ICACHE_SET_CACHABLE | HAL_ICACHE_DEFAULT_PAGE_SIZE | HAL_ICACHE_SET_VALID,0);
        diag_write_char('\n');
    }
#endif
        
        HAL_ICACHE_SET_CPLD(next_replacement, page_start , HAL_ICACHE_SET_CACHABLE | HAL_ICACHE_DEFAULT_PAGE_SIZE | HAL_ICACHE_SET_VALID);
        
        // reenable the cache without initializing the CPLD table
        //next_replacement++;
        }

        HAL_ICACHE_PROT_REENABLE();
    }

#ifdef HAL_CACHE_STATUS
 //   if (booted)
    {
        display_icache();
    }
#endif

    return 0;
}



externC cyg_uint32 cyg_hal_assure_sp_access(unsigned long sp)
{
    unsigned long enabled = 0;

    HAL_DCACHE_PROTECTION_ENABLED(enabled);

    if (enabled)
    {
        HAL_DCACHE_PROT_DISABLE();

        hal_bfin_prepare_dcache_entries();

        HAL_DCACHE_PROT_REENABLE();

            if (!cyg_hal_dcache_exists(sp))
            {
                add_dcpld_for(sp,0);
            }
            if (!cyg_hal_dcache_exists(sp-sizeof(HAL_SavedRegisters)))
            {
                add_dcpld_for(sp-sizeof(HAL_SavedRegisters), 0);
            }

            display_dcache();

        
    }
}


externC cyg_uint32 cyg_hal_exception_handler(HAL_SavedRegisters *regs)
{
    unsigned long cause = CYGNUM_HAL_BFIN_EXCEPTIONS_BASE + ((regs->seqstat) & 0x3F);
    
    int    delivertokernel = 1;

#ifdef HAL_EXCEPTION_DEBUG
    if ((booted) && (delivertokernel))
    {
        cyg_hal_diag_write_string("EXCEPTION ");
        cyg_hal_diag_write_hex((regs->seqstat) & 0x3F,2);
        unsigned long ipend = 0;
        GET_IPEND(ipend);
        cyg_hal_diag_write_string(" in INT ");
        cyg_hal_diag_write_hex(ipend,4);
        cyg_hal_diag_write_string(" ) at/by ");
        cyg_hal_diag_write_hex(regs->retx,8);
        diag_write_char('\n');

    }

#endif

#if defined(CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS)
    // We should decode the vector and pass a more appropriate
    // value as the second argument. For now we simply pass a
    // pointer to the saved registers. We should also divert
    // breakpoint and other debug vectors into the debug stubs.

    // This part of the kernel exception handling causes big problems with caching

    if (delivertokernel)
    {
        // Set the pointer to the registers of the current exception
        // context. At entry the GDB stub will expand the
        // HAL_SavedRegisters structure into a (bigger) register array.
        _hal_registers = regs;
        __handle_exception();
    }

#elif defined(CYGFUN_HAL_COMMON_KERNEL_SUPPORT) && defined(CYGPKG_HAL_EXCEPTIONS)
    // We should decode the vector and pass a more appropriate
    // value as the second argument. For now we simply pass a
    // pointer to the saved registers. We should also divert
    // breakpoint and other debug vectors into the debug stubs.

    // This part of the kernel exception handling causes big problems with caching

    if (delivertokernel)
    {
        
        cyg_hal_deliver_exception( CYGNUM_HAL_BFIN_EXCEPTIONS_BASE + (regs->seqstat & 0x3F), (CYG_ADDRWORD)regs );
    }

#else
    if (delivertokernel)
    {
        hal_diag_print_reg_dump(regs);
        panic("unhandled exception!");
    }
#endif


    return 0;
}

/*------------------------------------------------------------------------*/
/* default ISR                                                            */

#ifndef CYGSEM_HAL_VIRTUAL_VECTOR_SUPPORT
externC cyg_uint32 hal_default_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data)
{
    
    CYG_TRACE1(true, "Interrupt: %d", vector);
  //  CYG_FAIL("Spurious Interrupt!!!");
    
    return 1;
}

#else // CYGSEM_HAL_VIRTUAL_VECTOR_SUPPORT

externC cyg_uint32 hal_arch_default_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data)
{
    CYG_TRACE1(true, "Interrupt: %d", vector);

#ifdef HAL_INTERRUPT_DEBUG
    if (booted)
    {
        unsigned long help;
        cyg_hal_diag_write_string("INTERRUPT ");
        cyg_hal_diag_write_hex(vector ,8);
        cyg_hal_diag_write_string(" caught (unhandled) and masked");
        cyg_hal_diag_write_string("\n");
        HAL_INTERRUPT_MASK(vector);
    }
#endif

    return 1;
}


#endif // CYGSEM_HAL_VIRTUAL_VECTOR_SUPPORT

externC cyg_uint32 hal_arch_default_hardware_error_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data)
{
    CYG_TRACE1(true, "Interrupt: %d", vector);

        unsigned long help;

        __asm __volatile("R0 = SEQSTAT;"
                         "R0 >>= 14;"
                         "%0 = R0;" : "=r"(help) : : "R0");

        
        help = help & 0x1F;
        cyg_hal_diag_write_string("HW ERROR : ");
        cyg_hal_diag_write_hex(help ,8);
        cyg_hal_diag_write_string(" @ ");
        __asm __volatile("R0 = RETI;"
                         "%0 = R0;" : "=r"(help) : : "R0");
        cyg_hal_diag_write_hex(help ,8);
        cyg_hal_diag_write_string(" caught (unhandled)\n");

    return 1;
}

/*------------------------------------------------------------------------*/
/* data copy and bss zero functions                                       */

typedef void (CYG_SYM_ADDRESS)(void);

// All these must use this type of address to stop them being given relocations
// relative to $gp (i.e. assuming they would be in .sdata)


#ifdef CYG_HAL_STARTUP_UBOOT      
void hal_copy_data(void)
{

    char *    p = (char *)&_START_OF_SYSTEM_DATA;
    char *    q = (char *)&_rom_data_start;
    
    while( p != (char *)&_ram_data_end )
        *p++ = *q++;

}
#endif

/*------------------------------------------------------------------------*/

#ifdef CYGSEM_HAL_STOP_CONSTRUCTORS_ON_FLAG
cyg_bool cyg_hal_stop_constructors;
#endif

typedef void (*pfunc) (void);
extern pfunc __CTOR_LIST__[];
extern pfunc __CTOR_END__[];

void
cyg_hal_invoke_constructors(void)
{
#ifdef CYGSEM_HAL_STOP_CONSTRUCTORS_ON_FLAG
    static pfunc *p = &__CTOR_END__[-1];
    cyg_hal_stop_constructors = 0;

    for (; p >= __CTOR_LIST__; p--) {
        (*p) ();
        if (cyg_hal_stop_constructors) {
            p--;
            break;
        }
    }
#else
    pfunc *p;

    register int i = 0;

//    for (p = &__CTOR_END__[-1]; p >= __CTOR_LIST__; p--)

    for (p = &__CTOR_END__[-1]; p >= __CTOR_LIST__ ; p--)
    {
        if (   (*p == 0x0)
            || (*p == 0xFFFFFFFF) )
        {
        } else {
        (*p) ();
        }
    }
#endif
    
} // cyg_hal_invoke_constructors()

/*------------------------------------------------------------------------*/
/* Determine the index of the ls bit of the supplied mask.                */
extern unsigned long * hal_interrupt_sic_assign_mask;
cyg_uint32 hal_lsbit_index(cyg_uint32 mask)
{
    cyg_uint32 n = mask;

    static const signed char tab[64] =
    { -1, 0, 1, 12, 2, 6, 0, 13, 3, 0, 7, 0, 0, 0, 0, 14, 10,
      4, 0, 0, 8, 0, 0, 25, 0, 0, 0, 0, 0, 21, 27 , 15, 31, 11,
      5, 0, 0, 0, 0, 0, 9, 0, 0, 24, 0, 0 , 20, 26, 30, 0, 0, 0,
      0, 23, 0, 19, 29, 0, 22, 18, 28, 17, 16, 0
    };

    n &= ~(n-1UL);
    n = (n<<16)-n;
    n = (n<<6)+n;
    n = (n<<4)+n;
    
    return tab[n>>26];
}

cyg_uint32 hal_lsbit_index_int(cyg_uint32 mask)
{
    cyg_uint32 n = mask;

    static const signed char tab[64] =
    { -1, 0, 1, 12, 2, 6, 0, 13, 3, 0, 7, 0, 0, 0, 0, 14, 10,
      4, 0, 0, 8, 0, 0, 25, 0, 0, 0, 0, 0, 21, 27 , 15, 31, 11,
      5, 0, 0, 0, 0, 0, 9, 0, 0, 24, 0, 0 , 20, 26, 30, 0, 0, 0,
      0, 23, 0, 19, 29, 0, 22, 18, 28, 17, 16, 0
    };

    n &= ~(n-1UL);
    n = (n<<16)-n;
    n = (n<<6)+n;
    n = (n<<4)+n;

    return tab[n>>26];
}
/*------------------------------------------------------------------------*/
/* Determine the index of the ms bit of the supplied mask.                */

cyg_uint32 hal_msbit_index(cyg_uint32 mask)
{
    cyg_uint32 x = mask;    
    cyg_uint32 w;

    /* Phase 1: make word with all ones from that one to the right */
    x |= x >> 16;
    x |= x >> 8;
    x |= x >> 4;
    x |= x >> 2;
    x |= x >> 1;

    /* Phase 2: calculate number of "1" bits in the word        */
    w = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    w = (w & 0x33333333) + ((w >> 2) & 0x33333333);
    w = w + (w >> 4);
    w = (w & 0x000F000F) + ((w >> 8) & 0x000F000F);
    return (cyg_uint32)((w + (w >> 16)) & 0xFF) - 1;

}

/*------------------------------------------------------------------------*/
/* Delay for some number of useconds.                                     */
void 
hal_delay_us(int us)
{
    cyg_uint32 val1, val2;
    int diff;
    long usticks;
    long ticks;

    // Calculate the number of counter register ticks per microsecond.
    
    //usticks = (CYGNUM_HAL_RTC_PERIOD / 10000);
    usticks = 5;

    // Make sure that the value is not zero. This will only happen if the
    // CPU is running at < 2MHz.
    if( usticks == 0 ) usticks = 1;
    
    while( us > 0 )
    {
        int us1 = us;

        // Wait in bursts of less than 10000us to avoid any overflow
        // problems in the multiply.
        if( us1 > 10000 )
            us1 = 10000;

        us -= us1;

        ticks = us1 * usticks;

        HAL_CLOCK_READ(&val1);
        while (ticks > 0) {
            do {
                HAL_CLOCK_READ(&val2);
            } while (val1 == val2);
            diff = val2 - val1;
            if (diff < 0) diff += CYGNUM_HAL_RTC_PERIOD;
            ticks -= diff;
            val1 = val2;
        }
    }
}

/*------------------------------------------------------------------------*/


void hal_arch_program_new_stack(void *_func)
{
    externC void hal_program_new_stack( void *func, CYG_ADDRESS addr);
    hal_program_new_stack( (void *)_func, 0x0);
 //   hal_program_new_stack( (void *)_func,
 //                  (CYGMEM_REGION_ram + CYGMEM_REGION_ram_SIZE - sizeof(CYG_ADDRESS)) & ~15 );
}//*/

/*
void first_entry_to_thread(register void * anything)
{
   register unsigned long * IPEND = 0xFFE02108;

   register unsigned long   ipend = (*IPEND);

   if ((ipend & 0x7FEF)!=0)
   {
        __asm __volatile("R0 = %0; \n"
                         "RTI; \n" : : "r"(anything));
   } else {
        __asm __volatile("R0 = %0; \n"
                         "RETS = RETI; \n"
                         "RTS; \n" : : "r"(anything));
   }
}//*/
/*------------------------------------------------------------------------*/
/* Idle thread action  
                                                   */
/*
int main(int argc, char * argv[])
{
    return 0;
} //*/

//#include <cyg/infra/diag.h>

void hal_idle_thread_action( cyg_uint32 count )
{

    cyg_uint32 i = 0;
    for(i = 0; i<count; i++)
    {
        ;
    }

}

/***************************************************************************//*!
    * @brief        Flush a Memory Area
*
* @param[in]    start   Pointer to the start of the memory Segment
* @param[in]    bytes   Number of bytes to flush
*
* @return       None
*
* @details
*               Function flush the given memory segment by flushing out each
*               memory element. If the memory is cached and dirty, the data will
*               be written back to the next higher memory else it act as NOP.
*
* @sa           flushinv_area
*******************************************************************************/
void hal_flush_area(void *start, cyg_uint32 bytes)
{
    cyg_uint32 count;
    cyg_uint32 x;

    count = (bytes + 31U + (((cyg_uint32)start) & 31U))/32U;

    /* System Sync */
    asm volatile ("SSYNC;");

    /* Flush given cache area */
    for (x=0U; x < count; x++)
    {
        /* Flush the cache line at location start and increment start to next
           cache line */
        HAL_DCACHE_FLUSH(start);
        start+=32;
    }

    /* System Sync */
    asm volatile ("SSYNC;");
}

/***************************************************************************//*!
* @brief        Flush & Invalidate a Memory Area
*
* @param[in]    start   Pointer to the start of the memory Segment
* @param[in]    bytes   Number of bytes to flush
*
* @return       None
*
* @details
*               Function flush the given memory segment by flushing out each
*               memory element and it also invalidate the cache. If the memory
*               is cached and dirty, the data will be written back to the next
*               higher memory else it act as NOP
*
* @sa           flush_area
*******************************************************************************/
void hal_flushinv_area(void *start, cyg_uint32 bytes)
{
    cyg_uint32 count;
    cyg_uint32 x;

    count = (bytes + 31U + (((cyg_uint32)start) & 31U))/32U;

    /* System sync */
    asm volatile ("SSYNC;");

    /* Invalidate cache in the given area */
    for (x=0U; x < count; x++)
    {
        /* Inverse Flush the location start and move to next cache line */
        HAL_DCACHE_INVALIDATE(start);
        start+=32;
    }

    /* System Sync */
    asm volatile ("SSYNC;");
}

void _exit(void) {
    while(1) {
        // Loop until reset
    }
}
void _sbrk(void) {
    while(1) {
        // Loop until reset
    }
}

void _getpid(void) {
    while(1) {
        // Loop until reset
    }
}

void _kill(void) {
    while(1) {
        // Loop until reset
    }
}

void _close(void) {
    while(1) {
        // Loop until reset
    }
}

void _fstat(void) {
    while(1) {
        // Loop until reset
    }
}

void _isatty(void) {
    while(1) {
        // Loop until reset
    }
}

void _lseek(void) {
    while(1) {
        // Loop until reset
    }
}

void _read(void) {
    while(1) {
        // Loop until reset
    }
}

void _write(void) {
    while(1) {
        // Loop until reset
    }
}
/*------------------------------------------------------------------------*/
/* End of hal_misc.c                                                      */
