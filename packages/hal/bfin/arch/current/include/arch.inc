#ifndef CYGONCE_HAL_ARCH_INC
#define CYGONCE_HAL_ARCH_INC
##=============================================================================
##
##	arch.inc
##
##	BFIN assembler header file
##
##=============================================================================
#### // # ECOSGPLCOPYRIGHTBEGIN####
## -------------------------------------------
## This file is part of eCos, the Embedded Configurable Operating System.
## Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
##
## eCos is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 or (at your option) any later version.
##
## eCos is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with eCos; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
##
## As a special exception, if other files instantiate templates or use macros
## or inline functions from this file, or you compile this file and link it
## with other works to produce a work based on this file, this file does not
## by itself cause the resulting work to be covered by the GNU General Public
## License. However the source code for this file must still be made available
## in accordance with section (3) of the GNU General Public License.
##
## This exception does not invalidate any other reasons why a work based on
## this file might be covered by the GNU General Public License.
##
## Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
## at http://sources.redhat.com/ecos/ecos-license/
## -------------------------------------------
####ECOSGPLCOPYRIGHTEND####
##=============================================================================
######DESCRIPTIONBEGIN####
##
## Author(s): 	andre, Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
## Date:	    2006-05-24
## Purpose:	    Architecture definitions.
## Description:	This file contains various definitions and macros that are
##              useful for writing assembly code for the BFIN CPU family.
## Usage:
##		 #include <cyg/hal/arch.inc>
##		...
##		
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <cyg/hal/bfin.inc>
#include <cyg/hal/bfin-regs.h>
#include <cyg/hal/variant.inc>
#include <cyg/hal/var_intr_incl.h>



// this macro has the following conditions
//
// precondition : valid SP with enough space to store the context
//
// postcondition :
//                  context on stack
//                  SP is top of stack
//                  FP points to beginning of context structure
//                  R2, P1 changed
//                  R0, R1, R3:7 and P0, P1:5 unchanged
.macro savecpustate
    [--SP]  = FP             // #  save old frame pointer

    [--SP]  = USP            // #  save USP
    [--SP]  = ASTAT
    FP     = RETX
    [--SP]  = FP            // #  save RETX register as PC
    FP     = RETE
    [--SP]  = FP            // #  save RETE register on stack

    FP     = RETN
    [--SP]  = FP            // #  save RETN register on stack
    FP     = RETX
    [--SP]  = FP            // #  save RETX register on stack
    FP     = RETI
    [--SP]  = FP            // #  save RETI register on stack
                             // #  important not to perform [ --SP ] = RETI
                             // #  as this would disable interrupts
    FP     = RETS
    [--SP]  = FP            // #  save RETS register on stack

    [--SP]  = LC1            // #  store loop counter register on the stack
    [--SP]  = LC0

    [--SP]  = LB1            // #  store loop bottom registers on the stack
    [--SP]  = LB0

    [--SP]  = LT1            // #  store loop top registers on the stack
    [--SP]  = LT0

    [--SP]  = M3
    [--SP]  = M2
    [--SP]  = M1
    [--SP]  = M0

    [--SP]  = B3
    [--SP]  = B2
    [--SP]  = B1
    [--SP]  = B0

    [--SP]  = L3
    [--SP]  = L2
    [--SP]  = L1
    [--SP]  = L0

    [--SP]  = I3
    [--SP]  = I2
    [--SP]  = I1
    [--SP]  = I0

    [--SP]  = CYCLES2
    [--SP]  = CYCLES

    [--SP]  = SYSCFG
    
    [--SP]  = (R7:0,P5:0)    // #  multiple push will push in the order
                             // #  first R0 -> R7 and then P0 -> P5

    P1.H    = 0xFFE0
    P1.L    = 0x2104         // #  IMASK
    R2      = [P1]
    [--SP]  = R2
    P1.L    = 0x2108         // #  IPEND
    R2      = [P1]
    [--SP]  = R2
    [--SP]  = SEQSTAT
    
    R2      = A1.X // (S)
    [--SP]  = R2

    R2      = A0.X // (S)
    [--SP]  = R2

    R2      = A1.W 
    [--SP]  = R2

    R2      = A0.W 
    [--SP]  = R2

    P3.H    = 0xFFE0        //# d cpld fault register
    P3.L    = 0x000C
    R2      = [P3]
    [--SP]  = R2

    P3.H    = 0xFFE0        //# i cpld fault register
    P3.L    = 0x100C
    R2      = [P3]
    [--SP]  = R2

    FP      = SP
