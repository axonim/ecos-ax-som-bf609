#ifndef CYGONCE_HAL_HAL_ARCH_H
#define CYGONCE_HAL_HAL_ARCH_H

//==========================================================================
//
//      hal_arch.h
//
//      Architecture specific abstractions
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
// Date:         2013-08-11
// Purpose:      Define architecture abstractions
// Usage:        #include <cyg/hal/hal_arch.h>
//              
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef __ASSEMBLER__
#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/bfin-stub.h>
#include <cyg/hal/bfin-regs.h>

#include <cyg/hal/var_arch.h>

typedef struct 
{
    cyg_halint32    icpld_fault;     // ALU status register       */
    cyg_halint32    dcpld_fault;   // sequencer status register */
    // ALU status
    cyg_halint32	aw[2];     // 2 low 32bit values of accumulator registers		*/
    cyg_halint32	ax[2];     // 2 high 8bit values of accumulator registers sign extended	*/

    
// might require change of order to be able to push the value, due to prior indexing and storing in
// registers
    cyg_halint32	seqstat;   // sequencer status register	*/
    cyg_halint32    ipend;     // ipend register
    cyg_halint32    imask;     // imask register
	
    // These are common to all saved states
    cyg_halint32    p[6];      // 6 pointer registers       */
    cyg_halint32    r[8];      // 8 general purpose registers */

	// necessary to keep information about exception cause and so on */
		// r/o, not to be restored, save not really required	*/
    cyg_halint32	syscfg; 	// system configuration register	*/
    cyg_halint32	cycles; 	// cycles counter register	*/
    cyg_halint32	cycles2;	// cycles counter register	*/

    // these are the index registers
    cyg_halint32	i[4];		// 4 index registers		*/
    cyg_halint32	l[4];		// 4 length registers		*/
    cyg_halint32	b[4];		// 4 base registers		*/
    cyg_halint32	m[4];		// 4 modify registers		*/

    // these are loop registers to be saved
    // hardware loop, restore might incur penalty
    cyg_halint32	lt[2];		// 2 loop top registers		*/
    cyg_halint32	lb[2];		// 2 loop bottom registers	*/
    cyg_halint32	lc[2];		// 2 loop count registers	*/

    // return and system registers
    cyg_halint32	rets;		// return from subroutine register */
    cyg_halint32	reti;		// return from interrupt register  / program counter register of interrupted process */
    cyg_halint32	retx;		// return from exception register  */
    cyg_halint32	retn;		// return from NMI register	   */
    cyg_halint32	rete;		// return from emulation register  */
    cyg_halint32    pc;
    cyg_halint32    astat;     // ALU status register       */
    cyg_halint32    usp;        // old user mode stack pointer     */
 //   cyg_halint32    sp;         // old stack pointer     */
    cyg_halint32    fp;         // old frame pointer     */



} HAL_SavedRegisters;



//--------------------------------------------------------------------------
// Bit manipulation macros

externC cyg_uint32 hal_lsbit_index(cyg_uint32 mask);
externC cyg_uint32 hal_msbit_index(cyg_uint32 mask);

#define HAL_LSBIT_INDEX(index, mask) index = hal_lsbit_index(mask);

#define HAL_MSBIT_INDEX(index, mask) index = hal_msbit_index(mask);


//--------------------------------------------------------------------------
// Context switch macros.
// The arguments are pointers to locations where the stack pointer
// of the current thread is to be stored, and from where the sp of the
// next thread is to be fetched.
// 
// implementation in context.S

externC void hal_thread_switch_context( CYG_ADDRESS to, CYG_ADDRESS from );
externC void hal_thread_load_context( CYG_ADDRESS to )
    __attribute__ ((noreturn));

#define HAL_THREAD_SWITCH_CONTEXT(_fspptr_,_tspptr_)                    \
        hal_thread_switch_context( (CYG_ADDRESS)_tspptr_,               \
                                   (CYG_ADDRESS)_fspptr_);

