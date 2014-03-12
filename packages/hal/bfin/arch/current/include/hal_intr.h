#ifndef CYGONCE_HAL_HAL_INTR_H
#define CYGONCE_HAL_HAL_INTR_H

//==========================================================================
//
//      hal_intr.h
//
//      HAL Interrupt and clock support
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
// Date:         2006-05-22
// Purpose:      Define Interrupt support
// Description:  The macros defined here provide the HAL APIs for handling
//               interrupts and the clock.
//              
// Usage:
//              #include <cyg/hal/hal_intr.h>
//              ...
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>

#include <cyg/infra/cyg_type.h>
#include <cyg/hal/hal_io.h>


#include <cyg/hal/var_intr.h>

//--------------------------------------------------------------------------
// BFIN vectors. 

// These are the exception codes presented in the Cause register and
// correspond to VSRs. These values are the ones to use for HAL_VSR_GET/SET
// EMU interrupt
#define CYGNUM_HAL_VECTOR_INTR_0                        0
// reset interrupt
#define CYGNUM_HAL_VECTOR_INTR_1                        1
// NMI
#define CYGNUM_HAL_VECTOR_INTR_2                        2
#define CYGNUM_HAL_VECTOR_INTERRUPT                     CYGNUM_HAL_VECTOR_INTR_2
// exception interrupt, to be ignored
#define CYGNUM_HAL_VECTOR_INTR_3                        3
// reserved
#define CYGNUM_HAL_VECTOR_INTR_4                        4
// hardware error interrupt
#define CYGNUM_HAL_VECTOR_INTR_5                        5
// core timer interrupt
#define CYGNUM_HAL_VECTOR_INTR_6                        6

// software exception 0 - return from exception
#define CYGNUM_HAL_VECTOR_SOFT_0                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+0)
// software exception 1 - breakpoint
#define CYGNUM_HAL_VECTOR_BREAKPOINT                    CYGNUM_HAL_VECTOR_SOFT_1
#define CYGNUM_HAL_VECTOR_SOFT_1                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+1)
// software exception 2
#define CYGNUM_HAL_VECTOR_SOFT_2                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+2)
// software exception 3
#define CYGNUM_HAL_VECTOR_SOFT_3                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+3)
// software exception 4
#define CYGNUM_HAL_VECTOR_SOFT_4                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+4)
// software exception 5
#define CYGNUM_HAL_VECTOR_SOFT_5                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+5)
// software exception 6
#define CYGNUM_HAL_VECTOR_SOFT_6                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+6)
// software exception 7
#define CYGNUM_HAL_VECTOR_SOFT_7                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+7)
// software exception 8
#define CYGNUM_HAL_VECTOR_SOFT_8                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+8)
// software exception 9
#define CYGNUM_HAL_VECTOR_SOFT_9                        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+9)
// software exception 10
#define CYGNUM_HAL_VECTOR_SOFT_10                       (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+10)
// software exception 11
#define CYGNUM_HAL_VECTOR_SOFT_11                       (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+11)
// software exception 12
#define CYGNUM_HAL_VECTOR_SOFT_12                       (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+12)
// software exception 13
#define CYGNUM_HAL_VECTOR_SOFT_13                       (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+13)
// software exception 14
#define CYGNUM_HAL_VECTOR_SOFT_14                       (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+14)
// software exception 15
#define CYGNUM_HAL_VECTOR_SOFT_15                       (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+15)


// single step exception 0x10
#define CYGNUM_HAL_VECTOR_SINGLE_STEP                   (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+16)
// trace buffer full exception 0x11
#define CYGNUM_HAL_VECTOR_TRACEBUFFER_FULL              (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+17)
// undefined instruction 0x21
#define CYGNUM_HAL_VECTOR_UNDEFINED_INSTRUCTION         (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+33)

