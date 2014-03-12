#ifndef CYGONCE_DEVS_SPI_BF60X_H
#define CYGONCE_DEVS_SPI_BF60X_H

/** @file spi_bf60x.h
 *  @brief Analog Devices BF60x (Blackfin) SPI driver defines
 */

//==========================================================================
//
//      spi_bf60x.h
//
//      Analog Devices BF60x (Blackfin) SPI driver defines
//
//==========================================================================
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
// Author(s):     Aleksandr Loiko <aleksandr.loiko@axonim.by>
// Date:          2013-10-01
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <pkgconf/io_spi.h>

#include <cyg/infra/cyg_type.h>
#include <cyg/hal/drv_api.h>
#include <cyg/io/spi.h>

//-----------------------------------------------------------------------------
// ADIBF609 SPI BUS

/** 
 *  @brief: BF60x SPI bus structure 
*/
typedef struct cyg_spi_bf60x_bus_s
{
    // ---- Upper layer data ----

    cyg_spi_bus   spi_bus;                  /**< Upper layer SPI bus data */

    // ---- Lower layer data ----
     
    cyg_interrupt     spi_interrupt;        /**< SPI interrupt object */
    cyg_handle_t      spi_interrupt_handle; /**< SPI interrupt handle */
    cyg_drv_mutex_t   transfer_mx;          /**< Transfer mutex */
    cyg_drv_cond_t    transfer_cond;        /**< Transfer condition */
    cyg_bool          transfer_end;         /**< Transfer end flag */
    cyg_bool          cs_up;                /**< Chip Select up flag */
    cyg_vector_t      interrupt_number;     /**< SPI Interrupt Number */
    cyg_addrword_t    base;                 /**< Base Address of the SPI peripheral */
	cyg_uint32		  cs_mask;				/**< CS mask for Slave control register */
	cyg_uint8*		  tx_buf;               /**< Transmit buffer */
	cyg_uint8*		  rx_buf;               /**< Receive buffer */
	cyg_uint32*		  cnt;               	/**< Counter of bytes transfer */
    cyg_haladdress    dma_reg_base;    /**< Base address of DMA register block. */
    cyg_uint8         dma_tx_channel;  /**< TX DMA channel for this bus. */
    cyg_uint8         dma_rx_channel;  /**< RX DMA channel for this bus. */
    cyg_vector_t      dma_tx_intr;     /**< Interrupt used for DMA transmit. */
    cyg_vector_t      dma_rx_intr;     /**< Interrupt used for DMA receive. */
    cyg_bool          tx_dma_done;     /**< Flags used to signal completion. */
    cyg_bool          rx_dma_done;     /**< Flags used to signal completion. */
} cyg_spi_bf60x_bus_t;

//-----------------------------------------------------------------------------
// ADIBF609 SPI DEVICE

/** 
 *  @brief: BF60x SPI device structure 
*/
typedef struct cyg_spi_bf60x_device_s
{
    // ---- Upper layer data ----

    cyg_spi_device spi_device;  /**< Upper layer SPI device data */

    // ---- Lower layer data (configurable) ----

    cyg_uint8  dev_num;         /**< Device number */
    cyg_uint8  cl_pol;          /**< Clock polarity (0 - Active-high SPI CLK or 1 Active-low SPI CLK) */
    cyg_uint8  cl_pha;          /**< Clock phase    (0 - SPI CLK toggles from middle or 1 - SPI CLK toggles from start) */
    cyg_uint32 cl_brate;        /**< Clock baud rate */
    cyg_uint16 cs_up_udly;      /**< Delay in us between CS up and transfer start */
    cyg_uint16 cs_dw_udly;      /**< Delay in us between transfer end and CS down */
    cyg_uint16 tr_bt_udly;      /**< Delay in us between two transfers */

    // ---- Lower layer data (internal) ----

    cyg_bool   init;            /**< Is device initialized */
    cyg_uint32 cl_scbr;         /**< Value of SCBR (SPI clock) reg field */
    cyg_uint8  cl_div32;        /**< Divide SPI master clock by 32 */
} cyg_spi_bf60x_device_t;

//-----------------------------------------------------------------------------
// ADIBF609 SPI exported busses

#define SPI_CS_BITS_MASK		0xFE00

/* For backwards compatability  */
#define cyg_spi_bf60x_bus cyg_spi_bf60x_bus0

externC cyg_spi_bf60x_bus_t cyg_spi_bf60x_bus0;
externC cyg_spi_bf60x_bus_t cyg_spi_bf60x_bus1;

//-----------------------------------------------------------------------------

#endif // CYGONCE_DEVS_SPI_BF60X_H 

//-----------------------------------------------------------------------------
// End of spi_bf60x.h