.endm

.macro restorecpustate
    SP      += 0x8      // # skip icpld fault and dcpld fault

    R0      = [SP++]
    A0.W    = R0
    
    R0      = [SP++]
    A1.W    = R0
    
    R0      = [SP++]
    A0.X    = R0
    
    R0      = [SP++]
    A1.X    = R0
    
    
    SP      += 0x8       ;# skip SEQSTAT and IPEND as they are read only registers

    R2      = [SP++]
    P0.H    = 0xFFE0
    P0.L    = 0x2104       ;# IMASK
//    [P0]    = R2

    (R7:0,P5:0) = [SP++]   // #  multiple pop restoring general registers

    SYSCFG  = [SP++]
    CYCLES  = [SP++]
    CYCLES2 = [SP++]

    I0      = [SP++]
    I1      = [SP++]
    I2      = [SP++]
    I3      = [SP++]

    L0      = [SP++]
    L1      = [SP++]
    L2      = [SP++]
    L3      = [SP++]

    B0      = [SP++]
    B1      = [SP++]
    B2      = [SP++]
    B3      = [SP++]

    M0      = [SP++]
    M1      = [SP++]
    M2      = [SP++]
    M3      = [SP++]

    LT0     = [SP++]     // #  might incurr penalty if LC0!=0
    LT1     = [SP++]     // #  might incurr penalty if LC1!=0

    LB0     = [SP++]     // #  might incurr penalty if LC0!=0
    LB1     = [SP++]     // #  might incurr penalty if LC1!=0

    LC0     = [SP++]
    LC1     = [SP++]

    FP     = [SP++]
    RETS    = FP

    FP     = [SP++]
    RETI    = FP

    FP     = [SP++]
    RETX    = FP

    FP     = [SP++]
    RETN    = FP

    FP     = [SP++]
    RETE    = FP

    SP     += 0x4

    ASTAT   = [SP++]
    USP      = [SP++]

    FP       = [SP++]
.endm

.macro hal_switch_to_interrupt_stack
## this macro switches to the interrupt stack
##
## INVARIABLE :
##  this routine is NOT reentrant
##
## PRECON  :
##  interrupt nesting disabled
##
## POSTCON :
##      SP : is on interrupt stack
##      [__hal_bfin_program_stack_pointer] = program stack pointer
##
## CLOBBERED REGISTERS
##      R2, R1, CC

    R2.H    = __interrupt_stack_base;     // load the interrupt base  HI
    R2.L    = __interrupt_stack_base;     // load the interrupt base, LO
    R1      = SP;                   // load the current stack pointer

    CC      = R2 <= R1;             // test if the stack pointer is above or 
                                    // equal to the stack base
    IF !CC  JUMP 1f;                // jump forward to switch to interrupt stack
                                    // if not

    R2.H    = __interrupt_stack;    // load the interrupt stack  HI
    R2.L    = __interrupt_stack;    // load the interrupt stack, LO
    
    CC      = R1<=R2;               // check whether current stack pointer is
                                    // smaller or equal the stack
                                    // true if we are on the interrrupt stack
    IF CC   JUMP 2f;                // jump to end
    
    1:
        SP  = R2;                   // load the interrupt stack as new stack
                                    // pointer

    2:
        [--SP] = R1;                // save the old stack pointer
.endm

