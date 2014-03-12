##=============================================================================##
##	context.S
##
##	BFIN context switch code
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
## Author(s): 	andre
## Date:	2006-05-18
## Purpose:	BFIN context switch code
## Description:	This file contains implementations of the thread context 
##		switch routines. It also contains the longjmp() and setjmp()
##              routines.
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <pkgconf/hal.h>

#include <cyg/hal/arch.inc>

// #define BFIN_DEBUG_CONTEXT_LOAD

#------------------------------------------------------------------------------
# hal_thread_switch_context
# Switch thread contexts
# :     R0 :   address of sp of next thread to execute
# :     R1 :   address of sp save location of current thread
#
#       RETS    :   return address of function call
#
# c - calling convention via gcc for BlackFin
#   R[3:0], P[2:0], others          caller saved (clobbered registers)
#   R[7:4], P[5:3], others          callee saved
#
# this code saves the current state of the processor

FUNC_START(hal_thread_switch_context)
FUNC_START(_hal_thread_switch_context)

    [--SP]  = FP           ;# save old frame pointer

    [--SP]  = USP          ;# save USP
    [--SP]  = ASTAT

    FP     = RETX
    [--SP]  = FP          ;# save RETX register as PC
    
    FP     = RETE
    [--SP]  = FP          ;# save RETE register on stack

    FP     = RETN
    [--SP]  = FP          ;# save RETN register on stack
    FP     = RETX
    [--SP]  = FP          ;# save RETX register on stack
    FP     = RETI
    [--SP]  = FP          ;# save RETI register on stack
                           ;# important not to perform [ --SP ] = RETI
                           ;# as this would disable interrupts
    FP     = RETS
    [--SP]  = FP          ;# save RETS register on stack

    [--SP]  = LC1          ;# store loop counter register on the stack
    [--SP]  = LC0

    [--SP]  = LB1          ;# store loop bottom registers on the stack
    [--SP]  = LB0

    [--SP]  = LT1          ;# store loop top registers on the stack
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
    
    [--SP]  = (R7:0,P5:0)  ;# multiple push will push in the order
                           ;# first R0 -> R7 and then P0 -> P5

    P0.H    = 0xFFE0
    P0.L    = 0x2104       ;# IMASK
    R2      = [P0]
    [--SP]  = R2
    P0.L    = 0x2108       ;# IPEND
    R2      = [P0]
    [--SP]  = R2
    [--SP]  = SEQSTAT
    
    
    P0      = R1           ;# move the location where we should save the SP
                           ;# to P0, P0 is saved already, so is R1
                           ;# R1 will be destroyed by following code

    R1      = A1.X;# (S)
    [--SP]  = R1

    R1      = A0.X;# (S)
    [--SP]  = R1

    R1      = A1.W 
    [--SP]  = R1

    R1      = A0.W 
    [--SP]  = R1

    P3.H    = 0xFFE0        //# d cpld fault register
    P3.L    = 0x000C
    R2      = [P3]
    [--SP]  = R2

    P3.H    = 0xFFE0        //# i cpld fault register
    P3.L    = 0x100C
    R2      = [P3]
    [--SP]  = R2

    
    [P0]    = SP           ;# save current stack point of the stack
##    R0      = R1           ;# move the address from where to load the new context
                           ;# into R0 as the function load_context has only this one
                           ;# parameter passed through R0
    
   ;# drop through to hal_thread_load_context to load new context
    

#------------------------------------------------------------------------------
# hal_thread_switch_load
# R1 = address of sp of next thread to execute
# Note that this function is also the second half of hal_thread_switch_context
# and is simply dropped into from it.
#
# PRECON    :   R0      contains pointer to the new threads stack pointer
#               IPEND   contains the current BFIN HAL state
#
# INV       :   IPEND   checked at any time must be compatible with the state
#                       recorded at the beginning of this function until the first
#                       register is restored from the stack
#
# HINT      :   synchronization of calls to this function must be done externally
#
# SIDECON   :   any exceptions, or interrupts that might occur during this call
#               can be serviced without changing the system state
#
# POSTCON   :   all registers are restored as they were prior to calling this function
#               within this thread's context
#               the stack is restored
#               the thread will operate in the systems state it was suspended in
#
# HINT      :   ANY changes to the system state while this call operates should be
#               prevented
#               BUT this call can be interrupted at any time with another call to
#               hal_thread_switch_context
#               as the current state will be recorded in the context
#
# INFO      :   this code contains a synchronization mechanism that disables all
#               core interrupts to prevent system malfunction
#               this part of the code is short, it includes a RAISE instruction
#               during this call only INT 14 will be unmasked as it is needed to
#               continue this function

FUNC_START(hal_thread_load_context)
FUNC_START(_hal_thread_load_context)

    P0      = R0           ;# move location from where to load new stack point into
                           ;# pointer register P0
    SP      = [P0]

//  # do not touch SP

    R6 = [SP+0x1C]      ;# load the stored IPEND register into R0