// 0x22          /* illegal opcode combination */
#define CYGNUM_HAL_VECTOR_ILLEGAL_COMBINATION           (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+34)
// 0x23          /* attempt to read/write CPLD protected memory
#define CYGNUM_HAL_VECTOR_DATA_CPLD_PROTECTED           (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+35)
//#define CAUSE_D_MISAL   0x24          /* data misaligned */
#define CYGNUM_HAL_VECTOR_DATA_MISALIGNED               (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+36)
//#define CAUSE_UNRECOV   0x25          /* unrecoverable event */
#define CYGNUM_HAL_VECTOR_UNRECOVERABLE                 (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+37)
//#define CAUSE_D_CPLB_MISS 0x26        /* data CPLB miss */
#define CYGNUM_HAL_VECTOR_DATA_CPLD_MISS                (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+38)
//#define CAUSE_D_CPLB_MULT 0x27        /* more than one data CPLB entry matches address */
#define CYGNUM_HAL_VECTOR_DATA_CPLD_MULTIPLE            (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+39)
//#define CAUSE_EMWATCH   0x28          /* emulation watchpoint */
#define CYGNUM_HAL_VECTOR_EMU_WATCHPOINT                (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+40)
//#define CAUSE_I_MISAL   0x2A          /* instruction fetch misaligned */
#define CYGNUM_HAL_VECTOR_INSTRUCTION_MISALIGNED        (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+42)
//#define CAUSE_I_CPLB    0x2B          /* attempt to read/write CPLD protected memory
#define CYGNUM_HAL_VECTOR_INST_CPLD_PROTECTED           (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+43)
//#define CAUSE_I_CPLB_MISS 0x2C        /* instruction CPLB miss */
#define CYGNUM_HAL_VECTOR_INST_CPLD_MISS                (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+44)
//#define CAUSE_I_CPLB_MULT 0x2D        /* more than one instruction CPLB entry matches address */
#define CYGNUM_HAL_VECTOR_INST_CPLD_MULTIPLE            (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+45)
//#define CAUSE_ILLSUP    0x2E          /* illegal use of supervisor ressource */
#define CYGNUM_HAL_VECTOR_PROTECTED_RESSOURCE           (CYGNUM_HAL_BFIN_EXCEPTIONS_BASE+46)

#define CYGNUM_HAL_VSR_MIN                     CYGNUM_HAL_VECTOR_INTR_0
#define CYGNUM_HAL_VSR_MAX                     CYGNUM_HAL_VECTOR_PROTECTED_RESSOURCE
#define CYGNUM_HAL_VSR_COUNT                   (CYGNUM_HAL_VSR_MAX-CYGNUM_HAL_VSR_MIN+1)

// Exception vectors. These are the values used when passed out to an
// external exception handler using cyg_hal_deliver_exception()

#define CYGNUM_HAL_EXCEPTION_DATA_TLBMISS_ACCESS \
          CYGNUM_HAL_VECTOR_DATA_CPLD_MISS
#define CYGNUM_HAL_EXCEPTION_DATA_TLBMISS_WRITE \
          CYGNUM_HAL_VECTOR_DATA_CPLD_MISS
#define CYGNUM_HAL_EXCEPTION_DATA_UNALIGNED_ACCESS \
          CYGNUM_HAL_VECTOR_DATA_MISALIGNED
#define CYGNUM_HAL_EXCEPTION_DATA_UNALIGNED_WRITE \
          CYGNUM_HAL_VECTOR_DATA_MISALIGNED
#define CYGNUM_HAL_EXCEPTION_INSTRUCTION_BP CYGNUM_HAL_VECTOR_SOFT_1
#define CYGNUM_HAL_EXCEPTION_ILLEGAL_INSTRUCTION \
          CYGNUM_HAL_VECTOR_UNDEFINED_INSTRUCTION
#define CYGNUM_HAL_EXCEPTION_CODE_ACCESS         CYGNUM_HAL_VECTOR_PROTECTED_RESSOURCE
#define CYGNUM_HAL_EXCEPTION_DATA_ACCESS         CYGNUM_HAL_VECTOR_PROTECTED_RESSOURCE