.macro hal_switch_from_interrupt_stack
## this macro switches to the interrupt stack
##
## PRECON  :
##    each interrupt and exception calling routing has to clean up all the
##    stack space used prior to invoking this macro
##
##    interrupt nesting disabled
##
## POSTCON :
##    if last interrupt handling routine calling
##      SP : program stack pointer
##      [__hal_bfin_program_stack_pointer] = __interrupt_stack
##    else
##      SP : is on interrupt stack
##      [__hal_bfin_program_stack_pointer] = program stack pointer
##
## CLOBBERED REGISTERS
##      R1

        R1 = [SP++];
        SP = R1;
.endm

#ifndef CYGPKG_HAL_BFIN_CPU_INIT_DEFINED
     // #  Initialize CPU
    .macro  hal_cpu_init
        R0 = R0 ^ R0
        LC0 = R0
        LC1 = R0
        LT0 = R0
        LT1 = R0
        LB0 = R0
        LB1 = R0
        
        M0  = R0
        M1  = R0
        M2  = R0
        M3  = R0
        I0  = R0
        I1  = R0
        I2  = R0
        I3  = R0
        L0  = R0
        L1  = R0
        L2  = R0
        L3  = R0
        B0  = R0
        B1  = R0
        B2  = R0
        B3  = R0

        R1  = R0
        R2  = R0
        R3  = R0
        R4  = R0
        R5  = R0
        R6  = R0
        R7  = R0
        P0  = R0
        P1  = R0
        P2  = R0
        P3  = R0
        P4  = R0
        P5  = R0

        RETS  = R0
        RETE  = R0
        RETN  = R0
        RETX  = R0
        RETI  = R0
    .endm
#endif /* !CYGPKG_HAL_BFIN_CPU_INIT_DEFINED */

 // # ------------------------------------------------------------------------------
 // #  Diagnostics macros.
    
#ifndef CYGPKG_HAL_BFIN_DIAG_DEFINED

    .macro  hal_diag_init
    .endm

    .macro  hal_diag_excpt_start
    .endm

    .macro  hal_diag_intr_start

    .endm

    .macro  hal_diag_restore
    .endm
#endif  


 // # ------------------------------------------------------------------------------
 // #  Timer initialization.
    
#ifndef CYGPKG_HAL_BFIN_TIMER_DEFINED

    .macro  hal_timer_init
    .endm

#endif

 // # ------------------------------------------------------------------------------
 // #  MMU initialization.
    
#ifndef CYGPKG_HAL_BFIN_MMU_DEFINED

    .macro  hal_mmu_init
    .endm

#endif 

#ifndef CYGPKG_HAL_BFIN_CACHE_DEFINED

    .macro  hal_cache_init
        P0.L=0x1004
        P0.H=0xffe0
        R0  = R0 ^ R0
        R0.L = 0x1
        CLI  R2;
        SSYNC;
        .align 8
        [P0]=R0;
        SSYNC;
        STI  R2;
        P0.L=0x0004
        P0.H=0xffe0
        R0  = R0 ^ R0
        R0.L = 0x1
        CLI  R2;
        SSYNC;
        .align 8
        [P0]=R0;
        SSYNC;
        STI  R2;

    .endm

#endif 

 // # ------------------------------------------------------------------------------
 // #  Monitor initialization.
    
#ifndef CYGPKG_HAL_BFIN_MON_DEFINED

    .macro  hal_mon_init
    .endm

#endif

#ifndef CYGPKG_HAL_BFIN_ROMRAMCPY
    .macro hal_copy_data
        
    .endm
#endif

#ifndef CYGPKG_HAL_BFIN_MEMC_DEFINED

    .macro  hal_memc_init
      
    .endm

#endif


    

     // #  initialize all interrupts to disabled
