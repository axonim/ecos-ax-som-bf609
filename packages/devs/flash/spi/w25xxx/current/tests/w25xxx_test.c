//=============================================================================
//
//      m25pxx_test.c
//
//      SPI flash driver tests for Numonyx M25Pxx devices and compatibles.
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
// Author(s):   Chris Holgate
// Date:        2008-12-22
// Purpose:     Numonyx M25Pxx SPI flash driver stress tests.
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/system.h>

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/testcase.h>           // Test macros
#include <cyg/infra/cyg_ass.h>            // Assertion macros
#include <cyg/infra/diag.h>               // Diagnostic output

#include <cyg/hal/hal_arch.h>             // CYGNUM_HAL_STACK_SIZE_TYPICAL
#include <cyg/hal/hal_if.h>
#include <cyg/kernel/kapi.h>

#include <cyg/io/spi.h>
#include <cyg/io/flash.h>
#include <cyg/io/redboot.ldr.h>
#include <cyg/io/spi_bf60x.h>
#include <string.h>

externC cyg_spi_bf60x_bus_t cyg_spi_bf60x_bus0, cyg_spi_bf60x_bus1;

#define W25XXX_TEST_DEBUG

#ifdef W25XXX_TEST_DEBUG
    #define W25XXX_DEBUG(args...)     diag_printf(args)
#else
    #define W25XXX_DEBUG(args...)
#endif

#define CYGPKG_ADSP_BF609_EZ_KIT

#if defined(CYGPKG_IO_I2C) && defined(CYGPKG_ADSP_BF609_EZ_KIT)
#include <cyg/io/io.h>
#include <cyg/io/i2c.h>
#include <cyg/io/i2c_bfin_bf60x.h>
#include <cyg/io/w25xxx.h>

CYG_BFIN_BF60X_I2C_BUS(hal_bfin_bf60x_i2c0_bus,
                    0,	// Bus 0
                    4 );

CYG_I2C_DEVICE(cyg_i2c_bfin_bf60x_dev_mcp23017, 
               &hal_bfin_bf60x_i2c0_bus, 0x22, 0, 20000 /*25 kHz*/
              );
              
cyg_spi_bf60x_device_t w25xxx_spi_device = {
    .spi_device.spi_bus = &cyg_spi_bf60x_bus0.spi_bus,
    .dev_num = 1 ,                      // Only 1 device. 
    .cl_pol = 0,
    .cl_pha = 0,
    .cl_brate = 45000000,                // Nominal 45Mhz.
    .cs_up_udly = 1,
    .cs_dw_udly = 1,
    .tr_bt_udly = 1,
};

CYG_DEVS_FLASH_SPI_W25XXX_DRIVER(
    m25pxx_flash_device, 0, &(w25xxx_spi_device.spi_device)
);
#endif

#include CYGHWR_MEMORY_LAYOUT_H

//-----------------------------------------------------------------------------
// Thread data structures.

static cyg_uint32 stack [CYGNUM_HAL_STACK_SIZE_TYPICAL / 4];
static cyg_thread thread_data;
static cyg_handle_t thread_handle;

//-----------------------------------------------------------------------------
// Data transfer buffers.

#define BUF_SIZE 1024

static cyg_uint8 wbuf [BUF_SIZE];
static cyg_uint8 rbuf [BUF_SIZE];

//-----------------------------------------------------------------------------
// Print out version information.

void do_version (void)
{
    char *version = CYGACC_CALL_IF_MONITOR_VERSION();
    if (version != NULL) diag_printf("%s", version);
#ifdef HAL_PLATFORM_CPU
    diag_printf("Platform: %s (%s) %s\n", HAL_PLATFORM_BOARD, HAL_PLATFORM_CPU, HAL_PLATFORM_EXTRA);
#endif
    diag_printf("RAM: %p-%p, ", (void*)(CYGMEM_REGION_ram), (void*)(CYGMEM_REGION_ram + CYGMEM_REGION_ram_SIZE - 1));
    diag_printf("(HEAP: %p-%p)\n", (void*)(CYGMEM_SECTION_heap1), (void*)(CYGMEM_SECTION_heap1 + CYGMEM_SECTION_heap1_SIZE - 1));
}

