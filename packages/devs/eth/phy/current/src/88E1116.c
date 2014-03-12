//==========================================================================
//
//      dev/88E1116.c
//
//      Ethernet transceiver (PHY) support for Marvell 88E1116
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 2008 Free Software Foundation, Inc.                        
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
// Author(s):    ITR-GmbH
// Contributors:
// Date:         2012-08-10
// Purpose
// 
// Description:  Support for ethernet PHY Marvell 88E1116
//
//
//####DESCRIPTIONEND####
//
//==========================================================================


//==========================================================================
//                                INCLUDES
//==========================================================================
#include <pkgconf/system.h>
#include <pkgconf/devs_eth_phy.h>

#include <cyg/infra/cyg_type.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/drv_api.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_tables.h>

#include <cyg/io/eth_phy.h>
#include <cyg/io/eth_phy_dev.h>


//==========================================================================
//                                DEFINES
//==========================================================================
// PHY Control Register
#define MII_MARVELL_PHY_PAGE        22

#define MII_M1011_IEVENT        0x13
#define MII_M1011_IEVENT_CLEAR        0x0000

#define MII_M1011_IMASK            0x12
#define MII_M1011_IMASK_INIT        0x6400
#define MII_M1011_IMASK_CLEAR        0x0000

#define MII_M1011_PHY_SCR        0x10
#define MII_M1011_PHY_SCR_AUTO_CROSS    0x0060

#define MII_M1145_PHY_EXT_CR        0x14
#define MII_M1145_RGMII_RX_DELAY    0x0080
#define MII_M1145_RGMII_TX_DELAY    0x0002

#define MII_M1111_PHY_LED_CONTROL    0x18
#define MII_M1111_PHY_LED_DIRECT    0x4100
#define MII_M1111_PHY_LED_COMBINE    0x411c
#define MII_M1111_PHY_EXT_CR        0x14
#define MII_M1111_RX_DELAY        0x80
#define MII_M1111_TX_DELAY        0x2
#define MII_M1111_PHY_EXT_SR        0x1b

#define MII_M1111_HWCFG_MODE_MASK        0xf
#define MII_M1111_HWCFG_MODE_COPPER_RGMII    0xb
#define MII_M1111_HWCFG_MODE_FIBER_RGMII    0x3
#define MII_M1111_HWCFG_MODE_SGMII_NO_CLK    0x4
#define MII_M1111_HWCFG_MODE_COPPER_RTBI    0x9
#define MII_M1111_HWCFG_FIBER_COPPER_AUTO    0x8000
#define MII_M1111_HWCFG_FIBER_COPPER_RES    0x2000

#define MII_M1111_COPPER        0
#define MII_M1111_FIBER            1

#define MII_88E1121_PHY_MSCR_PAGE    2
#define MII_88E1121_PHY_MSCR_REG    21
#define MII_88E1121_PHY_MSCR_RX_DELAY    BIT(5)
#define MII_88E1121_PHY_MSCR_TX_DELAY    BIT(4)
#define MII_88E1121_PHY_MSCR_DELAY_MASK    (~(0x3 << 4))

#define MII_88E1318S_PHY_MSCR1_REG    16
#define MII_88E1318S_PHY_MSCR1_PAD_ODD    BIT(6)

#define MII_88E1121_PHY_LED_CTRL    16
#define MII_88E1121_PHY_LED_PAGE    3
#define MII_88E1121_PHY_LED_DEF        0x0030

#define MII_M1011_PHY_STATUS        0x11
#define MII_M1011_PHY_STATUS_1000    0x8000
#define MII_M1011_PHY_STATUS_100    0x4000
#define MII_M1011_PHY_STATUS_SPD_MASK    0xc000
#define MII_M1011_PHY_STATUS_FULLDUPLEX    0x2000
#define MII_M1011_PHY_STATUS_RESOLVED    0x0800
#define MII_M1011_PHY_STATUS_LINK    0x0400