#define CYGNUM_HAL_EXCEPTION_INTERRUPT      CYGNUM_HAL_VECTOR_SOFT_1

// Min/Max exception numbers and how many there are
#define CYGNUM_HAL_EXCEPTION_MIN                CYGNUM_HAL_BFIN_EXCEPTIONS_BASE
#define CYGNUM_HAL_EXCEPTION_MAX                CYGNUM_HAL_VSR_MAX

#define CYGNUM_HAL_EXCEPTION_COUNT           \
                 ( CYGNUM_HAL_EXCEPTION_MAX - CYGNUM_HAL_EXCEPTION_MIN + 1 )


#ifndef CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED

// the BFIN has 16 interrupts of which some are reserved, like 0, 3 and 4
// interrupt source is determined either by the entry point taken or the
// least significant bit set in IPEND (except bit 4)

// emulation
#define CYGNUM_HAL_INTERRUPT_0                0
// reset
#define CYGNUM_HAL_INTERRUPT_1                1
// NMI
#define CYGNUM_HAL_INTERRUPT_2                2
// exception (ignore settings)
#define CYGNUM_HAL_INTERRUPT_3                3
// reserved, ignore
#define CYGNUM_HAL_INTERRUPT_4                4
// hwr error
#define CYGNUM_HAL_INTERRUPT_5                5
// timer
#define CYGNUM_HAL_INTERRUPT_6                6



// The vector used by the Real time clock.

#ifndef CYGNUM_HAL_INTERRUPT_RTC
#define CYGNUM_HAL_INTERRUPT_RTC            CYGNUM_HAL_INTERRUPT_6
#endif

#define CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED

#endif

//--------------------------------------------------------------------------
// Static data used by HAL

// ISR tables
externC volatile CYG_ADDRESS    hal_interrupt_handlers[CYGNUM_HAL_ISR_COUNT];
externC volatile CYG_ADDRWORD   hal_interrupt_data[CYGNUM_HAL_ISR_COUNT];
externC volatile CYG_ADDRESS    hal_interrupt_objects[CYGNUM_HAL_ISR_COUNT];

// VSR table
externC volatile CYG_ADDRESS    hal_vsr_table[CYGNUM_HAL_VSR_MAX+1];

//--------------------------------------------------------------------------
// Default ISR
// The #define is used to test whether this routine exists, and to allow
// us to call it.

externC cyg_uint32 hal_default_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data);

#define HAL_DEFAULT_ISR hal_default_isr

//--------------------------------------------------------------------------
// Interrupt state storage

typedef cyg_uint32 CYG_INTERRUPT_STATE;

//--------------------------------------------------------------------------
// Interrupt control macros
#ifndef CYGHWR_HAL_INTERRUPT_ENABLE_DISABLE_RESTORE_DEFINED

#define HAL_DISABLE_INTERRUPTS(_old_)           \
{                                               \
    asm volatile (                              \
        "CLI R0;"                               \
        "R1.H = 0x0;" \
        "R1.L = 0x8000;" \
        "R1   = R1 & R0;"\
        "%0   = R1;"                            \
        : "=r"(_old_)                           \
        :                                       \
        : "R0", "R1"                                  \
        );                                      \
}
/*
#define HAL_DISABLE_INTERRUPTS(_old_)           \
{                                               \
    asm volatile (                              \
        "CLI R0;"                               \
        "R1.H = 0x0;" \
        "R1.L = 0xFFE0;" \
        "R1   = R1 & R0;"\
        "%0   = R1;"                            \
        : "=r"(_old_)                           \
        :                                       \
        : "R0", "R1"                                  \
        );                                      \
}
//*/

// R0.H was 0xffff before

