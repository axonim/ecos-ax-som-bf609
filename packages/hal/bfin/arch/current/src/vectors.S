##=============================================================================
##
##	vectors.S
##
##	BFIN exception vectors
##
##=============================================================================
#####ECOSGPLCOPYRIGHTBEGIN####
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
#####ECOSGPLCOPYRIGHTEND####
##=============================================================================
#######DESCRIPTIONBEGIN####
##
## Author(s): 	andre, Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
## Date:	    2006-09-16
## Purpose:	    BFIN exception vectors
## Description:	This file defines the code placed into the exception
##              vectors. It also contains the first level default VSRs
##		        that save and restore state for both exceptions and
##		        interrupts.
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <pkgconf/system.h>	
#include <pkgconf/hal.h>

#ifdef CYGPKG_KERNEL
#include <pkgconf/kernel.h>	
#endif

#define HAL_BFIN_IMPL_CAUSE
			
#include <cyg/hal/arch.inc>	
#include <cyg/hal/hal_arch.h>	
#include <cyg/hal/bfin-regs.h>

#define uboot
	
#ifdef at
#undef at
#endif
	.extern cyg_instrument
    .extern diag_write_hex


##entry point

    .section ".reset_vector","ax"

FUNC_START(reset_vector)
    P0.H = _start
    P0.L = _start
    JUMP (P0)
//    JUMP.L _start
## jump to startup code

##FUNC_END(reset_vector)
	
##-----------------------------------------------------------------------------
## Startup code

#ifndef CYG_HAL_STARTUP_RAM
    .section ".reset_vector","ax"
#else 
	.text
#endif
	
FUNC_START(_start)
## masking all interrupts
    CLI R0
    SSYNC

## setting up the led control
    hal_led_init

## check for the correct cpu
    check_for_correct_cpu

## ASSUMPTION: correct cpu type

## setup SYS CFG, enable CYCLE COUNTER
    R0=0x32
    SYSCFG=R0
    SSYNC

## clear cpu registers
## to avoid anomilies like hardware looping due
## to LC registers initialized to values != 0
    hal_cpu_init

## clear the software reset bit
    hal_clear_soft_reset

## setting up the core speeds
## this will require the following steps
##
## set up PLL clock, multipliers and dividers

#ifdef CYGNUM_HAL_BFIN_ENFORCE_SETTINGS
    #ifdef CYGPKG_HAL_BFIN_BF53x
        R0 = R0 ^ R0
        R0.L = PLL_MSEL_SET
        R0 <<= PLL_CTL_MSEL_SHIFT 

    #ifdef CYGNUM_HAL_BFIN_DF
        R1 = R1 ^ R1
        R1.L = 0x1
        R0 = R0 | R1
    #endif

        P0.L = 0x0
        P0.H = 0xFFC0

        CLI R1
        SSYNC
        W[P0] = R0.L
        IDLE
        STI R1

        
        wait_for_pll_lock:
        P0.H = 0xFFC0
        P0.L = 0x000C
        R0 = W[P0](Z)
        CC = BITTST(R0,5)
        IF ! CC jump wait_for_pll_lock

        
    // # the PLL has stabilised not start adjusting the
    // CCLK and SCLK

        R0.L = PLL_CSEL_SET
        R1.L = PLL_SSEL_SET
        R0   = R0 | R1
        P0.L = 0x0004
        P0.H = 0xFFC0

        SSYNC
        W[P0] = R0.L
        
        
    // now we are running on the desired speed
    // start setting up the SDRAM

    ## set up sdram    
         hal_sdram_init

    ## set up asynchronous ram
         hal_async_init

#elif defined CYGPKG_HAL_BFIN_BF60x
   .extern _hal_plf_program_clocks
   P0.H = _hal_plf_program_clocks
   P0.L = _hal_plf_program_clocks
   CALL (P0)

#endif //ifdef CYGPKG_HAL_BFIN_BF53x     

#endif

##



 // # Initialize hardware

## disable caching
    hal_cache_init

## reinitialize the cpu
    hal_cpu_init
## prepare the diagnostic output
    hal_diag_init
    
## disable cpu mmu
    hal_mmu_init
## initialize the memory controller
    hal_memc_init
## setup the interrupt EVT tables of the processor
## to point to the trampoline codes
    hal_intc_init
    
## setup the exception entry points in the
## VSR table to HAL handlers for page miss exceptions
    hal_excpt_init

#ifdef CYGPKG_HAL_BFIN_BF53x
## setup the SIC mask
    hal_bfin_init_sic_mask
## setup the SIC assingments to core interrupt levels
    hal_bfin_init_sic_assign
#endif
#ifdef CYGPKG_HAL_BFIN_BF60x
## enable SEC
    hal_bfin_init_sec
#endif

## enable the timer, this is necessary otherwise
## calls to hal_delay_us will stall infinitely
    hal_timer_init
## setup the serial interface for diagnostic output
    hal_ser_init

    hal_clock_init_setup
    
    // # setup interrupt stack to be used as stack  
    P0.H = __interrupt_stack
    P0.L = __interrupt_stack
    
    SP   = P0


    
#ifdef CYG_HAL_STARTUP_UBOOT

## copy the data from ROM to RAM
## ALL data is copied out to RAM, so that the manager
## does not rely on asynchronous RAM access
    .extern _hal_copy_data
    CALL ___hal_copy_data

#endif
    # reinitialize exceptions as VSR got overriden !!!
    hal_excpt_init
#ifdef CYGPKG_HAL_BFIN_BF53x
    hal_bfin_init_sic_mask
    hal_bfin_init_sic_assign
#endif

##	hal_mon_init

## disable all interrupts
    P0.L=0x2104
    P0.H=0xffe0
    P1.L=0x0000         // # mask all interrupts
    P1.H=0x0
    [P0]=P1
    SSYNC

    
## switch to relocated code

    
//#ifndef CYG_HAL_STARTUP_RAM
    
## in case we are the rom monitor we have to leave the
## reset state
## unmask interrupt 15
    P0.L=0x2104
    P0.H=0xffe0
    P1.L=0x8000         // # unmask int15
    P1.H=0x0
    [P0]=P1
    SSYNC
    