#define MII_M1116R_CONTROL_REG_MAC    21 

//==========================================================================
// Query the 100BASE-TX PHY Control Register and return a status bitmap
// indicating the state of the physical connection
//==========================================================================

#ifdef CYGDBG_DEVS_ETH_PHY
void
m88e1116_diag (eth_phy_access_t * f)
{

  cyg_uint32 i;
  cyg_uint16 reg;

  eth_phy_printf ("88E1116 PHY Register setings:\n");

  for (i = 0; i < 0x20; i++) {
    if (i % 2 == 0) {
      _eth_phy_read (f, i, f->phy_addr, &reg);
      eth_phy_printf ("r%02x: %04x ", i, reg);
    } else {
      _eth_phy_read (f, i, f->phy_addr, &reg);
      eth_phy_printf ("%04x\n", reg);
    }
  }
  /* Extended registers dump */
  for (i = 0x100; i < 0x108; i++) {
    if (i % 2 == 0) {
      _eth_phy_read (f, i, f->phy_addr, &reg);
      eth_phy_printf ("r%02x: %04x ", i, reg);
    } else {
      _eth_phy_read (f, i, f->phy_addr, &reg);
      eth_phy_printf ("%04x\n", reg);
    }
  }
  
}
#endif

void m88e1116_phy_reset(eth_phy_access_t *f)
{
    cyg_uint16 phy_reg;
    _eth_phy_read (f, 0, f->phy_addr, &phy_reg);
    phy_reg |= (0x8000);
    _eth_phy_write(f, 0, f->phy_addr, phy_reg);    
}

extern int m88e1116_phy_cfg(eth_phy_access_t *f, int mode)
{
    cyg_uint16 phy_reg;
  
#ifdef _SPEC_88E1116R_
    _eth_phy_read (f, PHY_BMCR, f->phy_addr, &phy_reg);
    phy_reg |= PHY_BMCR_RESET;
    _eth_phy_write(f, PHY_BMCR, f->phy_addr, phy_reg);

    CYGACC_CALL_IF_DELAY_US (500000);
  
    _eth_phy_write(f, MII_MARVELL_PHY_PAGE, f->phy_addr, 0);
  
    _eth_phy_read (f, MII_M1011_PHY_SCR, f->phy_addr, &phy_reg);
  
    phy_reg |= (7 << 12);       /* max number of gigabit attempts */
    phy_reg |= (1 << 11);       /* enable downshift */
    phy_reg |= MII_M1011_PHY_SCR_AUTO_CROSS;
    
    _eth_phy_write(f, MII_M1011_PHY_SCR, f->phy_addr, phy_reg);
    
    _eth_phy_write(f, MII_MARVELL_PHY_PAGE, f->phy_addr, 2);
    
    _eth_phy_read (f, MII_M1116R_CONTROL_REG_MAC, f->phy_addr, &phy_reg);
     
     phy_reg |= (1 << 5);
     phy_reg |= (1 << 4);
    
    _eth_phy_write(f, MII_M1116R_CONTROL_REG_MAC, f->phy_addr, phy_reg);
    
    _eth_phy_write(f, MII_MARVELL_PHY_PAGE, f->phy_addr, 0);
    
    _eth_phy_read (f, PHY_BMCR, f->phy_addr, &phy_reg);
    phy_reg |= PHY_BMCR_RESET;
    _eth_phy_write(f, PHY_BMCR, f->phy_addr, phy_reg);
    
    CYGACC_CALL_IF_DELAY_US (500000);
#else

    eth_phy_printf ("m88e1116_phy_cfg\n");

    _eth_phy_write(f, 22, f->phy_addr, 0);

    _eth_phy_read (f, 4, f->phy_addr, &phy_reg);
    phy_reg |= (1 << 11);
    phy_reg |= (1 << 10);
    _eth_phy_write(f, 4, f->phy_addr, phy_reg);


    _eth_phy_read (f, 16, f->phy_addr, &phy_reg);
    phy_reg |= (7 << 12);
    phy_reg |= (1 << 11);
    _eth_phy_write(f, 16, f->phy_addr, phy_reg);
    
    _eth_phy_write(f, 22, f->phy_addr, 2);
    
    _eth_phy_read (f, 21, f->phy_addr, &phy_reg);
    phy_reg |= (1 << 5);
    phy_reg |= (1 << 4);
    _eth_phy_write(f, 21, f->phy_addr, phy_reg);
    
    _eth_phy_write(f, 22, f->phy_addr, 0);
    
    _eth_phy_read (f, 0, f->phy_addr, &phy_reg);
    phy_reg |= (1 << 12);
    phy_reg |= (1 << 8);
    _eth_phy_write(f, 0, f->phy_addr, phy_reg);
    
    _eth_phy_read (f, 0, f->phy_addr, &phy_reg);
    phy_reg |= (0x8000);
    _eth_phy_write(f, 0, f->phy_addr, phy_reg);    

#endif
    
    return _eth_phy_state(f);
}

