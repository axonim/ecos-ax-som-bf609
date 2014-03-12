/** @file spi_bf60x.c
 *  @brief Blackfin BF60x SPI driver
 */

//==========================================================================
//
//      spi_bf60x.c
//
//      Blackfin BF60x SPI driver
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
// Author(s):     Aleksandr Loiko <aleksandr.loiko@axonim.by>
// Date:          2013-09-20
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <pkgconf/io_spi.h>
#include <pkgconf/devs_spi_bfin_bf60x.h>

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_ass.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/hal/drv_api.h>
#include <cyg/io/spi.h>
#include <cyg/io/spi_bf60x.h>
#include <cyg/error/codes.h>

// -------------------------------------------------------------------------
static void spi_bf60x_init_bus(cyg_spi_bf60x_bus_t * bus);

static cyg_uint32 spi_bf60x_ISR(cyg_vector_t vector, cyg_addrword_t data);

static void spi_bf60x_DSR(cyg_vector_t   vector,
                         cyg_ucount32   count,
                         cyg_addrword_t data);

static void spi_bf60x_transaction_begin(cyg_spi_device *dev);

static void spi_bf60x_transaction_transfer(cyg_spi_device  *dev,
                                          cyg_bool         polled,
                                          cyg_uint32       count,
                                          const cyg_uint8 *tx_data,
                                          cyg_uint8       *rx_data,
                                          cyg_bool         drop_cs);

static void spi_bf60x_transaction_tick(cyg_spi_device *dev,
                                      cyg_bool        polled,
                                      cyg_uint32      count);

static void spi_bf60x_transaction_end(cyg_spi_device* dev);

static int spi_bf60x_get_config(cyg_spi_device *dev,
                               cyg_uint32      key,
                               void           *buf,
                               cyg_uint32     *len);

static int spi_bf60x_set_config(cyg_spi_device *dev,
                               cyg_uint32      key,
                               const void     *buf,
                               cyg_uint32     *len);

// -------------------------------------------------------------------------
// BF60x SPI BUS

#ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS0
cyg_spi_bf60x_bus_t cyg_spi_bf60x_bus0 = {
    .spi_bus.spi_transaction_begin    = spi_bf60x_transaction_begin,
    .spi_bus.spi_transaction_transfer = spi_bf60x_transaction_transfer,
    .spi_bus.spi_transaction_tick     = spi_bf60x_transaction_tick,
    .spi_bus.spi_transaction_end      = spi_bf60x_transaction_end,
    .spi_bus.spi_get_config           = spi_bf60x_get_config,
    .spi_bus.spi_set_config           = spi_bf60x_set_config,
    .interrupt_number                 = CYGNUM_HAL_INTERRUPT_SPI0,
    .base                             = SPI0_REGBASE,
	.cs_mask						  = SPI_CS_BITS_MASK	
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS1
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS1 << 9)
#endif
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS2
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS2 << 10)
#endif
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS3
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS3 << 11)
#endif
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS4
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS4 << 12)
#endif 
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS5
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS5 << 13)
#endif 
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS6
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS6 << 14)
#endif 
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS7
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS7 << 15)
#endif  
};
CYG_SPI_DEFINE_BUS_TABLE(cyg_spi_bf60x_device_t, 0);
#endif //ifdef CYGHWR_DEVS_SPI_BFIN_ADIBF609_BUS0

#ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS1
cyg_spi_bf60x_bus_t cyg_spi_bf60x_bus1 = {
    .spi_bus.spi_transaction_begin    = spi_bf60x_transaction_begin,
    .spi_bus.spi_transaction_transfer = spi_bf60x_transaction_transfer,
    .spi_bus.spi_transaction_tick     = spi_bf60x_transaction_tick,
    .spi_bus.spi_transaction_end      = spi_bf60x_transaction_end,
    .spi_bus.spi_get_config           = spi_bf60x_get_config,
    .spi_bus.spi_set_config           = spi_bf60x_set_config,
    .interrupt_number                 = CYGNUM_HAL_INTERRUPT_SPI1,
    .base                             = SPI1_REGBASE,
	.cs_mask						  = SPI_CS_BITS_MASK
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS1
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS1 << 9)
#endif
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS2
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS2 << 10)
#endif
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS3
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS3 << 11)
#endif
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS4
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS4 << 12)
#endif 
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS5
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS5 << 13)
#endif 
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS6
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS6 << 14)
#endif 
#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS7
		& ~(CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS7 << 15)
