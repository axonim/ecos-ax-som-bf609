/** @file i2c_bfin_bf60x.c
 *  @brief I2C driver for BF60x
 */

//==========================================================================
//
//      i2c_bfin_bf60x.c
//
//      I2C driver for BF60x 
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
// Author(s):     Pavel Azizov <pavel.azizov@axonim.by>
//			
// Date:          2013-08-23
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/system.h>
#include <pkgconf/devs_i2c_bfin_bf60x.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/io/devtab.h>
#include <cyg/io/i2c.h>
#include <cyg/io/i2c_bfin_bf60x.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/drv_api.h>
#include <cyg/infra/diag.h>

#ifdef CYGOPT_IO_I2C_SUPPORT_DEBUG
    #define I2C_BF60X_DEBUG(args...)     diag_printf(args)
#else
    #define I2C_BF60X_DEBUG(args...)
#endif //CYGOPT_IO_I2C_SUPPORT_DEBUG

#define TWI_PRESCALE        ((CYGNUM_HAL_BFIN_PLL_FREQUENCY/CYGNUM_HAL_BFIN_SYSTEM_CLOCK_DIVIDER)/10 + 1)

#define CONTINUOUS_READ     1

static void
i2c_bfin_bf60x_send_manual_clocks(cyg_bfin_bf60x_i2c_extra *extra, cyg_uint32 numclocks);
void
i2c_bfin_bf60x_reinit(cyg_bfin_bf60x_i2c_extra * extra);

//////////////////////////////////////////////////////////////////////

