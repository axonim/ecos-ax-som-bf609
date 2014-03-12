//=============================================================================
//
//      m25p05.c
//
//      Standalone SPI loopback test.
//
//=============================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 2008, 2009 Free Software Foundation, Inc.
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   Axonim
// Date:        2008-11-27
// Purpose:     BF609 SPI M25P05 test
//
//####DESCRIPTIONEND####

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/testcase.h>         // Test macros
#include <cyg/infra/cyg_ass.h>          // Assertion macros
#include <cyg/infra/diag.h>             // Diagnostic output

#include <cyg/hal/hal_arch.h>           // CYGNUM_HAL_STACK_SIZE_TYPICAL
#include <cyg/kernel/kapi.h>

#include <cyg/io/spi.h>                 // Common SPI API
#include <cyg/io/spi_bf60x.h>            // bf60x data structures

#if defined(CYGPKG_IO_I2C) && defined(CYGPKG_HAL_BFIN_ADSP_BF609EZ)
#include <cyg/io/io.h>
#include <cyg/io/i2c.h>
#include <cyg/io/i2c_bfin_bf60x.h>
#endif

#include <string.h>


//---------------------------------------------------------------------------
// Thread data structures.

cyg_uint8 stack [CYGNUM_HAL_STACK_SIZE_TYPICAL];
cyg_thread thread_data;
cyg_handle_t thread_handle;

externC cyg_spi_bf60x_bus_t cyg_spi_bf60x_bus0, cyg_spi_bf60x_bus1;

#if defined(CYGPKG_IO_I2C) && defined(CYGPKG_HAL_BFIN_ADSP_BF609EZ)
CYG_BFIN_BF60X_I2C_BUS(hal_bfin_bf60x_i2c0_bus,
                    0,	// Bus 0
                    4 );

CYG_I2C_DEVICE(cyg_i2c_bfin_bf60x_dev_mcp23017, 
               &hal_bfin_bf60x_i2c0_bus, 0x22, 0, 20000 /*25 kHz*/
              );
#endif
              
//---------------------------------------------------------------------------
// SPI loopback device driver data structures.

cyg_spi_bf60x_device_t loopback_device = {
    .spi_device.spi_bus = &cyg_spi_bf60x_bus0.spi_bus,
    .dev_num = 1 ,                      // Only 1 device. 
    .cl_pol = 0,
    .cl_pha = 0,
    .cl_brate = 45000000,                // Nominal 45Mhz.
    .cs_up_udly = 1,
    .cs_dw_udly = 1,
    .tr_bt_udly = 1,
};

//---------------------------------------------------------------------------

char tx_data[] = "Testing, testing, 12, 123.";
char tx_data1[] = "Testing extended API...";
char tx_data2[] = "Testing extended API for a second transaction.";


#define CMD_WREN 0x06
#define CMD_WRDI 0x04
#define CMD_RDID 0x9F

#define M25PXX_POLLED true

//const char tx_wren[] = {}; // write enable 98
//const char tx_wrdi[] = {0x04}; // write disable 80
//const char tx_[] = {0xc0}; // read cmd + 6 bit addr

#define M25PXX_CMD_WREN  0x06  // Write enable.
#define M25PXX_CMD_READ  0x03  // Read data.
#define M25PXX_CMD_PP    0x02  // Page program.
#define M25PXX_DELAY_MS(_msdelay_) cyg_thread_delay (\
  1 + ((1000 * _msdelay_ * CYGNUM_HAL_RTC_DENOMINATOR) / (CYGNUM_HAL_RTC_NUMERATOR / 1000)))

char rx_data [sizeof(tx_data)];
char rx_data1 [sizeof(tx_data1)];
char rx_data2 [sizeof(tx_data2)];

//-----------------------------------------------------------------------------
// Send write enable command.

static inline void m25pxx_spi_wren(void)
{
  cyg_uint8 tx_buf [1] = { M25PXX_CMD_WREN };
  cyg_spi_transfer (&loopback_device.spi_device, M25PXX_POLLED, 1, tx_buf, NULL);
}

//-----------------------------------------------------------------------------
// Program a single page.

static inline void m25pxx_spi_pp(cyg_uint32 addr, cyg_uint8* wbuf, cyg_uint32 wbuf_len)
{
  cyg_uint8 tx_buf [4] = { M25PXX_CMD_PP,
    (cyg_uint8) (addr >> 16), (cyg_uint8) (addr >> 8), (cyg_uint8) (addr) };

  // Implement the program operation as a multistage SPI transaction.
  cyg_spi_transaction_begin (&loopback_device.spi_device);
  cyg_spi_transaction_transfer (&loopback_device.spi_device, M25PXX_POLLED, 4, tx_buf, NULL, false);
  cyg_spi_transaction_transfer (&loopback_device.spi_device, M25PXX_POLLED, wbuf_len, wbuf, NULL, false);
  cyg_spi_transaction_end (&loopback_device.spi_device);
}