## setup the waiting loop
## this will be jumped to before the processor will start
## serving the interrupt 15
    //R0.H = HI(_hal_bfin_pointless_loop)
    P0.H = _hal_bfin_pointless_loop
    //R0.L = LO(_hal_bfin_pointless_loop)
    P0.L = _hal_bfin_pointless_loop
    
    RETI    = P0        // # jump to pointless loop to wait for software interrupt
                        // # being serviced

## ASSUMPTION: the entry point is already set up in the EVT by the hal_intc_init
##             macro
    RAISE 15            // # raise interrupt number 15, raised here to avoid penalty



    RTI                 // # finish -> this will lead to hal_bfin_dummy_start

    P0.H = hal_bfin_dummy_start
    P0.L = hal_bfin_dummy_start
    

    JUMP (P0)
//#else

//## jump to the entry point of the HAL for further startup
//    P0.H = hal_bfin_dummy_start
//    P0.L = hal_bfin_dummy_start

//    JUMP (P0)

//#endif

    _hal_bfin_pointless_loop:
            hal_led_support 5
            JUMP _hal_bfin_pointless_loop

    .text

hal_bfin_dummy_start:

## disable all interrupts for further processing
    CLI R0
    SSYNC
    R0.H = 0x0
    R0.L = 0x8000
    STI R0
    SSYNC

 // # Zero BSS
    .extern hal_zero_bss
    P0.H = hal_zero_bss;
    P0.L = hal_zero_bss;
    CALL (P0);
//	CALL hal_zero_bss
 

    
 // # Call variant and platform HAL
 // # initialization routines.
	.extern	_hal_variant_init
    P0.H = _hal_variant_init;
    P0.L = _hal_variant_init;
    CALL (P0);

    hal_led_stack_support 1

## call platform initialization
	.extern	_hal_platform_init
    P0.H =  _hal_platform_init
    P0.L =  _hal_platform_init
	CALL	(P0)
    
 
## DEPENDENCIES: CACHING might be enabled from here due to hal_platform_init
    hal_led_stack_support 2


// # Call constructors
	.extern _cyg_hal_invoke_constructors
    CALL    _cyg_hal_invoke_constructors

        
    hal_led_stack_support 4
#if defined(CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS)
        .extern  _initialize_stub
     //   CALL     _initialize_stub
#endif

 // # Call cyg_start	
	
        
## allow interrupts by saving the RETI register
## this will clear bit 4 in the core interrupt IPEND register
        [--SP] = RETI
        P0     = [SP++]                 // # allow interrupts and clear stack

## set the booted flag to true
## some HAL related functions might produce output from now that the
## system has been setup for these purposes
## this might result in outputs for page replacements
        P0.L = _booted
        P0.H = _booted
        R0 = [P0]
        R0 += 1
        [P0] = R0

## call into the higher levels via cyg_start, this might invoke the
## user program or the kernel or other layers, we don't care
## this call should never return
        hal_led_stack_support 0
        .extern _cyg_start
        CALL _cyg_start                  // # call the kernel

## in case the call should ever return prevent the system from going rogue
## and loop infinitely
        hal_led_support 6
        hal_bfin_dummy_start_infinite_loop:
            JUMP hal_bfin_dummy_start_infinite_loop;


##FUNC_END(_start)


    .section ".interrupt_vectors","ax"


FUNC_START(_first_entry_to_thread)
    // this function must be called to ensure that we return to int 15 properly
    // in case the thread is first switched to as result of a interrupt
    // otherwise we STAY at that higher interrupt level and the thread would
    // block lower interrupts and get too much priviledges
    // do not touch R0
    P0.H = 0xffe0
    P0.L = 0x2108

    R1.H = 0xffff
    R2   = [P0]
    R1.L = 0x8010
    R1   = ~R1
    R2   = R2 & R1
    // get rid of all different possible set bits that we don't care about
    R1.L = 0x0

    CC   = R1 == R2
    IF CC JUMP first_entry_to_thread_no_reti
    RTI
    NOP
    NOP
    first_entry_to_thread_no_reti:
    P0 = RETI
    RETS = P0
    CSYNC
    RTS
    NOP
    NOP
##-----------------------------------------------------------------------------
		
##    
    // # put this section into the L1 instruction SRAM


##-----------------------------------------------------------------------------
## exception trampoline to enter VSR.
## Saves minimum machine state and calls external handling code.
##
## this routine leaves P0 on the stack, to be removed by the exception handler
##
## PRE-CONDITIONS :
##          the system state since the exception has entered the pipeline is
##          unchanged
##
##
## FUNCTION: this trampoline will determine the exception type and divert to the
##           approriate handler via a register based JUMP
##
## POST-CONDITIONS:
##
##      registers
##          P0 address of the VSR from table
##          USP is saved P0

.globl hal_bfin_exception;
hal_bfin_exception:
    SSYNC
## syncronize the system state


    USP    = P0
    P0.L   = __hal_exception_temporary_frame
    P0.H   = __hal_exception_temporary_frame
    [P0 + 0x8 ] = R0                 //# save R0, R1 and P0 because they have to be used
    R0          = ASTAT
    [P0 + 0x4 ] = R0                 //# save the flags on the stack
    [P0 + 0xC ] = R1                 
    [P0 + 0x10] = P1                 //# by the trampoline, what if user stack is not accessible?
##
##  EXC-STACK+0x00  entry point of exceptio handler 
##           +0x04  ASTAT
##           +0x08  R0
##           +0x0C  R1
##           +0x10  P1
##
##  P0 saved in USP
##

## get SEQSTAT
    R0     = SEQSTAT            //# get the seqstat register to work with
    R1     = R1^R1              // # clear the R1 register
## apply the mask 0x003f to SEQSTAT to get the lowest 6 bits
## as these are specifying exceptions
##  R0, R1 altered

    R1.L   = 0x003F             // # load the mask for masking away all irrelevant bits
    R0     = R0 & R1            // # get the exception cause


## get the hal_vsr_table base address in R1