/**
 *  \brief I2C Interrupt service routine
 *  
 *  \param [in] vec Interrupt vector number
 *  \param [in] data Interrupt specific data
 *  \return CYG_ISR_HANDLED | CYG_ISR_CALL_DSR
 *  
*/
static cyg_uint32
i2c_bfin_bf60x_isr(cyg_vector_t vec, cyg_addrword_t data)
{
    cyg_uint16 stat_reg;
    cyg_uint16 mask_reg;
    cyg_uint16 master_ctl_reg;
    cyg_uint16 master_stat_reg;
    cyg_uint16 txdata8;
    volatile cyg_uint16 dr;
    asm volatile("NOP;");
    volatile cyg_bfin_bf60x_i2c_extra *extra = data;

    // Read status & mask register
    I2C_R16(TWI_ISTAT, stat_reg);
    I2C_R16(TWI_IMSK, mask_reg);
    stat_reg &= mask_reg;

    // Switch states

    // What to do next depends on the current transfer mode.
    if (BFIN_BF60X_I2C_XFER_MODE_TX == extra->i2c_mode) {
		I2C_BF60X_DEBUG("i2c_bfin_bf60x_isr: BFIN_BF60X_I2C_XFER_MODE_TX\n");
        if (stat_reg & TWI_INTERRUPT_MERR)
        {
			I2C_BF60X_DEBUG("i2c_bfin_bf60x_isr: TWI_INTERRUPT_MERR\n");
            I2C_R16(TWI_MSTRSTAT, master_stat_reg);
            I2C_W16(TWI_MSTRCTL, 0x00);
			I2C_BF60X_DEBUG("master_stat_reg = %X\n", master_stat_reg);

            if (master_stat_reg & TWI_MSTRSTAT_LOSTARB)
            {
				I2C_BF60X_DEBUG("i2c_bfin_bf60x_isr: TWI_MSTRSTAT_LOSTARB\n");
                mask_reg &= ~(TWI_INTERRUPT_RXSERV | TWI_INTERRUPT_TXSERV | TWI_INTERRUPT_MCOMP | TWI_INTERRUPT_MERR);
                I2C_W16(TWI_IMSK, mask_reg);
                extra->i2c_flag |= I2C_FLAG_BFIN_BF60X_ARBLOST;
            }
            if (master_stat_reg & (TWI_MSTRSTAT_ANAK | TWI_MSTRSTAT_DNAK))
            {
				I2C_BF60X_DEBUG("i2c_bfin_bf60x_isr: TWI_MSTRSTAT_ANAK | TWI_MSTRSTAT_DNAK\n");
                mask_reg &= ~(TWI_INTERRUPT_RXSERV | TWI_INTERRUPT_TXSERV | TWI_INTERRUPT_MCOMP | TWI_INTERRUPT_MERR);
                I2C_W16(TWI_IMSK, mask_reg);
                extra->i2c_flag |= I2C_FLAG_BFIN_BF60X_NACK;
            }
			

            I2C_W16(TWI_MSTRSTAT, master_stat_reg);
            I2C_W16(TWI_ISTAT, TWI_INTERRUPT_MERR);
            
            HAL_INTERRUPT_ACKNOWLEDGE(extra->i2c_isr_id);
            return (CYG_ISR_HANDLED | CYG_ISR_CALL_DSR);
        }
	    // Check if MCOMP
        else if ((stat_reg & TWI_INTERRUPT_MCOMP) && (extra->i2c_count == 0))
        {
			I2C_BF60X_DEBUG("i2c_bfin_bf60x_isr: TWI_INTERRUPT_MCOMP\n");
            I2C_W16(TWI_ISTAT, TWI_INTERRUPT_MCOMP);
            
            I2C_R16(TWI_MSTRCTL, master_ctl_reg);
            master_ctl_reg &= ~TWI_MSTRCTL_RESTART;
            I2C_W16(TWI_MSTRCTL, master_ctl_reg);

            mask_reg &= ~(TWI_INTERRUPT_MCOMP | TWI_INTERRUPT_TXSERV);
    		I2C_W16(TWI_IMSK, mask_reg);

    		extra->i2c_flag |= I2C_FLAG_BFIN_BF60X_COMPLETED;
            
            HAL_INTERRUPT_ACKNOWLEDGE(extra->i2c_isr_id);
            return (CYG_ISR_HANDLED | CYG_ISR_CALL_DSR);
	    }
        // Check if TXSERV
	    else if (stat_reg & TWI_INTERRUPT_TXSERV)
	    {
			I2C_BF60X_DEBUG("i2c_bfin_bf60x_isr: TWI_INTERRUPT_TXSERV\n");
            I2C_W16(TWI_ISTAT, TWI_INTERRUPT_TXSERV);
            
            extra->i2c_count -= 1;

            if (extra->i2c_count != 0)
            {
                I2C_BF60X_DEBUG("i2c_bfin_bf60x_isr: extra->i2c_count = %d\n", extra->i2c_count);
                txdata8 = ((cyg_uint16) *extra->i2c_txbuf++) & 0x00FF;
                I2C_W16(TWI_TXDATA8, txdata8);
            }
    		else
    		{
				I2C_BF60X_DEBUG("i2c_bfin_bf60x_isr: TWI_INTERRUPT_TXSERV count = 0\n");
                if (extra->i2c_send_stop)
                {
                    I2C_R16(TWI_MSTRCTL, master_ctl_reg);
                    master_ctl_reg |= TWI_MSTRCTL_STOP;
    			    I2C_W16(TWI_MSTRCTL, master_ctl_reg);
    			    extra->i2c_send_stop = 0;
                }
                else
                {
                    // TODO Use byte counter to generate MCOMP
                    I2C_R16(TWI_MSTRCTL, master_ctl_reg);
                    master_ctl_reg |= TWI_MSTRCTL_RESTART;
                    I2C_W16(TWI_MSTRCTL, master_ctl_reg);
                }

                mask_reg |= TWI_INTERRUPT_MCOMP;
                mask_reg &= ~TWI_INTERRUPT_TXSERV;
    		    I2C_W16(TWI_IMSK, mask_reg);
    		}
            
            HAL_INTERRUPT_ACKNOWLEDGE(extra->i2c_isr_id);
            return (CYG_ISR_HANDLED);
	    }
    }
    else if (BFIN_BF60X_I2C_XFER_MODE_RX == extra->i2c_mode) {

        if (stat_reg & TWI_INTERRUPT_MERR)
        {
            I2C_R16(TWI_MSTRSTAT, master_stat_reg);
            I2C_W16(TWI_MSTRCTL, 0x00);

            if (master_stat_reg & TWI_MSTRSTAT_LOSTARB)
            {
                mask_reg &= ~(TWI_INTERRUPT_RXSERV | TWI_INTERRUPT_TXSERV | TWI_INTERRUPT_MCOMP | TWI_INTERRUPT_MERR);
                I2C_W16(TWI_IMSK, mask_reg);
                extra->i2c_flag |= I2C_FLAG_BFIN_BF60X_ARBLOST;
            }
            if (master_stat_reg & (TWI_MSTRSTAT_ANAK | TWI_MSTRSTAT_DNAK))
            {
                mask_reg &= ~(TWI_INTERRUPT_RXSERV | TWI_INTERRUPT_TXSERV | TWI_INTERRUPT_MCOMP | TWI_INTERRUPT_MERR);
                I2C_W16(TWI_IMSK, mask_reg);
                extra->i2c_flag |= I2C_FLAG_BFIN_BF60X_NACK;
            }

            I2C_W16(TWI_MSTRSTAT, master_stat_reg);
            I2C_W16(TWI_ISTAT, TWI_INTERRUPT_MERR);
        }
	    // Check if MCOMP
	    else if ((stat_reg & TWI_INTERRUPT_MCOMP) && (extra->i2c_count == 0))
	    {
            I2C_W16(TWI_ISTAT, TWI_INTERRUPT_MCOMP);

            mask_reg &= ~(TWI_INTERRUPT_MCOMP | TWI_INTERRUPT_RXSERV);
            I2C_W16(TWI_IMSK, mask_reg);

            extra->i2c_flag |= I2C_FLAG_BFIN_BF60X_COMPLETED;
            
            HAL_INTERRUPT_ACKNOWLEDGE(extra->i2c_isr_id);
            return (CYG_ISR_HANDLED | CYG_ISR_CALL_DSR);
        }
        // Check if RXSERV
        else if (stat_reg & TWI_INTERRUPT_RXSERV) 
        {
        I2C_W16(TWI_ISTAT, TWI_INTERRUPT_RXSERV);
#if CONTINUOUS_READ
            I2C_R16(TWI_RXDATA8, dr);
            dr &= 0x00FF;
            *(extra->i2c_rxbuf) = (cyg_uint8) dr;

            //TODO needs debugging with continuous read !!!
            extra->i2c_rxbuf += 1;
            extra->i2c_count -= 1;

            if(extra->i2c_count == 0)
            {
                mask_reg |= TWI_INTERRUPT_MCOMP;
                mask_reg &= ~TWI_INTERRUPT_RXSERV;
                I2C_W16(TWI_IMSK, mask_reg);
            }
#else
            I2C_R16(TWI_RXDATA8, dr);
            dr &= 0x00FF;
            *(extra->i2c_rxbuf) = (cyg_uint8) dr;

            //TODO needs debugging with continuous read !!!
            extra->i2c_rxbuf += 1;
            extra->i2c_count -= 1;

            if(extra->i2c_count == 0)
            {
                if (extra->i2c_send_stop)
                {
                    I2C_R16(TWI_MSTRCTL, master_ctl_reg);
                    master_ctl_reg |= TWI_MSTRCTL_STOP;
                    I2C_W16(TWI_MSTRCTL, master_ctl_reg);
                    extra->i2c_send_stop = 0;
                }

                mask_reg |= TWI_INTERRUPT_MCOMP;
                mask_reg &= ~TWI_INTERRUPT_RXSERV;
                I2C_W16(TWI_IMSK, mask_reg);
            }
#endif
            HAL_INTERRUPT_ACKNOWLEDGE(extra->i2c_isr_id);
            return (CYG_ISR_HANDLED);
	    }
    } 
    else {
        // Invalid state? Some kind of spurious interrupt?
        // Just ignore it.
    }

    //I2C_W16(TWI_FIFOCTL, TWI_FIFOCTL_TXFLUSH | TWI_FIFOCTL_RXFLUSH);
    //I2C_W16(TWI_FIFOCTL, 0);

}