.macro  hal_ser_init
        
        P0.H = 0xFFC0
        P0.L = 0x0424
        R0.L = 0x0
        W[P0] = R0      // disable UART clock
        P0.L = 0x040C
        R0.L = 0x0083
        W[P0] = R0
        R0.L = 0x0057
        P0.L = 0x0400
        W[P0] = R0
        P0.L = 0x0404
        R0.L = 0x0
        W[P0] = R0      // initialize DLL and DLH
        SSYNC
        P0.L = 0x040C   // line control register LCR
        R0.L = 0x0003   // set 8bit, 1 stop bit, no parity
        W[P0] = R0      // switch to THR and RBR
        SSYNC
        P0.L = 0x0410   // MCR
        R0.L = 0x0      // disable loop back
        W[P0] = R0
        P0.L = 0x0404   // IER
        R0.L = 0x0      // disabling interrupts
        W[P0] = R0
        P0.L = 0x0424
        R0.L = 0x1
        W[P0] = R0      // enable UART clock

        SSYNC

        P0.L  = 0x0D88  // DMA6_CONFIG
        R0    = 0x0     // disable DMA
     //   W[P0]  = R0
        
        SSYNC

        P0.L  = 0x0DC8  // DMA7_CONFIG
        R0    = 0x0     // disable DMA
    //    W[P0]  = R0
        

        SSYNC

    .endm

#ifndef CYGPKG_HAL_BFIN_INIT_SDRAMINIT
.macro  hal_sdram_selfrefresh
        P0.H = 0xFFC0;
        P0.L = 0x0A10;
        R0 = [P0];
        BITSET (R0, 24);
        [P0] = R0;
        SSYNC;
    .endm
#endif





#ifndef CYGPKG_HAL_BFIN_INIT_SOFTRESET
.macro  hal_clear_soft_reset
    P0.H = 0xFFC0;                      // # Software Reset Register (SWRST)
    P0.L = 0x0100;
    R0.L = W[P0];

    CC = BITTST(R0, 13);
    IF !CC JUMP no_soft_reset;

    R0 = 0x0000;
    W[P0] = R0;
    SSYNC;
    no_soft_reset:
    .endm
#endif

.macro  hal_put_char idx
        P0.H = 0xFFC0
        P0.L = 0x0400
        R0.L  = \idx
        W[P0] = R0.L
        SSYNC;
        1:
        P0.L = 0x0414
        R0.L = W[P0]
        CC = BITTST(R0, 5)
        IF !CC JUMP 1b
    .endm

#ifndef CYGPKG_HAL_BFIN_INTC_INIT_DEFINED

.macro  hal_bfin_intc_init_intr idx
        P0.H = 0xFFE0
        P0.L = 0x2000+(4*\idx)
        R0.H = hal_bfin_interrupt_trampoline_\idx
        R0.L = hal_bfin_interrupt_trampoline_\idx
        [P0]    = R0
        SSYNC
.endm

     // #  initialize all interrupts to disabled (fill Event Vector Table)
.macro  hal_intc_init
        
        hal_bfin_intc_init_intr 0
        hal_bfin_intc_init_intr 1
        hal_bfin_intc_init_intr 2

        P0.H = 0xFFE0      // #  load the interrupt controller (CEC)
        P0.L = 0x200C           // #  exception
        R0.H = hal_bfin_exception
        R0.L = hal_bfin_exception
        [P0]    = R0
        SSYNC

        
        hal_bfin_intc_init_intr 5
        hal_bfin_intc_init_intr 6
        hal_bfin_intc_init_intr 7
        hal_bfin_intc_init_intr 8
        hal_bfin_intc_init_intr 9
        hal_bfin_intc_init_intr 10
        hal_bfin_intc_init_intr 11
        hal_bfin_intc_init_intr 12
        hal_bfin_intc_init_intr 13
   //     hal_bfin_intc_init_intr 14


        .extern _hal_arch_default_hardware_error_isr
        P0.H = 0xFFE0
        P0.L = 0x2000+(4*5)
        R0.H = _hal_arch_default_hardware_error_isr
        R0.L = _hal_arch_default_hardware_error_isr
        [P0]    = R0
        SSYNC
// # set interrupt 14
        .extern hal_bfin_context_load_int14
        P0.H = 0xFFE0
        P0.L = 0x2000+(4*14)
        R0.H = hal_bfin_context_load_int14
        R0.L = hal_bfin_context_load_int14
        [P0]    = R0
        SSYNC        
        
        P0.H = 0xFFE0      // #  load the interrupt controller
        P0.L = 0x203C
        R0.H = hal_bfin_dummy_start
        R0.L = hal_bfin_dummy_start
        
        [P0]    = R0
        SSYNC
    .endm
	