## add the CYGNUM_HAL_BFIN_EXCEPTIONS_BASE to the exception cause
## to yield the handling VSR
    R1     = CYGNUM_HAL_BFIN_EXCEPTIONS_BASE
    R0     = R0 + R1
    R1.H   = hal_vsr_table
    R1.L   = hal_vsr_table  // # load the address of the VSR table into P0

## multiply VSR number by 4 as each VSR is 4 bytes in the table

    R0     <<= 2                // # multiply the number of the exception with 4, due to
                                // # 4 byte address entries in the VSR table
    R0     = R1 + R0            // # add the offset into the table (R0) to the table start (R1)

## R0 now contains the VSR table address

    P1     = R0                 // # move the address into P0 for loading

## load the address of the VSR from the table
##  P1 altered

    R0     = [P1]               // # load the destination address of the exception vector into R0

## R0 now contains the required address

    [P0]   = R0

## save address on the EXC-STACK at position +0x00

## restore the registers
##  P1, R1, R0 and ASTAT that had been altered
    P1     = [P0 + 0x10]
    R1     = [P0 + 0xC ]
    R0     = [P0 + 0x4 ]
    ASTAT  = R0
    R0     = [P0 + 0x8 ]

## override the exception stack base address in P0
## with entry point address to VSR
    P0     = [P0]
    
## P0 contains entry point to VSR
## old P0 saved in USP

    JUMP    (P0)                // # jump to the exception handling routine

## POSTCON: P0  = entry point to VSR
##          USP = saved P0
##
## VERIFIED FUNCTION on 21/09/2006


##    .end hal_bfin_exception

##-----------------------------------------------------------------------------
## interrupt trampoline to enter ISR.
## Saves minimum machine state and calls interrupt handling code.
##
## macro to create interrupt handler
##
## this macro approach is used to make the process of detecting the current
## interrupt called more efficient
## because the routine has to decode the IPEND register for the LSB set
##
## function, save the P0 register and store the interrupt number in P0
##           jump to the main interrupt ISR
##
## hal_bfin_interrupt_trampoline_x   and hal_bfin_interrupt_trampoline_x
##
##
## POSTCON : ASTAT
##           P0         <- SP
##           P0 contains core interrupt priority
    .macro  hal_bfin_interrupt_trampoline idx
hal_bfin_interrupt_trampoline_\idx:
    [--SP] = ASTAT                      // # save the flags on the stack
    [--SP] = P0                         // # save P0 on the stack
    P0 = \idx (Z)                       // # load the interrupt number into P0 zero extended
    JUMP hal_bfin_interrupt_common      // # jump to the common part
    .endm

    hal_bfin_interrupt_trampoline 0     // # emulation event, not important because the
                                        // # real vector is set by JTAG emulation

    hal_bfin_interrupt_trampoline 1     // # reset vector, this vector can be changed and
                                        // # called via RAISE 1
//    hal_bfin_interrupt_trampoline 2     // # NMI

    hal_bfin_interrupt_trampoline_2:
    JUMP.L __default_nmi_vsr

    ## interrupt 3 is handled via the exception trampoline as it represents exceptions
    ## interrupt 4 is reserved and not currently used by the standard BFIN architecture
    hal_bfin_interrupt_trampoline 5     // # Hardware error interrupt
    hal_bfin_interrupt_trampoline 6     // # Core timer interrupt
#ifndef CYGNUM_HAL_BFIN_INTERRUPTS_EXTENDED
    hal_bfin_interrupt_trampoline 7     // # general purpose interrupt 7
    hal_bfin_interrupt_trampoline 8     // # general purpose interrupt 8 
    
    hal_bfin_interrupt_trampoline 9     // # general purpose interrupt 9 
    hal_bfin_interrupt_trampoline 10    // # general purpose interrupt 10 
    hal_bfin_interrupt_trampoline 11    // # general purpose interrupt 11 
    hal_bfin_interrupt_trampoline 12    // # general purpose interrupt 12 
    hal_bfin_interrupt_trampoline 13    // # general purpose interrupt 13 
#endif
#ifdef CYGNUM_HAL_BFIN_INTERRUPTS_EXTENDED

    .macro  hal_bfin_sic_interrupt_trampoline idx
hal_bfin_interrupt_trampoline_\idx:
    [--SP] = ASTAT                      // # save the flags on the stack
    [--SP] = P0                         // # save P0 on the stack
    P0 = \idx (Z)                       // # load the interrupt number into P0 zero extended
    JUMP hal_bfin_sic_interrupt_common      // # jump to the common part
    .endm

    hal_bfin_sic_interrupt_trampoline 7     // # general purpose interrupt 7
    hal_bfin_sic_interrupt_trampoline 8     // # general purpose interrupt 8 
    
    hal_bfin_sic_interrupt_trampoline 9     // # general purpose interrupt 9 
    hal_bfin_sic_interrupt_trampoline 10    // # general purpose interrupt 10 
    hal_bfin_sic_interrupt_trampoline 11    // # general purpose interrupt 11 
    hal_bfin_sic_interrupt_trampoline 12    // # general purpose interrupt 12 
    hal_bfin_sic_interrupt_trampoline 13    // # general purpose interrupt 13 
#endif
    hal_bfin_interrupt_trampoline 14    // # general purpose interrupt 14 
    hal_bfin_interrupt_trampoline 15    // # general purpose interrupt 15 

##-----------------------------------------------------------------------------
## interrupt trampoline common to enter ISR.
## Saves minimum machine state and calls interrupt handling code.
##
## general purpose interrupts are disabled during this process
##
## attention, this routine must not be called by the exception vector
##
## PRECON  : ASTAT
##           P0         <- SP
##           P0 contains core interrupt priority
##
## POSTCON:
##
##          ASTAT
##          P0
##          R0  <- SP  (lower memory address)
##      registers
##          P0 address of the IVSR from VSR table
##          R0 number of the interrupt taken
##
## SIDE-EFFECTS : changes to the interface outgoing this function must be propagated to the
##                hal_bfin_sic_interrupt_trampoline
##                respect PRECOND of the default_interrupt_vsr