void write_redboot(void)
{
	cyg_flashaddr_t err_addr, base_addr;
	int status;
	cyg_uint32 i, j, r_sz, sz;
	
	diag_printf ("Function write_redboot: writes and verifies redboot.\n");
	base_addr = 0;
	
    // Erase block.
    status = cyg_flash_erase (base_addr, sizeof(redboot_ldr), &err_addr);
    if (status != FLASH_ERR_OK) {
        diag_printf ("Flash erase error : %s\n", cyg_flash_errmsg (status));
    }

	status = cyg_flash_program (0, redboot_ldr, sizeof(redboot_ldr), &err_addr);
	if (status != FLASH_ERR_OK) {
        diag_printf ("Flash write error : %s\n", cyg_flash_errmsg (status));
    }
	sz = sizeof(redboot_ldr);
	base_addr = 0;
	while (sz) {
		if (sz < BUF_SIZE) {
			r_sz = sz;
		} else {
			r_sz = BUF_SIZE;
		}
		sz -= r_sz;
		status = cyg_flash_read (base_addr, rbuf, r_sz, &err_addr);
		if (status != FLASH_ERR_OK) {
			diag_printf ("Flash read error : %s\n", cyg_flash_errmsg (status));
		} else if (memcmp (rbuf, redboot_ldr + base_addr, r_sz) != 0){
			for (i = base_addr; i < r_sz; i++) {
				if (rbuf[i] != redboot_ldr[i]) {
					diag_printf ("Flash read data corruption at %d\n", i);
					diag_printf ("%d : %x - %x\n", i, redboot_ldr[i], rbuf[i]);
				}
			}
			
		}
		base_addr += r_sz;
	}
	
	
}

//-----------------------------------------------------------------------------
// Run checkerboard and inverse checkerboard writes over each sector in turn.

cyg_uint32 run_all_tests_1
  (void)
{
    int status;
    cyg_uint32 i, j;
    cyg_uint32 errors = 0;
    cyg_flashaddr_t base_addr, err_addr;

    //FIXME
	diag_printf ("Test 1 - write and verify checkerboard and inverse checkerboard.\n");
    diag_printf ("m25pxx_flash_device.block_info->blocks = %d\n", m25pxx_flash_device.block_info->blocks);
    
    // Iterate over all flash sectors.
    for (i = 0; i < m25pxx_flash_device.block_info->blocks; i++) {
        base_addr =  m25pxx_flash_device.start + (i * m25pxx_flash_device.block_info->block_size);

        // Erase block.
        status = cyg_flash_erase (base_addr, 1, &err_addr);
        if (status != FLASH_ERR_OK) {
            diag_printf ("Flash erase error : %s\n", cyg_flash_errmsg (status));
            errors ++;
        }

        // Set up buffer with checkerboard.
        for (j = 0; j < BUF_SIZE;) {
           wbuf [j++] = 0x55;
           wbuf [j++] = 0xAA;
        }

        // Write the checkerboard to the entire sector.
        for (j = 0; j < m25pxx_flash_device.block_info->block_size; j += BUF_SIZE) {
            status = cyg_flash_program (base_addr + j, wbuf, BUF_SIZE, &err_addr);
            if (status != FLASH_ERR_OK) {
                diag_printf ("Flash write error : %s\n", cyg_flash_errmsg (status));
                errors ++;
            }
        }

        // Read back the checkerboard and verify.
        for (j = 0; j < m25pxx_flash_device.block_info->block_size; j += BUF_SIZE) {
            status = cyg_flash_read (base_addr + j, rbuf, BUF_SIZE, &err_addr);
            if (status != FLASH_ERR_OK) {
                diag_printf ("Flash read error : %s\n", cyg_flash_errmsg (status));
                errors ++;
            }
            else if (memcmp (rbuf, wbuf, BUF_SIZE) != 0) {
                diag_printf ("Flash read data corruption (0x%08X - 0x%08X).\n", base_addr + j, base_addr + j + BUF_SIZE - 1);
                errors ++;
            }
        }

        // Erase block.
        status = cyg_flash_erase (base_addr, 1, &err_addr);
        if (status != FLASH_ERR_OK) {
            diag_printf ("Flash erase error : %s\n", cyg_flash_errmsg (status));
            errors ++;
        }

        // Set up buffer with inverse checkerboard.
        for (j = 0; j < BUF_SIZE;) {
           wbuf [j++] = 0xAA;
           wbuf [j++] = 0x55;
        }

        // Write the checkerboard to the entire sector.
        for (j = 0; j < m25pxx_flash_device.block_info->block_size; j += BUF_SIZE) {
            status = cyg_flash_program (base_addr + j, wbuf, BUF_SIZE, &err_addr);
            if (status != FLASH_ERR_OK) {
                diag_printf ("Flash write error : %s\n", cyg_flash_errmsg (status));
                errors ++;
            }
        }
		memset(rbuf, 0, BUF_SIZE);
        // Read back the checkerboard and verify.
        for (j = 0; j < m25pxx_flash_device.block_info->block_size; j += BUF_SIZE) {
            status = cyg_flash_read (base_addr + j, rbuf, BUF_SIZE, &err_addr);
            if (status != FLASH_ERR_OK) {
                diag_printf ("Flash read error : %s\n", cyg_flash_errmsg (status));
                errors ++;
            }
            else if (memcmp (rbuf, wbuf, BUF_SIZE) != 0) {
                diag_printf ("Flash read data corruption (0x%08X - 0x%08X).\n", base_addr + j, base_addr + j + BUF_SIZE - 1);
                errors ++;
            }
        }
    }
    return errors;
}