#define HAL_THREAD_LOAD_CONTEXT(_tspptr_)                               \
        hal_thread_load_context( (CYG_ADDRESS)_tspptr_ );

externC void first_entry_to_thread(void * anything);
//--------------------------------------------------------------------------
// Context Initialization

// Initialize the context of a thread.
// Arguments:
// _sparg_ name of variable containing current sp, will be written with new sp
// _thread_ thread object address, passed as argument to entry point
// _entry_ entry point address.
// _id_ bit pattern used in initializing registers, for debugging.

#define HAL_THREAD_INIT_CONTEXT( _sparg_, _thread_, _entry_, _id_ ) { \
    register CYG_WORD _sp_ = ((CYG_WORD)_sparg_)-56;                   \
    register HAL_SavedRegisters *_regs_;                                                \
    int _i_;                                                                             \
    _sp_ = _sp_ & 0xFFFFFFF0;                                                           \
    _regs_ = (HAL_SavedRegisters *)(((_sp_) - sizeof(HAL_SavedRegisters))&0xFFFFFFF0);  \
    (_regs_)->aw[0] = (_id_);  (_regs_)->aw[1] = (_id_)|1;                              \
    (_regs_)->ax[0] = (_id_);  (_regs_)->ax[1] = (_id_)|1;                              \
    (_regs_)->r[6] = 0x800|1; (_regs_)->r[7] = (CYG_WORD)(_thread_);                               \
    for( _i_ = 0; _i_ < 6; _i_++ )  { (_regs_)->r[_i_] = 0x800|(7-_i_);                    \
                                      (_regs_)->p[_i_] = 0x200|(5-_i_); }                  \
    (_regs_)->astat = (0);  (_regs_)->seqstat = 0;                              \
    (_regs_)->syscfg = (0x36);                                                          \
    (_regs_)->cycles = (0);  (_regs_)->cycles2 = 0;                           \
    (_regs_)->ipend  = (0x8000);  (_regs_)->imask   = (0x7fff);                             \
    for( _i_ = 0; _i_ < 4; _i_++ )  { (_regs_)->i[_i_] = 0;                    \
                                      (_regs_)->b[_i_] = 0;                    \
                                      (_regs_)->m[_i_] = 0 ;                    \
                                      (_regs_)->l[_i_] = 0; }                  \
    (_regs_)->lt[0] = (0);  (_regs_)->lb[0] = (0); (_regs_)->lc[0] = 0;           \
    (_regs_)->lt[1] = (0);  (_regs_)->lb[1] = (0); (_regs_)->lc[1] = 0;           \
    (_regs_)->rets  = (CYG_WORD)(_entry_);                                                           \
    (_regs_)->retx  = (CYG_WORD)(_entry_);                                                           \
    (_regs_)->rete  = (CYG_WORD)(_entry_);                                                           \
    (_regs_)->retn  = (CYG_WORD)(_entry_);                                                           \
    (_regs_)->reti  = (CYG_WORD)(_entry_);                                                          \
    (_regs_)->pc  = (CYG_WORD)(first_entry_to_thread);                                                           \
    (_regs_)->fp    = _sp_;                                                     \
    (_regs_)->usp   = _sp_;                                                     \
    _sparg_ = (CYG_ADDRESS)_regs_;                                                      \
}
//*/

//#define HAL_THREAD_INIT_CONTEXT( _sparg_, _thread_, _entry_, _id_ ) _sparg_ = _id_;

//--------------------------------------------------------------------------
// Idle thread code.
// This macro is called in the idle thread loop, and gives the HAL the
// chance to insert code. Typical idle thread behaviour might be to halt the
// processor.

externC void hal_idle_thread_action(cyg_uint32 loop_count);

#define HAL_IDLE_THREAD_ACTION(_count_) hal_idle_thread_action(_count_)

