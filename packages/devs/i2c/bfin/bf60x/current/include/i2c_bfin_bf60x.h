#ifndef CYGONCE_DEVS_I2C_BFIN_BF60X_H
#define CYGONCE_DEVS_I2C_BFIN_BF60X_H

/** @file i2c_bfin_bf60x.h
 *  @brief Analog Devices BF60x (Blackfin) I2C driver defines
*/

//==========================================================================
//
//      i2c_bfin_bf60x.h
//
//      Analog Devices BF60x (Blackfin) I2C driver defines
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
// Author(s):     Pavel Azizov <pavel.azizov@axonim.by>
//			
// Date:          2013-08-23
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/drv_api.h>
#include <cyg/io/i2c.h>

/** @name TWI Controller Registers
 @{ **/
#define TWI_CLKDIV                  0x00                /**< TWI SCL Clock Divider */
#define TWI_CTL                     0x04                /**< TWI Control Register */
#define TWI_SLVCTL                  0x08                /**< TWI Slave Mode Control Register */
#define TWI_SLVSTAT                 0x0C                /**< TWI Slave Mode Status Register */
#define TWI_SLVADDR                 0x10                /**< TWI Slave Mode Address Register */
#define TWI_MSTRCTL                 0x14                /**< TWI Master Mode Control Register */
#define TWI_MSTRSTAT                0x18                /**< TWI Master Mode Status Register */
#define TWI_MSTRADDR                0x1C                /**< TWI Master Mode Address Register */
#define TWI_ISTAT                   0x20                /**< TWI Interrupt Status Register */
#define TWI_IMSK                    0x24                /**< TWI Interrupt Mask Register */
#define TWI_FIFOCTL                 0x28                /**< TWI FIFO Control Register */
#define TWI_FIFOSTAT                0x2C                /**< TWI FIFO Status Register */
#define TWI_TXDATA8                 0x80                /**< TWI FIFO Transmit Data Single-Byte Register */
#define TWI_TXDATA16                0x84                /**< TWI FIFO Transmit Data Double-Byte Register */
#define TWI_RXDATA8                 0x88                /**< TWI FIFO Receive Data Single-Byte Register */
#define TWI_RXDATA16                0x8C                /**< TWI FIFO Receive Data Double-Byte Register */
/** @} */

#define TWI_CTL_EN                  0x80                /**< Enable Module */
#define TWI_CTL_SSCB                0x200               /**< SCCB Compatibility */

/** @name TWI Master Mode Control Register fields
 @{ **/
#define TWI_MSTRCTL_EN              0x01                /**< Transfer Direction for Master */
#define TWI_MSTRCTL_DIR             0x04                /**< Transfer direction: 0 - transmit, 1 - receive */
#define TWI_MSTRCTL_FAST            0x08                /**< Fast Mode */
#define TWI_MSTRCTL_STOP            0x10                /**< Issue Stop Condition */
#define TWI_MSTRCTL_RESTART         0x20                /**< Repeat Start */
#define TWI_MSTRCTL_SDAOVER         0x4000              /**< Serial Data Override */
#define TWI_MSTRCTL_SCLOVER         0x8000              /**< Serial Clock Override */

#define TWI_MSTRCTL_DCNT_MASK       0x3FC0              /**< Data counter mask */
#define TWI_MSTRCTL_DCNT_OFFSET     6                   /**< Data counter offset */
/** @} */

/** @name TWI Master Mode Status Register fields
 @{ **/
#define TWI_MSTRSTAT_MPROG          0x01                /**< Master Transfer in Progress */
#define TWI_MSTRSTAT_LOSTARB        0x02                /**< Lost Arbitration */
#define TWI_MSTRSTAT_ANAK           0x04                /**< Address Not Acknowledged */
#define TWI_MSTRSTAT_DNAK           0x08                /**< Data Not Acknowledged */
#define TWI_MSTRSTAT_BUFRDERR       0x10                /**< Buffer Read Error */
#define TWI_MSTRSTAT_BUFWRERR       0x20                /**< Buffer Write Error */
#define TWI_MSTRSTAT_SDASEN         0x40                /**< Serial Data Sense */
#define TWI_MSTRSTAT_SCLSEN         0x80                /**< Serial Clock Sense */
#define TWI_MSTRSTAT_BUSBUSY        0x0100              /**< Bus Busy */
/** @} */

#define TWI_MSTRADDR_ADDR_MASK      0x7F                /**< Master Address Mask */