## this code must only be called by interrupts < 7 in extended mode
## or any core priority interrupt in non-extended mode
.globl hal_bfin_interrupt_common;
hal_bfin_interrupt_common:
    [--SP] = R0                 // # save R1:0 as they are needed as working registers
    [--SP] = R1                 // # by the trampoline

##
##  ASTAT
##  P0
##  R0
##  R1      <-SP
##

    R0     = P0                 // # move the value from P0 to R0 for computation

##  R0 contains core interrupt priority
    

##  load the base address of the VSR table
    R1.H   = hal_vsr_table  // #
    R1.L   = hal_vsr_table  // # load the address of the VSR table into P0

##  core priority of interupt is VSR number
##  multiply by 4 to get address of entry point
##  as every entry in VSR table is 4 bytes

    R0     <<= 2                // # multiply the number of the exception with 4, due to
                                // # 4 byte address entries in the ISR table

##  R1 now contains the address to the VSR table entry

    R1     = R1 + R0            // # add the offset into the table (R0) to the table start (R1)
    P0     = R1                 // # move the address into P0 for loading

##  restore interrupt number in R0

    R0     >>= 2                // # restore the interrupt number

##  R1 now contains the entry point of the interrupt VSR

    R1     = [P0]               // # load the destination address of the exception vector into R0
    P0     = R1                 // # move the address into the pointer register

##  entry point has now been moved to P0

    R1     = [SP++]             // # restore the registers R1

##
##  ASTAT
##  P0
##  R0      <- SP
##

##  P0 contains entry point to VSR
##  R0 contains interrupt number taken
##  all other registers unchanged

##  VERIFY: POSTCON satisfied
##          21/09/2006

    JUMP    (P0)                // # jump to the interrupt handling routine

_hal_end_end_intr:
    R2      = [SP++]
    R1      = [SP++]
    R0      = [SP++]
    P0      = [SP++]
    ASTAT   = [SP++]
    RTI

##    .end hal_bfin_interrupt_common

#ifdef CYGNUM_HAL_BFIN_INTERRUPTS_EXTENDED

## this interrupt common trampoline must only be used by core interrupts
## that are entry points of the system interrupt controller to the core
## it is assumed that the interrupt number in P0 is 7 or greater
## this is assured by previous trampoline code and setup during system startup

.globl hal_bfin_sic_interrupt_common;
hal_bfin_sic_interrupt_common:
    [--SP] = R0                 // # save R1:0 as they are needed as working registers
    R0     = P0                 // # move the value from P0 to R0 for computation
    [--SP] = R1                 // # by the trampoline
    [--SP] = R2                 
    [--SP] = R3                 
    [--SP] = P0
    [--SP] = P1
    [--SP] = P2
    [--SP] = RETS

##
##  the registers R0,R1,R2,R3 as well as P0,P1,P2 and RETS are volatile
##  for function calls and must be saved to survive the call
##  this trampoline contains a call to _hal_lsbit_index
##
##  all other registers are callee saved
##


##
##  ASTAT
##  P0
##  R0
##  R1
##  R2
##  R3
##  P0
##  P1
##  P2
##  RETS    <- SP
##
    
    R1     = R1 ^ R1
    R1     = 0x7
    R1     = R0 - R1            // # we now have the interrupt lane of the system interrupt controller in R1

    // load the mask of the system interrupts assigned to the core interrupt
    R2.H   = hal_interrupt_sic_assign_mask
    R2.L   = hal_interrupt_sic_assign_mask
    
    R1     <<= 2
    R2     = R2+R1
    P1     = R2
    R0     = [P1]
    // R0 now contains the sic assign mask

    // load the current ISR register
    P1.H   = 0xFFC0
    P1.L   = 0x0120
    R1     = [P1]
    // R1 contains the current status of interrupts asserted on the SIC

    // get rid of all interrupts asserted that are not assigned to this core interrupt
    R0     = R1 & R0
    // now mask off all interrupts that haven't been unmasked for processing but are
    // assigned to this core interrupt and are latched
    // load the current SIC_IMASK register
    P1.H   = 0xFFC0
    P1.L   = 0x010C
    R1     = [P1]
    R0     = R1 & R0
    // R0 contains all interrupts asserted and assigned to the current core interrupt
    

    CC = R0
    IF !CC JUMP hal_bfin_sic_interrupt_common_abort

    .extern _hal_lsbit_index
    CALL _hal_lsbit_index
    RETS   = [SP++]
    P2   = [SP++]
    P1   = [SP++]
    P0   = [SP++]
    R3   = [SP++]
    R2   = [SP++]

##
##  ASTAT
##  P0
##  R0
##  R1      <- SP
##

    // R0 now contains the interrupt number at the SIC to be handled
    // R0 should never be -1
    R1.H = 0x0
    R1.L = 0x7
    R0   = R1 + R0

    
    // we now have the interrupt that has to be called
    R1.H   = hal_vsr_table      // #
    R1.L   = hal_vsr_table      // # load the address of the VSR table into P0
    R0     <<= 2                // # multiply the number of the exception with 4, due to
                                // # 4 byte address entries in the ISR table
    R1     = R1 + R0            // # add the offset into the table (R0) to the table start (R1)
    P0     = R1                 // # move the address into P0 for loading
    R0     >>= 2                // # restore the interrupt number
    R1     = [P0]               // # load the destination address of the exception vector into R0
    P0     = R1                 // # move the address into the pointer register

    R1     = [SP++]             // # restore the registers R1

##
##  ASTAT
##  P0
##  R0      <- SP
## 

## R0 contains interrupt number taken
## P0 contains entry point to VSR routine
## POSTCON: satisfied
    
    JUMP    (P0)                // # jump to the interrupt handling routine

hal_bfin_sic_interrupt_common_abort:
    RETS   = [SP++]
    P2   = [SP++]
    P1   = [SP++]
    P0   = [SP++]
    R3   = [SP++]
    R2   = [SP++]
    R1   = [SP++]             // # restore the registers R1
    R0   = [SP++]     // # restore the two registers, used by interrupt trampoline
    P0   = [SP++]
    ASTAT = [SP++]

    RTI             // # return from interrupt

#endif

