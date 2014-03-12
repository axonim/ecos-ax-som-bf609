//==========================================================================
//
//      plf_misc.c
//
//      HAL platform miscellaneous functions
//
//==========================================================================
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
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Contributors: Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:         2013-08-10
// Purpose:      HAL miscellaneous functions
// Description:  This file contains miscellaneous functions provided by the
//               HAL.
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#include <pkgconf/hal.h>
#include <pkgconf/system.h>
#ifdef CYGPKG_DEVS_ETH_BF60X
#include <pkgconf/devs_eth_bf60x.h>
#endif
#ifdef CYGPKG_DEVS_SPI_BFIN_BF60x
#include <pkgconf/devs_spi_bfin_bf60x.h>
#endif
#ifdef CYGPKG_DEVS_PPI_BFIN_BF60X
#include <pkgconf/devs_ppi_bfin_bf60x.h>
#endif

#include <cyg/infra/cyg_type.h>         // Base types
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros

#include <cyg/hal/hal_arch.h>           // architectural definitions

#include <cyg/hal/hal_intr.h>           // Interrupt handling

#include <cyg/hal/hal_cache.h>          // Cache handling

#include <cyg/hal/hal_if.h>
#include <cyg/hal/bfin-regs.h>

#include <cyg/hal/adibf609.h>

void hal_plf_gpio_init(void);
void hal_plf_eppi_gpio_init(void);
void hal_plf_program_clocks(void) __attribute__ ((section (".init")));
void hal_plf_program_memory_controller(void) __attribute__ ((section (".init")));

#ifndef CONFIG_CGU_CTL_VAL
# define CONFIG_CGU_CTL_VAL ((CONFIG_VCO_MULT << 8) | CONFIG_CLKIN_HALF)
#endif

#ifndef CONFIG_CGU_DIV_VAL
# define CONFIG_CGU_DIV_VAL \
	((CONFIG_CCLK_DIV   << CSEL_P)   | \
	 (CONFIG_SCLK0_DIV  << S0SEL_P)  | \
	 (CONFIG_SCLK_DIV << SYSSEL_P) | \
	 (CONFIG_SCLK1_DIV  << S1SEL_P)  | \
	 (CONFIG_DCLK_DIV   << DSEL_P)   | \
	 (CONFIG_OCLK_DIV   << OSEL_P))
#endif

#define CONFIG_BFIN_GET_DCLK_M 		(CONFIG_BFIN_GET_DCLK/1000000)

#ifndef CONFIG_DMC_DDRCFG
#if ((CONFIG_BFIN_GET_DCLK_M != 125) && \
	(CONFIG_BFIN_GET_DCLK_M != 133) && \
	(CONFIG_BFIN_GET_DCLK_M != 150) && \
	(CONFIG_BFIN_GET_DCLK_M != 166) && \
	(CONFIG_BFIN_GET_DCLK_M != 200) && \
	(CONFIG_BFIN_GET_DCLK_M != 225) && \
	(CONFIG_BFIN_GET_DCLK_M != 250))
#error "DDR2 CLK must be in (125, 133, 150, 166, 200, 225, 250)MHz"
#endif
#endif

struct ddr_config {
	cyg_uint32 ddr_clk;
	cyg_uint32 dmc_ddrctl;
	cyg_uint32 dmc_ddrcfg;
	cyg_uint32 dmc_ddrtr0;
	cyg_uint32 dmc_ddrtr1;
	cyg_uint32 dmc_ddrtr2;
	cyg_uint32 dmc_ddrmr;
	cyg_uint32 dmc_ddrmr1;
    cyg_uint32 dmc_ddrphyctl3;
    cyg_uint32 dmc_ddrphyctl1;
};