/** @name TWI Interrupt fileds
 @{ **/
#define TWI_INTERRUPT_SINIT         0x01                /**< Slave Transfer Initiated */
#define TWI_INTERRUPT_SCOMP         0x02                /**< Slave Transfer Complete */
#define TWI_INTERRUPT_SERR          0x04                /**< Slave Transfer Error */
#define TWI_INTERRUPT_SOVF          0x08                /**< Slave Overflow */
#define TWI_INTERRUPT_MCOMP         0x10                /**< Master Transfer Complete */
#define TWI_INTERRUPT_MERR          0x20                /**< Master Transfer Error */
#define TWI_INTERRUPT_TXSERV        0x40                /**< Tx FIFO Service */
#define TWI_INTERRUPT_RXSERV        0x80                /**< Rx FIFO Service */
#define TWI_INTERRUPT_SDAI          0x4000              /**< Serial Data Interrupt */
#define TWI_INTERRUPT_SCLI          0x8000              /**< Serial Clock Interrupt */
/** @} */

/** @name TWI FIFO Control Register fields
 @{ **/
#define TWI_FIFOCTL_TXFLUSH         0x01                /**< Tx Buffer Flush */
#define TWI_FIFOCTL_RXFLUSH         0x02                /**< Rx Buffer Flush */
#define TWI_FIFOCTL_TXILEN          0x04                /**< Tx Buffer Interrupt Length */
#define TWI_FIFOCTL_RXILEN          0x08                /**< Rx Buffer Interrupt Length */
/** @} */

/** @name TWI FIFO Status Register fields
 @{ **/
#define TWI_FIFOSTAT_TXSTAT_MASK    0x03                /**< Tx Status Mask */
#define TWI_FIFOSTAT_RXSTAT_MASK    0x0C                /**< Rx Status Mask */

#define TWI_FIFOSTAT_TXSTAT_EMPTY   0x00                /**< Tx FIFO Empty */
#define TWI_FIFOSTAT_TXSTAT_BYTE    0x01                /**< Tx FIFO contains 1 byte */
#define TWI_FIFOSTAT_TXSTAT_FULL    0x03                /**< Tx FIFO contains 2 bytes */
#define TWI_FIFOSTAT_RXSTAT_EMPTY   0x00                /**< Rx FIFO Empty */
#define TWI_FIFOSTAT_RXSTAT_BYTE    0x04                /**< Rx FIFO contains 1 byte */
#define TWI_FIFOSTAT_RXSTAT_FULL    0x0C                /**< Rx FIFO contains 2 bytes */
/** @} */

/** @name I2C status software flags
 @{ **/
#define I2C_FLAG_BFIN_BF60X_COMPLETED   (1 << 0)
#define I2C_FLAG_BFIN_BF60X_ARBLOST     (1 << 1)
#define I2C_FLAG_BFIN_BF60X_NACK        (1 << 2)
#define I2C_FLAG_BFIN_BF60X_TIMEOUT     (1 << 3)
/** @} */

/** @name Redefined I2C register access
 @{ **/
#define I2C_BASE                        extra->i2c_twi_base
#define I2C_W8(r,x)                     HAL_WRITE_UINT8(I2C_BASE + (r), (x))
#define I2C_R8(r,x)                     HAL_READ_UINT8(I2C_BASE + (r), (x))
#define I2C_W16(r, x)                   HAL_WRITE_UINT16(I2C_BASE + (r), (x))
#define I2C_R16(r, x)                   HAL_READ_UINT16(I2C_BASE + (r), (x))
#define I2C_W32(r, x)                   HAL_WRITE_UINT32(I2C_BASE + (r), (x))
#define I2C_R32(r, x)                   HAL_READ_UINT32(I2C_BASE + (r), (x))
/** @} */

#define I2C_BUS_FREQ(_extra_)           ((_extra_)->i2c_bus_freq)

#ifndef CYGOPT_IO_I2C_TIMEOUT
#define CYGOPT_IO_I2C_TIMEOUT           (2000)
#endif

/** 
 *  @brief: BF60x I2C modes 
*/
typedef enum bfin_bf60x_i2c_xfer_mode {
    BFIN_BF60X_I2C_XFER_MODE_INVALID = 0x00,
    BFIN_BF60X_I2C_XFER_MODE_TX =      0x01,
    BFIN_BF60X_I2C_XFER_MODE_RX =      0x02
} bfin_bf60x_i2c_xfer_mode;