#define HAL_ENABLE_INTERRUPTS()                 \
{                                               \
    asm volatile (                              \
        "R0.L = 0x8000;"                        \
        "R0.H = 0x0000;"                        \
        "P0.H = _hal_intr_mask_applied;"        \
        "P0.L = _hal_intr_mask_applied;"        \
        "R1   = [P0];"                          \
        "R0   = R1 | R0;"                       \
        "[P0]  = R0;"                           \
        "SSYNC;"                                \
        "STI R0;"                               \
        "SSYNC;"                                \
        :                                       \
        :                                       \
        :  "R0", "R1", "P0"                     \
        );                                      \
}
/*
#define HAL_ENABLE_INTERRUPTS()                 \
{                                               \
    asm volatile (                              \
        "R0.L = 0x3FFF;"                        \
        "R0.H = 0x0000;"                        \
        "P0.H = _hal_intr_mask_applied;"        \
        "P0.L = _hal_intr_mask_applied;"        \
        "[P0] = R0;"                            \
        "SSYNC;"                                \
        "STI R0;"                               \
        "SSYNC;"                                \
        :                                       \
        :                                       \
        :  "R0", "P0"                           \
        );                                      \
}
//*/

// RESTORE interrupts must be changed to include the
// newly set interrupts

//         "STI    %0;"                            

#define HAL_RESTORE_INTERRUPTS(_old_)           \
{                                               \
    asm volatile (                              \
        "CLI R1;"                               \
        "R0 = %0;"                              \
        "P0.H = _hal_intr_mask_applied;"        \
        "P0.L = _hal_intr_mask_applied;"        \
        "SSYNC;"                                \
        "R2   = [P0];"                          \
        "SSYNC;"                                \
        "R1 = R1 ^ R1;"                         \
        "CC = R0;"                              \
        "IF CC R1 = R2;"                        \
        "STI    R1;"                            \
        "SSYNC;"                                \
        :                                       \
        : "r"(_old_)                            \
        : "R0", "R1", "R2", "P0"                            \
        );                                      \
}

/*
#define HAL_QUERY_INTERRUPTS( _state_ )         \
{                                               \
    asm volatile (                              \
        "CLI R0;"                        \
        "STI R0;"                        \
        "CSYNC;"                                \
        "R1.H = 0x0;" \
        "R1.L = 0xFFE0;" \
        "R1   = R1 & R0;"\
        "%0   = R1;"                            \
        : "=r"(_state_)                         \
        :                                       \
        : "R0", "R1"                            \
        );                                      \
}//*/

#define HAL_QUERY_INTERRUPTS( _state_ )         \
{                                               \
    asm volatile (                              \
        "CLI R0;"                        \
        "STI R0;"                        \
        "CSYNC;"                                \
        "R1.H = 0x0;" \
        "R1.L = 0x8000;" \
        "R1   = R1 & R0;"\
        "%0   = R1;"                            \
        : "=r"(_state_)                         \
        :                                       \
        : "R0", "R1"                            \
        );                                      \
}

#endif // CYGHWR_HAL_INTERRUPT_ENABLE_DISABLE_RESTORE_DEFINED

//--------------------------------------------------------------------------
// Routine to execute DSRs using separate interrupt stack

#ifdef  CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK
//externC void hal_interrupt_stack_call_pending_DSRs(void);
//#define HAL_INTERRUPT_STACK_CALL_PENDING_DSRS() 
//    hal_interrupt_stack_call_pending_DSRs()

// these are offered solely for stack usage testing
// if they are not defined, then there is no interrupt stack.
#define HAL_INTERRUPT_STACK_BASE cyg_interrupt_stack_base
#define HAL_INTERRUPT_STACK_TOP  cyg_interrupt_stack
// use them to declare these extern however you want:
//       extern char HAL_INTERRUPT_STACK_BASE[];
//       extern char HAL_INTERRUPT_STACK_TOP[];
// is recommended
#endif

//--------------------------------------------------------------------------
// Vector translation.
// For chained interrupts we only have a single vector though which all
// are passed. For unchained interrupts we have a vector per interrupt.

#ifndef HAL_TRANSLATE_VECTOR

#if defined(CYGIMP_HAL_COMMON_INTERRUPTS_CHAIN)

#define HAL_TRANSLATE_VECTOR(_vector_,_index_) (_index_) = 0

#else