#endif  
};
CYG_SPI_DEFINE_BUS_TABLE(cyg_spi_bf60x_device_t, 1);
#endif //CYGHWR_DEVS_SPI_BFIN_ADIBF609_BUS1

/** @name SPI Status register bits
 @{ **/
#define SPI_STAT_TFF			0x800000	/**< SPI_TFIFO Full */
#define SPI_STAT_RFE			0x400000	/**< SPI_RFIFO Empty */
#define SPI_STAT_FCS			0x100000	/**< FCS */
#define SPI_STAT_TFS			0x70000		/**< SPI_TFIFO Status */
#define SPI_STAT_RFS			0x7000		/**< SPI_RFIFO Status */
#define SPI_STAT_TF				0x800		/**< Transmit Finish Indication */
#define SPI_STAT_RF				0x400		/**< Receive Finish Indication */
#define SPI_STAT_TS				0x200		/**< Transmit Start */
#define SPI_STAT_RS				0x100		/**< Receive Start */
#define SPI_STAT_MF				0x80		/**< Mode Fault Indication */
#define SPI_STAT_TC				0x40		/**< Transmit Collision Indication */
#define SPI_STAT_TUR			0x20		/**< Transmit Underrun Indication */
#define SPI_STAT_ROR			0x10		/**< Receive Overrun Indication */
#define SPI_STAT_TUWM			0x4			/**< Transmit Urgent Watermark Breached */
#define SPI_STAT_RUWM			0x2			/**< Receive Urgent Watermark Breached */
#define SPI_STAT_SPIF			0x1 		/**< SPI Finished */

#define SPI_STAT_TFS_0			0x0			/**< Full TFIFO */
#define SPI_STAT_TFS_1			0x10000		/**< 25% empty TFIFO */
#define SPI_STAT_TFS_2			0x20000		/**< 50% empty TFIFO */
#define SPI_STAT_TFS_3			0x30000 	/**< 75% empty TFIFO */
#define SPI_STAT_TFS_4			0x40000 	/**< Empty TFIFO */
/** @} */

/** @name Interrupt Mask Register bits
 @{ **/
#define SPI_IMSK_RUWM			0x2		/**< Receive Urgent Watermark Interrupt Mask */
#define SPI_IMSK_TUWM			0x4		/**< Transmit Urgent Watermark Interrupt Mask */
#define SPI_IMSK_ROR			0x10	/**< Receive Overrun Interrupt Mask */
#define SPI_IMSK_TUR			0x20	/**< Transmit Underrun Interrupt Mask */
#define SPI_IMSK_TC				0x40 	/**< Transmit Collision Interrupt Mask */
#define SPI_IMSK_MF				0x80	/**< Mode Fault Interrupt Mask */
#define SPI_IMSK_RS				0x100	/**< Receive Start Interrupt Mask */
#define SPI_IMSK_TS				0x200	/**< Transmit Start Interrupt Mask */
#define SPI_IMSK_RF				0x400	/**< Receive Finish Interrupt Mask */
#define SPI_IMSK_TF				0x800	/**< Transmit Finish Interrupt Mask */
/** @} */

/** @name SPI Control Register bits
 @{ **/
#define SPI_CTL_EN				0x1         /**< Enable */
#define SPI_CTL_MSTR			0x2         /**< Master/Slave */
#define SPI_CTL_PSSE			0x4         /**< Protected Slave Select Enable */
#define SPI_CTL_ODM				0x8         /**< Open Drain Mode */
#define SPI_CTL_CPHA			0x10        /**< Clock Phase */
#define SPI_CTL_CPOL			0x20        /**< Clock Polarity */
#define SPI_CTL_ASSEL			0x40        /**< Slave Select Pin Control */
#define SPI_CTL_SELST			0x80        /**< Slave Select Polarity Between Transfers */
#define SPI_CTL_EMISO			0x100       /**< Enable MISO */
#define SPI_CTL_SIZE			0x600       /**< Word Transfer Size */
	#define SPI_CTL_SIZE_8b		0x0         /**< 8-bit word */
	#define SPI_CTL_SIZE_16b	0x200       /**< 16-bit word */
	#define SPI_CTL_SIZE_32b	0x400       /**< 32-bit word */