static struct ddr_config ddr_config_table[] = {
	[0] = {
		.ddr_clk    = 125, 	/* 125MHz */
		.dmc_ddrctl = 0x00000904,
		.dmc_ddrcfg = 0x00000422,
		.dmc_ddrtr0 = 0x20705212,
		.dmc_ddrtr1 = 0x201003CF,
		.dmc_ddrtr2 = 0x00320107,
		.dmc_ddrmr  = 0x00000422,
		.dmc_ddrmr1 = 0x4,
        .dmc_ddrphyctl3 = 0x050000C4,
	},
	[1] = {
		.ddr_clk    = 133, 	/* 133MHz */
		.dmc_ddrctl = 0x00000904,
		.dmc_ddrcfg = 0x00000422,
		.dmc_ddrtr0 = 0x20806313,
		.dmc_ddrtr1 = 0x2013040D,
		.dmc_ddrtr2 = 0x00320108,
		.dmc_ddrmr  = 0x00000632,
		.dmc_ddrmr1 = 0x4,
        .dmc_ddrphyctl3 = 0x050000C4,
	},
	[2] = {
		.ddr_clk    = 150, 	/* 150MHz */
		.dmc_ddrctl = 0x00000904,
		.dmc_ddrcfg = 0x00000422,
		.dmc_ddrtr0 = 0x20A07323,
		.dmc_ddrtr1 = 0x20160492,
		.dmc_ddrtr2 = 0x00320209,
		.dmc_ddrmr  = 0x00000632,
		.dmc_ddrmr1 = 0x4,
        .dmc_ddrphyctl3 = 0x050000C4,
	},
	[3] = {
		.ddr_clk    = 166, 	/* 166MHz */
		.dmc_ddrctl = 0x00000904,
		.dmc_ddrcfg = 0x00000422,
		.dmc_ddrtr0 = 0x20A07323,
		.dmc_ddrtr1 = 0x2016050E,
		.dmc_ddrtr2 = 0x00320209,
		.dmc_ddrmr  = 0x00000632,
		.dmc_ddrmr1 = 0x4,
	},
	[4] = {
		.ddr_clk    = 200, 	/* 200MHz */
		.dmc_ddrctl = 0x00000904,
		.dmc_ddrcfg = 0x00000422,
		.dmc_ddrtr0 = 0x20a07323,
		.dmc_ddrtr1 = 0x2016050f,
		.dmc_ddrtr2 = 0x00320509,
		.dmc_ddrmr  = 0x00000632,
		.dmc_ddrmr1 = 0x4,
        .dmc_ddrphyctl3 = 0x050000C4,
	},
	[5] = {
		.ddr_clk    = 225, 	/* 225MHz */
		.dmc_ddrctl = 0x00000904,
		.dmc_ddrcfg = 0x00000422,
		.dmc_ddrtr0 = 0x20E0A424,
		.dmc_ddrtr1 = 0x302006DB,
		.dmc_ddrtr2 = 0x0032020D,
		.dmc_ddrmr  = 0x00000842,
		.dmc_ddrmr1 = 0x4,
        .dmc_ddrphyctl3 = 0x050000C4,
	},
	[6] = {
		.ddr_clk    = 250, 	/* 250MHz */
		.dmc_ddrctl = 0x00000904,
		.dmc_ddrcfg = 0x00000422,
		.dmc_ddrtr0 = 0x20E0A424,
		.dmc_ddrtr1 = 0x3020079E,
		.dmc_ddrtr2 = 0x0032020D,
		.dmc_ddrmr  = 0x00000842,
		.dmc_ddrmr1 = 0x4,
        .dmc_ddrphyctl3 = 0x050000C4,
        .dmc_ddrphyctl1 = 0,
	},
};

extern unsigned long _ram_data_start;
extern unsigned long _ram_data_end;
extern unsigned long _rom_data_start;
extern unsigned long _rom_system_data_start;
extern unsigned long _START_OF_HAL;
extern unsigned long _END_OF_HAL;
extern unsigned long _START_OF_SYSTEM_DATA;
extern unsigned long _END_OF_SYSTEM_DATA;

void hal_platform_init(void)
{
	// Set up eCos/ROM interfaces
    hal_if_init();

    hal_plf_gpio_init();
	hal_plf_eppi_gpio_init();
    
#if CYGSEM_HAL_ENABLE_ICACHE_ON_STARTUP
    HAL_ICACHE_ENABLE();
#else
    HAL_ICACHE_DISABLE();
#endif


#if CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
    HAL_DCACHE_ENABLE();
#else
    HAL_DCACHE_DISABLE();
#endif


}

void
__cxa_atexit(void (*arg1)(void*), void* arg2, void* arg3)
{
}

void*   __dso_handle = (void*) &__dso_handle;


/*------------------------------------------------------------------------*/
/* Reset support                                                          */