//    # load current IPEND in R1
    P0.H    = 0xFFE0
    P0.L    = 0x2108       ;# IPEND
    R7      = [P0]

    
//    # clear the bits 15 for interrupt 15
//    # 4 for pending interrupt hasn't saved RETI yet
//    # and all non maskable events as they can't cause a context switch
//    # by the handler

//    # clear all bits that do not determine the interrupt state of the current
//    # and new contexts

#ifdef BFIN_DEBUG_CONTEXT_LOAD
    R0      = R7
    .extern _diag_write_hex
    CALL _diag_write_hex
    
    R0      = '>'
    .extern _diag_write_char
    CALL _diag_write_char
    
    R0      = R6
    .extern _diag_write_hex
    CALL _diag_write_hex
#endif

    R2.L    = 0x7FE0    ;# load mask into R2
    R2.H    = 0x0

    R6      = R6 & R2
    CC      = R6        ;# load the is zero flag into R0
    R6      = CC
//                        # set if the new thread was suspended with interrupts pending
    R7      = R7 & R2
    CC      = R7        ;# load the is zero flag into R1
    R7      = CC
//                        # set if the new thread was suspended with interrupts pending

    R5      = R6 ^ R7   ;# do a binary XOR on the flags this will yield a zero
//                        # if the current state and the state of the new thread are compatible

#ifdef BFIN_DEBUG_CONTEXT_LOAD
    R0      = '|'
    .extern _diag_write_char
    CALL _diag_write_char
    R0      = R7
    .extern _diag_write_hex
    CALL _diag_write_hex

    R0      = '>'
    .extern _diag_write_char
    CALL _diag_write_char
    
    R0      = R6
    .extern _diag_write_hex
    CALL _diag_write_hex

    R0      = '='
    .extern _diag_write_char
    CALL _diag_write_char
    
    R0      = R5
    .extern _diag_write_hex
    CALL _diag_write_hex
#endif
    
//    # eCOS BFIN HAL system state
//    # --------------------------
//    #
//    # BFIN user level   : IPEND & 0x0000FFE0 yields 0x00008000
//    # BFIN system level : IPEND & 0x0000FFE0 yields a value NOT equal to 0x00008000
//    #
//    # the 5 most lower bits are never set while switching contexts as no context switch
//    # can be initiated in an EXCEPTION, NON-MASKABLE INT or RESET state
//    # a EMULATION is no normal operating state and does not yield switching
//    # the most upper 16 bits are never used by the processor

//    # compatible states
//    # -----------------
//    #
//    # states are compatible if they classify the SAME eCOS BFIN HAL system state.
//    #
//    # compatible state have equal eCOS BFIN HAL system states
//    #
//    # meaning: both state are BFIN user level OR both states are BFIN system level
//    #
//    # NOT COMPATIBLE are ONE state is BFIN user level while the other is BFIN system level


    CC = R5

    IF !CC JUMP hal_thread_load_context_continue

    CSYNC               ;# prevent speculative execution

//    # being here means:
//    # the two BFIN HAL system state of the new and current thread are NOT compatible
//    # we only need to care about the new thread's state as the current thread's
//    # state will be opposite

    CC = R6   ;  # load CC with 1 if not zero -> new thread was in BFIN system level
              //# load CC with 0 if zero     -> new thread was in BFIN user level
    IF CC JUMP hal_bfin_switch_to_system_level

    CSYNC

#ifdef BFIN_DEBUG_CONTEXT_LOAD
    R0      = 'D'
    .extern _diag_write_char
    CALL _diag_write_char
#endif
    

    P0.H = hal_thread_load_context_continue
    P0.L = hal_thread_load_context_continue

    RETI = P0

    RTI       ;  # return from interrupt and continue with loading the thread's context

    hal_bfin_switch_to_system_level:

//     # we are in BFIN user level and need to switch to BFIN system level

    CLI R5
    SSYNC
    R1.H = 0x0
    R1.L = 0x4000 ;  # allow INT 14 only
    STI R1
    SSYNC

   
#ifdef BFIN_DEBUG_CONTEXT_LOAD
    R0      = 'U'
    .extern _diag_write_char
    CALL _diag_write_char
#endif
    
    RAISE 14  ;  # this will only work for being in supervisor mode of the processor
//              # the processor should never fall back to user mode while running eCOS

//              # keep busy waiting here for interrupt 14 to be serviced
    hal_bfin_context_switch_infinite_loop:
            JUMP hal_bfin_context_switch_infinite_loop

    .globl hal_bfin_context_load_int14
    hal_bfin_context_load_int14:

    
    STI R5
    SSYNC

    
	
#------------------------------------------------------------------------------
# hal_thread_swsitch_load
# R1 = address of sp of next thread to execute
# Note that this function is also the second half of hal_thread_switch_context
# and is simply dropped into from it.
	
