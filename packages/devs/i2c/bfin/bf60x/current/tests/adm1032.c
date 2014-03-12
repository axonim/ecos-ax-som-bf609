//==========================================================================
//
//        LM75a.c
//
//        Simple I2C test of I2C thermo sensor(lm75a) on AT91SAM9x board
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
// Author(s):     Pavel Azizov
// Company:       AXONIM
// Contributors:
// Date:          2013-08-27
// Description:   Simple I2C read/write test of I2C thermo sensor (adm1032)
//####DESCRIPTIONEND####


#include <pkgconf/system.h>

#include <cyg/infra/testcase.h>         // test macros
#include <cyg/infra/cyg_ass.h>          // assertion macros
#include <cyg/infra/diag.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/hal/hal_io.h>
// Package requirements
#if defined(CYGPKG_IO_I2C) && defined(CYGPKG_KERNEL)

#include <pkgconf/kernel.h>
#include <cyg/io/io.h>
#include <cyg/io/i2c.h>
#include <cyg/io/i2c_bfin_bf60x.h>

// Package option requirements
#if defined(CYGFUN_KERNEL_API_C)

#include <cyg/hal/hal_arch.h>           // CYGNUM_HAL_STACK_SIZE_TYPICAL
#include <cyg/kernel/kapi.h>

//extern cyg_i2c_bus cyg_i2c_at91sam9x_bus0;

typedef struct st_thread_data
{
    cyg_thread   obj;
    long         stack[CYGNUM_HAL_STACK_SIZE_TYPICAL];
    cyg_handle_t hdl;
} thread_data_t;

//-----------------------------------------------------------------------------
// I2C bus 1
// 
CYG_BFIN_BF60X_I2C_BUS(hal_bfin_bf60x_i2c0_bus,
                    0,	// Bus 0
                    4 );

CYG_I2C_DEVICE(cyg_i2c_bfin_bf60x_dev_adm1032, 
               &hal_bfin_bf60x_i2c0_bus, 0x4C, 0, 20000 /*25 kHz*/
              );

CYG_I2C_DEVICE(cyg_i2c_at91_dev_at24c02, 
               &hal_bfin_bf60x_i2c0_bus, 0x50, 0, 20000 /*25 kHz*/
              );

CYG_I2C_DEVICE(cyg_i2c_bfin_bf60x_dev_mcp23017, 
               &hal_bfin_bf60x_i2c0_bus, 0x21, 0, 20000 /*25 kHz*/
              );

cyg_thread_entry_t i2c0_thread;
thread_data_t      i2c0_thread_data;

#define WORK_WITH_MCP23017	(0)
#define WORK_WITH_ADM1032	(1)
#define WORK_WITH_EEPROM	(0)

void i2c0_thread(cyg_addrword_t data)
{
    cyg_uint16 i = 0;
    cyg_uint8 rx_data[300];
    cyg_uint8 tx_data[300];

    diag_printf("I2C test - AX-SOM-BF609\n");

    {
        diag_printf("Next step\n");

#if WORK_WITH_MCP23017
        tx_data[0] = 0x00; // IODIRA
        tx_data[1] = 0x00; // Setup pins out

        cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_mcp23017);
        cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_mcp23017, true, &tx_data[0], 2, true);
        cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_mcp23017);

        tx_data[0] = 0x14; // OLATA
        tx_data[1] = 0x80; // RMII_CLK_EN

        cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_mcp23017);
        cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_mcp23017, true, &tx_data[0], 2, true);
        cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_mcp23017);

        tx_data[0] = 0x00;
        rx_data[0] = 0;
        cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_mcp23017);
        cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_mcp23017, true, &tx_data[0], 1, true);
        cyg_i2c_transaction_rx(&cyg_i2c_bfin_bf60x_dev_mcp23017, true, &rx_data[0], 2, true, true);
        cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_mcp23017);

        diag_printf("OLATA 1 = %d\n", rx_data[0]);
        diag_printf("OLATA 2 = %d\n", rx_data[1]);
#endif        

#if WORK_WITH_EEPROM
        cyg_i2c_transaction_begin(&cyg_i2c_at91_dev_at24c02);
        tx_data[0] = 0;	//Address 0
        cyg_i2c_transaction_tx(&cyg_i2c_at91_dev_at24c02, true, tx_data, 1, true);
        cyg_i2c_transaction_rx(&cyg_i2c_at91_dev_at24c02, true, rx_data, 1, true, true);
        cyg_i2c_transaction_end(&cyg_i2c_at91_dev_at24c02);

        diag_printf("Raw data EEPROM  = %02x \n", rx_data[0]);
#endif

#if WORK_WITH_ADM1032
        tx_data[0] = 0x04;
        tx_data[1] = 0x05;
        tx_data[2] = 0x06;
        tx_data[3] = 0x07;
        tx_data[4] = 0x08;

        cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_adm1032);
        cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &tx_data[0], 1, true);
        cyg_i2c_transaction_rx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &rx_data[0], 1, true, true);
        cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_adm1032);
        cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_adm1032);
        cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &tx_data[1], 1, true);
        cyg_i2c_transaction_rx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &rx_data[1], 1, true, true);
        cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_adm1032);
        cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_adm1032);
        cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &tx_data[2], 1, true);
        cyg_i2c_transaction_rx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &rx_data[2], 1, true, true);
        cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_adm1032);
        cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_adm1032);
        cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &tx_data[3], 1, true);
        cyg_i2c_transaction_rx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &rx_data[3], 1, true, true);
        cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_adm1032);
        cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_adm1032);
        cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &tx_data[4], 1, true);
        cyg_i2c_transaction_rx(&cyg_i2c_bfin_bf60x_dev_adm1032, true, &rx_data[4], 1, true, true);
        cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_adm1032);

        diag_printf("Raw data: %02x\n", rx_data[0]);
        diag_printf("Raw data: %02x\n", rx_data[1]);
        diag_printf("Raw data: %02x\n", rx_data[2]);
        diag_printf("Raw data: %02x\n", rx_data[3]);
        diag_printf("Raw data: %02x\n", rx_data[4]);
#endif

        cyg_thread_delay(100);
    }
    CYG_TEST_PASS_FINISH("I2C test OK");
}

void
cyg_start(void)
{
    CYG_TEST_INIT();

    cyg_thread_create(4, i2c0_thread,
                      (cyg_addrword_t) 0,
                      "i2c0_thread",
                      (void *) i2c0_thread_data.stack,
                      1024 * sizeof(long),
                      &i2c0_thread_data.hdl,
                      &i2c0_thread_data.obj);

    cyg_thread_resume(i2c0_thread_data.hdl);
    cyg_scheduler_start();
}

#else // CYGFUN_KERNEL_API_C
#define N_A_MSG "Needs kernel C API"
#endif

#else // CYGPKG_IO_CAN && CYGPKG_KERNEL
#define N_A_MSG "Needs I2C support and Kernel"
#endif

#ifdef N_A_MSG
void
cyg_start( void )
{
    CYG_TEST_INIT();
    CYG_TEST_NA( N_A_MSG);
}
#endif // N_A_MSG

// eof i2c_test.c