void hal_stamp_reset(void)
{
    HAL_DCACHE_DISABLE();
    HAL_ICACHE_DISABLE();

    // Init the watchdog timer.
    __asm __volatile ( "P0.H = 0xFFC0; \n \
                        P0.L = 0x0200; \n \
                        R0.L = 0x0AD0; \n \
                        W[P0] = R0; \n \
                        SSYNC; " : : : "P0", "R0"); // disable watchdog
    __asm __volatile ( "P0.H = 0xFFC0; \n \
                        P0.L = 0x0204; \n \
                        R0.H = 0x0; \n \
                        R0.H = 0x10; \n \
                        [P0] = R0; \n \
                        SSYNC; " : :  : "P0", "R0"); // set watchdog timer
                        
    __asm __volatile ( "P0.H = 0xFFE0; \n \
                        P0.L = 0x2004; \n \
                        R0.H = 0x2000; \n \
                        R0.H = 0x0000; \n \
                        [P0] = R0; \n \
                        SSYNC; " : :  : "P0", "R0"); // set reset entry point
    
    // enable watchdog
    __asm __volatile ( "P0.H = 0xFFC0; \n \
                        P0.L = 0x0200; \n \
                        R0.L = 0x0000; \n \
                        W[P0] = R0; \n \
                        SSYNC; " : : : "P0", "R0"); // disable watchdog

}

void hal_plf_program_clocks(void) 
{
	cyg_uint32 reg_buf;
	
	HAL_READ_UINT32(DMC0_STAT, reg_buf);
	if (reg_buf & MEMINITDONE) 
	{
		HAL_READ_UINT32(DMC0_CTL, reg_buf);
		reg_buf |= DMC0_CTL_SRREQ;
		HAL_WRITE_UINT32(DMC0_CTL, reg_buf);
		__builtin_bfin_ssync();
		do 
		{
			HAL_READ_UINT32(DMC0_STAT, reg_buf);
		} while (!(reg_buf & SRACK));
	}
	
	// Don't set the same value of MSEL and DF to CGU_CTL 
	HAL_READ_UINT32(CGU_CTL, reg_buf);
	if ((reg_buf & (MSEL_MASK | DF_MASK))
			!= CONFIG_CGU_CTL_VAL) 
	{
		HAL_WRITE_UINT32(CGU_DIV, CONFIG_CGU_DIV_VAL);
		HAL_WRITE_UINT32(CGU_CTL, CONFIG_CGU_CTL_VAL);
		do 
		{
			HAL_READ_UINT32(CGU_STAT, reg_buf);
		} while ((reg_buf & (CLKSALGN | PLLBP)) ||
				  !(reg_buf & PLLLK));
	}
	
	HAL_WRITE_UINT32(CGU_DIV, (CONFIG_CGU_DIV_VAL | UPDT));
	do 
	{
		HAL_READ_UINT32(CGU_STAT, reg_buf);
	} while (reg_buf & CLKSALGN);
	
	HAL_READ_UINT32(DMC0_STAT, reg_buf);
	if (reg_buf & MEMINITDONE) 
	{
		HAL_READ_UINT32(DMC0_CTL, reg_buf);
		reg_buf &= ~DMC0_CTL_SRREQ;
		HAL_WRITE_UINT32(DMC0_CTL, reg_buf);
		__builtin_bfin_ssync();
		do 
		{
			HAL_READ_UINT32(DMC0_STAT, reg_buf);
		} while (reg_buf & SRACK);
	}
    
    hal_plf_program_memory_controller();
    
}