#define SPI_CTL_LSBF			0x1000      /**< Least Significant Bit First */
#define SPI_CTL_FCEN			0x2000      /**< Flow Control Enable */
#define SPI_CTL_FCCH			0x4000      /**< Flow Control Channel Selection */
#define SPI_CTL_FCPL			0x8000      /**< Flow Control Polarity */
#define SPI_CTL_FCWM			0x30000     /**< Flow Control Watermark */
    #define SPI_CTL_FCWM_100    0x0         /**< TFIFO empty or RFIFO full */
	#define SPI_CTL_FCWM_75		0x10000     /**< TFIFO 75% or more empty, or RFIFO 75% or more full */
	#define SPI_CTL_FCWM_50		0x20000     /**< TFIFO 50% or more empty, or RFIFO 50% or more full */
#define SPI_CTL_FMODE			0x40000     /**< Fast-Mode Enable */
#define SPI_CTL_MIOM			0x300000    /**< Multiple I/O Mode */
	#define SPI_CTL_MIOM_NO		0x0         /**< No MIOM (disabled) */
	#define SPI_CTL_MIOM_DIOM	0x100000    /**< DIOM operation */
	#define SPI_CTL_MIOM_QIOM	0x200000    /**< QIOM operation */
#define SPI_CTL_SOSI			0x400000    /**< Start on MOSI */
/** @} */

/** @name SPI Transmit Control Register
 @{ **/
#define SPI_TXCTL_TUWM			0x70000		/**< Transmit FIFO Urgent Watermark */
	#define SPI_TXCTL_TUWM_25	0x10000		/**< 25% empty TFIFO */
	#define SPI_TXCTL_TUWM_50	0x20000		/**< 50% empty TFIFO */
	#define SPI_TXCTL_TUWM_75	0x30000		/**< 75% empty TFIFO */
	#define SPI_TXCTL_TUWM_0	0x40000		/**< Empty TFIFO */
#define SPI_TXCTL_TRWM			0x3000		/**< Transmit FIFO Regular Watermark */
#define SPI_TXCTL_TDU			0x100		/**< Transmit Data Under-run */
#define SPI_TXCTL_TDR			0x70		/**< Transmit Data Request */
#define SPI_TXCTL_TWCEN			0x8			/**< Transmit Word Counter Enable */
#define SPI_TXCTL_TTI			0x4			/**< Transmit Transfer Initiate */
#define SPI_TXCTL_TEN			0x1			/**< Transmit Enable */
/** @} */

/** @name SPI Receive Control Register
 @{ **/
#define SPI_RXCTL_RUWM			0x70000		/**< Receive FIFO Urgent Watermark */
#define SPI_RXCTL_RRWM			0x3000		/**< Receive FIFO Regular Watermark */
#define SPI_RXCTL_RDU			0x100		/**< Receive Data Overrun */
#define SPI_RXCTL_RDR			0x70		/**< Receive Data Request */
#define SPI_RXCTL_RWCEN			0x8			/**< Receive Word Counter Enable */
#define SPI_RXCTL_RTI			0x4			/**< Receive Transfer Initiate */
#define SPI_RXCTL_REN			0x1			/**< Receive Enable */
/** @} */

