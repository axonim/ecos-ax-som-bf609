//=============================================================================
//
//      ser16c550c.c
//
//      Simple driver for the 16c550c serial controllers on the STAMP board
//
//=============================================================================
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Contributors:Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:        2013-09-09
// Description: Simple driver for the 16c550c serial controller
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/hal.h>
#include <pkgconf/system.h>
#include CYGBLD_HAL_PLATFORM_H

#include <cyg/hal/hal_arch.h>           // SAVE/RESTORE GP macros
#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/hal_if.h>             // interface API
#include <cyg/hal/hal_intr.h>           // HAL_ENABLE/MASK/UNMASK_INTERRUPTS
#include <cyg/hal/hal_misc.h>           // Helper functions
#include <cyg/hal/drv_api.h>            // CYG_ISR_HANDLED
#include <cyg/hal/bfin-regs.h>
#include <pkgconf/hal_bfin_adibf609.h>

//-----------------------------------------------------------------------------
// There is only one port.
#define UART0_BASE      0xFFC02000

//-----------------------------------------------------------------------------
// Define the serial registers. The Atlas board is equipped with a 16550C
// serial chip.
#define REVID_OFFSET                 0x0         /* UARTn Revision ID Register */
#define CTL_OFFSET                   0x4         /* UARTn Control Register */
#define STAT_OFFSET                  0x8         /* UARTn Status Register */
#define SCR_OFFSET                   0xC         /* UARTn Scratch Register */
#define CLK_OFFSET                   0x10        /* UARTn Clock Rate Register */
#define IMSK_OFFSET                  0x14        /* UARTn Interrupt Mask Register */
#define IMSK_SET_OFFSET              0x18        /* UARTn Interrupt Mask Set Register */
#define IMSK_CLR_OFFSET              0x1C        /* UARTn Interrupt Mask Clear Register */
#define RBR_OFFSET                   0x20        /* UARTn Receive Buffer Register */
#define THR_OFFSET                   0x24        /* UARTn Transmit Hold Register */
#define TAIP_OFFSET                  0x28        /* UARTn Transmit Address/Insert Pulse Register */
#define TSR_OFFSET                   0x2C        /* UARTn Transmit Shift Register */
#define RSR_OFFSET                   0x30        /* UARTn Receive Shift Register */
#define TXCNT_OFFSET                 0x34        /* UARTn Transmit Counter Register */
#define RXCNT_OFFSET                 0x38        /* UARTn Receive Counter Register */

// The data status register bits (UARTn_STAT).
#define STAT_DR      0x01            // data ready
#define STAT_OE      0x02            // overrun error
#define STATR_PE     0x04            // parity error
#define STAT_FE      0x08            // framing error
#define STAT_BI      0x10            // break interrupt
#define STAT_THRE    0x20            // transmitter holding register empty
#define STAT_TEMT    0x80            // transmitter register empty
#define STAT_TFI	 0x100			 // Transmission Finished Indicator
#define STAT_ASTKY	 0x200			 // Address Sticky
#define STAT_ADDR 	 0x400			 // Reception On-going
#define STAT_RO	     0x800			 // Reception On-going
#define STAT_SCTS	 0x1000			 // Sticky CTS
#define STAT_CTS	 0x10000		 // Clear to Send
#define STAT_RFCS	 0x20000		 // Receive FIFO Count Status

// Line Control Register
#define LCR_WL5 0x0000  // Word length -- 5 bits
#define LCR_WL6 0x100	// 6 bits
#define LCR_WL7 0x200	// 7 bits
#define LCR_WL8 0x300	// 8 bits
#define LCR_SB1 0x0000  // Number of stop bits -- 1
#define LCR_SB1_5 0x3000  // 1.5 
#define LCR_SB2 0x1000  // 2
#define LCR_PN  0x00    // Parity mode - none
#define LCR_PE  0xC000  // Parity mode - even
#define LCR_PO  0x4000  // Parity mode - odd
#define LCR_PM  0x14000 // Forced "mark" parity
#define LCR_PS  0x1C000 // Forced "space" parity

//-----------------------------------------------------------------------------
typedef struct {
    cyg_uint32 base;
    cyg_int32 msec_timeout;
    int isr_vector;
} channel_data_t;

static channel_data_t channels[1] = {
    { (cyg_uint32) UART0_BASE, 1000, CYGNUM_HAL_INTERRUPT_DEBUG_UART}
};

//-----------------------------------------------------------------------------
// Set the baud rate

// static void
// cyg_hal_plf_serial_set_baud(cyg_uint8* port, cyg_uint16 baud_divisor)
// {
    // cyg_uint8 _lcr;

    
    // //HAL_READ_UINT32(port+SER_16550_LCR, _lcr);