#define HAL_TRANSLATE_VECTOR(_vector_,_index_) (_index_) = (_vector_)

#endif

#endif

//--------------------------------------------------------------------------
// Interrupt and VSR attachment macros

#define HAL_INTERRUPT_IN_USE( _vector_, _state_)                          \
    CYG_MACRO_START                                                       \
    cyg_uint32 _index_;                                                   \
    HAL_TRANSLATE_VECTOR ((_vector_), _index_);                           \
                                                                          \
    if( hal_interrupt_handlers[_index_] == (CYG_ADDRESS)HAL_DEFAULT_ISR ) \
        (_state_) = 0;                                                    \
    else                                                                  \
        (_state_) = 1;                                                    \
    CYG_MACRO_END

#define HAL_INTERRUPT_ATTACH( _vector_, _isr_, _data_, _object_ )           \
{                                                                           \
    cyg_uint32 _index_;                                                     \
    HAL_TRANSLATE_VECTOR( _vector_, _index_ );                              \
                                                                            \
    if( hal_interrupt_handlers[_index_] == (CYG_ADDRESS)HAL_DEFAULT_ISR )   \
    {                                                                       \
        hal_interrupt_handlers[_index_] = (CYG_ADDRESS)_isr_;               \
        hal_interrupt_data[_index_] = (CYG_ADDRWORD)_data_;                 \
        hal_interrupt_objects[_index_] = (CYG_ADDRESS)_object_;             \
    }                                                                       \
}

#define HAL_INTERRUPT_DETACH( _vector_, _isr_ )                         \
{                                                                       \
    cyg_uint32 _index_;                                                 \
    HAL_TRANSLATE_VECTOR( _vector_, _index_ );                          \
                                                                        \
    if( hal_interrupt_handlers[_index_] == (CYG_ADDRESS)_isr_ )         \
    {                                                                   \
        hal_interrupt_handlers[_index_] = (CYG_ADDRESS)HAL_DEFAULT_ISR; \
        hal_interrupt_data[_index_] = 0;                                \
        hal_interrupt_objects[_index_] = 0;                             \
    }                                                                   \
}

#define HAL_VSR_GET( _vector_, _pvsr_ )                 \
    *(_pvsr_) = (void (*)())hal_vsr_table[_vector_];
    

#define HAL_VSR_SET( _vector_, _vsr_, _poldvsr_ ) CYG_MACRO_START         \
    if  (  (_vector_ == CYGNUM_HAL_VECTOR_DATA_CPLD_MISS)                  \
       || (_vector_ == CYGNUM_HAL_VECTOR_INST_CPLD_MISS)  )                \
        ;                                                                 \
    else {                                                                \
    if( (void*)_poldvsr_ != NULL)                                         \
        *(CYG_ADDRESS *)_poldvsr_ = (CYG_ADDRESS)hal_vsr_table[_vector_]; \
    hal_vsr_table[_vector_] = (CYG_ADDRESS)_vsr_;                         \
    } \
CYG_MACRO_END

// This is an ugly name, but what it means is: grab the VSR back to eCos
// internal handling, or if you like, the default handler.  But if
// cooperating with GDB and CygMon, the default behaviour is to pass most
// exceptions to CygMon.  This macro undoes that so that eCos handles the
// exception.  So use it with care.

externC void __default_exception_vsr(void);
externC void __default_interrupt_vsr(void);
externC void __break_vsr_springboard(void);

#define HAL_VSR_SET_TO_ECOS_HANDLER( _vector_, _poldvsr_ ) CYG_MACRO_START  \
    HAL_VSR_SET( _vector_, _vector_ == CYGNUM_HAL_VECTOR_INTERRUPT          \
                              ? (CYG_ADDRESS)__default_interrupt_vsr        \
                              : _vector_ == CYGNUM_HAL_VECTOR_BREAKPOINT    \
                                ? (CYG_ADDRESS)__break_vsr_springboard      \
                                : (CYG_ADDRESS)__default_exception_vsr,     \
                 _poldvsr_ );                                               \
CYG_MACRO_END