.macro  hal_clock_init_setup
#ifdef CYGPKG_REDBOOT
#ifndef CYGBLD_BUILD_REDBOOT_WITH_IOMEM
        P0.H = 0xffe0
        P0.L = 0x3000
        R0   = R0 ^ R0
        R0   = 0x1
        [P0] = R0
        SSYNC
        P0.L = 0x3008
        R0   = R0 ^ R0
        R0.L = 0x63
        [P0] = R0
        P0.L = 0x3004
        R0.H = 0x0000
        R0.L = CYGNUM_HAL_RTC_PERIOD
        [P0] = R0
        P0.L = 0x3000
        R0   = 0x7
        [P0] = R0
        SSYNC
#endif // # CYGBLD_BUILD_REDBOOT_WITH_IOMEM
#endif // # CYGPKG_REDBOOT
.endm

.macro  hal_excpt_init
.extern hal_bfin_dcpld_handle
# register ICPLD miss handler
        R0     = 0x2C            // # get the exception cause
        R1     = CYGNUM_HAL_BFIN_EXCEPTIONS_BASE
        R0     = R0 + R1
        R1.H   = hal_vsr_table
        R1.L   = hal_vsr_table  // # load the address of the VSR table into P0
        R0    <<= 2
        R0     = R1 + R0            // # add the offset into the table (R0) to the table start (R1)
        P0     = R0                 // # move the address into P0 for loading
        R0.L   = __hal_icpld_miss_exception_vsr
        R0.H   = __hal_icpld_miss_exception_vsr
        [P0]   = R0                 // # move the address into the pointer register
        SSYNC
# register ICPLD miss handler
        R0     = 0x2D            // # get the exception cause
        R1     = CYGNUM_HAL_BFIN_EXCEPTIONS_BASE
        R0     = R0 + R1
        R1.H   = hal_vsr_table
        R1.L   = hal_vsr_table  // # load the address of the VSR table into P0
        R0    <<= 2
        R0     = R1 + R0            // # add the offset into the table (R0) to the table start (R1)
        P0     = R0                 // # move the address into P0 for loading
        R0.L   = __hal_icpld_miss_exception_vsr
        R0.H   = __hal_icpld_miss_exception_vsr
        [P0]   = R0                 // # move the address into the pointer register
        SSYNC

# register DCPLD miss handler
        R0     = 0x26            // # get the exception cause
        R1     = CYGNUM_HAL_BFIN_EXCEPTIONS_BASE
        R0     = R0 + R1
        R1.H   = hal_vsr_table
        R1.L   = hal_vsr_table  // # load the address of the VSR table into P0
        R0    <<= 2
        R0     = R1 + R0            // # add the offset into the table (R0) to the table start (R1)
        P0     = R0                 // # move the address into P0 for loading
        R0.L   = __hal_dcpld_miss_exception_vsr
        R0.H   = __hal_dcpld_miss_exception_vsr
        [P0]   = R0                 // # move the address into the pointer register
        SSYNC
# register DCPLD miss handler
        R0     = 0x23            // # get the exception cause
        R1     = CYGNUM_HAL_BFIN_EXCEPTIONS_BASE
        R0     = R0 + R1
        R1.H   = hal_vsr_table
        R1.L   = hal_vsr_table  // # load the address of the VSR table into P0
        R0    <<= 2
        R0     = R1 + R0            // # add the offset into the table (R0) to the table start (R1)
        P0     = R0                 // # move the address into P0 for loading
        R0.L   = __hal_dcpld_miss_exception_vsr
        R0.H   = __hal_dcpld_miss_exception_vsr
        [P0]   = R0                 // # move the address into the pointer register
        SSYNC