FUNC_START(__default_nmi_vsr)

    hal_cache_init

    R0 = R0 ^ R0
    .extern _panic
    P0.H = _panic
    P0.L = _panic
    CALL (P0)
//  CALL _panic

__default_nmi_vsr_idle:
            CLI R0
            IDLE
            STI R0
            JUMP __default_nmi_vsr_idle
            

##-----------------------------------------------------------------------------
## Default exception VSR.
## Saves machine state and calls external handling code.
## normal handling code
##
## PRECON : P0  = entry point to VSR
##          USP = saved P0
FUNC_START(___default_exception_vsr)	
FUNC_START(__default_exception_vsr)

    ## we enter with the following conditions
    ##
    ## all registers unchanged since exception occurred except for P0, SP
    ## P0 on top of stack
    ## pop of P0 and ASTAT will restore state when exception occurred including SP

    P0  = USP            // # restore P0

## we did restore the entire CPU state as prior to exception
## we now have the state as prior to raising the exception
## save current stack point in USP
    USP = SP


## switch to exception stack, as no double exceptions should ever occur
## no previous stack operating on stack
    SP.H = __hal_exception_temporary_frame_stack
    SP.L = __hal_exception_temporary_frame_stack


    [--SP] = R0                 // # by the trampoline
    [--SP] = R1                 // # by the trampoline
    [--SP] = R2                 
    [--SP] = R3                 
    [--SP] = P0
    [--SP] = P1
    [--SP] = P2
    [--SP] = RETS    

    R0 = USP

    .extern _cyg_hal_assure_sp_access
    P0.H = _cyg_hal_assure_sp_access
    P0.L = _cyg_hal_assure_sp_access
    CALL (P0)
//  CALL _cyg_hal_assure_sp_access

    RETS   = [SP++]
    P2   = [SP++]
    P1   = [SP++]
    P0   = [SP++]
    R3   = [SP++]
    R2   = [SP++]
    R1   = [SP++]             // # restore the registers R1
    R0   = [SP++]     // # restore the two registers, used by interrupt trampoline

    SP = USP

    savecpustate

## context is saved on threads stack

 // # The machine state is now all saved on the stack.
# VERIFY :  cpu state is now on the stack in form of a HAL_Saved_Registers structure


    R4     = SYSCFG
    BITCLR(R4,0)   // clear single stepping
    SYSCFG = R4

	hal_diag_excpt_start
	
#ifdef HAL_BFIN_IMPL_CAUSE
# VERIFY : set the return address for this exception to be the VSR itself for
#          handling on lower priority
#          RETX has been saved by savecpustate macro

    P0.L = __default_exception_vsr_continue
    P0.H = __default_exception_vsr_continue
    RETX = P0
# VERIFY : return from exception and continue with the continue position
    RTX
    NOP
    NOP

    .align 8
# VERIFY : ASSUMPTION 1:  running on the priority level of the causing activity
#          ASSUMPTION 2:  exception causing activity will not run while exception
#                         is handled
#
#   1      RTX has caused to return from exception to the level causing it
#          there might have been interrupts in between the RTX and the resume of
#          this call but the call returned to desired position
#   2      return address to resume thread was in RETX, this has been replaced with
#          VSR entry point, this means that the thread has not been continued yet
#          continue thread possible via another RTX only as state restore necessary to
#          restore all registers 
FUNC_START(__default_exception_vsr_continue)
#endif

    
# VERIFY : FP has not been touched since savecpu state
#          save cpu state returns pointer to HAL_Saved_Registers structure in FP

    R0 = FP                         // # first argument is pointer to register
                                    // # dump

# VERIFY : _cyg_hal_exception_handler expects pointer to HAL_Saved_Registers structure as
#          first argument in R0

# CASE NOT DEF HAL_BFIN_IMPL_CAUSE
#           _cyg_hal_exception_handler must not cause another exception
#           cannot be verified generally as user might be able to set own
#           exception handlers which can cause exception due to code errors or
#           page misses -> core double fault condition, unrecoverable event
#
#   RESULT: exceptions must be prevented, they must not occur in this code
#   TIMING: this code runs with highest priority and no other code can run
#           this will increase INTERRUPT LATENCY
#           this situation may constitue a priority inversion, as a user thread
#           can block a higher priority interrupt
#           this code can NOT be interrupted by any interrupt or other thread
#           
#           PRIORITY INVERSION possible and likely
#
#   SIDECON handling code MUST NOT invoke activities that could lead to scheduling
#           as scheduler must not be activated, same conditions as ISR code
# 
# CASE DEF HAL_BFIN_IMPL_CAUSE
#           this part of the code is not running in an exception state as we returned
#           to lower level event
#           exceptions can be caused by the handler due to same situations as
#           for previous case but they will NOT cause a core double fault
#           as the exception is not caused while processing at the exception priority level
#
#   RESULT: exceptions can be caused, exception handling code is treated by the core as
#           every other code that is not running at exception priority
#   TIMING: code will run at the priority level of the code causing the exception
#           this code will run as the code itself, if the code was subject to scheduling
#           the code can NOT gain more processing time by executing an exception as
#           scheduling activities are allowed
#           the exception is not blocking higher priority events
#           NO priority inversion
#
#           NO PRIORITY INVERSION
#
#   SIDECON handling code can invoke any function thread code can, same condition as to interrupted
#           code apply
#           
#           SCHEDULING activity might be possible, must be strictly avoided if interrupts or scheduler
#           is disabled
#
# GENERAL:  exception handling routines SHOULD NOT use result in scheduling activities
#           CASE DEF HAL_BFIN_IMPL_CAUSE is saver as paging/MMU exceptions do not produce core double
#           fault, no priority inversion, scheduling might continue

	.extern	_cyg_hal_exception_handler
    P0.H = _cyg_hal_exception_handler
    P0.L = _cyg_hal_exception_handler
    CALL (P0)
//	CALL	_cyg_hal_exception_handler // # call C code

#ifdef HAL_BFIN_IMPL_CAUSE
# VERIFY: PRECON VSR for exception 0 has not been changed
#         EXCPT 0 will lead to exception return entry point
	.align 8
    EXCPT 0
    NOP
    NOP
    NOP
    NOP