// //  _lcr |= LCR_DL;
    // //_lcr = 0x83;

    // //HAL_WRITE_UINT8(port+SER_16550_LCR, _lcr);
    // //asm volatile ("SSYNC;");

    // HAL_WRITE_UINT8(port+SER_16550_DLM, baud_divisor >> 8);
    // //HAL_WRITE_UINT8(port+SER_16550_DLM, 0x0);
    // HAL_WRITE_UINT8(port+SER_16550_DLL, baud_divisor & 0xff);
    // //HAL_WRITE_UINT8(port+SER_16550_DLL, 0x57);
    // asm volatile ("SSYNC;");
    
  // //  _lcr &= ~LCR_DL;
    // _lcr= 0x3;
    // asm volatile ("SSYNC;");
    // HAL_WRITE_UINT8(port+SER_16550_LCR, _lcr);
// }

//-----------------------------------------------------------------------------
// The minimal init, get and put functions. All by polling.

void
cyg_hal_plf_serial_init_channel(void* __ch_data)
{
    cyg_uint32 port;
    cyg_uint32 buf_reg;
	cyg_uint32 divisor;

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
    if (__ch_data == 0)
      __ch_data = (void*)&channels[0];

    port = ((channel_data_t*)__ch_data)->base;

    // disable UART clock for changing properties
    HAL_WRITE_UINT32(port+CTL_OFFSET, 0);
//    *((cyg_uint32*)(port+CTL_OFFSET)) = 0;
    // enable UART clock
    HAL_WRITE_UINT32(port+CTL_OFFSET, 1);
//    *((cyg_uint32*)(port+CTL_OFFSET)) = 1;

    // Set databits, stopbits and parity.
    buf_reg = LCR_WL8 | LCR_SB1 | LCR_PN | 1; // 8-bits, 1 stop bit, parity none
    HAL_WRITE_UINT32(port+CTL_OFFSET, buf_reg);
    asm volatile ("SSYNC;");
    
    // Disable port interrupts while changing hardware
    HAL_WRITE_UINT32(port+IMSK_CLR_OFFSET, 0);

    // Set baud rate.
	// HAL_READ_UINT32(port+CLK_OFFSET, buf_reg);
	// if (buf_reg & 0x10000000) 
	// {
	//	divisor = (CYGNUM_HAL_BFIN_SYSTEM_CLOCK / CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL_BAUD) | 0x10000000;
	// } 
	// else {
		divisor = (CYGNUM_HAL_BFIN_S0CLK_CLOCK * 1000000) / 16 / CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL_BAUD;
	// }
    
	//set baud rate
	HAL_WRITE_UINT32(port+CLK_OFFSET, divisor);
	
	HAL_READ_UINT32(port+CTL_OFFSET, buf_reg);
	buf_reg &= ~0x2;
    HAL_WRITE_UINT32(port+CTL_OFFSET, buf_reg); 
    
    // Don't allow interrupts.
    HAL_WRITE_UINT32(port+IMSK_CLR_OFFSET, 0xFFFFFFFF);

 //    // enable UART clock for changing properties
	// HAL_READ_UINT32(port+CTL_OFFSET, buf_reg);
	// buf_reg |= 0x1;
 //    HAL_WRITE_UINT32(port+CTL_OFFSET, buf_reg); 
    asm volatile ("SSYNC;");
    
}

void
cyg_hal_plf_serial_putc(void* __ch_data, cyg_uint8 __ch)
{
    cyg_uint32 port;
    cyg_uint32 reg_buf;

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
    if (__ch_data == 0)
      __ch_data = (void*)&channels[0];

    port = ((channel_data_t*)__ch_data)->base;

    CYGARC_HAL_SAVE_GP();

    do {
        HAL_READ_UINT32(port+STAT_OFFSET, reg_buf);
    } while (!(reg_buf & STAT_THRE));
    asm volatile ("SSYNC;");
    // Now, the transmit buffer is empty
    HAL_WRITE_UINT32(port+THR_OFFSET, (cyg_uint32)__ch);
    asm volatile ("SSYNC;");
    // Hang around until the character has been safely sent.
    do {
        HAL_READ_UINT32(port+STAT_OFFSET, reg_buf);
    } while (!(reg_buf & STAT_THRE));


    CYGARC_HAL_RESTORE_GP();
}