/**
 *  \brief I2C Deferred service routine
 *  
 *  \param [in] vec Interrupt vector number
 *  \param [in] count
 *  \param [in] data DSR specific data
 *  \return None
 *  
*/
static void
i2c_bfin_bf60x_dsr(cyg_vector_t vec, cyg_ucount32 count, cyg_addrword_t data)
{
    I2C_BF60X_DEBUG("i2c_bfin_bf60x_dsr\n");
    cyg_bfin_bf60x_i2c_extra *extra = (cyg_bfin_bf60x_i2c_extra *) data;
    extra->i2c_completed = 1;
    cyg_drv_cond_signal(&extra->i2c_wait);
}

/**
 *  \brief Set up I2C bus timing
 *  
 *  \param [in] extra I2C driver specific data
 *  \param [in] delay I2C lower/upper clock period
 *  \return None
 *  
*/
static void
i2c_bfin_bf60x_delay(cyg_bfin_bf60x_i2c_extra *extra, cyg_uint32 delay)
{
    cyg_uint32 cdiv = 0;

    // see SCL Clock Divider Register description in BF60x Hardware Reference
    cdiv = delay / 100;
    I2C_W16(TWI_CLKDIV, ((cdiv & 0xFF) << 8) | ((cdiv & 0xFF)));

    extra->i2c_delay = delay;
}