void hal_plf_program_memory_controller(void)
{
	volatile cyg_uint32 buf_reg;
	cyg_uint32 dlldatacycle;
	cyg_uint32 dll_ctl;
	cyg_uint32 i = 0;
	
	if (CONFIG_BFIN_GET_DCLK_M ==  125)
		i = 0;
	else if (CONFIG_BFIN_GET_DCLK_M ==  133)
		i = 1;
	else if (CONFIG_BFIN_GET_DCLK_M ==  150)
		i = 2;
	else if (CONFIG_BFIN_GET_DCLK_M ==  166)
		i = 3;
	else if (CONFIG_BFIN_GET_DCLK_M ==  200)
		i = 4;
	else if (CONFIG_BFIN_GET_DCLK_M ==  225)
		i = 5;
	else if (CONFIG_BFIN_GET_DCLK_M ==  250)
		i = 6;
		
#ifndef CONFIG_DMC_DDRCFG
	HAL_WRITE_UINT32(DMC0_CFG, ddr_config_table[i].dmc_ddrcfg);
#else
	HAL_WRITE_UINT32(DMC0_CFG, CONFIG_DMC_DDRCFG);
#endif
#ifndef CONFIG_DMC_DDRTR0
	HAL_WRITE_UINT32(DMC0_TR0, ddr_config_table[i].dmc_ddrtr0);
#else
	HAL_WRITE_UINT32(DMC0_TR0, CONFIG_DMC_DDRTR0);
#endif
#ifndef CONFIG_DMC_DDRTR1
	HAL_WRITE_UINT32(DMC0_TR1, ddr_config_table[i].dmc_ddrtr1);
#else
	HAL_WRITE_UINT32(DMC0_TR1, CONFIG_DMC_DDRTR1);
#endif
#ifndef CONFIG_DMC_DDRTR2
	HAL_WRITE_UINT32(DMC0_TR2, ddr_config_table[i].dmc_ddrtr2);
#else
	HAL_WRITE_UINT32(DMC0_TR2, CONFIG_DMC_DDRTR2);
#endif
#ifndef CONFIG_DMC_DDRMR
	HAL_WRITE_UINT32(DMC0_MR, ddr_config_table[i].dmc_ddrmr);
#else
	HAL_WRITE_UINT32(DMC0_MR, CONFIG_DMC_DDRMR);
#endif
#ifdef CYGPKG_HAL_BFIN_BF609
#ifndef CONFIG_DMC_PHY_CTL3
	HAL_WRITE_UINT32(DMC0_PHY_CTL3, ddr_config_table[i].dmc_ddrphyctl3);
#else
	HAL_WRITE_UINT32(DMC0_PHY_CTL3, CONFIG_DMC_PHY_CTL3);
#endif
#endif
#ifndef CONFIG_DMC_PHY_CTL1
	HAL_WRITE_UINT32(DMC0_PHY_CTL1, ddr_config_table[i].dmc_ddrphyctl1);
#else
	HAL_WRITE_UINT32(DMC0_PHY_CTL1, CONFIG_DMC_PHY_CTL1);
#endif
#ifndef CONFIG_DMC_DDRCTL
	HAL_WRITE_UINT32(DMC0_CTL, ddr_config_table[i].dmc_ddrctl);
#else
	HAL_WRITE_UINT32(DMC0_CTL, CONFIG_DMC_DDRCTL);
#endif
	__builtin_bfin_ssync();
    	
	do 
	{
		HAL_READ_UINT32(DMC0_STAT, buf_reg);
	} while (!(buf_reg & MEMINITDONE));
	
	HAL_READ_UINT32(DMC0_STAT, buf_reg);
	dlldatacycle = (buf_reg & PHYRDPHASE) >> PHYRDPHASE_OFFSET;
    dlldatacycle = 5;
	HAL_READ_UINT32(DMC0_DLLCTL, dll_ctl); //FIXME conform int and uint types
	dll_ctl &= 0x0ff;
	HAL_WRITE_UINT32(DMC0_DLLCTL, (dll_ctl | (dlldatacycle << DATACYC_OFFSET)));
	
	__builtin_bfin_ssync();
    
	do
	{
		HAL_READ_UINT32(DMC0_STAT, buf_reg);
	} while (!(buf_reg & DLLCALDONE));
    
    buf_reg = 1000000;
    
    while (buf_reg--);
    
}