/** 
 *  @brief: BF60x I2C device driver data 
*/
typedef struct cyg_bfin_bf60x_i2c_extra {
    cyg_uint32       i2c_twi_base;          /**< Base address */
    cyg_uint8        i2c_twi_num;           /**< Interface number */
    cyg_uint8        i2c_addr;              /**< I2C address */
    cyg_uint32       i2c_count;             /**< Byte count */
    cyg_uint8*       i2c_txbuf;             /**< Tx buffer pointer */
    cyg_uint8*       i2c_rxbuf;             /**< Rx baffer pointer */
    cyg_bool         i2c_rxnak;             /**< Send NAK */
    cyg_bool         i2c_send_start;        /**< Send start condition */
    cyg_bool         i2c_send_stop;         /**< Send stop condition */
    cyg_bool         i2c_send_nack;         /**< Send NACK */
    bfin_bf60x_i2c_xfer_mode i2c_mode;      /**< Rx, Tx mode */

    cyg_uint32       i2c_flag;              /**< Software checked flag */
    cyg_uint32       i2c_delay;             /**< I2C lower/upper clock period */

    cyg_uint8        i2c_lost_arb;      	/**< Error condition leading to loss of arbitration */

    cyg_drv_mutex_t  i2c_lock; 			    /**< For synchronizing between DSR and foreground */
    cyg_drv_cond_t   i2c_wait;
    cyg_handle_t     i2c_interrupt_handle;	/**< For initializing the interrupt */
    cyg_uint8        i2c_completed;     	/**< Set by DSR, checked by thread */
    cyg_interrupt    i2c_interrupt_data;    /**< Interrupt data */

    cyg_uint32      i2c_isr_id;             /**< ISR number */
    cyg_uint32      i2c_isr_pri;            /**< ISR priority */
    // lower layer data
    cyg_uint8 int_addr_sz;                  /**< most chips do have internal address size */
} cyg_bfin_bf60x_i2c_extra;

/** 
 *  @brief: BF60x I2C device structure 
*/
typedef struct cyg_i2c_bfin_bf60x_dev_s
{
    // upper layer data
    cyg_i2c_device   i2c_device;    /**< upper layer i2c device structure */
    cyg_uint8        i2c_addr;      /**< i2c device address */
    cyg_uint32       i2c_count;     /**< i2c count */

    // lower layer data
    cyg_uint8 int_addr_sz;          /**< most chips do have internal address size */
} cyg_i2c_bfin_bf60x_dev_t;

#define CYG_I2C_BF60X_DEVICE_(_name_, _bus_, _address_, _flags_, _delay_, _int_adr_sz_) \
    static cyg_bfin_bf60x_i2c_extra _name_ = { \
                .i2c_device = { \
                .i2c_bus = _bus_, \
                .i2c_address = _address_, \
                .i2c_flags = _flags_, \
                .i2c_delay = _delay_, \
            }, \
            .int_addr_sz = _int_adr_sz_, \
    }
    
//==========================================================================
// I2C driver interface
//==========================================================================
void        i2c_bfin_bf60x_init(struct cyg_i2c_bus*);
cyg_uint32  i2c_bfin_bf60x_tx(const cyg_i2c_device*, 
                                       cyg_bool, const cyg_uint8*, 
                                       cyg_uint32, cyg_bool);
cyg_uint32  i2c_bfin_bf60x_rx(const cyg_i2c_device*, 
                                       cyg_bool, cyg_uint8*, 
                                       cyg_uint32, cyg_bool, cyg_bool);
void        i2c_bfin_bf60x_stop(const cyg_i2c_device*);



//==========================================================================
// I2C bus declaration macros
//=========================================================================

# define CYG_BFIN_BF60X_I2C_BUS(_name_, _twi_num_, _isr_pri_)           \
   static cyg_bfin_bf60x_i2c_extra _name_ ## _extra = {                 \
       .i2c_twi_num = _twi_num_,                                        \
       .i2c_isr_pri = _isr_pri_                                         \
  } ;                                                                   \
  CYG_I2C_BUS(_name_,                                                   \
              i2c_bfin_bf60x_init,                                   \
              i2c_bfin_bf60x_tx,                                     \
              i2c_bfin_bf60x_rx,                                     \
              i2c_bfin_bf60x_stop,                                   \
              (void*) & ( _name_ ## _extra)) ;

#endif