//-----------------------------------------------------------------------------
// Write and verify counting sequence over all device.

cyg_uint32 run_all_tests_2
  (void)
{
    int status;
    cyg_uint32 i, j;
    cyg_uint32 errors = 0;
    cyg_flashaddr_t base_addr, err_addr;

    diag_printf ("Test 2 - write and verify counting sequence.\n");
    diag_printf ("m25pxx_flash_device.block_info->blocks = %d\n", m25pxx_flash_device.block_info->blocks);
    
    // Erase all flash sectors.
    for (i = 0; i < m25pxx_flash_device.block_info->blocks; i++) {
        base_addr =  m25pxx_flash_device.start + (i * m25pxx_flash_device.block_info->block_size);

        // Erase block.
        status = cyg_flash_erase (base_addr, 1, &err_addr);
        if (status != FLASH_ERR_OK) {
            diag_printf ("Flash erase error : %s\n", cyg_flash_errmsg (status));
            errors ++;
        }
    }

    // Write counting sequence.
    base_addr =  m25pxx_flash_device.start;
    for (i = 0; i < (m25pxx_flash_device.end - m25pxx_flash_device.start) / 4;) {
        for (j = 0; j < BUF_SIZE;) {
            wbuf [j++] = (cyg_uint8) ((i >> 0) & 0xFF);
            wbuf [j++] = (cyg_uint8) ((i >> 8) & 0xFF);
            wbuf [j++] = (cyg_uint8) ((i >> 16) & 0xFF);
            wbuf [j++] = (cyg_uint8) ((i >> 24) & 0xFF);            
            i++;
        }
        status = cyg_flash_program (base_addr, wbuf, BUF_SIZE, &err_addr);
        if (status != FLASH_ERR_OK) {
            diag_printf ("Flash write error : %s\n", cyg_flash_errmsg (status));
            errors ++;
        }
        base_addr += BUF_SIZE;
    }

    // Verify counting sequence.
    base_addr = m25pxx_flash_device.start;
    for (i = 0; i < (m25pxx_flash_device.end - m25pxx_flash_device.start) / 4;) {
        for (j = 0; j < BUF_SIZE;) {
            wbuf [j++] = (cyg_uint8) ((i >> 0) & 0xFF);
            wbuf [j++] = (cyg_uint8) ((i >> 8) & 0xFF);
            wbuf [j++] = (cyg_uint8) ((i >> 16) & 0xFF);
            wbuf [j++] = (cyg_uint8) ((i >> 24) & 0xFF);            
            i++;
        }
        status = cyg_flash_read (base_addr, rbuf, BUF_SIZE, &err_addr);
        if (status != FLASH_ERR_OK) {
            diag_printf ("Flash read error : %s\n", cyg_flash_errmsg (status));
            errors ++;
        }
        else if (memcmp (rbuf, wbuf, BUF_SIZE) != 0) {
            diag_printf ("Flash read data corruption (0x%08X - 0x%08X).\n", base_addr, base_addr + BUF_SIZE - 1);
            errors ++;
        }
        base_addr += BUF_SIZE;
    }
    return errors;
}

//-----------------------------------------------------------------------------
// Perform non-aligned buffer read/write tests, spanning sector boundaries.