/**
 *  \brief Send clocks manually
 *  
 *  \param [in] extra I2C driver specific data
 *  \param [in] numclocks Number of clocks to send
 *  \return None
 *  
 *  \details Faulty slave devices, may drive SDA low after a transfer
 *           finishes. To release the bus this code generates up to 
 *           extra clocks.
*/
static void
i2c_bfin_bf60x_send_manual_clocks(cyg_bfin_bf60x_i2c_extra *extra, cyg_uint32 numclocks)
{
    /* Faulty slave devices, may drive SDA low after a transfer
     * finishes. To release the bus this code generates up to 
     * extra clocks.
     */
    do {
        I2C_W16(TWI_MSTRCTL, TWI_MSTRCTL_SCLOVER);
        hal_delay_us(10);
        I2C_W16(TWI_MSTRCTL, 0);
        hal_delay_us(10);
    } while (numclocks--);

    I2C_W16(TWI_MSTRCTL, TWI_MSTRCTL_SDAOVER | TWI_MSTRCTL_SCLOVER);
    hal_delay_us(10);
    I2C_W16(TWI_MSTRCTL, TWI_MSTRCTL_SDAOVER);
    hal_delay_us(10);
    I2C_W16(TWI_MSTRCTL, 0);
}

/**
 *  \brief A transfer has been started. Wait for completion
 *  
 *  \param [in] extra I2C driver specific data
 *  \return None
*/
static inline void
i2c_bfin_bf60x_doit(cyg_bfin_bf60x_i2c_extra * extra)
{
#ifndef CYGOPT_IO_I2C_SUPPORT_TIMEOUTS
    while ((extra->i2c_flag & (I2C_FLAG_BFIN_BF60X_COMPLETED | I2C_FLAG_BFIN_BF60X_ARBLOST | I2C_FLAG_BFIN_BF60X_NACK)) == 0){
	    cyg_drv_mutex_lock(&(extra->i2c_lock));
	    cyg_drv_dsr_lock();
	    while (!extra->i2c_completed) {
		    cyg_drv_cond_wait(&(extra->i2c_wait));
	    }
	    cyg_drv_dsr_unlock();
	    cyg_drv_mutex_unlock(&(extra->i2c_lock));
    }
   
    if (extra->i2c_flag & I2C_FLAG_BFIN_BF60X_ARBLOST)
        i2c_bfin_bf60x_reinit(extra);

   extra->i2c_flag = 0;
#else
   while ((extra->i2c_flag & (I2C_FLAG_BFIN_BF60X_COMPLETED | I2C_FLAG_BFIN_BF60X_ARBLOST | I2C_FLAG_BFIN_BF60X_NACK)) == 0){
	    cyg_drv_mutex_lock(&(extra->i2c_lock));
	    cyg_drv_dsr_lock();
	    if (!cyg_cond_timed_wait(&(extra->i2c_wait), cyg_current_time() + CYGOPT_IO_I2C_TIMEOUT))
	    {
  	        cyg_drv_dsr_unlock();
    		cyg_drv_mutex_unlock(&(extra->i2c_lock));
    		extra->i2c_completed = 0;
    		extra->i2c_flag |= I2C_FLAG_BFIN_BF60X_TIMEOUT;
    		break;
	    }
	    cyg_drv_dsr_unlock();
	    cyg_drv_mutex_unlock(&(extra->i2c_lock));
    }
   
    if (extra->i2c_flag & (I2C_FLAG_BFIN_BF60X_ARBLOST | I2C_FLAG_BFIN_BF60X_TIMEOUT))
        i2c_bfin_bf60x_reinit(extra);

   extra->i2c_flag = 0;
#endif
}

