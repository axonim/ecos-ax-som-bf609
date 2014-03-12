//==========================================================================
//
//      dev/KSZ9021.c
//
//      Ethernet transceiver (PHY) support for Micrel KSZ9021
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
// Author(s):    Ant Micro <www.antmicro.com>
// Contributors:
// Date:         2012-08-10
// Purpose
// 
// Based on KSZ8041.c:
// Author(s):    Uwe Kindler <uwe_kindler@web.de>
// Contributors: oli@snr.ch
//
// Description:  Support for ethernet PHY Micrel KSZ9021
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
#define PHY_CONTROL                             0x1f
#define PHY_CONTROL_SPEED_1000_MASK             (1 << 6)                
#define PHY_CONTROL_SPEED_100_MASK              (1 << 5)
#define PHY_CONTROL_SPEED_10_MASK               (1 << 4)
#define PHY_CONTROL_DUPLEX_MASK                 (1 << 3)
#define PHY_CONTROL_INTERRUPT_LEVEL_MASK        (1 << 14)

//==========================================================================
// Query the 100BASE-TX PHY Control Register and return a status bitmap
// indicating the state of the physical connection
//==========================================================================

#ifdef CYGDBG_DEVS_ETH_PHY
void
ksz9021_diag (eth_phy_access_t * f)
{

  cyg_uint32 i;
  cyg_uint16 reg;

  eth_phy_printf ("KSZ9021 PHY Register setings:\n");

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

void ksz9021_phy_set_advertise(eth_phy_access_t *f, cyg_uint16 link_speed)
{
	cyg_uint16 regval16;
	
	//Auto-negotiation setup
	_eth_phy_read (f, 4, f->phy_addr, &regval16);
	if (link_speed >= 100) {
		regval16 |= (1 << 8);	// advertise 100Mbps F 
		regval16 |= (1 << 7);	// advertise 100Mbps H 
	} else {
		regval16 &= ~(1 << 8);	// advertise 100Mbps F 
		regval16 &= ~(1 << 7);	// advertise 100Mbps H 
	}
	if (link_speed >= 10) {
		regval16 |= (1 << 6);	// advertise 10Mbps F 
		regval16 |= (1 << 5);	// advertise 10Mbps H 
	} else {
		regval16 &= ~(1 << 6);	// advertise 10Mbps F 
		regval16 &= ~(1 << 5);	// advertise 10Mbps H
	}
	_eth_phy_write (f, 4, f->phy_addr, regval16);
	
	/* 1000BASE-T control register */
	_eth_phy_read (f, 9, f->phy_addr, &regval16);
	if (link_speed == 1000) {
		regval16 |= (1 << 9);	/* advertise 1000Mbps F */
		regval16 |= (1 << 8);	/* advertise 1000Mbps H */
	} else {
		regval16 &= ~(1 << 9);	/* advertise 1000Mbps F */
		regval16 &= ~(1 << 8);	/* advertise 1000Mbps H */
	}
	_eth_phy_write (f, 9, f->phy_addr, regval16);
}

void ksz9021_phy_reset(eth_phy_access_t *f)
{
	cyg_uint16 regval16;
	cyg_uint16 seconds;
	//_eth_phy_read (f, PHY_BMSR, f->phy_addr, &phy_state)

	_eth_phy_read (f, 0, f->phy_addr, &regval16);

	regval16 |= 0x8000;

	_eth_phy_write (f, 0, f->phy_addr, regval16);

	seconds = 0;
	_eth_phy_read (f, 0, f->phy_addr, &regval16);
	while (regval16 & 0x8000) {
		CYGACC_CALL_IF_DELAY_US(1000000);
		seconds++;
		if (seconds > 2) { /* stalled if reset unfinished after 2 seconds */
			eth_phy_printf("Phy reset stalled \n");
			return;
		}
		_eth_phy_read (f, 0, f->phy_addr, &regval16);
	}
}

extern int ksz9021_phy_cfg(eth_phy_access_t *f, int mode)
{
	cyg_uint16 regval16;
	cyg_uint16 link_speed;
	
	int i = 0;
	
	// Auto-negotiation advertisment register

	_eth_phy_read(f, 4, f->phy_addr, &regval16);
	regval16 |= (1 << 11);	// asymetric pause
	regval16 |= (1 << 10);	// MAC pause implemented
	_eth_phy_write(f, 4, f->phy_addr, regval16);
	
	// set tx and rx pad delays
	_eth_phy_write(f, 260, f->phy_addr, 0xf0f0);
	_eth_phy_write(f, 261, f->phy_addr, 0);
	
	// control reg
	_eth_phy_read(f, 0, f->phy_addr, &regval16);
	//regval16 = zynq_eth_phy_read(ZYNQ_ETH_MDIO_ADDR, 0);
	regval16 |= (1 << 12); //auto-negotiation enable
	regval16 |= (1 << 8);  //enable full duplex
	_eth_phy_write(f, 4, f->phy_addr, regval16);

	//try to establish link with highest possible speed
	ksz9021_phy_set_advertise(f, 1000); 
	ksz9021_phy_reset(f);	
	//start auto-negotiation
	eth_phy_printf("Start Auto-negotiation\n");
	return _eth_phy_state(f);
}

static bool
ksz9021_stat (eth_phy_access_t * f, int *state)
{

  cyg_uint16 phy_state;
  cyg_uint16 phy_100ctrl_reg;
  cyg_uint32 tries;
  cyg_uint32 ms;

#ifdef  CYGDBG_DEVS_ETH_PHY
  ksz9021_diag (f);
#endif

  if (_eth_phy_read (f, PHY_BMSR, f->phy_addr, &phy_state)) {
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
        // Wait for 1 second
        //
        for (ms = 0; ms < 1000; ++ms) {
          CYGACC_CALL_IF_DELAY_US (1000);	// 1 ms
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
      
      _eth_phy_read (f, PHY_CONTROL, f->phy_addr, &phy_100ctrl_reg);

        if ( phy_100ctrl_reg & PHY_CONTROL_SPEED_1000_MASK )    /* 1000Mbps */
                *state |= ETH_PHY_STAT_1000MB;
        else if ( phy_100ctrl_reg & PHY_CONTROL_SPEED_100_MASK )/* 100Mbps */
                *state |= ETH_PHY_STAT_100MB;            //if speed is 10Mbps this bit stays 0

        //determine duplex
        if ( phy_100ctrl_reg & PHY_CONTROL_DUPLEX_MASK )
                *state |= ETH_PHY_STAT_FDX;
      
      return true;
    }
  }
  return false;
}

_eth_phy_dev ("Micrel KSZ9021", 0x00221611, ksz9021_stat)