cyg_uint32 run_all_tests_3
  (void)
{
    int status;
    cyg_uint32 i, count;
    cyg_uint32 errors = 0;
    cyg_flashaddr_t base_addr, err_addr;

    diag_printf ("Test 3 - test non-aligned writes and reads.\n");
    
    // Fill the write buffer with a basic counting sequence.
    count = 0;
    for (i = 0; i < BUF_SIZE;) {
        wbuf [i++] = (cyg_uint8) ((count >> 0) & 0xFF);
        wbuf [i++] = (cyg_uint8) ((count >> 8) & 0xFF);
        count++;
    }

    // Assuming 256 byte pages gives 256 possible alignments.
    base_addr = m25pxx_flash_device.start + m25pxx_flash_device.block_info->block_size;
    for (i = 1; i <= 256; i++) {

        // Erase sectors either side of sector boundary.
        status = cyg_flash_erase (base_addr - 1, 1, &err_addr);
        if (status != FLASH_ERR_OK) {
            diag_printf ("Flash erase error : %s\n", cyg_flash_errmsg (status));
            errors ++;
        }
        status = cyg_flash_erase (base_addr, 1, &err_addr);
        if (status != FLASH_ERR_OK) {
            diag_printf ("Flash erase error : %s\n", cyg_flash_errmsg (status));
            errors ++;
        }

        // Write data spanning sector boundary.        
        status = cyg_flash_program (base_addr - i, wbuf, BUF_SIZE, &err_addr);
        if (status != FLASH_ERR_OK) {
            diag_printf ("Flash write error : %s\n", cyg_flash_errmsg (status));
            errors ++;
        }

        // Verify data spanning sector boundary.
        status = cyg_flash_read (base_addr - i, rbuf, BUF_SIZE, &err_addr);
        if (status != FLASH_ERR_OK) {
            diag_printf ("Flash read error : %s\n", cyg_flash_errmsg (status));
            errors ++;
        }
        else if (memcmp (rbuf, wbuf, BUF_SIZE) != 0) {
            diag_printf ("Flash read data corruption (0x%08X - 0x%08X).\n", base_addr - i, base_addr - i + BUF_SIZE - 1);
            errors ++;
        }

        // Use next sector boundary for next test.
        base_addr += m25pxx_flash_device.block_info->block_size;
        if (base_addr >= m25pxx_flash_device.end)
            base_addr = m25pxx_flash_device.start + m25pxx_flash_device.block_info->block_size;
    }
    return errors;
}

//-----------------------------------------------------------------------------
// Run all M25Pxx SPI interface loopback tests.

void run_all_tests 
    (void)
{
    cyg_uint32 errors = 0;

    // Check that the device is intialised.
    if (m25pxx_flash_device.start == m25pxx_flash_device.end) {
        diag_printf ("M25Pxx device not initialised.\n");
        errors ++;
        goto out;
    }

    // Run the tests.

    //errors += run_all_tests_1 ();
    errors += run_all_tests_2 ();
    //errors += run_all_tests_3 ();

out:
    diag_printf ("----\nTests complete - detected %d errors in total.\n", errors);
    if (errors == 0) {
    	diag_printf ("M25Pxx driver tests ran OK.");
    	//write_redboot();
    	CYG_TEST_PASS_FINISH ("----\nRedboot loaded\n");
    } else {
        CYG_TEST_FAIL_FINISH ("M25Pxx driver test FAILED.");
    }
}

//-----------------------------------------------------------------------------
// User startup - tests are run in their own thread.

void cyg_user_start
    (void)
{
#if defined(CYGPKG_IO_I2C) && defined(CYGPKG_ADSP_BF609_EZ_KIT)
    cyg_uint8 tx_data[4];
    cyg_uint32 result;
    diag_printf ("----\nSPI FLASH CS ENABLED.\n");
    tx_data[0] = 0x01; // IODIRB
    tx_data[1] = ~(1 << 4); // SPIFLASH_CS_EN OUT

    cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_mcp23017);
    result |= cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_mcp23017, true, &tx_data[0], 2, true);
    cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_mcp23017);
    
    tx_data[0] = 0x15; // OLATB
    tx_data[1] = ~(1 << 4); // SPIFLASH_CS_EN
    
    cyg_i2c_transaction_begin(&cyg_i2c_bfin_bf60x_dev_mcp23017);
    result |= cyg_i2c_transaction_tx(&cyg_i2c_bfin_bf60x_dev_mcp23017, true, &tx_data[0], 2, true);
    cyg_i2c_transaction_end(&cyg_i2c_bfin_bf60x_dev_mcp23017);
#endif

    do_version();
    diag_printf ("----\nChecking for W25xxx compatible devices.\n");
    cyg_flash_set_global_printf((cyg_flash_printf *)&diag_printf);
    if (cyg_flash_init(NULL) == CYG_FLASH_ERR_OK) {
		CYG_TEST_INIT();
		cyg_thread_create(
			10,                                   // Arbitrary priority
			(cyg_thread_entry_t*) run_all_tests,      // Thread entry point
			0,                                    // 
			"test_thread",                        // Thread name
			(cyg_uint8*) stack,                   // Stack
			CYGNUM_HAL_STACK_SIZE_TYPICAL,        // Stack size
			&thread_handle,                       // Thread handle
			&thread_data                          // Thread data structure
		);
		cyg_thread_resume(thread_handle);
		cyg_scheduler_start();
	} else {
		while (1);
	}
}

//=============================================================================