FUNC_START(hal_thread_load_context_continue)
FUNC_START(_hal_thread_load_context_continue)
#ifdef BFIN_DEBUG_CONTEXT_LOAD
    R7      = R0
    R0      = 'L'
    .extern _diag_write_char
    CALL _diag_write_char
    R0      = '\n'
    .extern _diag_write_char
    CALL _diag_write_char
    R0      = R7
#endif

    SP      += 0x8     ; // # skip icpld fault and dcpld fault

  ;  R0      = SP
    
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
    P0.L    = 0x2104     ;# skip IMASK


    (R7:0,P5:0) = [SP++] ;# multiple pop restoring general registers

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

    LT0     = [SP++]   ;# might incurr penalty if LC0!=0
    LT1     = [SP++]   ;# might incurr penalty if LC1!=0

    LB0     = [SP++]   ;# might incurr penalty if LC0!=0
    LB1     = [SP++]   ;# might incurr penalty if LC1!=0

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

    SP += 0x4;
    
    ASTAT   = [SP++]
    USP      = [SP++]
    
    
    FP       = [SP++]

    RTS                   ;# return from function call



#------------------------------------------------------------------------------
# HAL longjmp, setjmp implementations
# hal_setjmp saves only to callee save registers R7:4, P5:3, SP, FP, RETS, RETI, RETX, RETN, RETI
# into buffer supplied in a0[arg0]
# Note: These definitions are repeated in hal_arch.h. If changes are required
# remember to update both sets.

# do we need to store ALL RETx registers?

#define CYGARC_JMP_BUF_SP        0x0
#define CYGARC_JMP_BUF_FP        0x4
#define CYGARC_JMP_BUF_R7        0x8
#define CYGARC_JMP_BUF_R6        0xc
#define CYGARC_JMP_BUF_R5        0x10
#define CYGARC_JMP_BUF_R4        0x14
#define CYGARC_JMP_BUF_P5        0x18
#define CYGARC_JMP_BUF_P4        0x1c
#define CYGARC_JMP_BUF_P3        0x20
#define CYGARC_JMP_BUF_RETS      0x24
#define CYGARC_JMP_BUF_RETI      0x28
#define CYGARC_JMP_BUF_RETX      0x2c
#define CYGARC_JMP_BUF_RETN      0x30
#define CYGARC_JMP_BUF_RETE      0x34

#define CYGARC_JMP_BUF_SIZE     0x38


FUNC_START(hal_setjmp)
FUNC_START(_hal_setjmp)
        P0  = R0               ;# move pointer provided into P0 pointer register for indexing
        [P0+CYGARC_JMP_BUF_SP]   = SP
        [P0+CYGARC_JMP_BUF_FP]   = FP
        [P0+CYGARC_JMP_BUF_R7]   = R7
        [P0+CYGARC_JMP_BUF_R6]   = R6
        [P0+CYGARC_JMP_BUF_R5]   = R5
        [P0+CYGARC_JMP_BUF_R4]   = R4
        [P0+CYGARC_JMP_BUF_P5]   = P5
        [P0+CYGARC_JMP_BUF_P4]   = P4
        [P0+CYGARC_JMP_BUF_P3]   = P3
        FP = RETS
        [P0+CYGARC_JMP_BUF_RETS] = FP
        FP = RETI
        [P0+CYGARC_JMP_BUF_RETI] = FP
        FP = RETX
        [P0+CYGARC_JMP_BUF_RETX] = FP
        FP = RETN
        [P0+CYGARC_JMP_BUF_RETN] = FP
        FP = RETE
        [P0+CYGARC_JMP_BUF_RETE] = FP

        R0 = R0 ^ R0
        FP = [P0+CYGARC_JMP_BUF_FP]
        
        RTS


FUNC_START(hal_longjmp)
FUNC_START(_hal_longjmp)
        P0 = R0 ;# R0 as first parameter will contain the address where to get the information from
        SP = [P0+CYGARC_JMP_BUF_SP];# R1 as second parameter will contain the return value -> must be moved to R0

        R7 = [P0+CYGARC_JMP_BUF_R7]
        R6 = [P0+CYGARC_JMP_BUF_R6]
        R5 = [P0+CYGARC_JMP_BUF_R5]
        R4 = [P0+CYGARC_JMP_BUF_R4]
        P5 = [P0+CYGARC_JMP_BUF_P5]
        P4 = [P0+CYGARC_JMP_BUF_P4]
        P3 = [P0+CYGARC_JMP_BUF_P3]
        
        FP = [P0+CYGARC_JMP_BUF_RETS]
        RETS = FP
        FP = [P0+CYGARC_JMP_BUF_RETI]
        RETI = FP
        
        FP = [P0+CYGARC_JMP_BUF_RETX]
        RETX = FP
        
        FP = [P0+CYGARC_JMP_BUF_RETN]
        RETN = FP
        
        FP = [P0+CYGARC_JMP_BUF_RETE]
        RETE = FP
        
        FP = [P0+CYGARC_JMP_BUF_FP]
        
        R0 = R1;# return value into register

        RTS   ;# return
	
	
#------------------------------------------------------------------------------
# end of context.S
