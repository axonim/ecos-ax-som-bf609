#ifndef CYGONCE_HAL_VAR_INTR_H
#define CYGONCE_HAL_VAR_INTR_H

//==========================================================================
//
//      var_intr.h
//
//      60x Interrupt and clock support
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
// Author(s):    Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:         2013-09-20
// Purpose:      BFIN 60x Interrupt support
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <pkgconf/hal_bfin.h>
#include <pkgconf/hal_bfin_bf60x.h>

#include <cyg/hal/plf_intr.h>
#include <cyg/hal/var_intr_incl.h>

#define HAL_INTERRUPT_MASK( _vector_ )     \
{                                          \
        unsigned int internal_vector;      \
        if (_vector_ > 15)                 \
        {                                  \
            internal_vector=_vector_ - 16; \
            asm volatile (                 \
            "CLI R0;"                      \
            "R1 = %0 << 0x03;"             \
            "R2.H = 0xFFCA;"               \
            "R2.L = 0x4800;"               \
            "R1 = R1 + R2;"                \
            "P0 = R1;"                     \
            "R2 = [P0];"                   \
            "R1 = 0x5;"                    \
            "R1 = ~R1;"                    \
            "R2 = R2 & R1;"                \
            "[P0] = R2;"                   \
            "STI R0;"                      \
            "SSYNC;"                       \
            :                              \
            : "r"(internal_vector)         \
            : "R0", "R1", "R2" ,"P0"       \
            );                             \
        }                                  \
        else {                             \
            asm volatile (                 \
            "CLI R1;" \
            "R0   = R0 ^ R0;"                  \
            "P0.H = _hal_intr_mask_applied;"   \
            "P0.L = _hal_intr_mask_applied;"   \
            "R2   = [P0];"                     \
            "R0   = 0x1;"                      \
            "R0   <<= %0;"                     \
            "R0   = ~R0;"                      \
            "R2   = R2 & R0;"                  \
            "R0.H = 0x0;"                      \
            "R0.L = 0x8000;"                   \
            "R1   = R1 & R0;"                  \
            "CC   = R1;"                       \
            "IF CC R1 = R2;"                   \
            "[P0]   = R2;"                     \
            "STI R1;"                          \
            "SSYNC;"                           \
            :                                  \
            : "r"(_vector_)                    \
            : "R0", "R1", "R2", "P0"           \
            );                                 \
        }                                      \
}



#define HAL_INTERRUPT_UNMASK( _vector_ ) \
{                                          \
        unsigned int internal_vector;      \
        if (_vector_ > 15)                 \
        {                                  \
            internal_vector=_vector_ - 16; \
            asm volatile (                 \
            "CLI R0;"                      \
            "R1 = %0 << 0x03;"             \
            "R2.H = 0xFFCA;"               \
            "R2.L = 0x4800;"               \
            "R1 = R1 + R2;"                \
            "P0 = R1;"                     \
            "R2 = [P0];"                   \
            "R1 = 0x5;"                    \
            "R2 = R2 | R1;"                \
            "[P0] = R2;"                   \
            "STI R0;"                      \
            "SSYNC;"                       \
            :                              \
            : "r"(internal_vector)         \
            : "R0", "R1", "R2", "P0"       \
            );                             \
        }                                  \
        else {                             \
        asm volatile (                     \
            "CLI R1;"                      \
            "R0   = R0 ^ R0;"                  \
            "P0.H = _hal_intr_mask_applied;"   \
            "P0.L = _hal_intr_mask_applied;"   \
            "R2   = [P0];"                     \
            "R0   = 0x1;"                      \
            "R0   <<= %0;"                     \ 
            "R2   = R2 | R0;"                  \
            "R0.H = 0x0;"                      \
            "R0.L = 0x8000;"                   \
            "R1   = R1 & R0;"                  \
            "CC   = R1;"                       \
            "IF CC R1 = R2;"                   \
            "[P0]   = R2;"                     \
            "STI R1;"                          \
            "SSYNC;"                           \
            :                                  \
            : "r"(_vector_)                    \
            : "R0", "R1", "R2", "P0"           \
            );                                 \
        }                                      \
}

#define HAL_INTERRUPT_ACKNOWLEDGE( _vector_ ) \
{                                      \
    unsigned int internal_vector;      \
    if (_vector_ > 15)                 \
    {                                  \
        internal_vector = _vector_ - 16;\
        asm volatile (                 \
        "P0.H = 0xFFCA;"               \
        "P0.L = 0x400C;"               \
        "[P0] = %0;"                   \
        "SSYNC;"                       \
        :                              \
        : "r"(internal_vector)         \
        : "R0", "P0"                   \
        );                             \
    }                                  \
}

#define HAL_INTERRUPT_CONFIGURE( _vector_, _level_, _up_ )

#define HAL_INTERRUPT_SET_LEVEL( _vector_, _level_ )

#define CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED
//--------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_VAR_INTR_H
// End of imp_intr.h
