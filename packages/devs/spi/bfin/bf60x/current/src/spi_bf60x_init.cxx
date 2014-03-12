/** @file spi_bf60x_init.cxx
 *  @brief Blackfin BF60x SPI driver
 */
//==========================================================================
//
//      spi_bf60x_init.cxx
//
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2009 Free Software Foundation, Inc.
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
// Author(s):     
// Date:          2013-08-23
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <cyg/infra/cyg_type.h>

// This file is not needed if we support CYGBLD_ATTRIB_C_INIT_PRI, as the
// init happens directly in spi_bf60x.c then.
//#ifndef CYGBLD_ATTRIB_C_INIT_PRI

// -------------------------------------------------------------------------

externC void cyg_spi_bf60x_bus_init(void);

class cyg_spi_bf60x_bus_init_class {
public:
    cyg_spi_bf60x_bus_init_class(void) {
        cyg_spi_bf60x_bus_init();
    }
};

// -------------------------------------------------------------------------

static cyg_spi_bf60x_bus_init_class spi_bf60x_bus_init CYGBLD_ATTRIB_INIT_PRI(CYG_INIT_BUS_SPI);

//#endif // ifndef CYGBLD_ATTRIB_C_INIT_PRI

// -------------------------------------------------------------------------