/**
 *  \brief Finalize I2C transfer
 *  
 *  \param [in] extra I2C driver specific data
 *  \return None
*/
static inline void
i2c_bfin_bf60x_stopit(cyg_bfin_bf60x_i2c_extra * extra)
{
    I2C_W16(TWI_FIFOCTL, TWI_FIFOCTL_TXFLUSH | TWI_FIFOCTL_RXFLUSH);
    I2C_W16(TWI_FIFOCTL, 0);
    extra->i2c_lost_arb = 0;
    extra->i2c_mode = BFIN_BF60X_I2C_XFER_MODE_INVALID;
}

/**
 *  \brief Start I2C transfer
 *  
 *  \param [in] extra I2C driver specific data
 *  \param [in] address I2C device address
 *  \param [in] addr_size I2C device address size (deprecated)
 *  \return Return_Description
 *  
 *  \details TRUE if transfer started, FALSE if not
 */
static cyg_bool
i2c_bfin_bf60x_handle_xfer(cyg_bfin_bf60x_i2c_extra * extra, int address, int addr_size)
{
    cyg_uint32 tmp 	= 0;
    cyg_uint16 clkdiv;
    cyg_uint16 mask_reg; 
    cyg_uint16 master_ctl_reg;
    cyg_uint16 txdata8;
    cyg_uint8  i 	= addr_size;

    // Nothing to send or receive
    if (extra->i2c_count == 0)
        return 0;

    I2C_R16(TWI_IMSK, mask_reg);

    I2C_W16(TWI_FIFOCTL, TWI_FIFOCTL_TXFLUSH | TWI_FIFOCTL_RXFLUSH);
    I2C_W16(TWI_FIFOCTL, 0);

    // TX transfer
    if (extra->i2c_mode == BFIN_BF60X_I2C_XFER_MODE_TX) {

        // Load device address
        I2C_W16(TWI_MSTRADDR, address & TWI_MSTRADDR_ADDR_MASK);

        txdata8 = ((cyg_uint16) *extra->i2c_txbuf++) & 0x00FF;

        // Load first data byte
        I2C_W16(TWI_TXDATA8, txdata8);

        I2C_W16(TWI_IMSK, (mask_reg | TWI_INTERRUPT_MERR | TWI_INTERRUPT_TXSERV));

        // Prepare Master control register
        master_ctl_reg = (TWI_MSTRCTL_EN | TWI_MSTRCTL_DCNT_MASK);

        I2C_R16(TWI_CLKDIV, clkdiv);
        if ((clkdiv & 0xFF) < 100)
        {
            master_ctl_reg |= TWI_MSTRCTL_FAST;
        }

        I2C_W16(TWI_MSTRCTL, master_ctl_reg);
    } else
    {
        // Load device address
        I2C_W16(TWI_MSTRADDR, address & TWI_MSTRADDR_ADDR_MASK);

        I2C_W16(TWI_IMSK, (mask_reg | TWI_INTERRUPT_MERR | TWI_INTERRUPT_RXSERV));

        // Prepare Master control register
#ifdef CONTINUOUS_READ
        master_ctl_reg = (TWI_MSTRCTL_EN | TWI_MSTRCTL_DIR | ((cyg_uint8) extra->i2c_count << 6));
#else
        master_ctl_reg = (TWI_MSTRCTL_EN | TWI_MSTRCTL_DIR | (1 << 6));
#endif

        I2C_R16(TWI_CLKDIV, clkdiv);
        if ((clkdiv & 0xFF) < 100)
        {
            master_ctl_reg |= TWI_MSTRCTL_FAST;
        }

        I2C_W16(TWI_MSTRCTL, master_ctl_reg);
    }

    return 1;
}