static bool
m88e1116_stat (eth_phy_access_t * f, int *state)
{

  cyg_uint16 phy_state;
  cyg_uint16 phy_100ctrl_reg;
  cyg_uint32 tries;
  cyg_uint32 ms;
  
  eth_phy_printf ("m88e1116_stat\n");

  if (_eth_phy_read (f, PHY_BMSR, f->phy_addr, &phy_state)) {
    eth_phy_printf ("m88e1116_stat, PHY_BMSR = 0x%X\n", phy_state);
    if ((phy_state & PHY_BMSR_AUTO_NEG) == 0) {

      eth_phy_printf ("... waiting for auto-negotiation\n");

      for (tries = 0; tries < CYGINT_DEVS_ETH_PHY_AUTO_NEGOTIATION_TIME;
           tries++) {
        
        if (_eth_phy_read (f, PHY_BMSR, f->phy_addr, &phy_state)) {
          if ((phy_state & PHY_BMSR_AUTO_NEG) != 0) {
            break;
          }
        } else {
          eth_phy_printf ("error: _eth_phy_read()\n");
        }
        
        //
        // Wait for 10 second
        //
        for (ms = 0; ms < 10000; ++ms) {
          CYGACC_CALL_IF_DELAY_US (1000);    // 1 ms
        }
        eth_phy_printf (".");
      }
      eth_phy_printf ("\n");
    }
    
    if ((phy_state & PHY_BMSR_AUTO_NEG) != 0) {
      *state = 0;
      if ((phy_state & PHY_BMSR_LINK) != 0) {
        *state |= ETH_PHY_STAT_LINK;
      }
      
      _eth_phy_read (f, MII_M1011_PHY_STATUS, f->phy_addr, &phy_100ctrl_reg);
      eth_phy_printf ("m88e1116_stat, MII_M1011_PHY_STATUS = 0x%X\n", phy_100ctrl_reg);
      
        //determine duplex
        if ( phy_100ctrl_reg & MII_M1011_PHY_STATUS_FULLDUPLEX )
                *state |= ETH_PHY_STAT_FDX;      
                
        phy_100ctrl_reg &= MII_M1011_PHY_STATUS_SPD_MASK;
      
        if ( phy_100ctrl_reg & MII_M1011_PHY_STATUS_1000 )    /* 1000Mbps */
                *state |= ETH_PHY_STAT_1000MB;
        else if ( phy_100ctrl_reg & MII_M1011_PHY_STATUS_100 )/* 100Mbps */
                *state |= ETH_PHY_STAT_100MB;            //if speed is 10Mbps this bit stays 0

         *state |= ETH_PHY_STAT_LINK;
      
      return true;
    }
  }
  return false;
}

_eth_phy_dev ("Marvell 88E1116", 0x01410e40, m88e1116_stat)