static cyg_bool
cyg_hal_plf_serial_getc_nonblock(void* __ch_data, cyg_uint8* ch)
{
    cyg_uint32 port;
    cyg_uint32 buf_reg;

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
    if (__ch_data == 0)
      __ch_data = (void*)&channels[0];

    port = ((channel_data_t*)__ch_data)->base;
    asm volatile ("SSYNC;");
    HAL_READ_UINT32(port+STAT_OFFSET, buf_reg);
    if (!(buf_reg & STAT_DR))
        return false;
    asm volatile ("SSYNC;");
    HAL_READ_UINT32(port+RBR_OFFSET, buf_reg);
	
	*ch = (cyg_uint8)buf_reg;

    return true;
}

cyg_uint8
cyg_hal_plf_serial_getc(void* __ch_data)
{
    cyg_uint8 ch;
    CYGARC_HAL_SAVE_GP();

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
    if (__ch_data == 0)
      __ch_data = (void*)&channels[0];

    while(!cyg_hal_plf_serial_getc_nonblock(__ch_data, &ch));

    CYGARC_HAL_RESTORE_GP();
    return ch;
}

static void
cyg_hal_plf_serial_write(void* __ch_data, const cyg_uint8* __buf, 
                         cyg_uint32 __len)
{
    CYGARC_HAL_SAVE_GP();

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
    if (__ch_data == 0)
      __ch_data = (void*)&channels[0];

    while(__len-- > 0)
        cyg_hal_plf_serial_putc(__ch_data, *__buf++);

    CYGARC_HAL_RESTORE_GP();
}

static void
cyg_hal_plf_serial_read(void* __ch_data, cyg_uint8* __buf, cyg_uint32 __len)
{
    CYGARC_HAL_SAVE_GP();

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
    if (__ch_data == 0)
      __ch_data = (void*)&channels[0];

    while(__len-- > 0)
        *__buf++ = cyg_hal_plf_serial_getc(__ch_data);

    CYGARC_HAL_RESTORE_GP();
}


cyg_bool
cyg_hal_plf_serial_getc_timeout(void* __ch_data, cyg_uint8* ch)
{
    int delay_count;
    channel_data_t* chan;
    cyg_bool res;
    CYGARC_HAL_SAVE_GP();

    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
    if (__ch_data == 0)
      __ch_data = (void*)&channels[0];

    chan = (channel_data_t*)__ch_data;

    delay_count = chan->msec_timeout * 10; // delay in .1 ms steps

    for(;;) {
        res = cyg_hal_plf_serial_getc_nonblock(__ch_data, ch);
        if (res || 0 == delay_count--)
            break;
        CYGACC_CALL_IF_DELAY_US(100);
    }

    CYGARC_HAL_RESTORE_GP();
    return res;
}

/*
static int
cyg_hal_plf_serial_control( void *__ch_data, __comm_control_cmd_t __func, ...) */

static int
cyg_hal_plf_serial_control(void *__ch_data, __comm_control_cmd_t __func, ...)
{
    static int irq_state = 0;
    channel_data_t* chan;
    cyg_uint8 ier;
    int ret = 0;

	CYGARC_HAL_SAVE_GP();
    // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // Go ahead and assume it is channels[0].
    if (__ch_data == 0)
      __ch_data = (void*)&channels[0];

    chan = (channel_data_t*)__ch_data;
	
    switch (__func) {
    case __COMMCTL_IRQ_ENABLE:
        // irq_state = 1;

        // HAL_READ_UINT8(chan->base + SER_16550_IER, ier);
        // ier |= SIO_IER_ERDAI;
        // HAL_WRITE_UINT8(chan->base + SER_16550_IER, ier);

        // HAL_INTERRUPT_SET_LEVEL(chan->isr_vector, 1);
        // HAL_INTERRUPT_UNMASK(chan->isr_vector);
        break;
    case __COMMCTL_IRQ_DISABLE:
        // ret = irq_state;
        // irq_state = 0;

        // HAL_READ_UINT8(chan->base + SER_16550_IER, ier);
        // ier &= ~SIO_IER_ERDAI;
        // HAL_WRITE_UINT8(chan->base + SER_16550_IER, ier);

        // HAL_INTERRUPT_MASK(chan->isr_vector);
        break;
    case __COMMCTL_DBG_ISR_VECTOR:
        // ret = chan->isr_vector;
        break;
    case __COMMCTL_SET_TIMEOUT:
    {
        va_list ap;

        va_start(ap, __func);

        ret = chan->msec_timeout;
        chan->msec_timeout = va_arg(ap, cyg_uint32);

        va_end(ap);
    }        
    break;
    case __COMMCTL_SETBAUD:
    // {
        // cyg_uint32 baud_rate;
        // cyg_uint16 baud_divisor;
        // cyg_uint8* port = chan->base;
        // va_list ap;

        // va_start(ap, __func);
        // baud_rate = va_arg(ap, cyg_uint32);
        // va_end(ap);

        // switch (baud_rate)
        // {
        // case 2400:   baud_divisor = BAUD_2400;   break;
        // case 4800:   baud_divisor = BAUD_4800;   break;
        // case 7200:   baud_divisor = BAUD_7200;   break;
        // case 9600:   baud_divisor = BAUD_9600;   break;
        // case 14400:  baud_divisor = BAUD_14400;  break;
        // case 19200:  baud_divisor = BAUD_19200;  break;
        // case 38400:  baud_divisor = BAUD_38400;  break;
        // case 57600:  baud_divisor = BAUD_57600;  break;
        // case 115200: baud_divisor = BAUD_115200; break;
        // case 230400: baud_divisor = BAUD_230400; break;
        // default:     return -1;                  break; // Invalid baud rate selected
        // }

        // // Disable port interrupts while changing hardware
        // HAL_READ_UINT8(port+SER_16550_IER, ier);
        // HAL_WRITE_UINT8(port+SER_16550_IER, 0);

        // // Set baud rate.
        // cyg_hal_plf_serial_set_baud(port, baud_divisor);

        // // Reenable interrupts if necessary
        // HAL_WRITE_UINT8(port+SER_16550_IER, ier);
    // }
    break;

    case __COMMCTL_GETBAUD:
        break;
    default:
        break;
    }
	
    CYGARC_HAL_RESTORE_GP();
    return ret;
} //*/