.align 8
FUNC_START(__default_exception_vsr_return)
    
# VERIFY:
#   exception trampoline POSTCON state P0 is saved in USP
#   reverse these changes so that we have the same state as before causing
#   exception 0, except for changed system state

   P0 = USP            // # restore flags saved by the return call

#endif // # HAL_BFIN_IMPL_CAUSE
	
# VERIFY: task left for exception VSR is to restore the context and return 

    restorecpustate        // # restore the cpu state of the interrupted task
    
# VERIFY: restore state restored all registers



    RTX                    // # return to the interrupted task
    				
##FUNC_END(__default_exception_vsr)

##-----------------------------------------------------------------------------
## ICPLD miss exception VSR.
## Saves machine state and calls ICPLD miss code
## normal handling code
##
## PRECON : P0  = entry point to VSR
##          USP = saved P0
##
## POSTCON: all registers as before exception occurred
##          (working registers)
##
FUNC_START(__hal_icpld_miss_exception_vsr)    

    ## we enter with the following conditions
    ##
    ## all registers unchanged since exception occurred except for P0, SP
    ## P0 on top of stack
    ## pop of P0 and ASTAT will restore state when exception occurred including SP

    P0  = USP            // # restore P0 out of USP

## we now have the state as prior to raising the exception
## save current stack point in USP
    USP = SP

## switch to exception stack, as no double exceptions should ever occur
## no previous stack operating on stack
    SP.H = __hal_exception_temporary_frame_stack
    SP.L = __hal_exception_temporary_frame_stack

## save the current threads context on the stack
## including USP
    
    savecpustate

## 

 // # The machine state is now all saved on the stack.

    R4     = SYSCFG
    BITCLR(R4,0)   // clear single stepping
    SYSCFG = R4

    hal_diag_excpt_start
    

    R0 = FP                         // # first argument is pointer to register
                                    // # dump

   
    .extern _cyg_hal_icache_miss
    P0.H = _cyg_hal_icache_miss
    P0.L = _cyg_hal_icache_miss
    CALL (P0)
//  CALL    _cyg_hal_icache_miss // # call C code

## exception is either handled when arriving here
    
    restorecpustate        // # restore the cpu state of the interrupted task

## we now have the context restore, are still on the exception stack
## USP is the old stack pointer

    SP = USP

## old stack pointer has been restored
## all changes reversed
## we have the old context

## return from exception
    
    RTX                    // # return to the interrupted task
## POSTCON: all registers as before exception occurred
##          (working registers)
##
## VERIFIED: 21/09/2006

##FUNC_END(__hal_icpld_miss_exception_vsr)

##-----------------------------------------------------------------------------
## DCPLD miss exception VSR.
## Saves machine state and calls external handling code.
## normal handling code
##
## PRECON :
##
##  ASTAT
##  P0 <- SP
FUNC_START(__hal_dcpld_miss_exception_vsr)    

    ## we enter with the following conditions
    ##
    ## all registers unchanged since exception occurred except for P0, SP
    ## P0 on top of stack
    ## pop of P0 and ASTAT will restore state when exception occurred including SP

    P0  = USP            // # restore P0 out of USP

## we now have the state as prior to raising the exception
## save current stack point in USP
    USP = SP

## switch to exception stack, as no double exceptions should ever occur
## no previous stack operating on stack
    SP.H = __hal_exception_temporary_frame_stack
    SP.L = __hal_exception_temporary_frame_stack

## save the current threads context on the stack
## including USP
    
    savecpustate

## 

 // # The machine state is now all saved on the stack.

    R4     = SYSCFG
    BITCLR(R4,0)   // clear single stepping
    SYSCFG = R4

    hal_diag_excpt_start
    
    R0 = FP                         // # first argument is pointer to register
                                    // # dump

    .extern _cyg_hal_dcache_miss
    P0.H = _cyg_hal_dcache_miss
    P0.L = _cyg_hal_dcache_miss
    CALL (P0)
//  CALL    _cyg_hal_dcache_miss // # call C code

## exception is either handled when arriving here
    
    restorecpustate        // # restore the cpu state of the interrupted task

## we now have the context restore, are still on the exception stack
## USP is the old stack pointer

    SP = USP

## old stack pointer has been restored
## all changes reversed
## we have the old context

## return from exception
    
    RTX                    // # return to the interrupted task
## POSTCON: all registers as before exception occurred
##          (working registers)
##
## VERIFIED: 21/09/2006
                    
##FUNC_END(__hal_dcpld_miss_exception_vsr)


##------------------------------------------------------------------------------
## Default interrupt VSR.
## Saves machine state and calls appropriate ISR. When done, calls
## interrupt_end() to finish up and possibly reschedule.	
##
## PRECON :
##
##  ASTAT
##  P0
##  R0 <- SP

FUNC_START(__default_interrupt_vsr)

	
 // # We enter here with all of the CPU state still
 // # in its registers except:
 // # P0 = address of this function
 // # R0 = interrupt number taken
    
 	savecpustate

    
 // # The machine state is now all saved on the stack.
    // # FP points to the saved state
    // # P0 and R0 haven't been touched by the savecpustate code

    // # this code is needed to allow disabling RETI after
    // # switching back to the user stack
    // # to avoid having to disable interrupt while the kernel is
    // # processing interrupt_end()
#ifdef CYGSEM_HAL_COMMON_INTERRUPTS_ALLOW_NESTING
    P0   = RETI
    [--SP] = P0
#endif
   // hal_led_stack_support 6
    // R0 untouched, interrupt number

#ifdef CYGFUN_HAL_COMMON_KERNEL_SUPPORT
	.extern	_cyg_scheduler_sched_lock
	//P0.H = HI(_cyg_scheduler_sched_lock)
    P0.H = _cyg_scheduler_sched_lock
    //P0.L = LO(_cyg_scheduler_sched_lock)
    P0.L = _cyg_scheduler_sched_lock
    R1   = [P0]
    R1   += 1
	[P0] = R1

#endif

    // R0 untouched, interrupt number

