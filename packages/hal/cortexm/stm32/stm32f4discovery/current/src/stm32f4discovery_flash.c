/*==========================================================================
//
//      stm32f4discovery_flash.c
//
//      Cortex-M4 STM32F4-Discovery Flash setup
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 2013 Free Software Foundation, Inc.                        
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
// Author(s):    jld
// Based on:     stm32x0g_eval flash setup by jlarmour
// Date:         2013-06-06
// Description:  
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <cyg/io/flash_dev.h>
#include <cyg/io/stm32_flash.h>

CYG_FLASH_DRIVER(hal_stm32_flash,
                 &cyg_stm32_flash_funs,
                 0,
                 0x08000000,
                 0,
                 0,
                 0,
                 &hal_stm32_flash_priv
);

//--------------------------------------------------------------------------
// EOF stm32f4discovery_flash.c