# register DCPLD miss handler
        R0     = 0x27            // # get the exception cause
        R1     = CYGNUM_HAL_BFIN_EXCEPTIONS_BASE
        R0     = R0 + R1
        R1.H   = hal_vsr_table
        R1.L   = hal_vsr_table  // # load the address of the VSR table into P0
        R0    <<= 2
        R0     = R1 + R0            // # add the offset into the table (R0) to the table start (R1)
        P0     = R0                 // # move the address into P0 for loading
        R0.L   = __hal_dcpld_miss_exception_vsr
        R0.H   = __hal_dcpld_miss_exception_vsr
        [P0]   = R0                 // # move the address into the pointer register
        SSYNC

#ifdef HAL_BFIN_IMPL_CAUSE
        R0     = R0 ^ R0            // # get the exception cause
        R1     = CYGNUM_HAL_BFIN_EXCEPTIONS_BASE
        R0     = R0 + R1
        R1.H   = hal_vsr_table
        R1.L   = hal_vsr_table  // # load the address of the VSR table into P0
        R0    <<= 2
        R0     = R1 + R0            // # add the offset into the table (R0) to the table start (R1)
        P0     = R0                 // # move the address into P0 for loading
        R0.L   = __default_exception_vsr_return
        R0.H   = __default_exception_vsr_return
        [P0]   = R0                 // # move the address into the pointer register
        SSYNC
#endif
    .endm


#endif

.macro check_for_correct_cpu
    P0.H = 0xFFC0                    // # FIXME CPU_ID register
    P0.L = 0x0014
    R0   = [P0]
    R1.H = (HAL_BFIN_CPU_ID_MASK >>16)
    R1.L = (HAL_BFIN_CPU_ID_MASK & 0xFFFF)
    R1   = R1 & R0
    R2.H = (HAL_BFIN_CPU_ID >>16)
    R2.L = (HAL_BFIN_CPU_ID & 0xFFFF)
    CC   = R1 == R2
    IF !CC JUMP check_for_correct_cpu_abort
    JUMP.L check_for_correct_cpu_continue
    check_for_correct_cpu_abort:
        hal_put_char 'e'
        hal_put_char 'C'
        hal_put_char 'o'
        hal_put_char 's'
        hal_put_char '-'
        hal_put_char 'H'
        hal_put_char 'A'
        hal_put_char 'L'
        hal_put_char '-'
        hal_put_char 'B'
        hal_put_char 'F'
        hal_put_char 'i'
        hal_put_char 'n'
        hal_put_char '\n'
        hal_put_char '\r'
        hal_put_char 'E'
        hal_put_char 'R'
        hal_put_char 'R'
        hal_put_char 'O'
        hal_put_char 'R'
        hal_put_char ':'
        hal_put_char ' '
        hal_put_char 'w'
        hal_put_char 'r'
        hal_put_char 'o'
        hal_put_char 'n'
        hal_put_char 'g'
        hal_put_char ' '
        hal_put_char 'c'
        hal_put_char 'p'
        hal_put_char 'u'
        hal_put_char '\n'
        hal_put_char '\r'
        hal_put_char 's'
        hal_put_char 'y'
        hal_put_char 's'
        hal_put_char 't'
        hal_put_char 'e'
        hal_put_char 'm'
        hal_put_char ' '
        hal_put_char 'h'
        hal_put_char 'a'
        hal_put_char 'l'
        hal_put_char 't'
        hal_put_char 'e'
        hal_put_char 'd'
        hal_put_char ' '
        hal_put_char 't'
        hal_put_char 'o'
        hal_put_char ' '
        hal_put_char 'p'
        hal_put_char 'r'
        hal_put_char 'e'
        hal_put_char 'v'
        hal_put_char 'e'
        hal_put_char 'n'
        hal_put_char 't'
        hal_put_char ' '
        hal_put_char 'd'
        hal_put_char 'a'
        hal_put_char 'm'
        hal_put_char 'a'
        hal_put_char 'g'
        hal_put_char 'e'
        hal_put_char '\n'
        hal_put_char '\r'
        hal_put_char ' '

    check_for_correct_cpu_loop: IDLE
       JUMP check_for_correct_cpu_loop
        
    check_for_correct_cpu_continue: NOP
.endm

 // # ------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_ARCH_INC
 // #  end of arch.inc