/**
 *  \brief Disable all I2C interrupts
 *  
 *  \param [in] extra I2C driver specific data
 *  \return None
*/
static void
i2c_bfin_bf60x_disable_ints(cyg_bfin_bf60x_i2c_extra * extra)
{
    I2C_W32(extra->i2c_twi_base + TWI_IMSK, 0x00);
}

/**
 *  \brief Initialize driver & hardware state
 *  
 *  \param [in] bus I2C bus structure
 *  \return None
 *  
 *  \details Details
 */
void
i2c_bfin_bf60x_init(struct cyg_i2c_bus *bus)
{
    cyg_bfin_bf60x_i2c_extra *extra = (cyg_bfin_bf60x_i2c_extra *) bus->i2c_extra;

    extra->i2c_completed = 0;
    extra->i2c_lost_arb	 = 0;
    extra->i2c_flag	 = 0;
    extra->i2c_mode	 = BFIN_BF60X_I2C_XFER_MODE_INVALID;

    if (!extra->i2c_twi_num){
	      extra->i2c_twi_base    = TWI0_BASE;
	      extra->i2c_isr_id      = CYGNUM_HAL_INTERRUPT_TWI0;
    } 
    else {
	      extra->i2c_twi_base    = TWI1_BASE;
	      extra->i2c_isr_id      = CYGNUM_HAL_INTERRUPT_TWI1;
    }

    cyg_drv_mutex_init(&extra->i2c_lock);
    cyg_drv_cond_init(&extra->i2c_wait, &extra->i2c_lock);
    cyg_drv_interrupt_create(extra->i2c_isr_id,
                             extra->i2c_isr_pri,
                             (cyg_addrword_t)extra,
                             i2c_bfin_bf60x_isr,
                             i2c_bfin_bf60x_dsr,
                             &(extra->i2c_interrupt_handle),
                             &(extra->i2c_interrupt_data));
    cyg_drv_interrupt_attach(extra->i2c_interrupt_handle);
    cyg_drv_interrupt_unmask(extra->i2c_isr_id);

    // Disable all TWI IRQs
    I2C_W16(TWI_IMSK, 0x00);

    //Set TWI prescaler, enable TWI
    I2C_W16(TWI_CTL, (TWI_PRESCALE & 0x7F) | TWI_CTL_EN);

    i2c_bfin_bf60x_send_manual_clocks(extra, 9);
}

/**
 *  \brief ReInitialize driver & hardware state
 *  
 *  \param [in] extra I2C driver specific data
 *  \return None
*/
void
i2c_bfin_bf60x_reinit(cyg_bfin_bf60x_i2c_extra * extra)
{
    // Disable all TWI IRQs
    I2C_W16(TWI_IMSK, 0x00);
    // Clear TWI interrupts
    I2C_W16(TWI_ISTAT, 0xFF);

    i2c_bfin_bf60x_send_manual_clocks(extra, 9);

    // Clear master status bits
    I2C_W16(TWI_MSTRSTAT, 0x3F);
    // Clear master control register
    I2C_W16(TWI_MSTRCTL, 0x00);
}