//--------------------------------------------------------------------------
// Execution reorder barrier.
// When optimizing the compiler can reorder code. In multithreaded systems
// where the order of actions is vital, this can sometimes cause problems.
// This macro may be inserted into places where reordering should not happen.
// The "memory" keyword is potentially unnecessary, but it is harmless to
// keep it.

#define HAL_REORDER_BARRIER() asm volatile ( "" : : : "memory" )


//--------------------------------------------------------------------------
// Breakpoint support
// HAL_BREAKPOINT() is a code sequence that will cause a breakpoint to
// happen if executed.
// HAL_BREAKINST is the value of the breakpoint instruction and
// HAL_BREAKINST_SIZE is its size in bytes.
// HAL_BREAKINST_TYPE is the type.

#define HAL_BREAKPOINT(_label_)                 \
asm volatile (" .globl  " #_label_ ";"          \
              #_label_":"                       \
              " excpt 0x01"                      \
    );

#define HAL_BREAKINST           0x00a1

#define HAL_BREAKINST_SIZE      2

#define HAL_BREAKINST_TYPE      cyg_uint16

//--------------------------------------------------------------------------
// Exception handling function.
// This function is defined by the kernel according to this prototype. It is
// invoked from the HAL to deal with any CPU exceptions that the HAL does
// not want to deal with itself. It usually invokes the kernel's exception
// delivery mechanism.

externC void cyg_hal_deliver_exception( CYG_WORD code, CYG_ADDRWORD data );


//--------------------------------------------------------------------------
// Thread register state manipulation for GDB support.

// Default to a 32 bit register size for GDB register dumps.
#ifndef CYG_HAL_GDB_REG
#define CYG_HAL_GDB_REG CYG_WORD32
#endif

// Translate a stack pointer as saved by the thread context macros above into
// a pointer to a HAL_SavedRegisters structure.
#define HAL_THREAD_GET_SAVED_REGISTERS( _sp_, _regs_ )          \
        (_regs_) = (HAL_SavedRegisters *)(_sp_)