void hal_plf_eppi_gpio_init(void)
{
	volatile cyg_uint32 fer_reg, mux_reg, reset_mask;
#ifdef CYGPKG_DEVS_PPI_BFIN_BF60X
	#ifdef CYGPKG_DEVS_PPI_BFIN_BF60X_CHANNEL0
		// configure I/O for EPPI0
		HAL_READ_UINT32(PORTD_MUX, mux_reg);
		// D20, D21
		fer_reg = ((1 << 12) | (1 << 15));
		mux_reg &= ~((3 << 24) | (3 << 30)); //clear necessary bits
		mux_reg |= ((1 << 24) | (1 << 30));
		HAL_WRITE_UINT32(PORTD_FER_SET, fer_reg);
		HAL_WRITE_UINT32(PORTD_MUX, mux_reg);
		
		HAL_READ_UINT32(PORTE_MUX, mux_reg);
		// CLK, D16-D19, D22, D23, FS1, FS2, FS3
		fer_reg = ((1 << 9) | (1 << 3) | (1 << 4) | (1 << 0) | (1 << 1)
			| (1 << 2) | (1 << 5) | (1 << 8) | (1 << 7) | (1 << 6));
		mux_reg &= ~((3 << 18) | (3 << 6) | (3 << 8) | (3 << 0) | (3 << 2)
			| (3 << 4) | (3 << 10) | (3 << 16) | (3 << 14) | (3 << 12));
		mux_reg |= ((1 << 18) | (1 << 6) | (1 << 8) | (1 << 0) | (1 << 2)
			| (1 << 4) | (1 << 10) | (1 << 16) | (1 << 14) | (1 << 12));
		HAL_WRITE_UINT32(PORTE_FER_SET, fer_reg);
		HAL_WRITE_UINT32(PORTE_MUX, mux_reg);
		
		// D0-D15
		fer_reg = 0xFFFF; //0..15 bits
		mux_reg = 0x55555555; //Multiplexed Function 1 for all 16 pins
		HAL_WRITE_UINT32(PORTF_FER_SET, fer_reg);
		HAL_WRITE_UINT32(PORTF_MUX, mux_reg);
	#endif //ifdef CYGPKG_DEVS_PPI_BFIN_BF60X_CHANNEL0
	
	#ifdef CYGPKG_DEVS_PPI_BFIN_BF60X_CHANNEL1
		// configure I/O for EPPI1
		HAL_READ_UINT32(PORTB_MUX, mux_reg);
		// CLK, FS1, FS3
		fer_reg = ((1 << 14) | (1 << 13) | (1 << 15));
		mux_reg &= ~((3 << 28) | (3 << 26) | (3 << 30)); //clear necessary bits
		mux_reg |= ((1 << 28) | (1 << 26) | (1 << 30));
		HAL_WRITE_UINT32(PORTB_FER_SET, fer_reg);
		HAL_WRITE_UINT32(PORTB_MUX, mux_reg);
		
		// D0-D15
		fer_reg = 0xFFFF; //0..15 bits
		mux_reg = 0x55555555; //Multiplexed Function 1 for all 16 pins
		HAL_WRITE_UINT32(PORTC_FER_SET, fer_reg);
		HAL_WRITE_UINT32(PORTC_MUX, mux_reg);
		
		HAL_READ_UINT32(PORTD_MUX, mux_reg);
		// D16, D17
		fer_reg = ((1 << 0) | (1 << 1));
		mux_reg &= ~((3 << 0) | (3 << 2));
		mux_reg |= ((1 << 0) | (1 << 2));
		HAL_WRITE_UINT32(PORTD_FER_SET, fer_reg);
		HAL_WRITE_UINT32(PORTD_MUX, mux_reg);
	#endif //ifdef CYGPKG_DEVS_PPI_BFIN_BF60X_CHANNEL1
	
	#ifdef CYGPKG_DEVS_PPI_BFIN_BF60X_CHANNEL2
		// configure I/O for EPPI2
		// D0-D15
		fer_reg = 0xFFFF; //0..15 bits
		mux_reg = 0x55555555; //Multiplexed Function 1 for all 16 pins
		HAL_WRITE_UINT32(PORTA_FER_SET, fer_reg);
		HAL_WRITE_UINT32(PORTA_MUX, mux_reg);
		
		HAL_READ_UINT32(PORTB_MUX, mux_reg);
		// CLK, D16, D17, FS1, FS2, FS3
		fer_reg = ((1 << 0) | (1 << 7) | (1 << 8) | (1 << 1) | (1 << 2) | (1 << 3));
		mux_reg &= ~((3 << 0) | (3 << 14) | (3 << 16) | (3 << 2) | (3 << 4) | (3 << 6));
		mux_reg = ((1 << 0) | (1 << 14) | (1 << 16) | (1 << 2) | (1 << 4) | (1 << 6));
		HAL_WRITE_UINT32(PORTB_FER_SET, fer_reg);
		HAL_WRITE_UINT32(PORTB_MUX, mux_reg);
	#endif //ifdef CYGPKG_DEVS_PPI_BFIN_BF60X_CHANNEL2
#endif
}