// A buffer of 64 zeros for dummy transmits
static const cyg_uint32 zeros[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

// -------------------------------------------------------------------------

// If C constructor with init priority functionality is not in compiler,
// rely on spi_bf60x_init.cxx to init us.
//#ifndef CYGBLD_ATTRIB_C_INIT_PRI
//# define CYGBLD_ATTRIB_C_INIT_PRI(x)
//#endif

//void CYGBLD_ATTRIB_C_INIT_PRI(CYG_INIT_BUS_SPI)
void cyg_spi_bf60x_bus_init(void)
{

#ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS0
   spi_bf60x_init_bus(&cyg_spi_bf60x_bus0);
#endif //ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS0

#ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS1
   spi_bf60x_init_bus(&cyg_spi_bf60x_bus1);
#endif //ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS1

}

// -------------------------------------------------------------------------
/****************************************************************************/
/**
*
* @brief    BF60x SPI bus initialization function from eCOS
*
* @return   none
*
*****************************************************************************/
static void spi_bf60x_init_bus(cyg_spi_bf60x_bus_t * spi_bus)
{
    // Create and attach SPI interrupt object
    cyg_drv_interrupt_create(spi_bus->interrupt_number,
                             4,
                             (cyg_addrword_t)spi_bus,
                             spi_bf60x_ISR,
                             spi_bf60x_DSR,
                             &spi_bus->spi_interrupt_handle,
                             &spi_bus->spi_interrupt);

	cyg_drv_interrupt_attach(spi_bus->spi_interrupt_handle);
	
	// Unmask the SPI int
	cyg_drv_interrupt_unmask(spi_bus->interrupt_number);
	
    // Init transfer mutex and condition
    cyg_drv_mutex_init(&spi_bus->transfer_mx);
    cyg_drv_cond_init(&spi_bus->transfer_cond,
                      &spi_bus->transfer_mx);

    // Init flags
    spi_bus->transfer_end = true;
    spi_bus->cs_up        = false;
	
	// Write to the SPI_SLVSEL register, setting one or more of the SPI select enablebits. This ensures that the desired slaves are properly deselected while the master is configured. 
	HAL_WRITE_UINT32(spi_bus->base+SPI_SLVSEL, SPI_CS_BITS_MASK);
	asm volatile ("SSYNC;");
	
    // Call upper layer bus init
    CYG_SPI_BUS_COMMON_INIT(&spi_bus->spi_bus);
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus interrupt service routine
*
* @param    vector     - Number of ISR
* @param    data       - Pointer to data structure (with driver handle)
*
* @return   ISR return condition
*
*****************************************************************************/
static cyg_uint32
spi_bf60x_ISR(cyg_vector_t vector, cyg_addrword_t data)
{
    cyg_uint32 stat;
	cyg_uint32 val;
    cyg_spi_bf60x_bus_t * spi_bus = (cyg_spi_bf60x_bus_t *)data;
    // Read the status register and disable
    // the SPI int events that have occurred

    HAL_READ_UINT32(spi_bus->base+SPI_STAT, stat);
    HAL_WRITE_UINT32(spi_bus->base+SPI_IMSK_CLR, stat);
	asm volatile ("SSYNC;");

	cyg_drv_interrupt_acknowledge(vector);

    return CYG_ISR_CALL_DSR;
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus interrupt service routine
*
* @param    vector     - Number of ISR
* @patam    count      - Unused parameter
* @param    data       - Pointer to data structure (with driver handle)
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_DSR(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data)
{
    cyg_spi_bf60x_bus_t *spi_bus = (cyg_spi_bf60x_bus_t *) data;
    cyg_uint32 stat;
	cyg_uint32 val;
	
    // Read the status register and
    // check for transfer completion

    HAL_READ_UINT32(spi_bus->base+SPI_STAT, stat);
	
	if(stat & SPI_IMSK_TUWM)
    {	
		cyg_uint32 a = *spi_bus->cnt;
		if (!(*spi_bus->cnt))
		{
			// Transfer ended
			spi_bus->transfer_end = true;
			cyg_drv_cond_signal(&spi_bus->transfer_cond);
		}
		else
		{
			if (NULL != spi_bus->tx_buf)
			{
				val = *spi_bus->tx_buf++;
			}
			else
			{
				val = 0;
			}
			
			HAL_WRITE_UINT32(spi_bus->base+SPI_TFIFO, val);
			asm volatile ("SSYNC;");
			(*(spi_bus->cnt))--;

			HAL_READ_UINT32(spi_bus->base+SPI_RFIFO, val);

			if (NULL != spi_bus->rx_buf)
			{
				*spi_bus->rx_buf++ = val;
			}
			
			//Enable interrupt for continuing transfer
			HAL_WRITE_UINT32(spi_bus->base+SPI_IMSK_SET, stat);
			asm volatile ("SSYNC;");			
		}
    }
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus baud rate calculator function
*
* @param    dev     - SPI device handle
*
* @return   none
*
*****************************************************************************/
static cyg_bool
spi_bf60x_calc_scbr(cyg_spi_bf60x_device_t *dev)
{
    cyg_uint32 scbr;
    cyg_bool   res = true;

    // Calculate SCBR from baud rate
	scbr = CYGNUM_HAL_BFIN_SYSTEM_CLOCK * 1000000 / dev->cl_brate - 1;
	
	dev->cl_scbr  = scbr;

    return res;
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus baud rate calculator function
*
* @param    spi_bus -  SPI bus pointer
* @param    val     -  <strong>1</strong>-CS is Low, <strong>0</strong>-CS is High.
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_set_npcs(cyg_spi_bf60x_bus_t *spi_bus, volatile cyg_uint32 val)
{
	cyg_uint32 ctr;
	
	asm volatile ("nop");	//dirty fix for anomaly connected with data load from FP
	
	//check correct CS according to cs_mask
	if (!((~val) & (spi_bus->cs_mask))) {
	
		ctr = (val & SPI_CS_BITS_MASK) | (((~val) & SPI_CS_BITS_MASK) >> 8);
			
		HAL_WRITE_UINT32(spi_bus->base+SPI_SLVSEL, ctr);
		asm volatile ("SSYNC;");
	
	}
      
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus start transfer function
*
* @param    dev     - SPI device handle
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_start_transfer(cyg_spi_bf60x_device_t *dev)
{
    cyg_spi_bf60x_bus_t *spi_bus = (cyg_spi_bf60x_bus_t *)dev->spi_device.spi_bus;

    if (spi_bus->cs_up)
        return;

    // Force minimal delay between two transfers - in case two transfers
    // follow each other w/o delay, then we have to wait here in order for
    // the peripheral device to detect cs transition from inactive to active.
    CYGACC_CALL_IF_DELAY_US(dev->tr_bt_udly);

    // Raise CS

    spi_bf60x_set_npcs(spi_bus,~(1<<(dev->dev_num + 8)));

    CYGACC_CALL_IF_DELAY_US(dev->cs_up_udly);

    spi_bus->cs_up = true;
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus drop CS function
*
* @param    dev     - SPI device handle
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_drop_cs(cyg_spi_bf60x_device_t *dev)
{
    cyg_spi_bf60x_bus_t *spi_bus = (cyg_spi_bf60x_bus_t *)dev->spi_device.spi_bus;

    if (!spi_bus->cs_up)
       return;

    // Drop CS

    CYGACC_CALL_IF_DELAY_US(dev->cs_dw_udly);
    spi_bf60x_set_npcs(spi_bus, SPI_CS_BITS_MASK);
    spi_bus->cs_up = false;
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus transfer with IRQ function. 
*              
* @param    dev          - SPI device handle
* @param    count        - Number of bytes to transmit.
* @param    tx_data      - Pointer to TX buffer.
* @param    rx_data      - Pointer to RX buffer.
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_transfer(cyg_spi_bf60x_device_t *dev,
                  cyg_uint32             count,
                  const cyg_uint8       *tx_data,
                  cyg_uint8             *rx_data)
{
	cyg_uint32 val;
	cyg_spi_bf60x_bus_t *spi_bus = (cyg_spi_bf60x_bus_t *)dev->spi_device.spi_bus;
	
    cyg_drv_mutex_lock(&spi_bus->transfer_mx);
		
    // Send next byte over the wire
	spi_bus->tx_buf = tx_data;
	spi_bus->rx_buf = rx_data;
	spi_bus->cnt = &count;
        
    spi_bus->transfer_end = false;
		
	// Enable the SPI int events we are interested in
	HAL_WRITE_UINT32(spi_bus->base+SPI_IMSK_SET, SPI_IMSK_TUWM);
	asm volatile ("SSYNC;");
        
	// Wait for its completion
    cyg_drv_dsr_lock();
            
    while (!spi_bus->transfer_end){
        cyg_drv_cond_wait(&spi_bus->transfer_cond);
	}
            
    cyg_drv_dsr_unlock();
		
	cyg_drv_mutex_unlock(&spi_bus->transfer_mx);
	
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus transfer function in poll mode without IRQ. 
*
* @param    dev          - SPI bus handle
* @param    count        - Number of bytes to transmit.
* @param    tx_data      - Pointer to TX buffer.
* @param    rx_data      - Pointer to RX buffer.
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_transfer_polled(cyg_spi_bf60x_device_t *dev,
                         cyg_uint32             count,
                         const cyg_uint8       *tx_data,
                         cyg_uint8             *rx_data)
{
    cyg_uint32 val;
    cyg_spi_bf60x_bus_t *spi_bus = (cyg_spi_bf60x_bus_t *)dev->spi_device.spi_bus;

    // Transmit and receive byte by byte
    while (count-- > 0)
    {
        // Wait for transmit data register empty
        do
        {
            HAL_READ_UINT32(spi_bus->base+SPI_STAT, val);
        } while ((val & SPI_STAT_TFF));

        // Send next byte over the wire
        if (NULL != tx_data)
            val = *tx_data++;
        else
            val = 0;
        HAL_WRITE_UINT32(spi_bus->base+SPI_TFIFO, val);
		asm volatile ("SSYNC;");

        // Wait for receive data register full
        do
        {
            HAL_READ_UINT32(spi_bus->base+SPI_STAT, val);
        } while ((val & SPI_STAT_RFE) );

        // Store received byte
        HAL_READ_UINT32(spi_bus->base+SPI_RFIFO, val);
        if (NULL != rx_data)
            *rx_data++ = (cyg_uint8)val;
    }
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus begin transaction function. 
*
* @param    dev          - SPI device handle
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_transaction_begin(cyg_spi_device *dev)
{
    cyg_spi_bf60x_device_t *bf60x_spi_dev = (cyg_spi_bf60x_device_t *) dev;
    cyg_spi_bf60x_bus_t *spi_bus =
      (cyg_spi_bf60x_bus_t *)bf60x_spi_dev->spi_device.spi_bus;
    cyg_uint32 val;

    if (!bf60x_spi_dev->init)
    {
        bf60x_spi_dev->init = true;
        spi_bf60x_calc_scbr(bf60x_spi_dev);
    }
	
	HAL_WRITE_UINT32(spi_bus->base+SPI_CLK, bf60x_spi_dev->cl_scbr);
	asm volatile ("SSYNC;");
		
	val = 0;
	HAL_WRITE_UINT32(spi_bus->base+SPI_CTL, val);
	asm volatile ("SSYNC;");
		
	val |= (SPI_CTL_MSTR | SPI_CTL_EN);
	
    if (bf60x_spi_dev->cl_pol)
        val |= SPI_CTL_CPOL;

    if (bf60x_spi_dev->cl_pha)
        val |= SPI_CTL_CPHA;
		
    HAL_WRITE_UINT32(spi_bus->base+SPI_CLK, bf60x_spi_dev->cl_scbr);
	asm volatile ("SSYNC;");
		
	// Enable the SPI controller
	HAL_WRITE_UINT32(spi_bus->base+SPI_RXCTL, SPI_RXCTL_REN);
	asm volatile ("SSYNC;");
	
	HAL_WRITE_UINT32(spi_bus->base+SPI_TXCTL, SPI_TXCTL_TEN | SPI_TXCTL_TTI | SPI_TXCTL_TUWM_0);
	asm volatile ("SSYNC;");
		
    HAL_WRITE_UINT32(spi_bus->base+SPI_CTL, val);
	asm volatile ("SSYNC;");
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus transaction transfer function. 
*
* @param    dev          - SPI device handle
* @param    polled       - Poll mode flash: 1-Polled, 0-IRQ
* @param    count        - Number of bytes to transmit.
* @param    tx_data      - Pointer to TX buffer.
* @param    rx_data      - Pointer to RX buffer.
* @param    drop_cs      - Flag to drop chip select (1 - deselect device)
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_transaction_transfer(cyg_spi_device  *dev,
                              cyg_bool         polled,
                              cyg_uint32       count,
                              const cyg_uint8 *tx_data,
                              cyg_uint8       *rx_data,
                              cyg_bool         drop_cs)
{
    cyg_spi_bf60x_device_t *bf60x_spi_dev = (cyg_spi_bf60x_device_t *) dev;

    // Select the device if not already selected
    spi_bf60x_start_transfer(bf60x_spi_dev);

    // Perform the transfer
    if (polled)
        spi_bf60x_transfer_polled(bf60x_spi_dev, count, tx_data, rx_data);
    else
        spi_bf60x_transfer(bf60x_spi_dev, count, tx_data, rx_data);

    // Deselect the device if requested
    if (drop_cs)
        spi_bf60x_drop_cs(bf60x_spi_dev);
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus adding zeros function. 
*
* @param    dev          - SPI device handle
* @param    polled       - Poll mode flash: 1-Polled, 0-IRQ
* @param    count        - Number of bytes to transmit (Number of Ticks = count * PerByteClocks).
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_transaction_tick(cyg_spi_device *dev,
                          cyg_bool        polled,
                          cyg_uint32      count)
{
    cyg_spi_bf60x_device_t *bf60x_spi_dev = (cyg_spi_bf60x_device_t *) dev;

    // Transfer count zeros to the device - we don't touch the
    // chip select, the device could be selected or deselected.
    // It is up to the device driver to decide in which state the
    // device will be ticked.

    while (count > 0)
    {
        int tcnt = count > sizeof(zeros) ? sizeof(zeros) : count;

        if (polled)
            spi_bf60x_transfer_polled(bf60x_spi_dev, tcnt,
                                     (const cyg_uint8 *) zeros, NULL);
        else
            spi_bf60x_transfer(bf60x_spi_dev, tcnt,
                              (const cyg_uint8 *) zeros, NULL);

        count -= tcnt;
    }
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus finalize transaction function. 
*
* @param    dev          - SPI device handle
*
* @return   none
*
*****************************************************************************/
static void
spi_bf60x_transaction_end(cyg_spi_device* dev)
{
    cyg_spi_bf60x_device_t * bf60x_spi_dev = (cyg_spi_bf60x_device_t *)dev;
    cyg_spi_bf60x_bus_t *spi_bus =
      (cyg_spi_bf60x_bus_t *)bf60x_spi_dev->spi_device.spi_bus;

    // Disable the SPI controller
    HAL_WRITE_UINT32(spi_bus->base+SPI_CTL, 0);
	asm volatile ("SSYNC;");

    spi_bf60x_drop_cs((cyg_spi_bf60x_device_t *) dev);
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus get config function. 
*
* @param    dev          - SPI device handle
* @param    key          - IO control number of key
* @param    buf          - Pointer to BUF
* @param    len          - Pointer to length
*
* @return   Status of operation: ENOERR - all good, -EINVAL - fail to find IO control number
*
*****************************************************************************/
static int
spi_bf60x_get_config(cyg_spi_device *dev,
                    cyg_uint32      key,
                    void            *buf,
                    cyg_uint32      *len)
{
    cyg_spi_bf60x_device_t *bf60x_spi_dev = (cyg_spi_bf60x_device_t *) dev;

    switch (key)
    {
        case CYG_IO_GET_CONFIG_SPI_CLOCKRATE:
        {
            if (*len != sizeof(cyg_uint32))
                return -EINVAL;
            else
            {
                cyg_uint32 *cl_brate = (cyg_uint32 *)buf;
                *cl_brate = bf60x_spi_dev->cl_brate;
            }
        }
            break;
        default:
            return -EINVAL;
    }
    return ENOERR;
}

/****************************************************************************/
/**
*
* @brief    BF60x SPI bus set config function. 
*
* @param    dev          - SPI device handle
* @param    key          - IO control number of key
* @param    buf          - Pointer to BUF
* @param    len          - Pointer to length
*
* @return   Status of operation: ENOERR - all good, -EINVAL - fail to find IO control number
*
*****************************************************************************/
static int
spi_bf60x_set_config(cyg_spi_device *dev,
                    cyg_uint32      key,
                    const void      *buf,
                    cyg_uint32      *len)
{
    cyg_spi_bf60x_device_t *bf60x_spi_dev = (cyg_spi_bf60x_device_t *) dev;

    switch (key)
    {
        case CYG_IO_SET_CONFIG_SPI_CLOCKRATE:
        {
            if (*len != sizeof(cyg_uint32))
                return -EINVAL;
            else
            {
                cyg_uint32 old_cl_brate = bf60x_spi_dev->cl_brate;
				cyg_uint32 cl_brate     = *((cyg_uint32 *)buf);
                
                bf60x_spi_dev->cl_brate = cl_brate;

                if (!spi_bf60x_calc_scbr(bf60x_spi_dev))
                {
                    bf60x_spi_dev->cl_brate = old_cl_brate;
                    spi_bf60x_calc_scbr(bf60x_spi_dev);
                    return -EINVAL;
                }
            }
        }
        break;
        default:
            return -EINVAL;
    }
    return ENOERR;
}

// -------------------------------------------------------------------------
// EOF spi_bf60x.c