//--------------------------------------------------------------------------
// Interrupt controller access
// The default code here simply uses the fields present in the CP0 status
// and cause registers to implement this functionality.
// Beware of nops in this code. They fill delay slots and avoid CP0 hazards
// that might otherwise cause following code to run in the wrong state or
// cause a resource conflict.

#ifndef CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED

/*
#define HAL_INTERRUPT_MASK( _vector_ ) \
        asm volatile (                     \
        "CLI R1;" \
        "R0   = R0 ^ R0;"                  \
        "P0.H = _hal_intr_mask_applied;"   \
        "P0.L = _hal_intr_mask_applied;"   \
        "R2   = [P0];"                  \
        "R0   = 0x1;"                  \
        "R0   <<= %0;"                \
        "R0   = ~R0;"                       \
        "R2   = R2 & R0;"                   \
        "R1   = R1 & R0;"                   \
        "[P0]   = R2;"                  \
        "STI R1;"                        \
        "SSYNC;"                                \
        :                                  \
        : "r"(_vector_)                   \
        : "R0", "R1", "R2", "P0"                 \
        ) 
*/
#define HAL_INTERRUPT_MASK( _vector_ ) \
        asm volatile (                     \
        "CLI R1;" \
        "R0   = R0 ^ R0;"                  \
        "P0.H = _hal_intr_mask_applied;"   \
        "P0.L = _hal_intr_mask_applied;"   \
        "R2   = [P0];"                  \
        "R0   = 0x1;"                  \
        "R0   <<= %0;"                \
        "R0   = ~R0;"                       \
        "R2   = R2 & R0;"                   \
        "R0.H = 0x0;"                   \
        "R0.L = 0x8000;"                   \
        "R1   = R1 & R0;"               \
        "CC   = R1;"                    \
        "IF CC R1 = R2;"                \
        "[P0]   = R2;"                  \
        "STI R1;"                        \
        "SSYNC;"                                \
        :                                  \
        : "r"(_vector_)                   \
        : "R0", "R1", "R2", "P0"                 \
        ) 


#define HAL_INTERRUPT_UNMASK( _vector_ ) \
        asm volatile (                     \
        "CLI R1;" \
        "R0   = R0 ^ R0;"                  \
        "P0.H = _hal_intr_mask_applied;"   \
        "P0.L = _hal_intr_mask_applied;"   \
        "R2   = [P0];"                  \
        "R0   = 0x1;"                  \
        "R0   <<= %0;"                \
        "R2   = R2 | R0;"                   \
        "R0.H = 0x0;"                   \
        "R0.L = 0x8000;"                   \
        "R1   = R1 & R0;"               \
        "CC   = R1;"                    \
        "IF CC R1 = R2;"                \
        "[P0]   = R2;"                  \
        "STI R1;"                        \
        "SSYNC;"                                \
        :                                  \
        : "r"(_vector_)                   \
        : "R0", "R1", "R2", "P0"                 \
        )

#define HAL_INTERRUPT_ACKNOWLEDGE( _vector_ )

#define HAL_INTERRUPT_CONFIGURE( _vector_, _level_, _up_ )

#define HAL_INTERRUPT_SET_LEVEL( _vector_, _level_ )

//#define CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED

#endif

//--------------------------------------------------------------------------
// Clock control.


#ifndef CYGHWR_HAL_CLOCK_CONTROL_DEFINED

externC CYG_WORD32 cyg_hal_clock_period;
#define CYGHWR_HAL_CLOCK_PERIOD_DEFINED

// 0x63 will yield 99 which will be used as TSCALE value
// this will yield one COUNT of the core clock every
// 100 CORE CYCLES
// or dividing the core frequency by 100
// for 400 MHz the Core Timer will count with 400 kHz
// in order to reach a interrupt rate of 100 per second
// the TCOUNt register must be filled appropriately
// for 400 MHz this value would be 4000
// this will lead
// ticks per second = 400e6 / (4000 *100)
//                  = 400e6 / 4e6
//                  = 100
// the value of 1000 in the prescaler is fixed
// only the value for the counter can be changed
// this will mean the counter must be
// 100 * speed in MHz
// the reason for that is the accuracy of the clock