static int
cyg_hal_plf_serial_isr(void *__ch_data, int* __ctrlc, 
                       CYG_ADDRWORD __vector, CYG_ADDRWORD __data)
{
    int res = 0;
    cyg_uint8 _iir, c;
    channel_data_t* chan;
    CYGARC_HAL_SAVE_GP();

    // // Some of the diagnostic print code calls through here with no idea what the ch_data is.
    // // Go ahead and assume it is channels[0].
    // if (__ch_data == 0)
      // __ch_data = (void*)&channels[0];

    // chan = (channel_data_t*)__ch_data;

    // HAL_INTERRUPT_ACKNOWLEDGE(chan->isr_vector);

    // HAL_READ_UINT8(chan->base + SER_16550_IIR, _iir);
    // _iir &= SIO_IIR_ID_MASK;

    // *__ctrlc = 0;
    // if ((_iir == ISR_Rx_Avail) || (_iir == ISR_Rx_Char_Timeout)) {

        // HAL_READ_UINT8(chan->base + SER_16550_RBR, c);
    
        // if( cyg_hal_is_break( &c , 1 ) )
            // *__ctrlc = 1;

        res = CYG_ISR_HANDLED;
    // }

    CYGARC_HAL_RESTORE_GP();
    return res;
}

static void
cyg_hal_plf_serial_init(void)
{
    hal_virtual_comm_table_t* comm;
    int cur = CYGACC_CALL_IF_SET_CONSOLE_COMM(CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT);

    // Disable interrupts.
    HAL_INTERRUPT_MASK(channels[0].isr_vector);

       
    // Init channels
    cyg_hal_plf_serial_init_channel((void*)&channels[0]);

        
    // Setup procs in the vector table

    // Set channel 0
    CYGACC_CALL_IF_SET_CONSOLE_COMM(0);
    comm = CYGACC_CALL_IF_CONSOLE_PROCS();
    CYGACC_COMM_IF_CH_DATA_SET(*comm, &channels[0]);
    CYGACC_COMM_IF_WRITE_SET(*comm, cyg_hal_plf_serial_write);
    CYGACC_COMM_IF_READ_SET(*comm, cyg_hal_plf_serial_read);
    CYGACC_COMM_IF_PUTC_SET(*comm, cyg_hal_plf_serial_putc);
    CYGACC_COMM_IF_GETC_SET(*comm, cyg_hal_plf_serial_getc);
    CYGACC_COMM_IF_CONTROL_SET(*comm, cyg_hal_plf_serial_control);
    CYGACC_COMM_IF_DBG_ISR_SET(*comm, cyg_hal_plf_serial_isr);
    CYGACC_COMM_IF_GETC_TIMEOUT_SET(*comm, cyg_hal_plf_serial_getc_timeout); 
   
    // Restore original console
    CYGACC_CALL_IF_SET_CONSOLE_COMM(cur);

    // Disable interrupts.
    HAL_INTERRUPT_UNMASK(channels[0].isr_vector);

}

void
cyg_hal_plf_comms_init(void)
{
    static int initialized = 0;

    if (initialized)
        return;

    initialized = 1;


     
    cyg_hal_plf_serial_init();

}
//*/
//-----------------------------------------------------------------------------
// end of ser16c550c.c