/**
 *  \brief Transmit a buffer to a device
 *  
 *  \param [in] dev I2C device structure
 *  \param [in] send_start Send start condition
 *  \param [in] tx_data Tx data buffer
 *  \param [in] count Byte count
 *  \param [in] send_stop Send stop condition
 *  \return Remaining bytes count
*/
cyg_uint32
i2c_bfin_bf60x_tx(const cyg_i2c_device *dev,
               cyg_bool send_start,
               const cyg_uint8 *tx_data,
               cyg_uint32 count,
               cyg_bool send_stop)
{
    cyg_bfin_bf60x_i2c_extra *extra = (cyg_bfin_bf60x_i2c_extra *) dev->i2c_bus->i2c_extra;

    if (!count)
        return 0;

    extra->i2c_txbuf 		= (cyg_uint8 *)tx_data;
    extra->i2c_count 		= count;

    extra->i2c_send_start	= send_start;
    extra->i2c_send_stop	= send_stop;
    extra->i2c_completed 	= 0;
    extra->i2c_flag	 	    = 0;
    extra->i2c_mode		    = (bfin_bf60x_i2c_xfer_mode)BFIN_BF60X_I2C_XFER_MODE_TX;
	
	I2C_BF60X_DEBUG("i2c_bfin_bf60x_tx: dev->i2c_address = %x\n", dev->i2c_address);
	I2C_BF60X_DEBUG("i2c_bfin_bf60x_tx: count = %d\n", count);

    // set device specific speed
    i2c_bfin_bf60x_delay(extra, dev->i2c_delay);

    if (!extra->i2c_lost_arb) {
        if (!i2c_bfin_bf60x_handle_xfer(extra, dev->i2c_address, 0)) {
            return 0;
        }
        i2c_bfin_bf60x_doit(extra);
    }
	
    i2c_bfin_bf60x_stopit(extra);
    
    i2c_bfin_bf60x_disable_ints(extra);
	
	I2C_BF60X_DEBUG("i2c_bfin_bf60x_tx: extra->i2c_count = %d\n", extra->i2c_count);

    return extra->i2c_count;//(count - extra->i2c_count);
}

/**
 *  \brief Receive into a buffer from a device
 *  
 *  \param [in] dev I2C device structure
 *  \param [in] send_start Send start condition
 *  \param [in] rx_data Rx data buffer
 *  \param [in] count Byte count
 *  \param [in] send_nak Send NAK
 *  \param [in] send_stop Send stop condition
 *  \return Remaining bytes count
 *  
 *  \details there is no h/w specific way to send NACK
 */
cyg_uint32
i2c_bfin_bf60x_rx(const cyg_i2c_device *dev,
               cyg_bool send_start,
               cyg_uint8 *rx_data,
               cyg_uint32 count,
               cyg_bool send_nak,
               cyg_bool send_stop)
{
    cyg_bfin_bf60x_i2c_extra *extra = (cyg_bfin_bf60x_i2c_extra *) dev->i2c_bus->i2c_extra;

    if (!count)
        return 0;

    extra->i2c_rxbuf 		= rx_data;
#ifdef CONTINUOUS_READ
    extra->i2c_count        = count <= 254 ? count : 254;
#else
    extra->i2c_count 		= count;
#endif
    extra->i2c_send_start	= send_start;
    extra->i2c_send_stop	= send_stop;
    extra->i2c_send_nack	= send_nak;
    extra->i2c_flag	 	    = 0;
    extra->i2c_completed 	= 0;
    extra->i2c_mode		    = (bfin_bf60x_i2c_xfer_mode)BFIN_BF60X_I2C_XFER_MODE_RX;

    // set device specific speed
    i2c_bfin_bf60x_delay(extra, dev->i2c_delay);

    if (!extra->i2c_lost_arb) {
        if (!i2c_bfin_bf60x_handle_xfer(extra, dev->i2c_address, 0)) {
            return 0;
        }
        i2c_bfin_bf60x_doit(extra);
    }

    i2c_bfin_bf60x_stopit(extra);
    
    i2c_bfin_bf60x_disable_ints(extra);
    
    return (count - extra->i2c_count);
}

/**
 *  \brief Stop I2C transfer
 *  
 *  \param [in] dev I2C device structure
 *  \return None
*/
void
i2c_bfin_bf60x_stop(const cyg_i2c_device * dev)
{
    cyg_bfin_bf60x_i2c_extra *extra = (cyg_bfin_bf60x_i2c_extra *) dev->i2c_bus->i2c_extra;
    i2c_bfin_bf60x_stopit(extra);
}