void hal_plf_gpio_init(void)
{
	volatile cyg_uint32 buf_reg0;
	cyg_uint32 buf_reg1;
	
	// configure UART0 Rx, Tx
	buf_reg0 = ((1 << 7) | (1 << 8));
	HAL_WRITE_UINT32(PORTD_FER_SET, buf_reg0);
	HAL_READ_UINT32(PORTD_MUX, buf_reg0);
	buf_reg0 |= ((1 << 14) | (1 << 16));
	HAL_WRITE_UINT32(PORTD_MUX, buf_reg0);

#ifdef CYGPKG_DEVS_SPI_BFIN_BF60x
	#ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS0
		// configure I/O for SPI bus 0
		HAL_READ_UINT32(PORTD_MUX, buf_reg1);
		// MISO, MOSI, CLK
		buf_reg0 = ((1 << 2) | (1 << 3) | (1 << 4));
		buf_reg1 &= ~((3 << 4) | (3 << 6) | (3 << 8));
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS1
			buf_reg0 |= (1 << 11);
			buf_reg1 &= ~(3 << 22);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS1
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS2
			buf_reg0 |= (1 << 1);
			buf_reg1 |= (2 << 2);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS2
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS3
			buf_reg0 |= 1;
			buf_reg1 |= 2;
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS3
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS5
			buf_reg0 |= (1 << 9);
			buf_reg1 &= ~(3 << 18);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS5
		HAL_WRITE_UINT32(PORTD_FER_SET, buf_reg0);
		HAL_WRITE_UINT32(PORTD_MUX, buf_reg1);
		buf_reg0 = 0;
		HAL_READ_UINT32(PORTC_MUX, buf_reg1);
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS4
			buf_reg0 |= (1 << 15);
			buf_reg1 &= ~(3 << 30);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS4
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS6
			buf_reg0 |= (1 << 13);
			buf_reg1 &= ~(3 << 26);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS6
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS7
			buf_reg0 |= (1 << 12);
			buf_reg0 &= ~(3 << 24);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS7
		HAL_WRITE_UINT32(PORTC_FER_SET, buf_reg0);
		HAL_WRITE_UINT32(PORTC_MUX, buf_reg1);
	#endif //ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS0
		
	#ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS1
		// configure I/O for SPI bus 1
		HAL_READ_UINT32(PORTD_MUX, buf_reg1);
		// MISO, MOSI, CLK
		buf_reg0 = ((1 << 5) | (1 << 13) | (1 << 14));
		buf_reg1 &= ~((3 << 10) | (3 << 26) | (3 << 28));
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS1
			buf_reg0 |= (1 << 12);
			buf_reg1 &= ~(3 << 24);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS1
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS2
			buf_reg0 |= (1 << 15);
			buf_reg1 &= ~(3 << 30);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS2
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS3
			buf_reg0 |= (1 << 10);
			buf_reg1 |= (2 << 20);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS3
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS4
			buf_reg0 |= (1 << 9);
			buf_reg1 |= (2 << 18);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS4
		HAL_WRITE_UINT32(PORTD_FER_SET, buf_reg0);
		HAL_WRITE_UINT32(PORTD_MUX, buf_reg1);
		buf_reg0 = 0;
		HAL_READ_UINT32(PORTF_MUX, buf_reg1);
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS5
			buf_reg0 |= (1 << 8);
			buf_reg1 &= ~(3 << 16);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS5
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS6
			buf_reg0 |= (1 << 9);
			buf_reg1 &= ~(3 << 18);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS0_NPCS6
		HAL_WRITE_UINT32(PORTF_FER_SET, buf_reg0);
		HAL_WRITE_UINT32(PORTF_MUX, buf_reg1);
		buf_reg0 = 0;
		HAL_READ_UINT32(PORTC_MUX, buf_reg1);
		#ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS7
			buf_reg0 |= (1 << 14);
			buf_reg1 &= ~(3 << 28);
		#endif //ifdef CYGDAT_DEVS_SPI_BFIN_BF60x_BUS1_NPCS7
		HAL_WRITE_UINT32(PORTC_FER_SET, buf_reg0);
		HAL_WRITE_UINT32(PORTC_MUX, buf_reg1);
	#endif //ifdef CYGHWR_DEVS_SPI_BFIN_BF60x_BUS1
		
#endif //ifdef CYGPKG_DEVS_SPI_BFIN_BF60x
//#ifdef CYGPKG_DEVS_ETH_BF60X
    // HAL_READ_UINT32(PORTC_MUX, buf_reg1);
    // // PTPAUXIN, PTPCLKIN !!!! check with spi
    // buf_reg0 = ((1 << 11) | (1 << 13) );
    // buf_reg1 &= ((2 << 22) | (2 << 26) );

    // HAL_WRITE_UINT32(PORTC_FER_SET, buf_reg0);
    // HAL_WRITE_UINT32(PORTC_MUX, buf_reg1);

    #ifdef CYGPKG_DEVS_ETH_BF60X_EMAC0

//    diag_printf("Setup EMAC0 pins \n")
        // configure I/O for EMAC0
		HAL_READ_UINT32(PORTB_MUX, buf_reg1);
		// TXEN, REFCLK, PTPPPS
		buf_reg0 = ( (1 << 13) | (1 << 14) | (1 << 15));
		buf_reg1 &= ~((3 << 26) |(3 << 28) |(3 << 30));

        HAL_WRITE_UINT32(PORTB_FER_SET, buf_reg0);
        asm volatile ("SSYNC;");
		HAL_WRITE_UINT32(PORTB_MUX, buf_reg1);
        asm volatile ("SSYNC;");

        HAL_READ_UINT32(PORTC_MUX, buf_reg1);
		// RXD0, RXD1, TXD0,TXD1,ERxER, CRS, MDC, MDIO, P_PTP1_PPS
		buf_reg0 =   ((1 << 0) |(1 << 1) | (1 << 2)| (1 << 3) | (1 << 4) |(1 << 5)   |  (1 << 6) | (1 << 7)| (1 << 9));
		buf_reg1 &= ~((3 << 0) |(3 << 2) | (3 << 4)| (3 << 6) | (3 << 8) | (3 << 10) | (3 << 12) | (3 << 14)| (3 << 18));

        HAL_WRITE_UINT32(PORTC_FER_SET, buf_reg0);
        asm volatile ("SSYNC;");
		HAL_WRITE_UINT32(PORTC_MUX, buf_reg1);
        asm volatile ("SSYNC;");
        
		HAL_READ_UINT32(PORTD_MUX, buf_reg1);
		// PHYINT
		buf_reg0 = 0;//( (1 << 6) );
		buf_reg1 &= ~((3 << 12) );

        HAL_WRITE_UINT32(PORTB_FER_SET, buf_reg0);
        asm volatile ("SSYNC;");
		HAL_WRITE_UINT32(PORTB_MUX, buf_reg1);
        asm volatile ("SSYNC;");
    #endif // CYGHWR_DEVS_ETH_BFIN_BF60x_EMAC0
    
    #ifdef CYGPKG_DEVS_ETH_BF60X_EMAC1
    
		// configure I/O for EMAC1
        HAL_READ_UINT32(PORTC_MUX, buf_reg1);
		// PTPPPS
		buf_reg0 = ((1 << 9) );
		buf_reg1 &= ~((3 << 18) );

        HAL_WRITE_UINT32(PORTC_FER_SET, buf_reg0);
		HAL_WRITE_UINT32(PORTC_MUX, buf_reg1);
    
		HAL_READ_UINT32(PORTE_MUX, buf_reg1);
		// MDC, MDIO, CRS, RXD1
		buf_reg0 = ( (1 << 10) | (1 << 11) | (1 << 13) | (1 << 15));
		buf_reg1 &= ~((3 << 20) |(3 << 11) | (3 << 26) | (3 << 30));

        HAL_WRITE_UINT32(PORTE_FER_SET, buf_reg0);
		HAL_WRITE_UINT32(PORTE_MUX, buf_reg1);

		HAL_READ_UINT32(PORTG_MUX, buf_reg1);
		// RXD0, TXD1, TXD0, TXEN, REFCLK
		buf_reg0 = ( (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6));
		buf_reg1 &= ~((3 << 0) |(3 << 4) | (3 << 6) | (3 << 10) | (3 << 12));

        HAL_WRITE_UINT32(PORTG_FER_SET, buf_reg0);
		HAL_WRITE_UINT32(PORTG_MUX, buf_reg1);
        
    #endif // CYGHWR_DEVS_ETH_BFIN_BF60X_EMAC1
       
//#endif // CYGPKG_DEVS_ETH_BFIN_BF60X


}
/*------------------------------------------------------------------------*/
/* End of plf_misc.c                                                      */