// Copy a set of registers from a HAL_SavedRegisters structure into a
// GDB ordered array.    
#define HAL_GET_GDB_REGISTERS( _aregval_ , _regs_ )             \
{                                                               \
    CYG_HAL_GDB_REG *_regval_ = (CYG_HAL_GDB_REG *)(_aregval_); \
                                                                \
                                                                \
    _regval_[REG_SYSCFG] = _regs_->syscfg;                      \
                                                              \
    /* due to multiple push, the registers are. */             \
    /* stored in reverse order for p and r regs */             \
    _regval_[REG_R0]     = _regs_->r[7];                      \
    _regval_[REG_R1]     = _regs_->r[6];                      \
    _regval_[REG_R2]     = _regs_->r[5];                      \
    _regval_[REG_R3]     = _regs_->r[4];                      \
    _regval_[REG_R4]     = _regs_->r[3];                      \
    _regval_[REG_R5]     = _regs_->r[2];                      \
    _regval_[REG_R6]     = _regs_->r[1];                      \
    _regval_[REG_R7]     = _regs_->r[0];                      \
                                                              \
    _regval_[REG_P0]     = _regs_->p[5];                      \
    _regval_[REG_P1]     = _regs_->p[4];                      \
    _regval_[REG_P2]     = _regs_->p[3];                      \
    _regval_[REG_P3]     = _regs_->p[2];                      \
    _regval_[REG_P4]     = _regs_->p[1];                      \
    _regval_[REG_P5]     = _regs_->p[0];                      \
                                                              \
    /* the stack pointer is not fully correct. */             \
    /* depending on cpu state it could also be */             \
    /* SP+4 or SP+12 */             \
    _regval_[REG_SP]     = _regs_ + sizeof(HAL_SavedRegisters);                      \
    _regval_[REG_FP]     = _regs_->fp;                        \
                                                              \
    _regval_[REG_I0]     = _regs_->i[0];                      \
    _regval_[REG_I1]     = _regs_->i[1];                      \
    _regval_[REG_I2]     = _regs_->i[2];                      \
    _regval_[REG_I3]     = _regs_->i[3];                      \
                                                              \
    _regval_[REG_M0]     = _regs_->m[0];                      \
    _regval_[REG_M1]     = _regs_->m[1];                      \
    _regval_[REG_M2]     = _regs_->m[2];                      \
    _regval_[REG_M3]     = _regs_->m[3];                      \
                                                              \
    _regval_[REG_L0]     = _regs_->l[0];                      \
    _regval_[REG_L1]     = _regs_->l[1];                      \
    _regval_[REG_L2]     = _regs_->l[2];                      \
    _regval_[REG_L3]     = _regs_->l[3];                      \
                                                              \
    _regval_[REG_B0]     = _regs_->b[0];                      \
    _regval_[REG_B1]     = _regs_->b[1];                      \
    _regval_[REG_B2]     = _regs_->b[2];                      \
    _regval_[REG_B3]     = _regs_->b[3];                      \
                                                              \
    _regval_[REG_A0X]     = _regs_->ax[0];                      \
    _regval_[REG_A0W]     = _regs_->aw[0];                      \
    _regval_[REG_A1X]     = _regs_->ax[1];                      \
    _regval_[REG_A1W]     = _regs_->aw[1];                      \
                                                              \
    _regval_[REG_LC0]     = _regs_->lc[0];                      \
    _regval_[REG_LC1]     = _regs_->lc[1];                      \
                                                              \
    _regval_[REG_LT0]     = _regs_->lt[0];                      \
    _regval_[REG_LT1]     = _regs_->lt[1];                      \
                                                              \
    _regval_[REG_LB0]     = _regs_->lb[0];                      \
    _regval_[REG_LB1]     = _regs_->lb[1];                      \
                                                              \
    _regval_[REG_ASTAT]     = _regs_->astat;                      \
                                                              \
    _regval_[REG_RES]     = 0;                      \
                                                              \
    _regval_[REG_RETS]     = _regs_->rets;                      \
                                                              \
    /* assuming GDB entry via excpetion only -> RETX */       \
    _regval_[REG_PC]     = _regs_->pc;                      \
                                                              \
    _regval_[REG_RETX]     = _regs_->retx;                      \
    _regval_[REG_RETN]     = _regs_->retn;                      \
    _regval_[REG_RETE]     = _regs_->rete;                      \
                                                              \
    _regval_[REG_SEQSTAT]  = _regs_->seqstat;                      \
    _regval_[REG_IPEND]    = _regs_->ipend;                      \
    _regval_[REG_ORIGPC]   = _regs_->retx;                      \
    _regval_[REG_EXTRA1]   = _regs_->reti;                      \
    _regval_[REG_EXTRA2]   = _regs_->cycles;                      \
    _regval_[REG_EXTRA3]   = _regs_->cycles2;                      \
}

