#ifndef CYGONCE_HAL_PLF_IO_H
#define CYGONCE_HAL_PLF_IO_H

//=============================================================================
//
//      plf_io.h
//
//      platform specific IO support
//
//=============================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under    
// the terms of the GNU General Public License as published by the Free     
// Software Foundation; either version 2 or (at your option) any later      
// version.                                                                 
//
// eCos is distributed in the hope that it will be useful, but WITHOUT      
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    
// for more details.                                                        
//
// You should have received a copy of the GNU General Public License        
// along with eCos; if not, write to the Free Software Foundation, Inc.,    
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.            
//
// As a special exception, if other files instantiate templates or use      
// macros or inline functions from this file, or you compile this file      
// and link it with other works to produce a work based on this file,       
// this file does not by itself cause the resulting work to be covered by   
// the GNU General Public License. However the source code for this file    
// must still be made available in accordance with section (3) of the GNU   
// General Public License v2.                                               
//
// This exception does not invalidate any other reasons why a work based    
// on this file might be covered by the GNU General Public License.         
// -------------------------------------------                              
// ####ECOSGPLCOPYRIGHTEND####                                              
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    ysato
// Contributors: ysato
// Date:         2002-08-29
// Purpose:      H8MAX IO support
// Description:  The macros defined here provide the HAL APIs for handling
//               basic IO 
//              
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/system.h>
#include <cyg/hal/hal_endian.h>

//-----------------------------------------------------------------------------
// IDE interface macros
//
#define HAL_IDE_NUM_CONTROLLERS 1

// Initialize the IDE controller(s).
#define HAL_IDE_INIT() (HAL_IDE_NUM_CONTROLLERS)

#define IDE_CMD_ADDRESS 0x200000
#define IDE_CTL_ADDRESS 0x600000

#define HAL_IDE_READ_UINT8( __ctlr, __regno, __val )  \
    HAL_READ_UINT8(IDE_CMD_ADDRESS + ((__regno)<<1)+1 , (__val))
#define HAL_IDE_READ_UINT16( __ctlr, __regno, __val )  \
    hal_ide_read_uint16_swap(__regno,&(__val) )
#define HAL_IDE_READ_ALTSTATUS( __ctlr, __val )  \
    HAL_READ_UINT16(IDE_CTL_ADDRESS + 12, (__val))

#define HAL_IDE_WRITE_UINT8( __ctlr, __regno, __val )  \
    HAL_WRITE_UINT16(IDE_CMD_ADDRESS + ((__regno)<<1)+1 , (__val))
#define HAL_IDE_WRITE_UINT16( __ctlr, __regno, __val )  \
    hal_ide_write_uint16_swap(__regno, __val )
#define HAL_IDE_WRITE_CONTROL( __ctlr, __val )  \
    HAL_WRITE_UINT16(IDE_CTL_ADDRESS + 12, (__val))

static inline void hal_ide_read_uint16_swap(CYG_WORD16 __regno, CYG_WORD16 *__val )
{
    CYG_WORD16 d;
    d = *(volatile CYG_WORD16 *)(IDE_CMD_ADDRESS + (__regno <<1));
    *__val = CYG_LE16_TO_CPU(d);
}

static inline void hal_ide_write_uint16_swap(CYG_WORD16 __regno, CYG_WORD16 __val )
{
    *(volatile CYG_WORD16 *)(IDE_CMD_ADDRESS + (__regno <<1)) = CYG_CPU_TO_LE16(__val);
}
//-----------------------------------------------------------------------------
#endif
// end of plf_io.h