#define HAL_CLOCK_INITIALIZE( _period_ )        \
CYG_MACRO_START                                 \
    asm volatile (                              \
        "P0.H = 0xffe0\n"                       \
        "P0.L = 0x3000\n"                       \
        "R0   = R0 ^ R0\n"                      \
        "R0   = 0x1\n"                          \
        "[P0] = R0\n"                           \
        "SSYNC;\n"                              \
        "P0.L = 0x3008\n"                       \
        "R0   = R0 ^ R0\n"                      \
        "R0.L = 0x63\n"                         \
        "[P0] = R0\n"                           \
        "P0.L = 0x3004\n"                       \
        "[P0] = %0\n"                           \
        "P0.L = 0x3000\n"                       \
        "R0   = 0x7\n"                          \
        "[P0] = R0\n"                           \
        "SSYNC;\n"                              \
        :                                       \
        : "r"(_period_)                         \
        : "P0","R0"                             \
        );                                      \
    cyg_hal_clock_period = _period_;            \
CYG_MACRO_END

#define HAL_CLOCK_RESET( _vector_, _period_ )   \
CYG_MACRO_START                                 \
    asm volatile (                              \
        "P0.H = 0xffe0\n"                       \
        "P0.L = 0x3000\n"                       \
        "R0   = R0 ^ R0\n"                      \
        "R0   = 0x1\n"                          \
        "[P0] = R0\n"                           \
        "SSYNC;\n"                              \
        "P0.L = 0x3008\n"                       \
        "R0   = R0 ^ R0\n"                      \
        "R0.L = 0x63\n"                         \
        "[P0] = R0\n"                           \
        "P0.L = 0x3004\n"                       \
        "[P0] = %0\n"                           \
        "P0.L = 0x3000\n"                       \
        "R0   = 0x7\n"                          \
        "[P0] = R0\n"                           \
        "SSYNC;\n"                              \
        :                                       \
        : "r"(_period_)                         \
        : "P0","R0"                             \
        );                                      \
CYG_MACRO_END

#define HAL_CLOCK_READ( _pvalue_ )              \
CYG_MACRO_START                                 \
    register CYG_WORD32 result;                 \
    register CYG_WORD32 period;                 \
    asm volatile (                              \
        "P0.H = 0xffe0\n"                       \
        "P0.L = 0x300C\n"                       \
        "%0 = [P0]\n"                           \
        "P0.L = 0x3004\n"                       \
        "%1 = [P0]\n"                           \
        : "=r"(result),"=r"(period)             \
        :                                       \
        : "P0"                                  \
        );                                      \
    *(_pvalue_) = period - result;              \
CYG_MACRO_END

#define CYGHWR_HAL_CLOCK_CONTROL_DEFINED

#endif

#if defined(CYGVAR_KERNEL_COUNTERS_CLOCK_LATENCY) && \
    !defined(HAL_CLOCK_LATENCY)
#define HAL_CLOCK_LATENCY( _pvalue_ )                   \
CYG_MACRO_START                                         \
    register CYG_WORD32 _cval_;                         \
    HAL_CLOCK_READ(&_cval_);                            \
    *(_pvalue_) = _cval_ - cyg_hal_clock_period;        \
CYG_MACRO_END
#endif


//--------------------------------------------------------------------------
// Microsecond delay function provided in hal_misc.c
externC void hal_delay_us(int us);

#define HAL_DELAY_US(n)          hal_delay_us(n)

#define GET_IPEND(__ipend__) \
    __asm __volatile( "P0.H    = 0xFFE0 \n " \
                      "P0.L    = 0x2108 \n " \
                      "R0      = [P0] \n " \
                      "%0      = R0 \n " : "=r"(__ipend__) : : "P0", "R0" )
    
//--------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_HAL_INTR_H
// End of hal_intr.h