// Copy a GDB ordered array into a HAL_SavedRegisters structure.
#define HAL_SET_GDB_REGISTERS( _regs_ , _aregval_ )             \
{                                                               \
    CYG_HAL_GDB_REG *_regval_ = (CYG_HAL_GDB_REG *)(_aregval_); \
    _regs_->syscfg = _regval_[REG_SYSCFG];                      \
                                                              \
    /* due to multiple push, the registers are. */             \
    /* stored in reverse order for p and r regs */             \
    _regs_->r[7] = _regval_[REG_R0];                      \
    _regval_[REG_R1]     = _regs_->r[6];                      \
    _regval_[REG_R2]     = _regs_->r[5];                      \
    _regval_[REG_R3]     = _regs_->r[4];                      \
    _regval_[REG_R4]     = _regs_->r[3];                      \
    _regval_[REG_R5]     = _regs_->r[2];                      \
    _regval_[REG_R6]     = _regs_->r[1];                      \
    _regval_[REG_R7]     = _regs_->r[0];                      \
                                                              \
    _regval_[REG_P0]     = _regs_->p[5];                      \
    _regval_[REG_P1]     = _regs_->p[4];                      \
    _regval_[REG_P2]     = _regs_->p[3];                      \
    _regval_[REG_P3]     = _regs_->p[2];                      \
    _regval_[REG_P4]     = _regs_->p[1];                      \
    _regval_[REG_P5]     = _regs_->p[0];                      \
                                                              \
    /* the stack pointer is not fully correct. */             \
    /* depending on cpu state it could also be */             \
    /* SP+4 or SP+12 */             \
    _regval_[REG_SP]     = _regs_ + sizeof(HAL_SavedRegisters);                      \
    _regval_[REG_FP]     = _regs_->fp;                        \
                                                              \
    _regval_[REG_I0]     = _regs_->i[0];                      \
    _regval_[REG_I1]     = _regs_->i[1];                      \
    _regval_[REG_I2]     = _regs_->i[2];                      \
    _regval_[REG_I3]     = _regs_->i[3];                      \
                                                              \
    _regval_[REG_M0]     = _regs_->m[0];                      \
    _regval_[REG_M1]     = _regs_->m[1];                      \
    _regval_[REG_M2]     = _regs_->m[2];                      \
    _regval_[REG_M3]     = _regs_->m[3];                      \
                                                              \
    _regval_[REG_L0]     = _regs_->l[0];                      \
    _regval_[REG_L1]     = _regs_->l[1];                      \
    _regval_[REG_L2]     = _regs_->l[2];                      \
    _regval_[REG_L3]     = _regs_->l[3];                      \
                                                              \
    _regval_[REG_B0]     = _regs_->b[0];                      \
    _regval_[REG_B1]     = _regs_->b[1];                      \
    _regval_[REG_B2]     = _regs_->b[2];                      \
    _regval_[REG_B3]     = _regs_->b[3];                      \
                                                              \
    _regval_[REG_A0X]     = _regs_->ax[0];                      \
    _regval_[REG_A0W]     = _regs_->aw[0];                      \
    _regval_[REG_A1X]     = _regs_->ax[1];                      \
    _regval_[REG_A1W]     = _regs_->aw[1];                      \
                                                              \
    _regval_[REG_LC0]     = _regs_->lc[0];                      \
    _regval_[REG_LC1]     = _regs_->lc[1];                      \
                                                              \
    _regval_[REG_LT0]     = _regs_->lt[0];                      \
    _regval_[REG_LT1]     = _regs_->lt[1];                      \
                                                              \
    _regval_[REG_LB0]     = _regs_->lb[0];                      \
    _regval_[REG_LB1]     = _regs_->lb[1];                      \
                                                              \
    _regval_[REG_ASTAT]     = _regs_->astat;                      \
                                                              \
    _regval_[REG_RES]     = 0;                      \
                                                              \
    _regval_[REG_RETS]     = _regs_->rets;                      \
                                                              \
    /* assuming GDB entry via excpetion only -> RETX */       \
    _regval_[REG_PC]     = _regs_->pc;                      \
                                                              \
    _regval_[REG_RETX]     = _regs_->retx;                      \
    _regval_[REG_RETN]     = _regs_->retn;                      \
    _regval_[REG_RETE]     = _regs_->rete;                      \
                                                              \
    _regval_[REG_SEQSTAT]  = _regs_->seqstat;                      \
    _regval_[REG_IPEND]    = _regs_->ipend;                      \
    _regval_[REG_ORIGPC]   = _regs_->retx;                      \
    _regval_[REG_EXTRA1]   = _regs_->reti;                      \
    _regval_[REG_EXTRA2]   = _regs_->cycles;                      \
    _regval_[REG_EXTRA3]   = _regs_->cycles2;                      \
}