//-----------------------------------------------------------------------------
// Implement fast reads to the specified buffer.
static inline void m25pxx_spi_fread
  (cyg_uint32 addr, cyg_uint8* rbuf, cyg_uint32 rbuf_len)
{
  cyg_uint8 tx_buf [5] = { M25PXX_CMD_READ,
    (cyg_uint8) (addr >> 16), (cyg_uint8) (addr >> 8), (cyg_uint8) (addr) };

  // Implement the read operation as a multistage SPI transaction.
  cyg_spi_transaction_begin (&loopback_device.spi_device);
  cyg_spi_transaction_transfer (&loopback_device.spi_device, M25PXX_POLLED, 4, tx_buf, NULL, false);
  cyg_spi_transaction_transfer (&loopback_device.spi_device, M25PXX_POLLED, rbuf_len, NULL, rbuf, false);
  cyg_spi_transaction_end (&loopback_device.spi_device);
}

//---------------------------------------------------------------------------
// Run single loopback transaction using simple transfer API call.

void run_test_1 (cyg_bool polled)
{
//    diag_printf ("Test 1 : Simple transfer test ).\n");
    tx_data[0] = CMD_WREN;
    
    //while(1)
    //{
//	cyg_spi_transfer (&loopback_device.spi_device, 0, 1,
//	    (const cyg_uint8*) tx_data, (cyg_uint8*) rx_data);
//    }
//    diag_printf ("1    Rx data : %s\n", rx_data);
    tx_data[0] = CMD_RDID;
    tx_data[1] = 0x0;
    tx_data[2] = 0x0;
    tx_data[3] = 0x0;
    tx_data[4] = 0x0;

    //while(1){
	cyg_spi_transfer (&loopback_device.spi_device, polled, 5, 
	    (const cyg_uint8*) tx_data, (cyg_uint8*) rx_data);
	diag_printf("2    RX DATA : 0x%x,0x%x,0x%x,0x%x,0x%x\n", rx_data[0], rx_data[1], rx_data[2], rx_data[3],rx_data[4]);
    //}
//    diag_printf ("2    Rx data : %s\n", rx_data);

}

//---------------------------------------------------------------------------
// Run two loopback transactions using extended transfer API.

void run_test_2 (cyg_bool polled)
{
	m25pxx_spi_wren();
	m25pxx_spi_pp(0, tx_data1, sizeof(tx_data1));
	M25PXX_DELAY_MS(500);
	m25pxx_spi_fread(0, rx_data1, sizeof(rx_data1));
	diag_printf ("    Tx data 1 : %s\n", tx_data1);
    diag_printf ("    Rx data 1 : %s\n", rx_data1);
}

//---------------------------------------------------------------------------
// Run all PL022 SPI interface loopback tests.

void run_tests (void)
{
#if defined(CYGPKG_IO_I2C) && defined(CYGPKG_HAL_BFIN_ADSP_BF609EZ)
    cyg_uint8 tx_data[4];
    cyg_uint32 result;

    tx_data[0] = 0x01; // IODIRB
    tx_data[1] = ~(1 << 4); // SPIFLASH_CS_EN OUT

    cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_mcp23017);
    result |= cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_mcp23017, true, &tx_data[0], 2, true);
    cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_mcp23017);
    
    tx_data[0] = 0x15; // OLATB
    tx_data[1] = ~(1 << 4); // SPIFLASH_CS_EN
#endif
    
    diag_printf ("Running BF609 SPI driver tests.\n");
    run_test_1 (true); 
    //run_test_1 (false); 
    run_test_2 (true); 
    //run_test_2 (false); 
    CYG_TEST_PASS_FINISH ("SPI driver tests ran OK");
}

//---------------------------------------------------------------------------
// User startup - tests are run in their own thread.

void cyg_user_start(void)
{
    CYG_TEST_INIT();
    cyg_thread_create(
        10,                                   // Arbitrary priority
        (cyg_thread_entry_t*) run_tests,      // Thread entry point
        0,                                    // 
        "test_thread",                        // Thread name
        &stack[0],                            // Stack
        CYGNUM_HAL_STACK_SIZE_TYPICAL,        // Stack size
        &thread_handle,                       // Thread handle
        &thread_data                          // Thread data structure
    );
    cyg_thread_resume(thread_handle);
    cyg_scheduler_start();
}

//=============================================================================
