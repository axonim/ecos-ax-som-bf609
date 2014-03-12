//==========================================================================
//
//      adsp_bf609ez_i2c_bitbang.c
//
//      I2c device definition for adsp_bf609ez board(s)
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
// Author(s):       Siarhei Vasiliuk, Aleksandr Loiko
// Date:            2013-10-04
// Purpose:         I2C bitbang device board support
// Description:     Instantiates the structures for I2C devices
//
//####DESCRIPTIONEND####
//
//========================================================================*/
#include <cyg/io/i2c.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_diag.h>
#include <pkgconf/hal.h>
#include <cyg/hal/adsp_bf609ez.h>
#include <cyg/io/i2c_bfin_bf60x.h>

static cyg_bool
hal_adsp_bf609ez_i2c_bitbang(cyg_i2c_bus* bus, cyg_i2c_bitbang_op op)
{
    cyg_bool result = 0;
    cyg_uint16 val16;
    cyg_uint32 base = TWI0_BASE;
    switch (op) {
        case CYG_I2C_BITBANG_INIT:
        {
            /* controller init */
            // enable TWI
            HAL_WRITE_UINT16(base + TWI_CTL, TWI_CTL_EN);
        } break;

        case CYG_I2C_BITBANG_SCL_HIGH:
        {
            HAL_READ_UINT16(base + TWI_MSTRCTL, val16);
            val16 &=~TWI_MSTRCTL_SCLOVER;
            HAL_WRITE_UINT16(base + TWI_MSTRCTL, val16);       
        } break;

        case CYG_I2C_BITBANG_SCL_LOW:
        {
            HAL_READ_UINT16(base + TWI_MSTRCTL, val16);
            val16 |=TWI_MSTRCTL_SCLOVER;
            HAL_WRITE_UINT16(base + TWI_MSTRCTL, val16);       
        } break;

        case CYG_I2C_BITBANG_SDA_HIGH:
        {
            HAL_READ_UINT16(base + TWI_MSTRCTL, val16);
            val16 &=~TWI_MSTRCTL_SDAOVER;
            HAL_WRITE_UINT16(base + TWI_MSTRCTL, val16);       
        } break;

        case CYG_I2C_BITBANG_SDA_LOW:
        {
            HAL_READ_UINT16(base + TWI_MSTRCTL, val16);
            val16 |=TWI_MSTRCTL_SDAOVER;
            HAL_WRITE_UINT16(base + TWI_MSTRCTL, val16);       
        } break;
        
        case CYG_I2C_BITBANG_SCL_HIGH_CLOCKSTRETCH:
        {
            HAL_READ_UINT16(base + TWI_MSTRCTL, val16);
            val16 &=~TWI_MSTRCTL_SCLOVER;
            HAL_WRITE_UINT16(base + TWI_MSTRCTL, val16);       

            /* wait here until clock is high */
           int i = 0;
            do {
                 HAL_READ_UINT16(base + TWI_MSTRSTAT, val16);
            }while ((val16 & TWI_MSTRSTAT_SCLSEN));    /* Pin Data Status */
        } break;

        case CYG_I2C_BITBANG_SDA_READ:
        {
            HAL_READ_UINT16(base + TWI_MSTRCTL, val16);
            val16 &=~TWI_MSTRCTL_SDAOVER;
            HAL_WRITE_UINT16(base + TWI_MSTRCTL, val16); 
            HAL_READ_UINT16(base + TWI_MSTRSTAT, val16);
            if (val16 & TWI_MSTRSTAT_SDASEN)
            {
                result = 0;
            } else {
                result = 1;
            }    
        } break;

        case CYG_I2C_BITBANG_SCL_LOW_SDA_INPUT:
        {
            HAL_READ_UINT16(base + TWI_MSTRCTL, val16);
            val16 |=TWI_MSTRCTL_SCLOVER;
            HAL_WRITE_UINT16(base + TWI_MSTRCTL, val16);  
            HAL_READ_UINT16(base + TWI_MSTRCTL, val16);
            val16 &=~TWI_MSTRCTL_SDAOVER;
            HAL_WRITE_UINT16(base + TWI_MSTRCTL, val16);  
        } break;

        default:
        {
        } break;
    }
    return result;
}


// /* instantiate the buses */
CYG_I2C_BITBANG_BUS(hal_adsp_bf609ez_i2c_bus, &hal_adsp_bf609ez_i2c_bitbang);

CYG_I2C_DEVICE(cyg_i2c_adsp_bf609ez_dev_mcp23017,
               &hal_adsp_bf609ez_i2c_bus,
               0x21,
               0x00,
               2000);