//--------------------------------------------------------------------------
// HAL setjmp

#define CYGARC_JMP_BUF_SIZE 16  

typedef cyg_uint32 hal_jmp_buf[CYGARC_JMP_BUF_SIZE];

externC int hal_setjmp(hal_jmp_buf env);
externC void hal_longjmp(hal_jmp_buf env, int val);

//-------------------------------------------------------------------------
// Idle thread code.
// This macro is called in the idle thread loop, and gives the HAL the
// chance to insert code. Typical idle thread behaviour might be to halt the
// processor.

externC void hal_idle_thread_action(cyg_uint32 loop_count);

#define HAL_IDLE_THREAD_ACTION(_count_) hal_idle_thread_action(_count_)

//--------------------------------------------------------------------------
// Minimal and sensible stack sizes: the intention is that applications
// will use these to provide a stack size in the first instance prior to
// proper analysis.  Idle thread stack should be this big.

//    THESE ARE NOT INTENDED TO BE MICROMETRICALLY ACCURATE FIGURES.
//           THEY ARE HOWEVER ENOUGH TO START PROGRAMMING.
// YOU MUST MAKE YOUR STACKS LARGER IF YOU HAVE LARGE "AUTO" VARIABLES!

// This is not a config option because it should not be adjusted except
// under "enough rope" sort of disclaimers.

// Typical case stack frame size: return link + 4 pushed registers + some locals.
#define CYGNUM_HAL_STACK_FRAME_SIZE (48)

// Stack needed for a context switch:
#define CYGNUM_HAL_STACK_CONTEXT_SIZE ((52)*4)



// Interrupt + call to ISR, interrupt_end() and the DSR
#define CYGNUM_HAL_STACK_INTERRUPT_SIZE (4+2*CYGNUM_HAL_STACK_CONTEXT_SIZE) 

#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK

// An interrupt stack which is large enough for all possible interrupt
// conditions (and only used for that purpose) exists.  "User" stacks
// can be much smaller

#define CYGNUM_HAL_STACK_SIZE_MINIMUM (CYGNUM_HAL_STACK_CONTEXT_SIZE+      \
                                       CYGNUM_HAL_STACK_INTERRUPT_SIZE*2+  \
                                       CYGNUM_HAL_STACK_FRAME_SIZE*8)
#define CYGNUM_HAL_STACK_SIZE_TYPICAL (CYGNUM_HAL_STACK_SIZE_MINIMUM+1024)

#else // CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK 

// No separate interrupt stack exists.  Make sure all threads contain
// a stack sufficiently large.

#define CYGNUM_HAL_STACK_SIZE_MINIMUM (4096)
#define CYGNUM_HAL_STACK_SIZE_TYPICAL (4096)

#endif

#endif /* __ASSEMBLER__ */

// Convenience macros for accessing memory cached or uncached
//--------------------------------------------------------------------------
// Memory access macros

#define CYGARC_CACHED_ADDRESS(x)                       (x)
#define CYGARC_UNCACHED_ADDRESS(x)                     (x)
#define CYGARC_PHYSICAL_ADDRESS(x)                     (x)

// Macros for switching context between two eCos instances (jump from
// code in ROM to code in RAM or vice versa).

#define CYGARC_HAL_SAVE_GP()
#define CYGARC_HAL_RESTORE_GP()

/*
//--------------------------------------------------------------------------
// Macro for finding return address. 
#define CYGARC_HAL_GET_RETURN_ADDRESS(_x_, _dummy_) \
  asm volatile ( "move %0,$31;" : "=r" (_x_) )

#define CYGARC_HAL_GET_RETURN_ADDRESS_BACKUP(_dummy_)
*/
//--------------------------------------------------------------------------
#endif // CYGONCE_HAL_HAL_ARCH_H
// End of hal_arch.h