#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK
	hal_switch_to_interrupt_stack
#endif

 //   hal_led_stack_support 7
    // # interrupt number is still in R0

##	hal_diag_intr_start

#ifdef CYGSEM_HAL_COMMON_INTERRUPTS_ALLOW_NESTING

  ##  // # if we allow nesting we have to push the RETI register in order to
  ##  // # reenable maskable interrupts
    
    [--SP] = RETI
    R1     = [SP++]
#endif

    [--SP] = R0 // # save the interrupt number
    
//	hal_led_stack_support 1	
#if defined(CYGPKG_KERNEL_INSTRUMENT) && defined(CYGDBG_KERNEL_INSTRUMENT_INTR)

 // # Call cyg_instrument to record that this interrupt is being raised.
	
    R1    = R0              // # vector number
    R2    = R0	            // # interrupt number
    P0.H  = 0x0             // # zero R0
    R0.L  = 0x0301		    // # R0=  type = INTR,RAISE
	CALL _cyg_instrument	// # call instrument function
#endif
    
	R0 = [SP++]             // # get the interrupt number back

#ifdef CYGPKG_HAL_BFIN_BF60x
    R1 = 11;
    CC = R0 == R1;           // ETV11: SEC
    IF !CC jump skip;
    P0.H = 0xFFE0;           // CEC_SID
    P0.L = 0x2118;
    R0 = [P0];
    [P0] = R0;               // acknowledge
    R1 = 16
    R0 = R0 + R1;
#endif

skip:
    R0 <<= 2                 // # multiply the interrupt by 4
	
    //R1.H   = HI(hal_interrupt_handlers)  // #
    R1.H   = hal_interrupt_handlers  // #
    //R1.L   = LO(hal_interrupt_handlers)  // # load the address of the interrupt handler table into P0
    R1.L   = hal_interrupt_handlers  // # load the address of the interrupt handler table into P0
    //R2.H   = HI(hal_interrupt_data)      // # load the address of the interrupt data table into P0
    R2.H   = hal_interrupt_data      // # load the address of the interrupt data table into P0
    R1     = R1 + R0
    //R2.L   = LO(hal_interrupt_data)
    R2.L   = hal_interrupt_data
    P1     = R1
    R2     = R2 + R0
    P0     = [P1]                        // # load the handler entry point from table
    R0     >>= 2                         // # save the interrupt number
    P1     = R2
    R7     = R0                          // # restore the real interrupt number
    R1     = [P1]                        // # load the pointer to the data from table

 //   hal_led_stack_support 2
    // R7 contains interrupt number to survive function call
    CALL   (P0)                          // # call the interrupt service handler
    // R0 return value of ISR
    // R7 interrupt number

 //   hal_led_stack_support 4

#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK

    R6 = R0        // # save return state
	hal_switch_from_interrupt_stack
    R0 = R6
#endif 
	
#ifdef CYGFUN_HAL_COMMON_KERNEL_SUPPORT

 // # We only need to call _interrupt_end() when there is a kernel
 // # present to do any tidying up.
	
	//R1.H = HI(hal_interrupt_objects)
    R1.H = hal_interrupt_objects
    R7 <<= 2                        // # multiply by 4
    //R1.L = LO(hal_interrupt_objects) // # interrupt object table
    R1.L = hal_interrupt_objects // # interrupt object table
    R1 = R1 + R7
    P0 = R1
    R2 = FP                         // # saved cpu state
    // # R0 still contains the return value of the isr
    R1 = [P0]                       // # interrupt object pointer

//    hal_led_stack_support 1

    .extern	_interrupt_end
    P0.H = _interrupt_end
    P0.L = _interrupt_end
    CALL (P0)
//	CALL _interrupt_end	 // # call into C to finish off 

    
#endif


# we now disable interrupts again, it is not really necessary but we do it
# to avoid any complications

#ifdef CYGSEM_HAL_COMMON_INTERRUPTS_ALLOW_NESTING
    RETI = [SP++]
#endif

    ## we can restore the cpu state now

 //       hal_led_stack_support 4
    restorecpustate
    
    // # after restoring the cpu state we are in the state prior to saving
    // # P0 = address of this function
    // # R0 = interrupt number taken

    R0 = [SP++]     // # restore the two registers, used by interrupt trampoline
    P0 = [SP++]
    ASTAT = [SP++]

  //  hal_led_stack_support 0
    
    RTI             // # return from interrupt

		
##FUNC_END(__default_interrupt_vsr)


##-----------------------------------------------------------------------------
## Execute pending DSRs on the interrupt stack with interrupts enabled.
## Note: this can only be called from code running on a thread stack
	
#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK
	.extern cyg_interrupt_call_pending_DSRs

FUNC_START(hal_interrupt_stack_call_pending_DSRs)
FUNC_START(_hal_interrupt_stack_call_pending_DSRs)

	nop				 // # delay slot
    RTS
	
##FUNC_END(hal_interrupt_stack_call_pending_DSRs)	
#endif		

##-----------------------------------------------------------------------------
## Switch to a new stack.
## This is used in RedBoot to allow code to execute in a different
## stack context.

FUNC_START(_hal_program_new_stack)
FUNC_START(hal_program_new_stack)
 // # Arguments are:
 // # R0 = function to call
 // # R1 = stack pointer to use

	LINK 0
    
    R2  = SP                    // # save old stack ptr
    P0  = R0                    // # get function to call into pointer register
	SP  = R1       			    // # sp = new stack
//    SP  = SP - 32
    [--SP] = R2	
    [--SP] = RETS

	CALL (P0)

    RETS = [SP++]
    R2   = [SP++]
    SP   = R2
    
	UNLINK
    RTS
	
##FUNC_END(hal_program_new_stack)


##-----------------------------------------------------------------------------
## hal_zero_bss
## Zero bss. Done in assembler to be optimal rather than using memset,
## which would risk zeroing bss while using it.
        
FUNC_START(hal_zero_bss)
    R0.H = __bss_end
    R0.L = __bss_end
    
    R1.H = __bss_start
    R1.L = __bss_start
    
    
    R0    = R0 - R1
    R0  >>= 2                        // # number of iterations
    
    P0    = R1

    P1    = R0

    R1   = R1 ^ R1                   // # zero R1

    
## do loop for P1 interations
    LOOP __zero_bss_loop LC0 = P1;
    LOOP_BEGIN __zero_bss_loop;      // # start of loop
        [P0++] = R1
    LOOP_END __zero_bss_loop;        // # end of loop
    RTS
##FUNC_END(hal_zero_bss)

#ifndef CYG_HAL_STARTUP_RAM
    .section ".reset_vector","ax"
#else 
    .text
#endif

#ifndef CYG_HAL_STARTUP_RAM
FUNC_START(___hal_copy_data)
##    LINK 0x0
##    [--SP] = (R7:0,P5:0)
##    [--SP] = LC0
##    [--SP] = LT0
##    [--SP] = LB0

## copy hal
    R0.H = __size_of_hal
    R0.L = __size_of_hal
    
    R1.H = __sint_hal
    R1.L = __sint_hal 
    
    
 #   R0    = R0 - R1
    R0  >>= 2                        // # number of iterations
    
    P0    = R1

    R1.H = __rom_hal_code_start
    R1.L = __rom_hal_code_start
    
    P2    = R1
    
    P1    = R0

    R5.l  = 0x1
## do loop for P1 interations
    LOOP __hal_copy_loop LC0 = P1;
    LOOP_BEGIN __hal_copy_loop;      // # start of loop
        R1     = [P2++]
        [P0++] = R1
    LOOP_END __hal_copy_loop;        // # end of loop


## copy code and read-only data
    R0.H = __size_of_code
    R0.L = __size_of_code
    
    R1.H = _stext
    R1.L = _stext
    
    
 #   R0    = R0 - R1
    R0  >>= 2                        // # number of iterations
    
    P0    = R1

    R1.H = __rom_code_start
    R1.L = __rom_code_start
    
    P2    = R1
    
    P1    = R0

    R5.l  = 0x2
## do loop for P1 interations
    LOOP __code_copy_loop LC0 = P1;
    LOOP_BEGIN __code_copy_loop;      // # start of loop
        R1     = [P2++]
        [P0++] = R1
    LOOP_END __code_copy_loop;        // # end of loop



## copy system data
    R0.H = __size_of_system_data
    R0.L = __size_of_system_data
    
    R1.H = __START_OF_SYSTEM_DATA
    R1.L = __START_OF_SYSTEM_DATA
    
    
 #   R0    = R0 - R1
    R0  >>= 2                        // # number of iterations
    
    P0    = R1

    R1.H = __rom_system_data_start
    R1.L = __rom_system_data_start
    
    P2    = R1
    
    P1    = R0


    R5.l  = 0x3
## do loop for P1 interations
    LOOP __data_copy_loop LC0 = P1;
    LOOP_BEGIN __data_copy_loop;      // # start of loop
        R1     = [P2++]
        [P0++] = R1
    LOOP_END __data_copy_loop;        // # end of loop



## copy data
    R0.H = __size_of_data
    R0.L = __size_of_data
    
    R1.H = __ram_data_start
    R1.L = __ram_data_start
    
    
#    R0    = R0 - R1
    R0  >>= 2                        // # number of iterations
    
    P0    = R1

    R1.H = __rom_data_start
    R1.L = __rom_data_start
    
    P2    = R1
    
    P1    = R0


    R5.l  = 0x4
## do loop for P1 interations
    LOOP __data_copy_loop2 LC0 = P1;
    LOOP_BEGIN __data_copy_loop2;      // # start of loop
        R1     = [P2++]
        [P0++] = R1
    LOOP_END __data_copy_loop2;        // # end of loop

hal_led_stack_support 1


    SSYNC

##    LB0 = [SP++]
##    LT0 = [SP++]
##    LC0 = [SP++]
##    (R7:0,P5:0) = [SP++]
##    UNLINK
    RTS
##FUNC_END(hal_zero_bss)
#endif



##-----------------------------------------------------------------------------
##      stack, data, vsr section
##-----------------------------------------------------------------------------

##-----------------------------------------------------------------------------
## Interrupt Stack.
## Used during intialization and for executing ISRs.
	
.section ".system_data","aw"

	.balign 32
	.global cyg_interrupt_stack_base
    .global _cyg_interrupt_stack_base
    .global _interrupt_stack_base
cyg_interrupt_stack_base:
_cyg_interrupt_stack_base:
__interrupt_stack_base:
_interrupt_stack_base:
	.rept CYGNUM_HAL_COMMON_INTERRUPTS_STACK_SIZE
	.byte 0
	.endr
	.balign 32
	.global cyg_interrupt_stack
    .global _cyg_interrupt_stack
    .global _interrupt_stack
    .global __interrupt_stack
cyg_interrupt_stack:
_cyg_interrupt_stack:
__interrupt_stack:
_interrupt_stack:

	.long	0,0,0,0,0,0,0,0	

##-----------------------------------------------------------------------------
## VSR table.
## The main exception code indirects through here to find the VSR
## to execute for each architecture defined exception.


#------------------------------------------------------------------------------
# Interrupt vector tables.
# These tables contain the isr, data and object pointers used to deliver
# interrupts to user code.
# hal_interrupt_level contains the interrupt level set by 
# HAL_INTERRUPT_CONFIGURE().
# This is a default set that provide support only for the 14 external
# interrupts in the status/cause registers. Platforms or boards are expected
# to define their own versions of these if they have their own interrupt mappings.

.section ".system_data","aw"
	
__hal_bfin_program_stack_pointer:
    .long   0
.globl  hal_interrupt_saved_mask
    hal_interrupt_saved_mask:
    .long   0

.globl  _booted
    _booted:
    .long   0

.globl  _hal_intr_mask_applied
    _hal_intr_mask_applied:
    .long   0x8000

.globl __hal_exception_temporary_frame
__hal_exception_temporary_frame:
    .rept 200
    .long 0
    .endr
.globl __hal_exception_temporary_frame_stack
__hal_exception_temporary_frame_stack:


##-----------------------------------------------------------------------------
## end of vectors.S


