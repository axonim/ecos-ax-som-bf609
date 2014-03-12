#ifndef CYGONCE_HAL_BFIN_REGS_H
#define CYGONCE_HAL_BFIN_REGS_H
//========================================================================
//
//      bfin-regs.h
//
//      Register defines for BFIN processors
//
//========================================================================
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
//========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     andre, Pavel Azizov <pavel.azizov@axonim.by> AXONIM Devices
// Date:          2006-05-19
// Purpose:       
// Description:   Register defines for BFIN processors
// Usage:         
//
//####DESCRIPTIONEND####
//
//========================================================================

#include <pkgconf/hal.h>
#include <pkgconf/hal_bfin.h>

#include <cyg/hal/plf_sdram_defs.h>
#ifdef CYGPKG_HAL_BFIN_BF53x
    #include <pkgconf/hal_bfin_bf53x.h>
#endif
#ifdef CYGPKG_HAL_BFIN_BF60x
    #include <pkgconf/hal_bfin_bf60x.h>
#endif

#define REG32_SET_BITS(_reg_, _value_)			\
	CYG_MACRO_START								\
	{											\
		cyg_uint32 buf_reg;						\
		HAL_READ_UINT32(_reg_, buf_reg);		\
		buf_reg |= _value_;						\
		HAL_write_UINT32(_reg_, buf_reg);		\
	}											\
	CYG_MACRO_END								\
	
#define REG32_CLR_BITS(_reg_, _value_)			\
	CYG_MACRO_START								\
	{											\
		cyg_uint32 buf_reg;						\
		HAL_READ_UINT32(_reg_, buf_reg);		\
		buf_reg &= ~_value_;					\
		HAL_write_UINT32(_reg_, buf_reg);		\
	}											\
	CYG_MACRO_END								\

#define HI(x) (x >> 16)
#define LO(x) (x & 0xFFFF)

//TODO change PLL & clock divider
// the PLL divider register
#define PLL_DIV         0xFFC00004
#define PLL_DIV_CSEL    0x0030
#define PLL_DIV_SSEL    0x00F
#define PLL_DIV_CSEL_SHIFT 0x4

#define PLL_CTL         0xFFC00000
#define PLL_CTL_MSEL        0x7E00
#define PLL_CTL_MSEL_SHIFT  0x9
#define PLL_CTL_BYPASS      0x0100
#define PLL_CTL_OUTD        0x0080
#define PLL_CTL_IND         0x0040
#define PLL_CTL_PDWN        0x0020
#define PLL_CTL_STOPCK      0x0008
#define PLL_CTL_PLL_OFF     0x0002
#define PLL_CTL_DF          0x0001

#define PLL_STAT        0xFFC0000C
#define PLL_LOCKCNT     0xFFC00010

#ifdef CYGNUM_HAL_BFIN_CORE_CLOCK_DIVIDER_1
    #define PLL_CSEL_SET 0x00
#elif defined(CYGNUM_HAL_BFIN_CORE_CLOCK_DIVIDER_2)
    #define PLL_CSEL_SET 0x10
#elif defined(CYGNUM_HAL_BFIN_CORE_CLOCK_DIVIDER_4)
    #define PLL_CSEL_SET 0x20
#elif defined(CYGNUM_HAL_BFIN_CORE_CLOCK_DIVIDER_8)
    #define PLL_CSEL_SET 0x30
#else
    #error wrong PLL CORE DIVIDER setting
#endif

#ifdef CYGNUM_HAL_BFIN_SYSTEM_CLOCK_DIVIDER
    #if CYGNUM_HAL_BFIN_SYSTEM_CLOCK_DIVIDER<1
        #error wrong PLL SYSTEM DIVIDER setting
    #elif CYGNUM_HAL_BFIN_SYSTEM_CLOCK_DIVIDER>15
        #error wrong PLL SYSTEM DIVIDER setting
    #else
        #define PLL_SSEL_SET CYGNUM_HAL_BFIN_SYSTEM_CLOCK_DIVIDER
    #endif
#else
    #error wrong PLL SYSTEM DIVIDER setting
#endif

#ifdef CYGNUM_HAL_BFIN_PLL_MULTIPLIER
    #if CYGNUM_HAL_BFIN_PLL_MULTIPLIER<1
        #error wrong PLL SYSTEM DIVIDER setting
    #elif CYGNUM_HAL_BFIN_PLL_MULTIPLIER>64
        #error wrong PLL SYSTEM DIVIDER setting
    #else
        #ifdef CYGNUM_HAL_BFIN_PLL_MULTIPLIER_64
            #define PLL_MSEL_SET 0
        #else
            #define PLL_MSEL_SET CYGNUM_HAL_BFIN_PLL_MULTIPLIER
        #endif
    #endif
#else
    #error wrong PLL multiplier setting
#endif


#ifdef CYGPKG_HAL_BFIN_BF53x

#define EBIU_AMGCTL     0xFFC00A00
#define EBIU_AMGCTL_CDPRIO 0x0100
#define EBIU_AMGCTL_AMCKEN 0x0001
#define EBIU_AMGCTL_BANKSEL_MASK 0x000E
#define EBIU_AMGCTL_STD    0x00F0

#define EBIU_AMBCTL0    0xFFC00A04

#define EBIU_AMBCTL1    0xFFC00A08

#define EBIU_SDRRC      0xFFC00A18
#define EBIU_SDBCTL     0xFFC00A14
#define EBIU_SDGCTL     0xFFC00A10
#define EBIU_SDSTAT     0xFFC00A1C
	
/*
 * SDRAM CONTROLLER MASKS
 */

/* SDGCTL Masks */
#define SCTLE           0x00000001  /* Enable SCLK[0], /SRAS, /SCAS, /SWE, SDQM[3:0] */
#define CL_2            0x00000008  /* SDRAM CAS latency = 2 cycles */
#define CL_3            0x0000000C  /* SDRAM CAS latency = 3 cycles */
#define PFE         	0x00000010  /* Enable SDRAM prefetch */
#define PFP         	0x00000020  /* Prefetch has priority over AMC requests */
#define TRAS_1          0x00000040  /* SDRAM tRAS = 1 cycle */
#define TRAS_2          0x00000080  /* SDRAM tRAS = 2 cycles */
#define TRAS_3          0x000000C0  /* SDRAM tRAS = 3 cycles */
#define TRAS_4          0x00000100  /* SDRAM tRAS = 4 cycles */
#define TRAS_5          0x00000140  /* SDRAM tRAS = 5 cycles */
#define TRAS_6          0x00000180  /* SDRAM tRAS = 6 cycles */
#define TRAS_7          0x000001C0  /* SDRAM tRAS = 7 cycles */
#define TRAS_8          0x00000200  /* SDRAM tRAS = 8 cycles */
#define TRAS_9          0x00000240  /* SDRAM tRAS = 9 cycles */
#define TRAS_10         0x00000280  /* SDRAM tRAS = 10 cycles */
#define TRAS_11         0x000002C0  /* SDRAM tRAS = 11 cycles */
#define TRAS_12         0x00000300  /* SDRAM tRAS = 12 cycles */
#define TRAS_13         0x00000340  /* SDRAM tRAS = 13 cycles */
#define TRAS_14         0x00000380  /* SDRAM tRAS = 14 cycles */
#define TRAS_15         0x000003C0  /* SDRAM tRAS = 15 cycles */
#define TRP_1           0x00000800  /* SDRAM tRP = 1 cycle */
#define TRP_2           0x00001000  /* SDRAM tRP = 2 cycles */
#define TRP_3           0x00001800  /* SDRAM tRP = 3 cycles */
#define TRP_4           0x00002000  /* SDRAM tRP = 4 cycles */
#define TRP_5           0x00002800  /* SDRAM tRP = 5 cycles */
#define TRP_6           0x00003000  /* SDRAM tRP = 6 cycles */
#define TRP_7           0x00003800  /* SDRAM tRP = 7 cycles */
#define TRCD_1          0x00008000  /* SDRAM tRCD = 1 cycle */
#define TRCD_2          0x00010000  /* SDRAM tRCD = 2 cycles */
#define TRCD_3          0x00018000  /* SDRAM tRCD = 3 cycles */
#define TRCD_4          0x00020000  /* SDRAM tRCD = 4 cycles */
#define TRCD_5          0x00028000  /* SDRAM tRCD = 5 cycles */
#define TRCD_6          0x00030000  /* SDRAM tRCD = 6 cycles */
#define TRCD_7          0x00038000  /* SDRAM tRCD = 7 cycles */
#define TWR_1           0x00080000  /* SDRAM tWR = 1 cycle */
#define TWR_2           0x00100000  /* SDRAM tWR = 2 cycles */
#define TWR_3           0x00180000  /* SDRAM tWR = 3 cycles */
#define PUPSD           0x00200000  /* Power-up start delay */
#define PSM         	0x00400000  /* SDRAM power-up sequence = Precharge, mode register set, 8 CBR refresh cycles */
#define PSS         	0x00800000  /* enable SDRAM power-up sequence on next SDRAM access */
#define SRFS            0x01000000  /* Start SDRAM self-refresh mode */
#define EBUFE           0x02000000  /* Enable external buffering timing */
#define FBBRW           0x04000000  /* Fast back-to-back read write enable */
#define EMREN           0x10000000  /* Extended mode register enable */
#define TCSR            0x20000000  /* Temp compensated self refresh value 85 deg C */
#define CDDBG           0x40000000  /* Tristate SDRAM controls during bus grant */

/* EBIU_SDBCTL Masks */
#define EBE         	0x00000001  /* Enable SDRAM external bank */
#define EBSZ_16         0x00000000  /* SDRAM external bank size = 16MB */
#define EBSZ_32         0x00000002  /* SDRAM external bank size = 32MB */
#define EBSZ_64         0x00000004  /* SDRAM external bank size = 64MB */
#define EBSZ_128        0x00000006  /* SDRAM external bank size = 128MB */
#define EBCAW_8         0x00000000  /* SDRAM external bank column address width = 8 bits */
#define EBCAW_9         0x00000010  /* SDRAM external bank column address width = 9 bits */
#define EBCAW_10        0x00000020  /* SDRAM external bank column address width = 9 bits */
#define EBCAW_11        0x00000030  /* SDRAM external bank column address width = 9 bits */

/* EBIU_SDSTAT Masks */
#define SDCI            0x00000001  /* SDRAM controller is idle */
#define SDSRA           0x00000002  /* SDRAM SDRAM self refresh is active */
#define SDPUA           0x00000004  /* SDRAM power up active */
#define SDRS            0x00000008  /* SDRAM is in reset state */
#define SDEASE          0x00000010  /* SDRAM EAB sticky error status - W1C */
#define BGSTAT          0x00000020  /* Bus granted */

#elif defined (CYGPKG_HAL_BFIN_BF60x)

/* CGU_CTL Masks */
#define DF              (1 << 0)
#define MSEL            (0x7f << MSEL_P)
#define WIDLE           (1 << WIDLE_P)
#define LOCK            (1 << LOCK_P)

#define DF_P            0
#define MSEL_P          8
#define WIDLE_P         30
#define LOCK_P          31
#define MSEL_MASK       0x7F00
#define DF_MASK         0x1

/* CGU_STAT Masks */
#define PLLEN           (1 << 0)
#define PLLBP           (1 << 1)
#define PLLLK           (1 << 2)
#define CLKSALGN        (1 << 3)
#define CCBF0EN         (1 << 4)
#define CCBF1EN         (1 << 5)
#define SCBF0EN         (1 << 6)
#define SCBF1EN         (1 << 7)
#define DCBFEN          (1 << 8)
#define OCBFEN          (1 << 9)
#define ADRERR          (1 << 16)
#define LWERR           (1 << 17)
#define DIVERR          (1 << 18)
#define WDFMSERR        (1 << 19)
#define WDIVERR         (1 << 20)
#define PLLLKERR        (1 << 21)

#define CSEL_P          0
#define S0SEL_P         5
#define SYSSEL_P        8
#define S1SEL_P         13
#define DSEL_P          16
#define OSEL_P          22
#define ALGN_P          29
#define UPDT_P          30
#define LOCK_P          31

/* CGU_DIV Masks */
#define CSEL            (0x1f << CSEL_P)
#define S0SEL           (3 << S0SEL_P)
#define SYSSEL          (0x1f << SYSSEL_P)
#define S1SEL           (3 << S1SEL_P)
#define DSEL            (0x1f << DSEL_P)
#define OSEL            (0x7f << OSEL_P)
#define ALGN            (1 << ALGN_P)
#define UPDT            (1 << UPDT_P)
#define LOCK            (1 << LOCK_P)


/* CGU_CLKOUTSEL Masks */
#define CLKOUTSEL       (0xf << 0)
#define USBCLKSEL       (0x3f << 16)


#define CLKOUTSEL_CLKIN     0x0
#define CLKOUTSEL_CCLK      0x1
#define CLKOUTSEL_SYSCLK    0x2
#define CLKOUTSEL_SCLK0     0x3
#define CLKOUTSEL_SCLK1     0x4
#define CLKOUTSEL_DCLK      0x5
#define CLKOUTSEL_USB_PLL   0x6
#define CLKOUTSEL_OUTCLK    0x7
#define CLKOUTSEL_USB_CLKIN 0x8
#define CLKOUTSEL_WDOG      0x9
#define CLKOUTSEL_PMON      0xA
#define CLKOUTSEL_GND       0xB

/* CGU Registers */
#define CGU_CTL                    0xFFCA8000         /* CGU0 Control Register */
#define CGU_STAT                   0xFFCA8004         /* CGU0 Status Register */
#define CGU_DIV                    0xFFCA8008         /* CGU0 Divisor Register */
#define CGU_CLKOUTSEL              0xFFCA800C         /* CGU0 CLKOUT Select Register */

/* SMC Registers */
#define SMC_GCTL                   0xFFC16004         /* SMC0 SMC Control Register */
#define SMC_GSTAT                  0xFFC16008         /* SMC0 SMC Status Register */
#define SMC_B0CTL                  0xFFC1600C         /* SMC0 SMC Bank0 Control Register */
#define SMC_B0TIM                  0xFFC16010         /* SMC0 SMC Bank0 Timing Register */
#define SMC_B0ETIM                 0xFFC16014         /* SMC0 SMC Bank0 Extended Timing Register */
#define SMC_B1CTL                  0xFFC1601C         /* SMC0 SMC BANK1 Control Register */
#define SMC_B1TIM                  0xFFC16020         /* SMC0 SMC BANK1 Timing Register */
#define SMC_B1ETIM                 0xFFC16024         /* SMC0 SMC BANK1 Extended Timing Register */
#define SMC_B2CTL                  0xFFC1602C         /* SMC0 SMC BANK2 Control Register */
#define SMC_B2TIM                  0xFFC16030         /* SMC0 SMC BANK2 Timing Register */
#define SMC_B2ETIM                 0xFFC16034         /* SMC0 SMC BANK2 Extended Timing Register */
#define SMC_B3CTL                  0xFFC1603C         /* SMC0 SMC BANK3 Control Register */
#define SMC_B3TIM                  0xFFC16040         /* SMC0 SMC BANK3 Timing Register */
#define SMC_B3ETIM                 0xFFC16044         /* SMC0 SMC BANK3 Extended Timing Register */

/* DMC0 Registers */
#define DMC0_ID                    0xFFC80000         /* DMC0 Identification Register */
#define DMC0_CTL                   0xFFC80004         /* DMC0 Control Register */
#define DMC0_STAT                  0xFFC80008         /* DMC0 Status Register */
#define DMC0_EFFCTL                0xFFC8000C         /* DMC0 Efficiency Controller */
#define DMC0_PRIO                  0xFFC80010         /* DMC0 Priority ID Register */
#define DMC0_PRIOMSK               0xFFC80014         /* DMC0 Priority ID Mask */
#define DMC0_CFG                   0xFFC80040         /* DMC0 SDRAM Configuration */
#define DMC0_TR0                   0xFFC80044         /* DMC0 Timing Register 0 */
#define DMC0_TR1                   0xFFC80048         /* DMC0 Timing Register 1 */
#define DMC0_TR2                   0xFFC8004C         /* DMC0 Timing Register 2 */
#define DMC0_MSK                   0xFFC8005C         /* DMC0 Mode Register Mask */
#define DMC0_MR                    0xFFC80060         /* DMC0 Mode Shadow register */
#define DMC0_EMR1                  0xFFC80064         /* DMC0 EMR1 Shadow Register */
#define DMC0_EMR2                  0xFFC80068         /* DMC0 EMR2 Shadow Register */
#define DMC0_EMR3                  0xFFC8006C         /* DMC0 EMR3 Shadow Register */
#define DMC0_DLLCTL                0xFFC80080         /* DMC0 DLL Control Register */
#define DMC0_PHY_CTL0              0xFFC80090         /* DMC0 PHY Control 0 Register */
#define DMC0_PHY_CTL1              0xFFC80094         /* DMC0 PHY Control 1 Register */
#define DMC0_PHY_CTL2              0xFFC80098         /* DMC0 PHY Control 2 Register */
#define DMC0_PHY_CTL3              0xFFC8009C         /* DMC0 PHY Control 3 Register */
#define DMC0_PADCTL                0xFFC800C0         /* DMC0 PAD Control Register 0 */

/* DMC control bits */
#define DMC0_CTL_LPDDR			0x2
#define DMC0_CTL_INIT			0x4
#define DMC0_CTL_SRREQ			0x8
#define DMC0_CTL_PDREQ			0x10
#define DMC0_CTL_DPDREQ			0x20
#define DMC0_CTL_PREC			0x40
#define DMC0_CTL_ADDRMODE		0x100
#define DMC0_CTL_RDTOWR			0xE00
#define DMC0_CTL_PPREF			0x1000
#define DMC0_CTL_DLLCAL			0x2000

/* DMC status bits */
#define DMC_IDLE                0x1
#define MEMINITDONE             0x4
#define SRACK                   0x8
#define PDACK                   0x10
#define DPDACK                  0x20
#define DLLCALDONE              0x2000
#define PENDREF                 0xF0000
#define PHYRDPHASE              0xF00000
#define PHYRDPHASE_OFFSET       20

/* DMC DLL control bits */
#define DLLCALRDCNT             0xFF
#define DATACYC_OFFSET          8

#define PORT_FER                   0x0         /* Port x Function Enable Register offset */
#define PORT_FER_SET               0x4         /* Port x Function Enable Set Register offset */
#define PORT_FER_CLR               0x8         /* Port x Function Enable Clear Register offset */
#define PORT_DATA                  0xC         /* Port x GPIO Data Register offset */
#define PORT_DATA_SET              0x10        /* Port x GPIO Data Set Register offset */
#define PORT_DATA_CLR              0x14        /* Port x GPIO Data Clear Register offset */
#define PORT_DIR                   0x18        /* Port x GPIO Direction Register offset */
#define PORT_DIR_SET               0x1C        /* Port x GPIO Direction Set Register offset */
#define PORT_DIR_CLR               0x20        /* Port x GPIO Direction Clear Register offset */
#define PORT_INEN                  0x24        /* Port x GPIO Input Enable Register offset */
#define PORT_INEN_SET              0x28        /* Port x GPIO Input Enable Set Register offset */
#define PORT_INEN_CLR              0x2C        /* Port x GPIO Input Enable Clear Register offset */
#define PORT_MUX                   0x30         /* PORT Port x Multiplexer Control Register offset*/
#define PORT_DATA_TGL              0x34         /* PORT Port x GPIO Input Enable Toggle Register offset*/
#define PORT_POL                   0x38         /* PORT Port x GPIO Programming Inversion Register offset*/
#define PORT_POL_SET               0x3C         /* PORT Port x GPIO Programming Inversion Set Register offset*/
#define PORT_POL_CLR               0x40         /* PORT Port x GPIO Programming Inversion Clear Register offset*/
#define PORT_LOCK                  0x44         /* PORT Port x GPIO Lock Register offset*/
#define PORT_REVID                 0x7C         /* PORT Port x GPIO Revision ID offset*/

#define PORTA_FER                   0xFFC03000         /* PORTA Port x Function Enable Register */
#define PORTA_FER_SET               0xFFC03004         /* PORTA Port x Function Enable Set Register */
#define PORTA_FER_CLR               0xFFC03008         /* PORTA Port x Function Enable Clear Register */
#define PORTA_DATA                  0xFFC0300C         /* PORTA Port x GPIO Data Register */
#define PORTA_DATA_SET              0xFFC03010         /* PORTA Port x GPIO Data Set Register */
#define PORTA_DATA_CLR              0xFFC03014         /* PORTA Port x GPIO Data Clear Register */
#define PORTA_DIR                   0xFFC03018         /* PORTA Port x GPIO Direction Register */
#define PORTA_DIR_SET               0xFFC0301C         /* PORTA Port x GPIO Direction Set Register */
#define PORTA_DIR_CLR               0xFFC03020         /* PORTA Port x GPIO Direction Clear Register */
#define PORTA_INEN                  0xFFC03024         /* PORTA Port x GPIO Input Enable Register */
#define PORTA_INEN_SET              0xFFC03028         /* PORTA Port x GPIO Input Enable Set Register */
#define PORTA_INEN_CLR              0xFFC0302C         /* PORTA Port x GPIO Input Enable Clear Register */
#define PORTA_MUX                   0xFFC03030         /* PORTA Port x Multiplexer Control Register */
#define PORTA_DATA_TGL              0xFFC03034         /* PORTA Port x GPIO Input Enable Toggle Register */
#define PORTA_POL                   0xFFC03038         /* PORTA Port x GPIO Programming Inversion Register */
#define PORTA_POL_SET               0xFFC0303C         /* PORTA Port x GPIO Programming Inversion Set Register */
#define PORTA_POL_CLR               0xFFC03040         /* PORTA Port x GPIO Programming Inversion Clear Register */
#define PORTA_LOCK                  0xFFC03044         /* PORTA Port x GPIO Lock Register */
#define PORTA_REVID                 0xFFC0307C         /* PORTA Port x GPIO Revision ID */
#define PORTB_FER                   0xFFC03080         /* PORTB Port x Function Enable Register */
#define PORTB_FER_SET               0xFFC03084         /* PORTB Port x Function Enable Set Register */
#define PORTB_FER_CLR               0xFFC03088         /* PORTB Port x Function Enable Clear Register */
#define PORTB_DATA                  0xFFC0308C         /* PORTB Port x GPIO Data Register */
#define PORTB_DATA_SET              0xFFC03090         /* PORTB Port x GPIO Data Set Register */
#define PORTB_DATA_CLR              0xFFC03094         /* PORTB Port x GPIO Data Clear Register */
#define PORTB_DIR                   0xFFC03098         /* PORTB Port x GPIO Direction Register */
#define PORTB_DIR_SET               0xFFC0309C         /* PORTB Port x GPIO Direction Set Register */
#define PORTB_DIR_CLR               0xFFC030A0         /* PORTB Port x GPIO Direction Clear Register */
#define PORTB_INEN                  0xFFC030A4         /* PORTB Port x GPIO Input Enable Register */
#define PORTB_INEN_SET              0xFFC030A8         /* PORTB Port x GPIO Input Enable Set Register */
#define PORTB_INEN_CLR              0xFFC030AC         /* PORTB Port x GPIO Input Enable Clear Register */
#define PORTB_MUX                   0xFFC030B0         /* PORTB Port x Multiplexer Control Register */
#define PORTB_DATA_TGL              0xFFC030B4         /* PORTB Port x GPIO Input Enable Toggle Register */
#define PORTB_POL                   0xFFC030B8         /* PORTB Port x GPIO Programming Inversion Register */
#define PORTB_POL_SET               0xFFC030BC         /* PORTB Port x GPIO Programming Inversion Set Register */
#define PORTB_POL_CLR               0xFFC030C0         /* PORTB Port x GPIO Programming Inversion Clear Register */
#define PORTB_LOCK                  0xFFC030C4         /* PORTB Port x GPIO Lock Register */
#define PORTB_REVID                 0xFFC030FC         /* PORTB Port x GPIO Revision ID */
#define PORTC_FER                   0xFFC03100         /* PORTC Port x Function Enable Register */
#define PORTC_FER_SET               0xFFC03104         /* PORTC Port x Function Enable Set Register */
#define PORTC_FER_CLR               0xFFC03108         /* PORTC Port x Function Enable Clear Register */
#define PORTC_DATA                  0xFFC0310C         /* PORTC Port x GPIO Data Register */
#define PORTC_DATA_SET              0xFFC03110         /* PORTC Port x GPIO Data Set Register */
#define PORTC_DATA_CLR              0xFFC03114         /* PORTC Port x GPIO Data Clear Register */
#define PORTC_DIR                   0xFFC03118         /* PORTC Port x GPIO Direction Register */
#define PORTC_DIR_SET               0xFFC0311C         /* PORTC Port x GPIO Direction Set Register */
#define PORTC_DIR_CLR               0xFFC03120         /* PORTC Port x GPIO Direction Clear Register */
#define PORTC_INEN                  0xFFC03124         /* PORTC Port x GPIO Input Enable Register */
#define PORTC_INEN_SET              0xFFC03128         /* PORTC Port x GPIO Input Enable Set Register */
#define PORTC_INEN_CLR              0xFFC0312C         /* PORTC Port x GPIO Input Enable Clear Register */
#define PORTC_MUX                   0xFFC03130         /* PORTC Port x Multiplexer Control Register */
#define PORTC_DATA_TGL              0xFFC03134         /* PORTC Port x GPIO Input Enable Toggle Register */
#define PORTC_POL                   0xFFC03138         /* PORTC Port x GPIO Programming Inversion Register */
#define PORTC_POL_SET               0xFFC0313C         /* PORTC Port x GPIO Programming Inversion Set Register */
#define PORTC_POL_CLR               0xFFC03140         /* PORTC Port x GPIO Programming Inversion Clear Register */
#define PORTC_LOCK                  0xFFC03144         /* PORTC Port x GPIO Lock Register */
#define PORTC_REVID                 0xFFC0317C         /* PORTC Port x GPIO Revision ID */
#define PORTD_FER                   0xFFC03180         /* PORTD Port x Function Enable Register */
#define PORTD_FER_SET               0xFFC03184         /* PORTD Port x Function Enable Set Register */
#define PORTD_FER_CLR               0xFFC03188         /* PORTD Port x Function Enable Clear Register */
#define PORTD_DATA                  0xFFC0318C         /* PORTD Port x GPIO Data Register */
#define PORTD_DATA_SET              0xFFC03190         /* PORTD Port x GPIO Data Set Register */
#define PORTD_DATA_CLR              0xFFC03194         /* PORTD Port x GPIO Data Clear Register */
#define PORTD_DIR                   0xFFC03198         /* PORTD Port x GPIO Direction Register */
#define PORTD_DIR_SET               0xFFC0319C         /* PORTD Port x GPIO Direction Set Register */
#define PORTD_DIR_CLR               0xFFC031A0         /* PORTD Port x GPIO Direction Clear Register */
#define PORTD_INEN                  0xFFC031A4         /* PORTD Port x GPIO Input Enable Register */
#define PORTD_INEN_SET              0xFFC031A8         /* PORTD Port x GPIO Input Enable Set Register */
#define PORTD_INEN_CLR              0xFFC031AC         /* PORTD Port x GPIO Input Enable Clear Register */
#define PORTD_MUX                   0xFFC031B0         /* PORTD Port x Multiplexer Control Register */
#define PORTD_DATA_TGL              0xFFC031B4         /* PORTD Port x GPIO Input Enable Toggle Register */
#define PORTD_POL                   0xFFC031B8         /* PORTD Port x GPIO Programming Inversion Register */
#define PORTD_POL_SET               0xFFC031BC         /* PORTD Port x GPIO Programming Inversion Set Register */
#define PORTD_POL_CLR               0xFFC031C0         /* PORTD Port x GPIO Programming Inversion Clear Register */
#define PORTD_LOCK                  0xFFC031C4         /* PORTD Port x GPIO Lock Register */
#define PORTD_REVID                 0xFFC031FC         /* PORTD Port x GPIO Revision ID */
#define PORTE_FER                   0xFFC03200         /* PORTE Port x Function Enable Register */
#define PORTE_FER_SET               0xFFC03204         /* PORTE Port x Function Enable Set Register */
#define PORTE_FER_CLR               0xFFC03208         /* PORTE Port x Function Enable Clear Register */
#define PORTE_DATA                  0xFFC0320C         /* PORTE Port x GPIO Data Register */
#define PORTE_DATA_SET              0xFFC03210         /* PORTE Port x GPIO Data Set Register */
#define PORTE_DATA_CLR              0xFFC03214         /* PORTE Port x GPIO Data Clear Register */
#define PORTE_DIR                   0xFFC03218         /* PORTE Port x GPIO Direction Register */
#define PORTE_DIR_SET               0xFFC0321C         /* PORTE Port x GPIO Direction Set Register */
#define PORTE_DIR_CLR               0xFFC03220         /* PORTE Port x GPIO Direction Clear Register */
#define PORTE_INEN                  0xFFC03224         /* PORTE Port x GPIO Input Enable Register */
#define PORTE_INEN_SET              0xFFC03228         /* PORTE Port x GPIO Input Enable Set Register */
#define PORTE_INEN_CLR              0xFFC0322C         /* PORTE Port x GPIO Input Enable Clear Register */
#define PORTE_MUX                   0xFFC03230         /* PORTE Port x Multiplexer Control Register */
#define PORTE_DATA_TGL              0xFFC03234         /* PORTE Port x GPIO Input Enable Toggle Register */
#define PORTE_POL                   0xFFC03238         /* PORTE Port x GPIO Programming Inversion Register */
#define PORTE_POL_SET               0xFFC0323C         /* PORTE Port x GPIO Programming Inversion Set Register */
#define PORTE_POL_CLR               0xFFC03240         /* PORTE Port x GPIO Programming Inversion Clear Register */
#define PORTE_LOCK                  0xFFC03244         /* PORTE Port x GPIO Lock Register */
#define PORTE_REVID                 0xFFC0327C         /* PORTE Port x GPIO Revision ID */
#define PORTF_FER                   0xFFC03280         /* PORTF Port x Function Enable Register */
#define PORTF_FER_SET               0xFFC03284         /* PORTF Port x Function Enable Set Register */
#define PORTF_FER_CLR               0xFFC03288         /* PORTF Port x Function Enable Clear Register */
#define PORTF_DATA                  0xFFC0328C         /* PORTF Port x GPIO Data Register */
#define PORTF_DATA_SET              0xFFC03290         /* PORTF Port x GPIO Data Set Register */
#define PORTF_DATA_CLR              0xFFC03294         /* PORTF Port x GPIO Data Clear Register */
#define PORTF_DIR                   0xFFC03298         /* PORTF Port x GPIO Direction Register */
#define PORTF_DIR_SET               0xFFC0329C         /* PORTF Port x GPIO Direction Set Register */
#define PORTF_DIR_CLR               0xFFC032A0         /* PORTF Port x GPIO Direction Clear Register */
#define PORTF_INEN                  0xFFC032A4         /* PORTF Port x GPIO Input Enable Register */
#define PORTF_INEN_SET              0xFFC032A8         /* PORTF Port x GPIO Input Enable Set Register */
#define PORTF_INEN_CLR              0xFFC032AC         /* PORTF Port x GPIO Input Enable Clear Register */
#define PORTF_MUX                   0xFFC032B0         /* PORTF Port x Multiplexer Control Register */
#define PORTF_DATA_TGL              0xFFC032B4         /* PORTF Port x GPIO Input Enable Toggle Register */
#define PORTF_POL                   0xFFC032B8         /* PORTF Port x GPIO Programming Inversion Register */
#define PORTF_POL_SET               0xFFC032BC         /* PORTF Port x GPIO Programming Inversion Set Register */
#define PORTF_POL_CLR               0xFFC032C0         /* PORTF Port x GPIO Programming Inversion Clear Register */
#define PORTF_LOCK                  0xFFC032C4         /* PORTF Port x GPIO Lock Register */
#define PORTF_REVID                 0xFFC032FC         /* PORTF Port x GPIO Revision ID */
#define PORTG_FER                   0xFFC03300         /* PORTG Port x Function Enable Register */
#define PORTG_FER_SET               0xFFC03304         /* PORTG Port x Function Enable Set Register */
#define PORTG_FER_CLR               0xFFC03308         /* PORTG Port x Function Enable Clear Register */
#define PORTG_DATA                  0xFFC0330C         /* PORTG Port x GPIO Data Register */
#define PORTG_DATA_SET              0xFFC03310         /* PORTG Port x GPIO Data Set Register */
#define PORTG_DATA_CLR              0xFFC03314         /* PORTG Port x GPIO Data Clear Register */
#define PORTG_DIR                   0xFFC03318         /* PORTG Port x GPIO Direction Register */
#define PORTG_DIR_SET               0xFFC0331C         /* PORTG Port x GPIO Direction Set Register */
#define PORTG_DIR_CLR               0xFFC03320         /* PORTG Port x GPIO Direction Clear Register */
#define PORTG_INEN                  0xFFC03324         /* PORTG Port x GPIO Input Enable Register */
#define PORTG_INEN_SET              0xFFC03328         /* PORTG Port x GPIO Input Enable Set Register */
#define PORTG_INEN_CLR              0xFFC0332C         /* PORTG Port x GPIO Input Enable Clear Register */
#define PORTG_MUX                   0xFFC03330         /* PORTG Port x Multiplexer Control Register */
#define PORTG_DATA_TGL              0xFFC03334         /* PORTG Port x GPIO Input Enable Toggle Register */
#define PORTG_POL                   0xFFC03338         /* PORTG Port x GPIO Programming Inversion Register */
#define PORTG_POL_SET               0xFFC0333C         /* PORTG Port x GPIO Programming Inversion Set Register */
#define PORTG_POL_CLR               0xFFC03340         /* PORTG Port x GPIO Programming Inversion Clear Register */
#define PORTG_LOCK                  0xFFC03344         /* PORTG Port x GPIO Lock Register */
#define PORTG_REVID                 0xFFC0337C         /* PORTG Port x GPIO Revision ID */

/* =========================
        PINT's
   ========================= */
#define REG_PINT0_MSK_SET               0xFFC04000         /* PINT0 Pint Mask Set Register */
#define REG_PINT1_MSK_SET               0xFFC04100         /* PINT1 Pint Mask Set Register */
#define REG_PINT2_MSK_SET               0xFFC04200         /* PINT2 Pint Mask Set Register */
#define REG_PINT3_MSK_SET               0xFFC04300         /* PINT3 Pint Mask Set Register */
#define REG_PINT4_MSK_SET               0xFFC04400         /* PINT4 Pint Mask Set Register */
#define REG_PINT5_MSK_SET               0xFFC04500         /* PINT5 Pint Mask Set Register */

#define REG_PINT_MSK_SET               0x0
#define REG_PINT_MSK_CLR               0x4         /* Pint Mask Clear Register offset */
#define REG_PINT_REQ                   0x8         /* Pint Request Register offset */
#define REG_PINT_ASSIGN                0xC         /* Pint Assign Register offset */
#define REG_PINT_EDGE_SET              0x10         /* Pint Edge Set Register offset */
#define REG_PINT_EDGE_CLR              0x14         /* Pint Edge Clear Register offset */
#define REG_PINT_INV_SET               0x18         /* Pint Invert Set Register offset */
#define REG_PINT_INV_CLR               0x1C         /* Pint Invert Clear Register offset */
#define REG_PINT_PINSTATE              0x20         /* Pint Pinstate Register offset */
#define REG_PINT_LATCH                 0x24         /* Pint Latch Register offset */

/* =========================
        TWI
   ========================= */

#define TWI0_CLKDIV                 0xFFC01E00         /* TWI0 SCL Clock Divider */
#define TWI0_CTL                    0xFFC01E04         /* TWI0 Control Register */
#define TWI0_SLVCTL                 0xFFC01E08         /* TWI0 Slave Mode Control Register */
#define TWI0_SLVSTAT                0xFFC01E0C         /* TWI0 Slave Mode Status Register */
#define TWI0_SLVADDR                0xFFC01E10         /* TWI0 Slave Mode Address Register */
#define TWI0_MSTRCTL                0xFFC01E14         /* TWI0 Master Mode Control Registers */
#define TWI0_MSTRSTAT               0xFFC01E18         /* TWI0 Master Mode Status Register */
#define TWI0_MSTRADDR               0xFFC01E1C         /* TWI0 Master Mode Address Register */
#define TWI0_ISTAT                  0xFFC01E20         /* TWI0 Interrupt Status Register */
#define TWI0_IMSK                   0xFFC01E24         /* TWI0 Interrupt Mask Register */
#define TWI0_FIFOCTL                0xFFC01E28         /* TWI0 FIFO Control Register */
#define TWI0_FIFOSTAT               0xFFC01E2C         /* TWI0 FIFO Status Register */
#define TWI0_TXDATA8                0xFFC01E80         /* TWI0 FIFO Transmit Data Single-Byte Register */
#define TWI0_TXDATA16               0xFFC01E84         /* TWI0 FIFO Transmit Data Double-Byte Register */
#define TWI0_RXDATA8                0xFFC01E88         /* TWI0 FIFO Transmit Data Single-Byte Register */
#define TWI0_RXDATA16               0xFFC01E8C         /* TWI0 FIFO Transmit Data Double-Byte Register */
#define TWI1_CLKDIV                 0xFFC01F00         /* TWI1 SCL Clock Divider */
#define TWI1_CTL                    0xFFC01F04         /* TWI1 Control Register */
#define TWI1_SLVCTL                 0xFFC01F08         /* TWI1 Slave Mode Control Register */
#define TWI1_SLVSTAT                0xFFC01F0C         /* TWI1 Slave Mode Status Register */
#define TWI1_SLVADDR                0xFFC01F10         /* TWI1 Slave Mode Address Register */
#define TWI1_MSTRCTL                0xFFC01F14         /* TWI1 Master Mode Control Registers */
#define TWI1_MSTRSTAT               0xFFC01F18         /* TWI1 Master Mode Status Register */
#define TWI1_MSTRADDR               0xFFC01F1C         /* TWI1 Master Mode Address Register */
#define TWI1_ISTAT                  0xFFC01F20         /* TWI1 Interrupt Status Register */
#define TWI1_IMSK                   0xFFC01F24         /* TWI1 Interrupt Mask Register */
#define TWI1_FIFOCTL                0xFFC01F28         /* TWI1 FIFO Control Register */
#define TWI1_FIFOSTAT               0xFFC01F2C         /* TWI1 FIFO Status Register */
#define TWI1_TXDATA8                0xFFC01F80         /* TWI1 FIFO Transmit Data Single-Byte Register */
#define TWI1_TXDATA16               0xFFC01F84         /* TWI1 FIFO Transmit Data Double-Byte Register */
#define TWI1_RXDATA8                0xFFC01F88         /* TWI1 FIFO Receive Data Single-Byte Register */
#define TWI1_RXDATA16               0xFFC01F8C         /* TWI1 FIFO Receive Data Double-Byte Register */

#define TWI0_BASE                   0xFFC01E00         /**< TWI0 Base Register */
#define TWI1_BASE                   0xFFC01F00         /**< TWI1 Base Register */

#endif


#ifdef CYGPKG_HAL_BFIN_BF537
#define CONFIG_MEM_SIZE 64
#elif defined(CYGPKG_HAL_BFIN_BF533)
    #define CONFIG_MEM_SIZE 128
    #define SDRAM_WIDTH EBCAW_11
#elif defined(CYGPKG_HAL_BFIN_BF609)
    #define CONFIG_MEM_SIZE 256
#else
    #error please select a valid processor type
#endif

/* System Event Controller registers */
#ifdef CYGPKG_HAL_BFIN_BF60x
	#define SEC0_CCTL0                  0xFFCA4400         /* SEC0 Core Control Register n */
	#define SEC0_CCTL1                  0xFFCA4440         /* SEC0 Core Control Register n */
	#define SEC0_CSTAT0                 0xFFCA4404         /* SEC0 Core Status Register n */
	#define SEC0_CSTAT1                 0xFFCA4444         /* SEC0 Core Status Register n */
	#define SEC0_CPND0                  0xFFCA4408         /* SEC0 Core Pending IRQ Register n */
	#define SEC0_CPND1                  0xFFCA4448         /* SEC0 Core Pending IRQ Register n */
	#define SEC0_CACT0                  0xFFCA440C         /* SEC0 Core Active IRQ Register n */
	#define SEC0_CACT1                  0xFFCA444C         /* SEC0 Core Active IRQ Register n */
	#define SEC0_CPMSK0                 0xFFCA4410         /* SEC0 Core IRQ Priority Mask Register n */
	#define SEC0_CPMSK1                 0xFFCA4450         /* SEC0 Core IRQ Priority Mask Register n */
	#define SEC0_CGMSK0                 0xFFCA4414         /* SEC0 Core IRQ Group Mask Register n */
	#define SEC0_CGMSK1                 0xFFCA4454         /* SEC0 Core IRQ Group Mask Register n */
	#define SEC0_CPLVL0                 0xFFCA4418         /* SEC0 Core IRQ Priority Level Register n */
	#define SEC0_CPLVL1                 0xFFCA4458         /* SEC0 Core IRQ Priority Level Register n */
	#define SEC0_CSID0                  0xFFCA441C         /* SEC0 Core IRQ Source ID Register n */
	#define SEC0_CSID1                  0xFFCA445C         /* SEC0 Core IRQ Source ID Register n */
	#define SEC0_FCTL                   0xFFCA4010         /* SEC0 Fault Control Register */
	#define SEC0_FSTAT                  0xFFCA4014         /* SEC0 Fault Status Register */
	#define SEC0_FSID                   0xFFCA4018         /* SEC0 Fault Source ID Register */
	#define SEC0_FEND                   0xFFCA401C         /* SEC0 Fault End Register */
	#define SEC0_FDLY                   0xFFCA4020         /* SEC0 Fault Delay Register */
	#define SEC0_FDLY_CUR               0xFFCA4024         /* SEC0 Fault Delay Current Register */
	#define SEC0_FSRDLY                 0xFFCA4028         /* SEC0 Fault System Reset Delay Register */
	#define SEC0_FSRDLY_CUR             0xFFCA402C         /* SEC0 Fault System Reset Delay Current Register */
	#define SEC0_FCOPP                  0xFFCA4030         /* SEC0 Fault COP Period Register */
	#define SEC0_FCOPP_CUR              0xFFCA4034         /* SEC0 Fault COP Period Current Register */
	#define SEC0_GCTL                   0xFFCA4000         /* SEC0 Global Control Register */
	#define SEC0_GSTAT                  0xFFCA4004         /* SEC0 Global Status Register */
	#define SEC0_RAISE                  0xFFCA4008         /* SEC0 Global Raise Register */
	#define SEC0_END                    0xFFCA400C         /* SEC0 Global End Register */
	#define SEC0_SCTL0                  0xFFCA4800         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL1                  0xFFCA4808         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL2                  0xFFCA4810         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL3                  0xFFCA4818         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL4                  0xFFCA4820         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL5                  0xFFCA4828         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL6                  0xFFCA4830         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL7                  0xFFCA4838         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL8                  0xFFCA4840         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL9                  0xFFCA4848         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL10                 0xFFCA4850         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL11                 0xFFCA4858         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL12                 0xFFCA4860         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL13                 0xFFCA4868         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL14                 0xFFCA4870         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL15                 0xFFCA4878         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL16                 0xFFCA4880         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL17                 0xFFCA4888         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL18                 0xFFCA4890         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL19                 0xFFCA4898         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL20                 0xFFCA48A0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL21                 0xFFCA48A8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL22                 0xFFCA48B0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL23                 0xFFCA48B8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL24                 0xFFCA48C0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL25                 0xFFCA48C8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL26                 0xFFCA48D0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL27                 0xFFCA48D8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL28                 0xFFCA48E0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL29                 0xFFCA48E8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL30                 0xFFCA48F0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL31                 0xFFCA48F8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL32                 0xFFCA4900         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL33                 0xFFCA4908         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL34                 0xFFCA4910         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL35                 0xFFCA4918         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL36                 0xFFCA4920         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL37                 0xFFCA4928         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL38                 0xFFCA4930         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL39                 0xFFCA4938         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL40                 0xFFCA4940         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL41                 0xFFCA4948         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL42                 0xFFCA4950         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL43                 0xFFCA4958         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL44                 0xFFCA4960         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL45                 0xFFCA4968         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL46                 0xFFCA4970         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL47                 0xFFCA4978         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL48                 0xFFCA4980         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL49                 0xFFCA4988         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL50                 0xFFCA4990         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL51                 0xFFCA4998         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL52                 0xFFCA49A0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL53                 0xFFCA49A8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL54                 0xFFCA49B0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL55                 0xFFCA49B8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL56                 0xFFCA49C0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL57                 0xFFCA49C8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL58                 0xFFCA49D0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL59                 0xFFCA49D8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL60                 0xFFCA49E0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL61                 0xFFCA49E8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL62                 0xFFCA49F0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL63                 0xFFCA49F8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL64                 0xFFCA4A00         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL65                 0xFFCA4A08         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL66                 0xFFCA4A10         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL67                 0xFFCA4A18         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL68                 0xFFCA4A20         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL69                 0xFFCA4A28         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL70                 0xFFCA4A30         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL71                 0xFFCA4A38         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL72                 0xFFCA4A40         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL73                 0xFFCA4A48         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL74                 0xFFCA4A50         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL75                 0xFFCA4A58         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL76                 0xFFCA4A60         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL77                 0xFFCA4A68         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL78                 0xFFCA4A70         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL79                 0xFFCA4A78         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL80                 0xFFCA4A80         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL81                 0xFFCA4A88         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL82                 0xFFCA4A90         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL83                 0xFFCA4A98         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL84                 0xFFCA4AA0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL85                 0xFFCA4AA8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL86                 0xFFCA4AB0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL87                 0xFFCA4AB8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL88                 0xFFCA4AC0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL89                 0xFFCA4AC8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL90                 0xFFCA4AD0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL91                 0xFFCA4AD8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL92                 0xFFCA4AE0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL93                 0xFFCA4AE8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL94                 0xFFCA4AF0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL95                 0xFFCA4AF8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL96                 0xFFCA4B00         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL97                 0xFFCA4B08         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL98                 0xFFCA4B10         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL99                 0xFFCA4B18         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL100                0xFFCA4B20         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL101                0xFFCA4B28         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL102                0xFFCA4B30         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL103                0xFFCA4B38         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL104                0xFFCA4B40         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL105                0xFFCA4B48         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL106                0xFFCA4B50         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL107                0xFFCA4B58         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL108                0xFFCA4B60         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL109                0xFFCA4B68         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL110                0xFFCA4B70         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL111                0xFFCA4B78         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL112                0xFFCA4B80         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL113                0xFFCA4B88         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL114                0xFFCA4B90         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL115                0xFFCA4B98         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL116                0xFFCA4BA0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL117                0xFFCA4BA8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL118                0xFFCA4BB0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL119                0xFFCA4BB8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL120                0xFFCA4BC0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL121                0xFFCA4BC8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL122                0xFFCA4BD0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL123                0xFFCA4BD8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL124                0xFFCA4BE0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL125                0xFFCA4BE8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL126                0xFFCA4BF0         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL127                0xFFCA4BF8         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL128                0xFFCA4C00         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL129                0xFFCA4C08         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL130                0xFFCA4C10         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL131                0xFFCA4C18         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL132                0xFFCA4C20         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL133                0xFFCA4C28         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL134                0xFFCA4C30         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL135                0xFFCA4C38         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL136                0xFFCA4C40         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL137                0xFFCA4C48         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL138                0xFFCA4C50         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SCTL139                0xFFCA4C58         /* SEC0 IRQ Source Control Register n */
	#define SEC0_SSTAT0                 0xFFCA4804         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT1                 0xFFCA480C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT2                 0xFFCA4814         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT3                 0xFFCA481C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT4                 0xFFCA4824         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT5                 0xFFCA482C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT6                 0xFFCA4834         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT7                 0xFFCA483C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT8                 0xFFCA4844         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT9                 0xFFCA484C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT10                0xFFCA4854         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT11                0xFFCA485C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT12                0xFFCA4864         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT13                0xFFCA486C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT14                0xFFCA4874         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT15                0xFFCA487C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT16                0xFFCA4884         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT17                0xFFCA488C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT18                0xFFCA4894         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT19                0xFFCA489C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT20                0xFFCA48A4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT21                0xFFCA48AC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT22                0xFFCA48B4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT23                0xFFCA48BC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT24                0xFFCA48C4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT25                0xFFCA48CC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT26                0xFFCA48D4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT27                0xFFCA48DC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT28                0xFFCA48E4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT29                0xFFCA48EC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT30                0xFFCA48F4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT31                0xFFCA48FC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT32                0xFFCA4904         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT33                0xFFCA490C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT34                0xFFCA4914         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT35                0xFFCA491C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT36                0xFFCA4924         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT37                0xFFCA492C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT38                0xFFCA4934         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT39                0xFFCA493C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT40                0xFFCA4944         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT41                0xFFCA494C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT42                0xFFCA4954         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT43                0xFFCA495C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT44                0xFFCA4964         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT45                0xFFCA496C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT46                0xFFCA4974         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT47                0xFFCA497C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT48                0xFFCA4984         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT49                0xFFCA498C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT50                0xFFCA4994         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT51                0xFFCA499C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT52                0xFFCA49A4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT53                0xFFCA49AC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT54                0xFFCA49B4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT55                0xFFCA49BC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT56                0xFFCA49C4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT57                0xFFCA49CC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT58                0xFFCA49D4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT59                0xFFCA49DC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT60                0xFFCA49E4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT61                0xFFCA49EC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT62                0xFFCA49F4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT63                0xFFCA49FC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT64                0xFFCA4A04         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT65                0xFFCA4A0C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT66                0xFFCA4A14         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT67                0xFFCA4A1C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT68                0xFFCA4A24         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT69                0xFFCA4A2C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT70                0xFFCA4A34         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT71                0xFFCA4A3C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT72                0xFFCA4A44         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT73                0xFFCA4A4C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT74                0xFFCA4A54         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT75                0xFFCA4A5C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT76                0xFFCA4A64         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT77                0xFFCA4A6C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT78                0xFFCA4A74         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT79                0xFFCA4A7C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT80                0xFFCA4A84         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT81                0xFFCA4A8C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT82                0xFFCA4A94         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT83                0xFFCA4A9C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT84                0xFFCA4AA4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT85                0xFFCA4AAC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT86                0xFFCA4AB4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT87                0xFFCA4ABC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT88                0xFFCA4AC4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT89                0xFFCA4ACC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT90                0xFFCA4AD4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT91                0xFFCA4ADC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT92                0xFFCA4AE4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT93                0xFFCA4AEC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT94                0xFFCA4AF4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT95                0xFFCA4AFC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT96                0xFFCA4B04         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT97                0xFFCA4B0C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT98                0xFFCA4B14         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT99                0xFFCA4B1C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT100               0xFFCA4B24         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT101               0xFFCA4B2C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT102               0xFFCA4B34         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT103               0xFFCA4B3C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT104               0xFFCA4B44         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT105               0xFFCA4B4C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT106               0xFFCA4B54         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT107               0xFFCA4B5C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT108               0xFFCA4B64         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT109               0xFFCA4B6C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT110               0xFFCA4B74         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT111               0xFFCA4B7C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT112               0xFFCA4B84         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT113               0xFFCA4B8C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT114               0xFFCA4B94         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT115               0xFFCA4B9C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT116               0xFFCA4BA4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT117               0xFFCA4BAC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT118               0xFFCA4BB4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT119               0xFFCA4BBC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT120               0xFFCA4BC4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT121               0xFFCA4BCC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT122               0xFFCA4BD4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT123               0xFFCA4BDC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT124               0xFFCA4BE4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT125               0xFFCA4BEC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT126               0xFFCA4BF4         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT127               0xFFCA4BFC         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT128               0xFFCA4C04         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT129               0xFFCA4C0C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT130               0xFFCA4C14         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT131               0xFFCA4C1C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT132               0xFFCA4C24         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT133               0xFFCA4C2C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT134               0xFFCA4C34         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT135               0xFFCA4C3C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT136               0xFFCA4C44         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT137               0xFFCA4C4C         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT138               0xFFCA4C54         /* SEC0 IRQ Source Status Register n */
	#define SEC0_SSTAT139               0xFFCA4C5C         /* SEC0 IRQ Source Status Register n */

	#define SPI0_REGBASE                0xFFC40400
	#define SPI1_REGBASE                0xFFC40500
		#define SPI_CTL                    	   0x4         /* SPI Control Register */
		#define SPI_RXCTL                      0x8         /* SPI RX Control Register */
		#define SPI_TXCTL                      0xC         /* SPI TX Control Register */
		#define SPI_CLK                       0x10         /* SPI Clock Rate Register */
		#define SPI_DLY                       0x14         /* SPI Delay Register */
		#define SPI_SLVSEL                    0x18         /* SPI Slave Select Register */
		#define SPI_RWC                       0x1C         /* SPI Received Word-Count Register */
		#define SPI_RWCR                      0x20         /* SPI Received Word-Count Reload Register */
		#define SPI_TWC                       0x24         /* SPI Transmitted Word-Count Register */
		#define SPI_TWCR                      0x28         /* SPI Transmitted Word-Count Reload Register */
		#define SPI_IMSK                      0x30         /* SPI Interrupt Mask Register */
		#define SPI_IMSK_CLR                  0x34         /* SPI Interrupt Mask Clear Register */
		#define SPI_IMSK_SET                  0x38         /* SPI Interrupt Mask Set Register */
		#define SPI_STAT                      0x40         /* SPI Status Register */
		#define SPI_ILAT                      0x44         /* SPI Masked Interrupt Condition Register */
		#define SPI_ILAT_CLR                  0x48         /* SPI Masked Interrupt Clear Register */
		#define SPI_RFIFO                     0x50         /* SPI Receive FIFO Data Register */
		#define SPI_TFIFO                     0x58         /* SPI Transmit FIFO Data Register */

	#define EMAC0_REGBASE               0xFFC20000         /* EMAC0 registers baase */ 
	#define EMAC1_REGBASE               0xFFC22000         /* EMAC1 registers baase */ 


#endif //ifdef CYGPKG_HAL_BFIN_BF60x


#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS


#define REG_SIZE 4

/* Status register fields */

// register locations

#define SR_INTCTL_HI    0xFFE0

// interrupt and exception handler entry points
#define SR_EVT0         0x2000          /* emulation event */
#define SR_EVT1         0x2004          /* reset event */
#define SR_EVT2         0x2008          /* NMI event */
#define SR_EVT3         0x200C          /* exception event */
#define SR_EVT4         0x2010          /* RESERVED */
#define SR_EVT5         0x2014          /* HW error */
#define SR_EVT6         0x2018          /* core timer event */
#define SR_EVT7         0x201C          /* IVG7 event */
#define SR_EVT8         0x2020          /* IVG8 event */
#define SR_EVT9         0x2024          /* IVG9 event */
#define SR_EVT10        0x2028          /* IVG10 event */
#define SR_EVT11        0x202C          /* IVG11 event */
#define SR_EVT12        0x2030          /* IVG12 event */
#define SR_EVT13        0x2034          /* IVG13 event */
#define SR_EVT14        0x2038          /* IVG14 event */
#define SR_EVT15        0x203C          /* IVG15 event */

#define SR_IMASK_MMR_LO 0x2104
#define SR_IPEND_MMR_LO  0x2108
#define SR_ILAT_MMR_LO  0x210C
#define SR_IPRIO_MMR_LO 0x2110

#define SR_DSPID        0x5000          /* DSP identifier */
    #define SR_DSPID_ANALOG_DEV     0xE5000000
    #define SR_DSPID_MANUFACTURER   0xFF000000
    #define SR_DSPID_ARCH_MAJOR     0x00FF0000
    #define SR_DSPID_IMPLEMENTATION 0x0000FFFF


// bitmask for IPEND, ILAT and IMASK
#define SR_IBIT15       0x00008000      /*  IVG15 lowest priority, user threads */
#define SR_IBIT14       0x00004000      /*  IVG14 */
#define SR_IBIT13       0x00002000      /*  IVG13 */
#define SR_IBIT12       0x00001000      /*  IVG12 */
#define SR_IBIT11       0x00000800      /*  IVG11 */
#define SR_IBIT10       0x00000400      /*  IVG10 */
#define SR_IBIT9        0x00000200      /*  IVG9 */
#define SR_IBIT8        0x00000100      /*  IVG8 */
#define SR_IBIT7        0x00000080      /*  IVG7 */
#define SR_IBIT6        0x00000040      /*  Core Timer Interrupt IVTMR */
#define SR_IBIT5        0x00000020      /*  Hardware Error Interrupt IVHW */
// IPEND

// these bits only apply to IPEND and ILAT cause IMASK can't be changed
// non-maskable events
#define SR_IBIT4        0x00000008      /*  Exception */
#define SR_IBIT3        0x00000004      /*  Non-Maskable Interrupt (NMI) */
#define SR_IBIT2        0x00000002      /*  Reset */
#define SR_IBIT1        0x00000001      /*  Emulation */

// ASTAT definitions
#define ASTAT_VS      0x02000000      /*  sticky version of ASTAT_V */
#define ASTAT_V       0x01000000      /*  D_REG overflow */
#define ASTAT_AV1S    0x00080000      /*  sticky version of ASTAT_AV1 */
#define ASTAT_AV1     0x00040000      /*  set AC1 generates overflow */
#define ASTAT_AV0S    0x00020000      /*  sticky version of ASTAT_AV0 */
#define ASTAT_AV0     0x00010000      /*  set AC0 generates overflow */
#define ASTAT_AC1     0x00002000      /*  set AC1 generates carry */
#define ASTAT_AC0     0x00001000      /*  set AC0 generates carry */
#define ASTAT_RND_MOD 0x00000100      /*  set if biased rounding, else unbiased */
#define ASTAT_AQ      0x00000040      /*  quotient bit */
#define ASTAT_CC      0x00000020      /*  CC multiprupose flag */
#define ASTAT_VCPY    0x00000008      /*  identical to ASTAT_V (d_reg overflow) */
#define ASTAT_AC0CPY  0x00000004      /*  identical to ASTAT_AC0 (carry) */
#define ASTAT_AN      0x00000002      /*  set if result of last A0, A1, shifter operation is negative */
#define ASTAT_AZ      0x00000001      /*  set if result of last A0, A1, shifter operation is zero */

//SYSCFG definitions
#define SYSCFG_SNEN   0x00000004      /* sefl-nesting interrupts enabled */
#define SYSCFG_CCEN   0x00000002      /* cycle counter enabled */
#define SYSCFG_SSTEP  0x00000001      /* single step for user mode and interrupt based superuser mode, below EXCPT */

//SEQSTAT register
#define SEQSTAT_HWRCAUSE 0x0007C000   /* mask for hardware error cause */ //#define SEQSTAT_HWRCAUSE 0x0007B000 
#define SEQSTAT_SFTRESET 0x00002000   /* set if last core reset was software generated */
#define SEQSTAT_EXCAUSE  0x0000003F   /* mask for excause */

/* HWCAUSE */
#define HWCAUSE_SYSMMR  0x02          /* system mmr error */
#define HWCAUSE_MEMERR  0x03          /* memory addressing error */
#define HWCAUSE_PEROV   0x12          /* performance monitor overflow */
#define HWCAUSE_SOFTHWR 0x18          /* software raised HW error via RAISE 5 */

/* ExCause fields */
#define CAUSE_SINGLE    0x10          /* single step exception */
#define CAUSE_TBUFFULL  0x11          /* trace buffer full */
#define CAUSE_ILLOPC    0x21          /* undefined instruction */
#define CAUSE_OPCCOMB   0x22          /* illegal opcode combination */
#define CAUSE_D_CPLB    0x23          /* attempt to read/write CPLD protected memory */
#define CAUSE_D_MISAL   0x24          /* data misaligned */
#define CAUSE_UNRECOV   0x25          /* unrecoverable event */
#define CAUSE_D_CPLB_MISS 0x26        /* data CPLB miss */
#define CAUSE_D_CPLB_MULT 0x27        /* more than one data CPLB entry matches address */
#define CAUSE_EMWATCH   0x28          /* emulation watchpoint */
#define CAUSE_I_MISAL   0x2A          /* instruction fetch misaligned */
#define CAUSE_I_CPLB    0x2B          /* attempt to read/write CPLD protected memory */
#define CAUSE_I_CPLB_MISS 0x2C        /* instruction CPLB miss */
#define CAUSE_I_CPLB_MULT 0x2D        /* more than one instruction CPLB entry matches address */
#define CAUSE_ILLSUP    0x2E          /* illegal use of supervisor ressource */
#define CAUSE_SOFT      0x0F          /* software cause */
#define CAUSE_TESTSOFT  0x30          /* test for software exception */
#define CAUSE_ISSOFT    0x00          /* software exception */




#define ITEST_COMMAND   0xFFE01300
    #define ITEST_DATA0     0xFFE01400
    #define ITEST_DATA1     0xFFE01404

#define DTEST_COMMAND   0xFFE00300
    #define DTEST_DATA0     0xFFE00400
    #define DTEST_DATA1     0xFFE00404


#define HAL_DCACHE_DMEM_CONTROL    0xFFE00004
#define HAL_DCACHE_ENDCPLD         0x2
#define HAL_DCACHE_DMC             0xC
#define HAL_DCACHE_DCBS            0x10
#define HAL_DCACHE_PORT_PREF0      0x1000
#define HAL_DCACHE_PORT_PREF1      0x2000

#ifdef CYGPKG_HAL_BFIN_BF60x
	#define HAL_DCACHE_CBYPASS	   0x100
	#define HAL_DCACHE_RDCHK	   0x200
	#define HAL_DCACHE_PARSEL	   0x4000
	#define HAL_DCACHE_PARCTL	   0x8000
#endif //ifdef CYGPKG_HAL_BFIN_BF60x

// instruction cacheline options
#define HAL_DCACHE_DATA_BASE       0xFFE00200
#define HAL_DCACHE_ADDR_BASE       0xFFE00100

#define HAL_DCACHE_MMU_ENTRIES     0xF
#define HAL_DCACHE_MMU_ENTRY_MASK  ~HAL_DCACHE_MMU_ENTRIES

#ifdef CYGPKG_HAL_BFIN_BF53x
#define ANOMALY_05000158        0x200
#else
#define ANOMALY_05000158        0x00
#endif

//DCPLB_DATA bits
#define HAL_DCACHE_PAGE_SIZE_1K    0x00000000
#define HAL_DCACHE_PAGE_SIZE_4K    0x00010000
#define HAL_DCACHE_PAGE_SIZE_1M    0x00020000
#define HAL_DCACHE_PAGE_SIZE_4M    0x00030000
#define HAL_DCACHE_PAGE_SIZE_16K   0x00040000
#define HAL_DCACHE_PAGE_SIZE_64K   0x00050000
#define HAL_DCACHE_PAGE_SIZE_16M   0x00060000
#define HAL_DCACHE_PAGE_SIZE_64M   0x00070000
#define HAL_DCACHE_PAGE_SIZE_MASK  0x00070000

//DCPLB_DATA bits
#define HAL_DCACHE_SET_ALLOCATEWRITE   0x00008000
#define HAL_DCACHE_SET_WRITETHROUGH    0x00004000
#define HAL_DCACHE_SET_CACHABLE        0x00001000
#define HAL_DCACHE_SET_DIRTY           0x00000080
#define HAL_DCACHE_SET_ALLOW_SUPER_WR  0x00000010
#define HAL_DCACHE_SET_ALLOW_USER_WR   0x00000008
#define HAL_DCACHE_SET_ALLOW_USER_RD   0x00000004
#define HAL_DCACHE_SET_ALLOW_MASK      0x0000001C
#define HAL_DCACHE_SET_LOCK            0x00000002
#define HAL_DCACHE_SET_VALID           0x00000001

#define HAL_ICACHE_IMEM_CONTROL    0xFFE01004
#define HAL_ICACHE_ENICPLD         0x2
#define HAL_ICACHE_IMC             0x4
#define HAL_ICACHE_ILOCKMASK       0x78

#ifdef CYGPKG_HAL_BFIN_BF60x
	#define HAL_ICACHE_CBYPASS	   0x100
	#define HAL_ICACHE_RDCHK	   0x200
#endif //ifdef CYGPKG_HAL_BFIN_BF60x

#define HAL_ICACHE_LRUPRIORST      0x2000



// instruction cacheline options
#define HAL_ICACHE_DATA_BASE       0xFFE01200
#define HAL_ICACHE_ADDR_BASE       0xFFE01100

#define HAL_ICACHE_MMU_ENTRIES     0xF
#define HAL_ICACHE_MMU_ENTRY_MASK  ~HAL_ICACHE_MMU_ENTRIES

//ICPLB_DATA bits
#define HAL_ICACHE_PAGE_SIZE_1K    0x00000000
#define HAL_ICACHE_PAGE_SIZE_4K    0x00010000
#define HAL_ICACHE_PAGE_SIZE_1M    0x00020000
#define HAL_ICACHE_PAGE_SIZE_4M    0x00030000
#define HAL_ICACHE_PAGE_SIZE_MASK  0x00030000

#define HAL_ICACHE_SET_CACHABLE        0x00001000
#define HAL_ICACHE_SET_HIGH_LRUPRIO    0x00000100
#define HAL_ICACHE_SET_ALLOW_USER_RD   0x00000004
#define HAL_ICACHE_SET_LOCK            0x00000002
#define HAL_ICACHE_SET_VALID           0x00000001

#endif // ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

//==========================================================================
//                                EPPI
//==========================================================================

//--------------------------------------------------------------------------
// Register definition
//
#define EPPI0_BASE					0xFFC18000		    /* The beginning registers of EPPI0 bus */
#define EPPI1_BASE					0xFFC18400		    /* The beginning registers of EPPI1 bus */
#define EPPI2_BASE					0xFFC18800		    /* The beginning registers of EPPI2 bus */

#define EPPI_STAT_OFFSET            0                   /* Status Register */
#define EPPI_HCNT_OFFSET            0x4         	    /* Horizontal Transfer Count Register */
#define EPPI_HDLY_OFFSET           	0x8                 /* Horizontal Delay Count Register */
#define EPPI_VCNT_OFFSET           	0xC          		/* Vertical Transfer Count Register */
#define EPPI_VDLY_OFFSET            0x10         		/* Vertical Delay Count Register */
#define EPPI_FRAME_OFFSET           0x14         		/* Lines Per Frame Register */
#define EPPI_LINE_OFFSET            0x18         		/* Samples Per Line Register */
#define EPPI_CLKDIV_OFFSET          0x1C         		/* Clock Divide Register */
#define EPPI_CTL_OFFSET             0x20         		/* Control Register */
#define EPPI_FS1_WLHB_OFFSET        0x24         		/* FS1 Width Register / EPPI Horizontal Blanking Samples Per Line Register */
#define EPPI_FS1_PASPL_OFFSET       0x28         		/* FS1 Period Register / EPPI Active Samples Per Line Register */
#define EPPI_FS2_WLVB_OFFSET        0x2C         		/* FS2 Width Register / EPPI Lines Of Vertical Blanking Register */
#define EPPI_FS2_PALPF_OFFSET       0x30         		/* FS2 Period Register / EPPI Active Lines Per Field Register */
#define EPPI_IMSK_OFFSET            0x34         		/* Interrupt Mask Register */
#define EPPI_ODDCLIP_OFFSET         0x3C         		/* Clipping Register for ODD (Chroma) Data */
#define EPPI_EVENCLIP_OFFSET        0x40         		/* Clipping Register for EVEN (Luma) Data */
#define EPPI_FS1_DLY_OFFSET         0x44         		/* Frame Sync 1 Delay Value */
#define EPPI_FS2_DLY_OFFSET         0x48         		/* Frame Sync 2 Delay Value */
#define EPPI_CTL2_OFFSET            0x4C         		/* Control Register 2 */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_STAT                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_STAT_FLD                   15                               /* Current Field Received by EPPI */
#define BITP_EPPI_STAT_ERRDET                14                               /* Preamble Error Detected */
#define BITP_EPPI_STAT_PXPERR                 7                               /* PxP Ready Error */
#define BITP_EPPI_STAT_ERRNCOR                6                               /* Preamble Error Not Corrected */
#define BITP_EPPI_STAT_FTERRUNDR              5                               /* Frame Track Underflow */
#define BITP_EPPI_STAT_FTERROVR               4                               /* Frame Track Overflow */
#define BITP_EPPI_STAT_LTERRUNDR              3                               /* Line Track Underflow */
#define BITP_EPPI_STAT_LTERROVR               2                               /* Line Track Overflow */
#define BITP_EPPI_STAT_YFIFOERR               1                               /* Luma FIFO Error */
#define BITP_EPPI_STAT_CFIFOERR               0                               /* Chroma FIFO Error */

#define BITM_EPPI_STAT_FLD                   0x00008000  					/* Current Field Received by EPPI */
#define ENUM_EPPI_STAT_FIELD1                0x00000000  					/* FLD: Field 1 */
#define ENUM_EPPI_STAT_FIELD2                0x00008000  					/* FLD: Field 2 */

#define BITM_EPPI_STAT_ERRDET                0x00004000  					/* Preamble Error Detected */
#define ENUM_EPPI_STAT_NO_PRERR              0x00000000  					/* ERRDET: No preamble error detected */
#define ENUM_EPPI_STAT_PRERR                 0x00004000  					/* ERRDET: Preamble error detected */
#define BITM_EPPI_STAT_PXPERR                0x00000080  					/* PxP Ready Error */

#define BITM_EPPI_STAT_ERRNCOR               0x00000040  					/* Preamble Error Not Corrected */
#define ENUM_EPPI_STAT_NO_ERRNCOR            0x00000000  					/* ERRNCOR: No uncorrected preamble error has occurred */
#define ENUM_EPPI_STAT_ERRNCOR               0x00000040  					/* ERRNCOR: Preamble error detected but not corrected */

#define BITM_EPPI_STAT_FTERRUNDR             0x00000020  					/* Frame Track Underflow */
#define ENUM_EPPI_STAT_NO_FTERRUNDR          0x00000000  					/* FTERRUNDR: No Error Detected */
#define ENUM_EPPI_STAT_FTERRUNDR             0x00000020  					/* FTERRUNDR: Error Occurred */

#define BITM_EPPI_STAT_FTERROVR              0x00000010  					/* Frame Track Overflow */
#define ENUM_EPPI_STAT_NO_FTERROVR           0x00000000  					/* FTERROVR: No Error Detected */
#define ENUM_EPPI_STAT_FTERROVR              0x00000010  					/* FTERROVR: Error Occurred */

#define BITM_EPPI_STAT_LTERRUNDR             0x00000008  					/* Line Track Underflow */
#define ENUM_EPPI_STAT_NO_LTERRUNDR          0x00000000  					/* LTERRUNDR: No Error Detected */
#define ENUM_EPPI_STAT_LTERRUNDR             0x00000008  					/* LTERRUNDR: Error Occurred */

#define BITM_EPPI_STAT_LTERROVR              0x00000004  					/* Line Track Overflow */
#define ENUM_EPPI_STAT_NO_LTERROVR           0x00000000  					/* LTERROVR: No Error Detected */
#define ENUM_EPPI_STAT_LTERROVR              0x00000004  					/* LTERROVR: Error Occurred */

#define BITM_EPPI_STAT_YFIFOERR              0x00000002  					/* Luma FIFO Error */
#define ENUM_EPPI_STAT_NO_YFIFOERR           0x00000000  					/* YFIFOERR: No Error Detected */
#define ENUM_EPPI_STAT_YFIFOERR              0x00000002  					/* YFIFOERR: Error Occurred */

#define BITM_EPPI_STAT_CFIFOERR              0x00000001  					/* Chroma FIFO Error */
#define ENUM_EPPI_STAT_NO_CFIFOERR           0x00000000  					/* CFIFOERR: No Error Detected */
#define ENUM_EPPI_STAT_CFIFOERR              0x00000001  					/* CFIFOERR: Error Occurred */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_HCNT                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_HCNT_VALUE                  0                             /* Horizontal Transfer Count */
#define BITM_EPPI_HCNT_VALUE                 0x0000FFFF  					/* Horizontal Transfer Count */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_HDLY                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_HDLY_VALUE                  0                             /* Horizontal Delay Count */
#define BITM_EPPI_HDLY_VALUE                 0x0000FFFF  					/* Horizontal Delay Count */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_VCNT                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_VCNT_VALUE                  0                             /* Vertical Transfer Count */
#define BITM_EPPI_VCNT_VALUE                 0x0000FFFF  					/* Vertical Transfer Count */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_VDLY                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_VDLY_VALUE                  0                             /* Vertical Delay Count */
#define BITM_EPPI_VDLY_VALUE                 0x0000FFFF  					/* Vertical Delay Count */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_FRAME                           Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_FRAME_VALUE                 0                             /* Lines Per Frame */
#define BITM_EPPI_FRAME_VALUE                0x0000FFFF  					/* Lines Per Frame */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_LINE                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_LINE_VALUE                  0                             /* Samples Per Line */
#define BITM_EPPI_LINE_VALUE                 0x0000FFFF  					/* Samples Per Line */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_CLKDIV                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_CLKDIV_VALUE                0                             /* Internal Clock Divider */
#define BITM_EPPI_CLKDIV_VALUE               0x0000FFFF  					/* Internal Clock Divider */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_CTL                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_CTL_CLKGATEN               31                             /* Clock Gating Enable */
#define BITP_EPPI_CTL_MUXSEL                 30                             /* MUX Select */
#define BITP_EPPI_CTL_DMAFINEN               29                             /* DMA Finish Enable */
#define BITP_EPPI_CTL_DMACFG                 28                             /* One or Two DMA Channels Mode */
#define BITP_EPPI_CTL_RGBFMTEN               27                             /* RGB Formatting Enable */
#define BITP_EPPI_CTL_SPLTWRD                26                             /* Split Word */
#define BITP_EPPI_CTL_SUBSPLTODD             25                             /* Sub-Split Odd Samples */
#define BITP_EPPI_CTL_SPLTEO                 24                             /* Split Even and Odd Data Samples */
#define BITP_EPPI_CTL_SWAPEN                 23                             /* Swap Enable */
#define BITP_EPPI_CTL_PACKEN                 22                             /* Pack/Unpack Enable */
#define BITP_EPPI_CTL_SKIPEO                 21                             /* Skip Even or Odd */
#define BITP_EPPI_CTL_SKIPEN                 20                             /* Skip Enable */
#define BITP_EPPI_CTL_DMIRR                  19                             /* Data Mirroring */
#define BITP_EPPI_CTL_DLEN                   16                             /* Data Length */
#define BITP_EPPI_CTL_POLS                   14                             /* Frame Sync Polarity */
#define BITP_EPPI_CTL_POLC                   12                             /* Clock Polarity */
#define BITP_EPPI_CTL_SIGNEXT                11                             /* Sign Extension */
#define BITP_EPPI_CTL_IFSGEN                 10                             /* Internal Frame Sync Generation */
#define BITP_EPPI_CTL_ICLKGEN                 9                             /* Internal Clock Generation */
#define BITP_EPPI_CTL_BLANKGEN                8                             /* king Generation (ITU Output Mode) */
#define BITP_EPPI_CTL_ITUTYPE                 7                             /* ITU Interlace or Progressive */
#define BITP_EPPI_CTL_FLDSEL                  6                             /* Field Select/Trigger */
#define BITP_EPPI_CTL_FSCFG                   4                             /* Frame Sync Configuration */
#define BITP_EPPI_CTL_XFRTYPE                 2                             /* Transfer Type ( Operating Mode) */
#define BITP_EPPI_CTL_DIR                     1                             /* PPI Direction */
#define BITP_EPPI_CTL_EN                      0                             /* PPI Enable */

#define BITM_EPPI_CTL_CLKGATEN               0x80000000  					/* Clock Gating Enable */
#define ENUM_EPPI_CTL_CLKGATE_DIS            0x00000000  					/* CLKGATEN: Disable */
#define ENUM_EPPI_CTL_CLKGATE_EN             0x80000000  					/* CLKGATEN: Enable */

#define BITM_EPPI_CTL_MUXSEL                 0x40000000  					/* MUX Select */
#define ENUM_EPPI_CTL_MUXSEL0                0x00000000  					/* MUXSEL: Normal Operation */
#define ENUM_EPPI_CTL_MUXSEL1                0x40000000  					/* MUXSEL: Multiplexed Operation */

#define BITM_EPPI_CTL_DMAFINEN               0x20000000  					/* DMA Finish Enable */
#define ENUM_EPPI_CTL_FINISH_DIS             0x00000000  					/* DMAFINEN: No Finish Command */
#define ENUM_EPPI_CTL_FINISH_EN              0x20000000  					/* DMAFINEN: Enable Send Finish Command */

#define BITM_EPPI_CTL_DMACFG                 0x10000000  					/* One or Two DMA Channels Mode */
#define ENUM_EPPI_CTL_DMA1CHAN               0x00000000  					/* DMACFG: PPI uses one DMA Channel */
#define ENUM_EPPI_CTL_DMA2CHAN               0x10000000  					/* DMACFG: PPI uses two DMA Channels */

#define BITM_EPPI_CTL_RGBFMTEN               0x08000000  					/* RGB Formatting Enable */
#define ENUM_EPPI_CTL_RGBFMT_DIS             0x00000000  					/* RGBFMTEN: Disable RGB Formatted Output */
#define ENUM_EPPI_CTL_RGBFMT_EN              0x08000000  					/* RGBFMTEN: Enable RGB Formatted Output */

#define BITM_EPPI_CTL_SPLTWRD                0x04000000  					/* Split Word */
#define ENUM_EPPI_CTL_NO_WORDSPLIT           0x00000000  					/* SPLTWRD: PPI_DATA has (DLEN-1) bits of Y or Cr or Cb */
#define ENUM_EPPI_CTL_WORDSPLIT              0x04000000  					/* SPLTWRD: PPI_DATA contains 2 elements per word */

#define BITM_EPPI_CTL_SUBSPLTODD             0x02000000  					/* Sub-Split Odd Samples */
#define ENUM_EPPI_CTL_NO_SUBSPLIT            0x00000000  					/* SUBSPLTODD: Disable */
#define ENUM_EPPI_CTL_SUBSPLIT_ODD           0x02000000  					/* SUBSPLTODD: Enable */

#define BITM_EPPI_CTL_SPLTEO                 0x01000000  					/* Split Even and Odd Data Samples */
#define ENUM_EPPI_CTL_SPLTEO_DIS             0x00000000  					/* SPLTEO: Do Not Split Samples */
#define ENUM_EPPI_CTL_SPLTEO_EN              0x01000000  					/* SPLTEO: Split Even/Odd Samples */

#define BITM_EPPI_CTL_SWAPEN                 0x00800000  					/* Swap Enable */
#define ENUM_EPPI_CTL_SWAP_DIS               0x00000000  					/* SWAPEN: Disable */
#define ENUM_EPPI_CTL_SWAP_EN                0x00800000  					/* SWAPEN: Enable */
	
#define BITM_EPPI_CTL_PACKEN                 0x00400000  					/* Pack/Unpack Enable */
#define ENUM_EPPI_CTL_PACK_DIS               0x00000000  					/* PACKEN: Disable */
#define ENUM_EPPI_CTL_PACK_EN                0x00400000  					/* PACKEN: Enable */

#define BITM_EPPI_CTL_SKIPEO                 0x00200000  					/* Skip Even or Odd */
#define ENUM_EPPI_CTL_SKIPODD                0x00000000  					/* SKIPEO: Skip Odd Samples */
#define ENUM_EPPI_CTL_SKIPEVEN               0x00200000  					/* SKIPEO: Skip Even Samples */

#define BITM_EPPI_CTL_SKIPEN                 0x00100000  					/* Skip Enable */
#define ENUM_EPPI_CTL_NO_SKIP                0x00000000  					/* SKIPEN: No Samples Skipping */
#define ENUM_EPPI_CTL_SKIP                   0x00100000  					/* SKIPEN: Skip Alternate Samples */

#define BITM_EPPI_CTL_DMIRR                  0x00080000  					/* Data Mirroring */
#define ENUM_EPPI_CTL_NO_MIRROR              0x00000000  					/* DMIRR: No Data Mirroring */
#define ENUM_EPPI_CTL_MIRROR                 0x00080000  					/* DMIRR: Data Mirroring */

#define BITM_EPPI_CTL_DLEN                   0x00070000  					/* Data Length */
#define ENUM_EPPI_CTL_DLEN08                 0x00000000  					/* DLEN: 8 bits */
#define ENUM_EPPI_CTL_DLEN10                 0x00010000  					/* DLEN: 10 bits */
#define ENUM_EPPI_CTL_DLEN12                 0x00020000  					/* DLEN: 12 bits */
#define ENUM_EPPI_CTL_DLEN14                 0x00030000  					/* DLEN: 14 bits */
#define ENUM_EPPI_CTL_DLEN16                 0x00040000  					/* DLEN: 16 bits */
#define ENUM_EPPI_CTL_DLEN18                 0x00050000  					/* DLEN: 18 bits */
#define ENUM_EPPI_CTL_DLEN20                 0x00060000  					/* DLEN: 20 bits */
#define ENUM_EPPI_CTL_DLEN24                 0x00070000  					/* DLEN: 24 bits */

#define BITM_EPPI_CTL_POLS                   0x0000C000  					/* Frame Sync Polarity */
#define ENUM_EPPI_CTL_FS1HI_FS2HI            0x00000000  					/* POLS: FS1 and FS2 are active high */
#define ENUM_EPPI_CTL_FS1LO_FS2HI            0x00004000  					/* POLS: FS1 is active low. FS2 is active high */
#define ENUM_EPPI_CTL_FS1HI_FS2LO            0x00008000  					/* POLS: FS1 is active high. FS2 is active low */
#define ENUM_EPPI_CTL_FS1LO_FS2LO            0x0000C000  					/* POLS: FS1 and FS2 are active low */

#define BITM_EPPI_CTL_POLC                   0x00003000  					/* Clock Polarity */
#define ENUM_EPPI_CTL_POLC00                 0x00000000  					/* POLC: Clock/Sync polarity mode 0 */
#define ENUM_EPPI_CTL_POLC01                 0x00001000  					/* POLC: Clock/Sync polarity mode 1 */
#define ENUM_EPPI_CTL_POLC10                 0x00002000  					/* POLC: Clock/Sync polarity mode 2 */
#define ENUM_EPPI_CTL_POLC11                 0x00003000  					/* POLC: Clock/Sync polarity mode 3 */

#define BITM_EPPI_CTL_SIGNEXT                0x00000800  					/* Sign Extension */
#define ENUM_EPPI_CTL_ZEROFILL               0x00000000  					/* SIGNEXT: Zero Filled */
#define ENUM_EPPI_CTL_SIGNEXT                0x00000800  					/* SIGNEXT: Sign Extended */

#define BITM_EPPI_CTL_IFSGEN                 0x00000400  					/* Internal Frame Sync Generation */
#define ENUM_EPPI_CTL_EXTFS                  0x00000000  					/* IFSGEN: External Frame Sync */
#define ENUM_EPPI_CTL_INTFS                  0x00000400  					/* IFSGEN: Internal Frame Sync */

#define BITM_EPPI_CTL_ICLKGEN                0x00000200  					/* Internal Clock Generation */
#define ENUM_EPPI_CTL_EXTCLK                 0x00000000  					/* ICLKGEN: External Clock */
#define ENUM_EPPI_CTL_INTCLK                 0x00000200  					/* ICLKGEN: Internal Clock */

#define BITM_EPPI_CTL_BLANKGEN               0x00000100  					/* king Generation (ITU Output Mode) */
#define ENUM_EPPI_CTL_NO_BLANKGEN            0x00000000  					/* BLANKGEN: Disable */
#define ENUM_EPPI_CTL_BLANKGEN               0x00000100  					/* BLANKGEN: Enable */

#define BITM_EPPI_CTL_ITUTYPE                0x00000080  					/* ITU Interlace or Progressive */
#define ENUM_EPPI_CTL_INTERLACED             0x00000000  					/* ITUTYPE: Interlaced */
#define ENUM_EPPI_CTL_PROGRESSIVE            0x00000080  					/* ITUTYPE: Progressive */

#define BITM_EPPI_CTL_FLDSEL                 0x00000040  					/* Field Select/Trigger */
#define ENUM_EPPI_CTL_FLDSEL_LO              0x00000000  					/* FLDSEL: Field Mode 0 */
#define ENUM_EPPI_CTL_FLDSEL_HI              0x00000040  					/* FLDSEL: Field Mode 1 */

#define BITM_EPPI_CTL_FSCFG                  0x00000030  					/* Frame Sync Configuration */
#define ENUM_EPPI_CTL_SYNC0                  0x00000000  					/* FSCFG: Sync Mode 0 */
#define ENUM_EPPI_CTL_SYNC1                  0x00000010  					/* FSCFG: Sync Mode 1 */
#define ENUM_EPPI_CTL_SYNC2                  0x00000020  					/* FSCFG: Sync Mode 2 */
#define ENUM_EPPI_CTL_SYNC3                  0x00000030  					/* FSCFG: Sync Mode 3 */

#define BITM_EPPI_CTL_XFRTYPE                0x0000000C  					/* Transfer Type ( Operating Mode) */
#define ENUM_EPPI_CTL_ACTIVE656              0x00000000  					/* XFRTYPE: ITU656 Active Video Only Mode */
#define ENUM_EPPI_CTL_ENTIRE656              0x00000004  					/* XFRTYPE: ITU656 Entire Field Mode */
#define ENUM_EPPI_CTL_VERT656                0x00000008  					/* XFRTYPE: ITU656 Vertical Blanking Only Mode */
#define ENUM_EPPI_CTL_NON656                 0x0000000C  					/* XFRTYPE: Non-ITU656 Mode (GP Mode) */

#define BITM_EPPI_CTL_DIR                    0x00000002  					/* PPI Direction */
#define ENUM_EPPI_CTL_RXMODE                 0x00000000  					/* DIR: Receive Mode */
#define ENUM_EPPI_CTL_TXMODE                 0x00000002  					/* DIR: Transmit Mode */

#define BITM_EPPI_CTL_EN                     0x00000001  					/* PPI Enable */
#define ENUM_EPPI_CTL_DIS                    0x00000000  					/* EN: Disable */
#define ENUM_EPPI_CTL_EN                     0x00000001  					/* EN: Enable */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_FS2_WLVB                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_FS2_WLVB_F2VBAD            24                             /* Field 2 Vertical Blanking After Data */
#define BITP_EPPI_FS2_WLVB_F2VBBD            16                             /* Field 2 Vertical Blanking Before Data */
#define BITP_EPPI_FS2_WLVB_F1VBAD             8                             /* Field 1 Vertical Blanking After Data */
#define BITP_EPPI_FS2_WLVB_F1VBBD             0                             /* Field 1 Vertical Blanking Before Data */
#define BITM_EPPI_FS2_WLVB_F2VBAD            0xFF000000  					/* Field 2 Vertical Blanking After Data */
#define BITM_EPPI_FS2_WLVB_F2VBBD            0x00FF0000  					/* Field 2 Vertical Blanking Before Data */
#define BITM_EPPI_FS2_WLVB_F1VBAD            0x0000FF00  					/* Field 1 Vertical Blanking After Data */
#define BITM_EPPI_FS2_WLVB_F1VBBD            0x000000FF  					/* Field 1 Vertical Blanking Before Data */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_FS2_PALPF                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_FS2_PALPF_F2ACT            16                             /* Field 2 Active */
#define BITP_EPPI_FS2_PALPF_F1ACT             0                             /* Field 1 Active */
#define BITM_EPPI_FS2_PALPF_F2ACT            0xFFFF0000  					/* Field 2 Active */
#define BITM_EPPI_FS2_PALPF_F1ACT            0x0000FFFF  					/* Field 1 Active */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_IMSK                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_IMSK_PXPERR                 7                             /* PxP Ready Error Interrupt Mask */
#define BITP_EPPI_IMSK_ERRNCOR                6                             /* ITU Preamble Error Not Corrected Interrupt Mask */
#define BITP_EPPI_IMSK_FTERRUNDR              5                             /* Frame Track Underflow Error Interrupt Mask */
#define BITP_EPPI_IMSK_FTERROVR               4                             /* Frame Track Overflow Error Interrupt Mask */
#define BITP_EPPI_IMSK_LTERRUNDR              3                             /* Line Track Underflow Error Interrupt Mask */
#define BITP_EPPI_IMSK_LTERROVR               2                             /* Line Track Overflow Error Interrupt Mask */
#define BITP_EPPI_IMSK_YFIFOERR               1                             /* YFIFO Underflow or Overflow Error Interrupt Mask */
#define BITP_EPPI_IMSK_CFIFOERR               0                             /* CFIFO Underflow or Overflow Error Interrupt Mask */

#define BITM_EPPI_IMSK_PXPERR                0x00000080  					/* PxP Ready Error Interrupt Mask */
#define ENUM_EPPI_IMSK_PXPERR_UMSK           0x00000000  					/* PXPERR: Unmask Interrupt */
#define ENUM_EPPI_IMSK_PXPERR_MSK            0x00000080  					/* PXPERR: Mask Interrupt */

#define BITM_EPPI_IMSK_ERRNCOR               0x00000040  					/* ITU Preamble Error Not Corrected Interrupt Mask */
#define ENUM_EPPI_IMSK_ERRNCOR_UMSK          0x00000000  					/* ERRNCOR: Unmask Interrupt */
#define ENUM_EPPI_IMSK_ERRNCOR_MSK           0x00000040  					/* ERRNCOR: Mask Interrupt */

#define BITM_EPPI_IMSK_FTERRUNDR             0x00000020  					/* Frame Track Underflow Error Interrupt Mask */
#define ENUM_EPPI_IMSK_FTERRUNDR_UMSK        0x00000000  					/* FTERRUNDR: Unmask Interrupt */
#define ENUM_EPPI_IMSK_FTERRUNDR_MSK         0x00000020  					/* FTERRUNDR: Mask Interrupt */

#define BITM_EPPI_IMSK_FTERROVR              0x00000010  					/* Frame Track Overflow Error Interrupt Mask */
#define ENUM_EPPI_IMSK_FTERROVR_UMSK         0x00000000  					/* FTERROVR: Unmask Interrupt */
#define ENUM_EPPI_IMSK_FTERROVR_MSK          0x00000010  					/* FTERROVR: Mask Interrupt */

#define BITM_EPPI_IMSK_LTERRUNDR             0x00000008  					/* Line Track Underflow Error Interrupt Mask */
#define ENUM_EPPI_IMSK_LTERRUNDR_UMSK        0x00000000  					/* LTERRUNDR: Unmask Interrupt */
#define ENUM_EPPI_IMSK_LTERRUNDR_MSK         0x00000008  					/* LTERRUNDR: Mask Interrupt */

#define BITM_EPPI_IMSK_LTERROVR              0x00000004  					/* Line Track Overflow Error Interrupt Mask */
#define ENUM_EPPI_IMSK_LTERROVR_UMSK         0x00000000  					/* LTERROVR: Unmask Interrupt */
#define ENUM_EPPI_IMSK_LTERROVR_MSK          0x00000004  					/* LTERROVR: Mask Interrupt */

#define BITM_EPPI_IMSK_YFIFOERR              0x00000002  					/* YFIFO Underflow or Overflow Error Interrupt Mask */
#define ENUM_EPPI_IMSK_YFIFOERR_UMSK         0x00000000  					/* YFIFOERR: Unmask Interrupt */
#define ENUM_EPPI_IMSK_YFIFOERR_MSK          0x00000002  					/* YFIFOERR: Mask Interrupt */

#define BITM_EPPI_IMSK_CFIFOERR              0x00000001  					/* CFIFO Underflow or Overflow Error Interrupt Mask */
#define ENUM_EPPI_IMSK_CFIFOERR_UMSK         0x00000000 					/* CFIFOERR: Unmask Interrupt */
#define ENUM_EPPI_IMSK_CFIFOERR_MSK          0x00000001  					/* CFIFOERR: Mask Interrupt */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_ODDCLIP                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_ODDCLIP_HIGHODD            16                             /* High Odd Clipping Threshold (Chroma Data) */
#define BITP_EPPI_ODDCLIP_LOWODD             0                              /* Low Odd Clipping Threshold (Chroma Data) */
#define BITM_EPPI_ODDCLIP_HIGHODD            0xFFFF0000  					/* High Odd Clipping Threshold (Chroma Data) */
#define BITM_EPPI_ODDCLIP_LOWODD             0x0000FFFF  					/* Low Odd Clipping Threshold (Chroma Data) */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_EVENCLIP                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_EVENCLIP_HIGHEVEN          16                             /* High Even Clipping Threshold (Luma Data) */
#define BITP_EPPI_EVENCLIP_LOWEVEN            0                             /* Low Even Clipping Threshold (Luma Data) */
#define BITM_EPPI_EVENCLIP_HIGHEVEN          0xFFFF0000  					/* High Even Clipping Threshold (Luma Data) */
#define BITM_EPPI_EVENCLIP_LOWEVEN           0x0000FFFF  					/* Low Even Clipping Threshold (Luma Data) */

/* ------------------------------------------------------------------------------------------------------------------------
        EPPI_CTL2                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EPPI_CTL2_FS1FINEN               1          					/* HSYNC Finish Enable */

#define BITM_EPPI_CTL2_FS1FINEN              0x00000002  					/* HSYNC Finish Enable */
#define ENUM_EPPI_CTL2_FS2FIN_EN             0x00000000  					/* FS1FINEN: Finish sent after frame RX done */
#define ENUM_EPPI_CTL2_FS1FIN_EN             0x00000002  					/* FS1FINEN: Finish sent after frame/line RX done */

/* ==================================================
        DMA Channel Registers
   ================================================== */
   
#define REG_DMA0_DSCPTR_NXT             0xFFC41000         /* DMA0 Pointer to Next Initial Descriptor */
#define REG_DMA1_DSCPTR_NXT             0xFFC41080         /* DMA1 Pointer to Next Initial Descriptor */
#define REG_DMA2_DSCPTR_NXT             0xFFC41100         /* DMA2 Pointer to Next Initial Descriptor */
#define REG_DMA3_DSCPTR_NXT             0xFFC41180         /* DMA3 Pointer to Next Initial Descriptor */
#define REG_DMA4_DSCPTR_NXT             0xFFC41200         /* DMA4 Pointer to Next Initial Descriptor */
#define REG_DMA5_DSCPTR_NXT             0xFFC41280         /* DMA5 Pointer to Next Initial Descriptor */
#define REG_DMA6_DSCPTR_NXT             0xFFC41300         /* DMA6 Pointer to Next Initial Descriptor */
#define REG_DMA7_DSCPTR_NXT             0xFFC41380         /* DMA7 Pointer to Next Initial Descriptor */
#define REG_DMA8_DSCPTR_NXT             0xFFC41400         /* DMA8 Pointer to Next Initial Descriptor */
#define REG_DMA9_DSCPTR_NXT             0xFFC41480         /* DMA9 Pointer to Next Initial Descriptor */
#define REG_DMA10_DSCPTR_NXT            0xFFC05000         /* DMA10 Pointer to Next Initial Descriptor */
#define REG_DMA11_DSCPTR_NXT            0xFFC05080         /* DMA11 Pointer to Next Initial Descriptor */
#define REG_DMA12_DSCPTR_NXT            0xFFC05100         /* DMA12 Pointer to Next Initial Descriptor */
#define REG_DMA13_DSCPTR_NXT            0xFFC07000         /* DMA13 Pointer to Next Initial Descriptor */
#define REG_DMA14_DSCPTR_NXT            0xFFC07080         /* DMA14 Pointer to Next Initial Descriptor */
#define REG_DMA15_DSCPTR_NXT            0xFFC07100         /* DMA15 Pointer to Next Initial Descriptor */
#define REG_DMA16_DSCPTR_NXT            0xFFC07180         /* DMA16 Pointer to Next Initial Descriptor */
#define REG_DMA17_DSCPTR_NXT            0xFFC07200         /* DMA17 Pointer to Next Initial Descriptor */
#define REG_DMA18_DSCPTR_NXT            0xFFC07280         /* DMA18 Pointer to Next Initial Descriptor */
#define REG_DMA19_DSCPTR_NXT            0xFFC07300         /* DMA19 Pointer to Next Initial Descriptor */
#define REG_DMA20_DSCPTR_NXT            0xFFC07380         /* DMA20 Pointer to Next Initial Descriptor */
#define REG_DMA21_DSCPTR_NXT            0xFFC09000         /* DMA21 Pointer to Next Initial Descriptor */
#define REG_DMA22_DSCPTR_NXT            0xFFC09080         /* DMA22 Pointer to Next Initial Descriptor */
#define REG_DMA23_DSCPTR_NXT            0xFFC09100         /* DMA23 Pointer to Next Initial Descriptor */
#define REG_DMA24_DSCPTR_NXT            0xFFC09180         /* DMA24 Pointer to Next Initial Descriptor */
#define REG_DMA25_DSCPTR_NXT            0xFFC09200         /* DMA25 Pointer to Next Initial Descriptor */
#define REG_DMA26_DSCPTR_NXT            0xFFC09280         /* DMA26 Pointer to Next Initial Descriptor */
#define REG_DMA27_DSCPTR_NXT            0xFFC09300         /* DMA27 Pointer to Next Initial Descriptor */
#define REG_DMA28_DSCPTR_NXT            0xFFC09380         /* DMA28 Pointer to Next Initial Descriptor */
#define REG_DMA29_DSCPTR_NXT            0xFFC0B000         /* DMA29 Pointer to Next Initial Descriptor */
#define REG_DMA30_DSCPTR_NXT            0xFFC0B080         /* DMA30 Pointer to Next Initial Descriptor */
#define REG_DMA31_DSCPTR_NXT            0xFFC0B100         /* DMA31 Pointer to Next Initial Descriptor */
#define REG_DMA32_DSCPTR_NXT            0xFFC0B180         /* DMA32 Pointer to Next Initial Descriptor */
#define REG_DMA33_DSCPTR_NXT            0xFFC0D000         /* DMA33 Pointer to Next Initial Descriptor */
#define REG_DMA34_DSCPTR_NXT            0xFFC0D080         /* DMA34 Pointer to Next Initial Descriptor */
#define REG_DMA35_DSCPTR_NXT            0xFFC10000         /* DMA35 Pointer to Next Initial Descriptor */
#define REG_DMA36_DSCPTR_NXT            0xFFC10080         /* DMA36 Pointer to Next Initial Descriptor */
#define REG_DMA37_DSCPTR_NXT            0xFFC10100         /* DMA37 Pointer to Next Initial Descriptor */
#define REG_DMA38_DSCPTR_NXT            0xFFC12000         /* DMA38 Pointer to Next Initial Descriptor */
#define REG_DMA39_DSCPTR_NXT            0xFFC12080         /* DMA39 Pointer to Next Initial Descriptor */
#define REG_DMA40_DSCPTR_NXT            0xFFC12100         /* DMA40 Pointer to Next Initial Descriptor */
#define REG_DMA41_DSCPTR_NXT            0xFFC12180         /* DMA41 Pointer to Next Initial Descriptor */
#define REG_DMA42_DSCPTR_NXT            0xFFC14000         /* DMA42 Pointer to Next Initial Descriptor */
#define REG_DMA43_DSCPTR_NXT            0xFFC14080         /* DMA43 Pointer to Next Initial Descriptor */
#define REG_DMA44_DSCPTR_NXT            0xFFC14100         /* DMA44 Pointer to Next Initial Descriptor */
#define REG_DMA45_DSCPTR_NXT            0xFFC14180         /* DMA45 Pointer to Next Initial Descriptor */
#define REG_DMA46_DSCPTR_NXT            0xFFC14200         /* DMA46 Pointer to Next Initial Descriptor */

#define REG_DMA_DSCPTR_NXT			  0x0		   /* Next Initial Descriptor */
#define REG_DMA_ADDRSTART             0x4          /* Start Address of Current Buffer */
#define REG_DMA_CFG                   0x8          /* Configuration Register */
#define REG_DMA_XCNT                  0xC          /* Inner Loop Count Start Value */
#define REG_DMA_XMOD                  0x10         /* Inner Loop Address Increment */
#define REG_DMA_YCNT                  0x14         /* Outer Loop Count Start Value (2D only) */
#define REG_DMA_YMOD                  0x18         /* Outer Loop Address Increment (2D only) */
#define REG_DMA_DSCPTR_CUR            0x24         /* Current Descriptor Pointer */
#define REG_DMA_DSCPTR_PRV            0x28         /* Previous Initial Descriptor Pointer */
#define REG_DMA_ADDR_CUR              0x2C         /* Current Address */
#define REG_DMA_STAT                  0x30         /* Status Register */
#define REG_DMA_XCNT_CUR              0x34         /* Current Count(1D) or intra-row XCNT (2D) */
#define REG_DMA_YCNT_CUR              0x38         /* Current Row Count (2D only) */
#define REG_DMA_BWLCNT                0x40         /* Bandwidth Limit Count */
#define REG_DMA_BWLCNT_CUR            0x44         /* Bandwidth Limit Count Current */
#define REG_DMA_BWMCNT                0x48         /* Bandwidth Monitor Count */
#define REG_DMA_BWMCNT_CUR            0x4C         /* Bandwidth Monitor Count Current */

/* ------------------------------------------------------------------------------------------------------------------------
        DMA_CFG                              Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_DMA_CFG_PDRF                    28                               /* Peripheral Data Request Forward */
#define BITP_DMA_CFG_TWOD                    26                               /* Two Dimension Addressing Enable */
#define BITP_DMA_CFG_DESCIDCPY               25                               /* Descriptor ID Copy Control */
#define BITP_DMA_CFG_TOVEN                   24                               /* Trigger Overrun Error Enable */
#define BITP_DMA_CFG_TRIG                    22                               /* Generate Outgoing Trigger */
#define BITP_DMA_CFG_INT                     20                               /* Generate Interrupt */
#define BITP_DMA_CFG_NDSIZE                  16                               /* Next Descriptor Set Size */
#define BITP_DMA_CFG_TWAIT                   15                               /* Wait for Trigger */
#define BITP_DMA_CFG_FLOW                    12                               /* Next Operation */
#define BITP_DMA_CFG_MSIZE                    8                               /* Memory Transfer Word Size */
#define BITP_DMA_CFG_PSIZE                    4                               /* Peripheral Transfer Word Size */
#define BITP_DMA_CFG_CADDR                    3                               /* Use Current Address */
#define BITP_DMA_CFG_SYNC                     2                               /* Synchronize Work Unit Transitions */
#define BITP_DMA_CFG_WNR                      1                               /* Write/Read Channel Direction */
#define BITP_DMA_CFG_EN                       0                               /* DMA Channel Enable */

#define BITM_DMA_CFG_PDRF                    0x10000000             /* Peripheral Data Request Forward */
#define ENUM_DMA_CFG_PDAT_NOTFWD             0x00000000             /* PDRF: Peripheral Data Request Not Forwarded */
#define ENUM_DMA_CFG_PDAT_FWD                0x10000000             /* PDRF: Peripheral Data Request Forwarded */

#define BITM_DMA_CFG_TWOD                    0x04000000             /* Two Dimension Addressing Enable */
#define ENUM_DMA_CFG_ADDR1D                  0x00000000             /* TWOD: One-Dimensional Addressing */
#define ENUM_DMA_CFG_ADDR2D                  0x04000000             /* TWOD: Two-Dimensional Addressing */

#define BITM_DMA_CFG_DESCIDCPY               0x02000000             /* Descriptor ID Copy Control */
#define ENUM_DMA_CFG_NO_COPY                 0x00000000             /* DESCIDCPY: Never Copy */
#define ENUM_DMA_CFG_COPY                    0x02000000             /* DESCIDCPY: Copy on Work Unit Complete */

#define BITM_DMA_CFG_TOVEN                   0x01000000             /* Trigger Overrun Error Enable */
#define ENUM_DMA_CFG_TOV_DIS                 0x00000000             /* TOVEN: Ignore Trigger Overrun */
#define ENUM_DMA_CFG_TOV_EN                  0x01000000             /* TOVEN: Error on Trigger Overrun */

#define BITM_DMA_CFG_TRIG                    0x00C00000             /* Generate Outgoing Trigger */
#define ENUM_DMA_CFG_NO_TRIG                 0x00000000             /* TRIG: Never assert Trigger */
#define ENUM_DMA_CFG_XCNT_TRIG               0x00400000             /* TRIG: Trigger when XCNTCUR reaches 0 */
#define ENUM_DMA_CFG_YCNT_TRIG               0x00800000             /* TRIG: Trigger when YCNTCUR reaches 0 */

#define BITM_DMA_CFG_INT                     0x00300000             /* Generate Interrupt */
#define ENUM_DMA_CFG_NO_INT                  0x00000000             /* INT: Never assert Interrupt */
#define ENUM_DMA_CFG_XCNT_INT                0x00100000             /* INT: Interrupt when X Count Expires */
#define ENUM_DMA_CFG_YCNT_INT                0x00200000             /* INT: Interrupt when Y Count Expires */
#define ENUM_DMA_CFG_PERIPH_INT              0x00300000             /* INT: Peripheral Interrupt */

#define BITM_DMA_CFG_NDSIZE                  0x00070000             /* Next Descriptor Set Size */
#define ENUM_DMA_CFG_FETCH01                 0x00000000             /* NDSIZE: Fetch one Descriptor Element */
#define ENUM_DMA_CFG_FETCH02                 0x00010000             /* NDSIZE: Fetch two Descriptor Elements */
#define ENUM_DMA_CFG_FETCH03                 0x00020000             /* NDSIZE: Fetch three Descriptor Elements */
#define ENUM_DMA_CFG_FETCH04                 0x00030000             /* NDSIZE: Fetch four Descriptor Elements */
#define ENUM_DMA_CFG_FETCH05                 0x00040000             /* NDSIZE: Fetch five Descriptor Elements */
#define ENUM_DMA_CFG_FETCH06                 0x00050000             /* NDSIZE: Fetch six Descriptor Elements */
#define ENUM_DMA_CFG_FETCH07                 0x00060000             /* NDSIZE: Fetch seven Descriptor Elements */

#define BITM_DMA_CFG_TWAIT                   0x00008000             /* Wait for Trigger */
#define ENUM_DMA_CFG_NO_TRGWAIT              0x00000000             /* TWAIT: Begin Work Unit Automatically (No Wait) */
#define ENUM_DMA_CFG_TRGWAIT                 0x00008000             /* TWAIT: Wait for Trigger (Halt before Work Unit) */

#define BITM_DMA_CFG_FLOW                    0x00007000             /* Next Operation */
#define ENUM_DMA_CFG_STOP                    0x00000000             /* FLOW: STOP - Stop */
#define ENUM_DMA_CFG_AUTO                    0x00001000             /* FLOW: AUTO - Autobuffer */
#define ENUM_DMA_CFG_DSCLIST                 0x00004000             /* FLOW: DSCL - Descriptor List */
#define ENUM_DMA_CFG_DSCARRAY                0x00005000             /* FLOW: DSCA - Descriptor Array */
#define ENUM_DMA_CFG_DODLIST                 0x00006000             /* FLOW: Descriptor On Demand List */
#define ENUM_DMA_CFG_DODARRAY                0x00007000             /* FLOW: Descriptor On Demand Array */

#define BITM_DMA_CFG_MSIZE                   0x00000700             /* Memory Transfer Word Size */
#define ENUM_DMA_CFG_MSIZE01                 0x00000000             /* MSIZE: 1 Byte */
#define ENUM_DMA_CFG_MSIZE02                 0x00000100             /* MSIZE: 2 Bytes */
#define ENUM_DMA_CFG_MSIZE04                 0x00000200             /* MSIZE: 4 Bytes */
#define ENUM_DMA_CFG_MSIZE08                 0x00000300             /* MSIZE: 8 Bytes */
#define ENUM_DMA_CFG_MSIZE16                 0x00000400             /* MSIZE: 16 Bytes */
#define ENUM_DMA_CFG_MSIZE32                 0x00000500             /* MSIZE: 32 Bytes */

#define BITM_DMA_CFG_PSIZE                   0x00000070             /* Peripheral Transfer Word Size */
#define ENUM_DMA_CFG_PSIZE01                 0x00000000             /* PSIZE: 1 Byte */
#define ENUM_DMA_CFG_PSIZE02                 0x00000010             /* PSIZE: 2 Bytes */
#define ENUM_DMA_CFG_PSIZE04                 0x00000020             /* PSIZE: 4 Bytes */
#define ENUM_DMA_CFG_PSIZE08                 0x00000030             /* PSIZE: 8 Bytes */

#define BITM_DMA_CFG_CADDR                   0x00000008             /* Use Current Address */
#define ENUM_DMA_CFG_LD_STARTADDR            0x00000000             /* CADDR: Load Starting Address */
#define ENUM_DMA_CFG_LD_CURADDR              0x00000008             /* CADDR: Use Current Address */

#define BITM_DMA_CFG_SYNC                    0x00000004             /* Synchronize Work Unit Transitions */
#define ENUM_DMA_CFG_NO_SYNC                 0x00000000             /* SYNC: No Synchronization */
#define ENUM_DMA_CFG_SYNC                    0x00000004             /* SYNC: Synchronize  Channel */

#define BITM_DMA_CFG_WNR                     0x00000002             /* Write/Read Channel Direction */
#define ENUM_DMA_CFG_READ                    0x00000000             /* WNR: Transmit (Read from memory) */
#define ENUM_DMA_CFG_WRITE                   0x00000002             /* WNR: Receive (Write to memory) */

#define BITM_DMA_CFG_EN                      0x00000001             /* DMA Channel Enable */
#define ENUM_DMA_CFG_DIS                     0x00000000             /* EN: Disable */
#define ENUM_DMA_CFG_EN                      0x00000001             /* EN: Enable */

/* ------------------------------------------------------------------------------------------------------------------------
        DMA_DSCPTR_PRV                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_DMA_DSCPTR_PRV_DESCPPREV         2                               /* Pointer for Previous Descriptor Element */
#define BITP_DMA_DSCPTR_PRV_PDPO              0                               /* Previous Descriptor Pointer Overrun */
#define BITM_DMA_DSCPTR_PRV_DESCPPREV        0xFFFFFFFC             /* Pointer for Previous Descriptor Element */
#define BITM_DMA_DSCPTR_PRV_PDPO             0x00000001             /* Previous Descriptor Pointer Overrun */

/* ------------------------------------------------------------------------------------------------------------------------
        DMA_STAT                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_DMA_STAT_TWAIT                  20                               /* Trigger Wait Status */
#define BITP_DMA_STAT_FIFOFILL               16                               /* FIFO Fill Status */
#define BITP_DMA_STAT_MBWID                  14                               /* Memory Bus Width */
#define BITP_DMA_STAT_PBWID                  12                               /* Peripheral Bus Width */
#define BITP_DMA_STAT_RUN                     8                               /* Run Status */
#define BITP_DMA_STAT_ERRC                    4                               /* Error Cause */
#define BITP_DMA_STAT_PIRQ                    2                               /* Peripheral Interrupt Request */
#define BITP_DMA_STAT_IRQERR                  1                               /* Error Interrupt */
#define BITP_DMA_STAT_IRQDONE                 0                               /* Work Unit/Row Done Interrupt */

#define BITM_DMA_STAT_TWAIT                  0x00100000             /* Trigger Wait Status */
#define ENUM_DMA_STAT_NOTRIGRX               0x00000000             /* TWAIT: No trigger received */
#define ENUM_DMA_STAT_TRIGRX                 0x00100000             /* TWAIT: Trigger received */

#define BITM_DMA_STAT_FIFOFILL               0x00070000             /* FIFO Fill Status */
#define ENUM_DMA_STAT_FIFOEMPTY              0x00000000             /* FIFOFILL: Empty */
#define ENUM_DMA_STAT_FIFO25                 0x00010000             /* FIFOFILL: Empty < FIFO = 1/4 Full */
#define ENUM_DMA_STAT_FIFO50                 0x00020000             /* FIFOFILL: 1/4 Full < FIFO = 1/2 Full */
#define ENUM_DMA_STAT_FIFO75                 0x00030000             /* FIFOFILL: 1/2 Full < FIFO = 3/4 Full */
#define ENUM_DMA_STAT_FIFONEARFULL           0x00040000             /* FIFOFILL: 3/4 Full < FIFO = Full */
#define ENUM_DMA_STAT_FIFOFULL               0x00070000             /* FIFOFILL: Full */

#define BITM_DMA_STAT_MBWID                  0x0000C000             /* Memory Bus Width */
#define ENUM_DMA_STAT_MBUS02                 0x00000000             /* MBWID: 2 Bytes */
#define ENUM_DMA_STAT_MBUS04                 0x00004000             /* MBWID: 4 Bytes */
#define ENUM_DMA_STAT_MBUS08                 0x00008000             /* MBWID: 8 Bytes */
#define ENUM_DMA_STAT_MBUS16                 0x0000C000             /* MBWID: 16 Bytes */

#define BITM_DMA_STAT_PBWID                  0x00003000             /* Peripheral Bus Width */
#define ENUM_DMA_STAT_PBUS01                 0x00000000             /* PBWID: 1 Byte */
#define ENUM_DMA_STAT_PBUS02                 0x00001000             /* PBWID: 2 Bytes */
#define ENUM_DMA_STAT_PBUS04                 0x00002000             /* PBWID: 4 Bytes */
#define ENUM_DMA_STAT_PBUS08                 0x00003000             /* PBWID: 8 Bytes */

#define BITM_DMA_STAT_RUN                    0x00000700             /* Run Status */
#define ENUM_DMA_STAT_STOPPED                0x00000000             /* RUN: Idle/Stop State */
#define ENUM_DMA_STAT_DSCFETCH               0x00000100             /* RUN: Descriptor Fetch */
#define ENUM_DMA_STAT_DATAXFER               0x00000200             /* RUN: Data Transfer */
#define ENUM_DMA_STAT_TRGWAIT                0x00000300             /* RUN: Waiting for Trigger */
#define ENUM_DMA_STAT_ACKWAIT                0x00000400             /* RUN: Waiting for Write ACK/FIFO Drain to Peripheral */

#define BITM_DMA_STAT_ERRC                   0x00000070             /* Error Cause */
#define ENUM_DMA_STAT_CFGERR                 0x00000000             /* ERRC: Configuration Error */
#define ENUM_DMA_STAT_ILLWRERR               0x00000010             /* ERRC: Illegal Write Occurred While Channel Running */
#define ENUM_DMA_STAT_ALGNERR                0x00000020             /* ERRC: Address Alignment Error */
#define ENUM_DMA_STAT_MEMERR                 0x00000030             /* ERRC: Memory Access/Fabric Error */
#define ENUM_DMA_STAT_TRGOVERR               0x00000050             /* ERRC: Trigger Overrun */
#define ENUM_DMA_STAT_BWMONERR               0x00000060             /* ERRC: Bandwidth Monitor Error */

#define BITM_DMA_STAT_PIRQ                   0x00000004             /* Peripheral Interrupt Request */
#define ENUM_DMA_STAT_NO_PIRQ                0x00000000             /* PIRQ: No Interrupt */
#define ENUM_DMA_STAT_PIRQ                   0x00000004             /* PIRQ: Interrupt Signaled by Peripheral */

#define BITM_DMA_STAT_IRQERR                 0x00000002             /* Error Interrupt */
#define ENUM_DMA_STAT_NO_IRQERR              0x00000000             /* IRQERR: No Error */
#define ENUM_DMA_STAT_IRQERR                 0x00000002             /* IRQERR: Error Occurred */

#define BITM_DMA_STAT_IRQDONE                0x00000001             /* Work Unit/Row Done Interrupt */
#define ENUM_DMA_STAT_NO_IRQ                 0x00000000             /* IRQDONE: Inactive */
#define ENUM_DMA_STAT_IRQDONE                0x00000001             /* IRQDONE: Active */

/* ------------------------------------------------------------------------------------------------------------------------
        DMA_BWLCNT                           Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_DMA_BWLCNT_VALUE                 0                     /* Bandwidth Limit Count */
#define BITM_DMA_BWLCNT_VALUE                 0x0000FFFF,           /* Bandwidth Limit Count */

/* ------------------------------------------------------------------------------------------------------------------------
        DMA_BWLCNT_CUR                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_DMA_BWLCNT_CUR_VALUE             0                               /* Bandwidth Limit Count Current */
#define BITM_DMA_BWLCNT_CUR_VALUE            0x0000FFFF			    /* Bandwidth Limit Count Current */

/* ==================================================
        Video Subsystem Registers Registers
   ================================================== */

/* =========================
        VID0
   ========================= */
#define REG_VID0_CONN                   0xFFC1D000         /* VID0 Video Subsystem Connect Register */

/* =========================
        VID
   ========================= */
/* ------------------------------------------------------------------------------------------------------------------------
        VID_CONN                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_VID_CONN_PPI2BCAST              23                               /* PPI_2 Broadcast Mode */
#define BITP_VID_CONN_PPI1BCAST              22                               /* PPI_1 Broadcast Mode */
#define BITP_VID_CONN_PPI0BCAST              21                               /* PPI_0 Broadcast Mode */
#define BITP_VID_CONN_PPI2TX                 16                               /* PPI_2_TX Connectivity */
#define BITP_VID_CONN_PPI1TX                 12                               /* PPI_1_TX Connectivity */
#define BITP_VID_CONN_PPI0TX                  8                               /* PPI_0_TX Connectivity */
#define BITP_VID_CONN_PVP0IN                  4                               /* PVP_IN Connectivity */
#define BITP_VID_CONN_PIXC0IN                 0                               /* PIXC_IN Connectivity */
#define BITM_VID_CONN_PPI2BCAST              0x00800000  /* PPI_2 Broadcast Mode */
#define BITM_VID_CONN_PPI1BCAST              0x00400000  /* PPI_1 Broadcast Mode */
#define BITM_VID_CONN_PPI0BCAST              0x00200000  /* PPI_0 Broadcast Mode */
#define BITM_VID_CONN_PPI2TX                 0x000F0000  /* PPI_2_TX Connectivity */
#define BITM_VID_CONN_PPI1TX                 0x0000F000  /* PPI_1_TX Connectivity */
#define BITM_VID_CONN_PPI0TX                 0x00000F00  /* PPI_0_TX Connectivity */
#define BITM_VID_CONN_PVP0IN                 0x000000F0  /* PVP_IN Connectivity */
#define BITM_VID_CONN_PIXC0IN                0x0000000F  /* PIXC_IN Connectivity */

/* ==================================================
        Reset Control Unit Registers
   ================================================== */

/* =========================
        RCU0
   ========================= */
#define REG_RCU0_CTL                    0xFFCA6000         /* RCU0 Control Register */
#define REG_RCU0_STAT                   0xFFCA6004         /* RCU0 Status Register */
#define REG_RCU0_CRCTL                  0xFFCA6008         /* RCU0 Core Reset Control Register */
#define REG_RCU0_CRSTAT                 0xFFCA600C         /* RCU0 Core Reset Status Register */
#define REG_RCU0_SIDIS                  0xFFCA6010         /* RCU0 System Interface Disable Register */
#define REG_RCU0_SISTAT                 0xFFCA6014         /* RCU0 System Interface Status Register */
#define REG_RCU0_SVECT_LCK              0xFFCA6018         /* RCU0 SVECT Lock Register */
#define REG_RCU0_BCODE                  0xFFCA601C         /* RCU0 Boot Code Register */
#define REG_RCU0_SVECT0                 0xFFCA6020         /* RCU0 Software Vector Register 0 */
#define REG_RCU0_SVECT1                 0xFFCA6024         /* RCU0 Software Vector Register 1 */

/* =========================
        RCU
   ========================= */
/* ------------------------------------------------------------------------------------------------------------------------
        RCU_CTL                              Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_RCU_CTL_LOCK                    31                               /* Lock */
#define BITP_RCU_CTL_RSTOUTDSRT               2                               /* Reset Out Deassert */
#define BITP_RCU_CTL_RSTOUTASRT               1                               /* Reset Out Assert */
#define BITP_RCU_CTL_SYSRST                   0                               /* System Reset */
#define BITM_RCU_CTL_LOCK                    0x80000000  /* Lock */
#define BITM_RCU_CTL_RSTOUTDSRT              0x00000004  /* Reset Out Deassert */
#define BITM_RCU_CTL_RSTOUTASRT              0x00000002  /* Reset Out Assert */
#define BITM_RCU_CTL_SYSRST                  0x00000001  /* System Reset */

/* ------------------------------------------------------------------------------------------------------------------------
        RCU_STAT                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_RCU_STAT_RSTOUTERR              18                               /* Reset Out Error */
#define BITP_RCU_STAT_LWERR                  17                               /* Lock Write Error */
#define BITP_RCU_STAT_ADDRERR                16                               /* Address Error */
#define BITP_RCU_STAT_BMODE                   8                               /* Boot Mode */
#define BITP_RCU_STAT_RSTOUT                  5                               /* Reset Out Status */
#define BITP_RCU_STAT_SWRST                   3                               /* Software Reset */
#define BITP_RCU_STAT_SSRST                   2                               /* System Source Reset */
#define BITP_RCU_STAT_HBRST                   1                               /* Hibernate Reset */
#define BITP_RCU_STAT_HWRST                   0                               /* Hardware Reset */
#define BITM_RCU_STAT_RSTOUTERR              0x00040000  /* Reset Out Error */
#define BITM_RCU_STAT_LWERR                  0x00020000  /* Lock Write Error */
#define BITM_RCU_STAT_ADDRERR                0x00010000  /* Address Error */
#define BITM_RCU_STAT_BMODE                  0x00000F00  /* Boot Mode */
#define BITM_RCU_STAT_RSTOUT                 0x00000020  /* Reset Out Status */
#define BITM_RCU_STAT_SWRST                  0x00000008  /* Software Reset */
#define BITM_RCU_STAT_SSRST                  0x00000004  /* System Source Reset */
#define BITM_RCU_STAT_HBRST                  0x00000002  /* Hibernate Reset */
#define BITM_RCU_STAT_HWRST                  0x00000001  /* Hardware Reset */

/* ------------------------------------------------------------------------------------------------------------------------
        RCU_CRCTL                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_RCU_CRCTL_LOCK                  31                               /* Lock */
#define BITP_RCU_CRCTL_CR0                    0                               /* Core Reset n */
#define BITP_RCU_CRCTL_CR1                    1                               /* Core Reset n */
#define BITM_RCU_CRCTL_LOCK                  0x80000000  /* Lock */
#define BITM_RCU_CRCTL_CR0                   0x00000001  /* Core Reset n */
#define BITM_RCU_CRCTL_CR1                   0x00000002  /* Core Reset n */

/* ------------------------------------------------------------------------------------------------------------------------
        RCU_CRSTAT                           Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_RCU_CRSTAT_CR0                   0                               /* Core Reset n */
#define BITP_RCU_CRSTAT_CR1                   1                               /* Core Reset n */
#define BITM_RCU_CRSTAT_CR0                  0x00000001  /* Core Reset n */
#define BITM_RCU_CRSTAT_CR1                  0x00000002  /* Core Reset n */

/* ------------------------------------------------------------------------------------------------------------------------
        RCU_SIDIS                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_RCU_SIDIS_LOCK                  31                               /* Lock */
#define BITP_RCU_SIDIS_SI0                    0                               /* System Interface n */
#define BITP_RCU_SIDIS_SI1                    1                               /* System Interface n */
#define BITM_RCU_SIDIS_LOCK                  0x80000000  /* Lock */
#define BITM_RCU_SIDIS_SI0                   0x00000001  /* System Interface n */
#define BITM_RCU_SIDIS_SI1                   0x00000002  /* System Interface n */

/* ------------------------------------------------------------------------------------------------------------------------
        RCU_SISTAT                           Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_RCU_SISTAT_SI0                   0                               /* System Interface n */
#define BITP_RCU_SISTAT_SI1                   1                               /* System Interface n */
#define BITM_RCU_SISTAT_SI0                  0x00000001  /* System Interface n */
#define BITM_RCU_SISTAT_SI1                  0x00000002  /* System Interface n */

/* ------------------------------------------------------------------------------------------------------------------------
        RCU_SVECT_LCK                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_RCU_SVECT_LCK_LOCK              31                               /* Lock */
#define BITP_RCU_SVECT_LCK_SVECT0             0                               /* Software Vector Register n */
#define BITP_RCU_SVECT_LCK_SVECT1             1                               /* Software Vector Register n */
#define BITM_RCU_SVECT_LCK_LOCK              0x80000000  /* Lock */
#define BITM_RCU_SVECT_LCK_SVECT0            0x00000001  /* Software Vector Register n */
#define BITM_RCU_SVECT_LCK_SVECT1            0x00000002  /* Software Vector Register n */

/* ------------------------------------------------------------------------------------------------------------------------
        RCU_BCODE                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_RCU_BCODE_LOCK                  31                               /* Lock */
#define BITP_RCU_BCODE_VALUE                  0                               /* Boot Code */
#define BITP_RCU_BCODE_BCODE                  0                               /* Boot Code (legacy macro with older name) */
#define BITM_RCU_BCODE_LOCK                  0x80000000  /* Lock */
#define BITM_RCU_BCODE_VALUE                 0x7FFFFFFF  /* Boot Code */
#define BITM_RCU_BCODE_BCODE                 0x7FFFFFFF  /* Boot Code (legacy macro with older name)*/

/* ==================================================
        PVP Registers
   ================================================== */
/* ------------------------------------------------------------------------------------------------------------------------
        PVP                             Block level enums
   ------------------------------------------------------------------------------------------------------------------------ */
#define ENUM_PVP_GCFG                   0x00          /* PVP Block ID Code for GCFG */
#define ENUM_PVP_OPF0                   0x01          /* PVP Block ID Code for OPF0 */
#define ENUM_PVP_OPF1                   0x02          /* PVP Block ID Code for OPF1 */
#define ENUM_PVP_OPF2                   0x03          /* PVP Block ID Code for OPF2 */
#define ENUM_PVP_OPF3                   0x04          /* PVP Block ID Code for OPF3 */
#define ENUM_PVP_PEC                    0x05          /* PVP Block ID Code for PEC */
#define ENUM_PVP_IIM0                   0x06          /* PVP Block ID Code for IIM0 */
#define ENUM_PVP_IIM1                   0x07          /* PVP Block ID Code for IIM1 */
#define ENUM_PVP_ACU                    0x08          /* PVP Block ID Code for ACU */
#define ENUM_PVP_UDS                    0x0A          /* PVP Block ID Code for UDS */
#define ENUM_PVP_IPF0                   0x0C          /* PVP Block ID Code for IPF0 */
#define ENUM_PVP_IPF1                   0x0E          /* PVP Block ID Code for IPF1 */
#define ENUM_PVP_CNV0                   0x10          /* PVP Block ID Code for CNV0 */
#define ENUM_PVP_CNV1                   0x14          /* PVP Block ID Code for CNV1 */
#define ENUM_PVP_CNV2                   0x18          /* PVP Block ID Code for CNV2 */
#define ENUM_PVP_CNV3                   0x1C          /* PVP Block ID Code for CNV3 */
#define ENUM_PVP_THC0                   0x20          /* PVP Block ID Code for THC0 */
#define ENUM_PVP_THC1                   0x28          /* PVP Block ID Code for THC1 */
#define ENUM_PVP_PMA                    0x30          /* PVP Block ID Code for PMA */

/* =========================
        PVP0
   ========================= */
#define REG_PVP0_CTL                    0xFFC1A004         /* PVP0 Control */
#define REG_PVP0_IMSK0                  0xFFC1A008         /* PVP0 Interrupt Mask n */
#define REG_PVP0_IMSK1                  0xFFC1A00C         /* PVP0 Interrupt Mask n */
#define REG_PVP0_STAT                   0xFFC1A010         /* PVP0 Status */
#define REG_PVP0_ILAT                   0xFFC1A014         /* PVP0 Interrupt Latch Status n */
#define REG_PVP0_IREQ0                  0xFFC1A018         /* PVP0 Interrupt Request n */
#define REG_PVP0_IREQ1                  0xFFC1A01C         /* PVP0 Interrupt Request n */
#define REG_PVP0_OPF0_CFG               0xFFC1A020         /* PVP0 OPFn (Camera Pipe) Configuration */
#define REG_PVP0_OPF1_CFG               0xFFC1A040         /* PVP0 OPFn (Camera Pipe) Configuration */
#define REG_PVP0_OPF2_CFG               0xFFC1A060         /* PVP0 OPFn (Camera Pipe) Configuration */
#define REG_PVP0_OPF0_CTL               0xFFC1A024         /* PVP0 OPFn (Camera Pipe) Control */
#define REG_PVP0_OPF1_CTL               0xFFC1A044         /* PVP0 OPFn (Camera Pipe) Control */
#define REG_PVP0_OPF2_CTL               0xFFC1A064         /* PVP0 OPFn (Camera Pipe) Control */
#define REG_PVP0_OPF3_CFG               0xFFC1A080         /* PVP0 OPF3 (Memory Pipe) Configuration */
#define REG_PVP0_OPF3_CTL               0xFFC1A084         /* PVP0 OPF3 (Memory Pipe) Control */
#define REG_PVP0_PEC_CFG                0xFFC1A0A0         /* PVP0 PEC Configuration */
#define REG_PVP0_PEC_CTL                0xFFC1A0A4         /* PVP0 PEC Control */
#define REG_PVP0_PEC_D1TH0              0xFFC1A0A8         /* PVP0 PEC Lower Hysteresis Threshold */
#define REG_PVP0_PEC_D1TH1              0xFFC1A0AC         /* PVP0 PEC Upper Hysteresis Threshold */
#define REG_PVP0_PEC_D2TH0              0xFFC1A0B0         /* PVP0 PEC Weak Zero Crossing Threshold */
#define REG_PVP0_PEC_D2TH1              0xFFC1A0B4         /* PVP0 PEC Strong Zero Crossing Threshold */
#define REG_PVP0_IIM0_CFG               0xFFC1A0C0         /* PVP0 IIMn Configuration */
#define REG_PVP0_IIM1_CFG               0xFFC1A0E0         /* PVP0 IIMn Configuration */
#define REG_PVP0_IIM0_CTL               0xFFC1A0C4         /* PVP0 IIMn Control */
#define REG_PVP0_IIM1_CTL               0xFFC1A0E4         /* PVP0 IIMn Control */
#define REG_PVP0_IIM0_SCALE             0xFFC1A0C8         /* PVP0 IIMn Scaling Values */
#define REG_PVP0_IIM1_SCALE             0xFFC1A0E8         /* PVP0 IIMn Scaling Values */
#define REG_PVP0_IIM0_SOVF_STAT         0xFFC1A0CC         /* PVP0 IIMn Signed Overflow Status */
#define REG_PVP0_IIM1_SOVF_STAT         0xFFC1A0EC         /* PVP0 IIMn Signed Overflow Status */
#define REG_PVP0_IIM0_UOVF_STAT         0xFFC1A0D0         /* PVP0 IIMn Unsigned Overflow Status */
#define REG_PVP0_IIM1_UOVF_STAT         0xFFC1A0F0         /* PVP0 IIMn Unsigned Overflow Status */
#define REG_PVP0_ACU_CFG                0xFFC1A100         /* PVP0 ACU Configuration */
#define REG_PVP0_ACU_CTL                0xFFC1A104         /* PVP0 ACU Control */
#define REG_PVP0_ACU_OFFSET             0xFFC1A108         /* PVP0 ACU SUM Constant */
#define REG_PVP0_ACU_FACTOR             0xFFC1A10C         /* PVP0 ACU PROD Constant */
#define REG_PVP0_ACU_SHIFT              0xFFC1A110         /* PVP0 ACU Shift Constant */
#define REG_PVP0_ACU_MIN                0xFFC1A114         /* PVP0 ACU Lower Sat Threshold Min */
#define REG_PVP0_ACU_MAX                0xFFC1A118         /* PVP0 ACU Upper Sat Threshold Max */
#define REG_PVP0_UDS_CFG                0xFFC1A140         /* PVP0 UDS Configuration */
#define REG_PVP0_UDS_CTL                0xFFC1A144         /* PVP0 UDS Control */
#define REG_PVP0_UDS_OHCNT              0xFFC1A148         /* PVP0 UDS Output HCNT */
#define REG_PVP0_UDS_OVCNT              0xFFC1A14C         /* PVP0 UDS Output VCNT */
#define REG_PVP0_UDS_HAVG               0xFFC1A150         /* PVP0 UDS HAVG */
#define REG_PVP0_UDS_VAVG               0xFFC1A154         /* PVP0 UDS VAVG */
#define REG_PVP0_IPF0_CFG               0xFFC1A180         /* PVP0 IPF0 (Camera Pipe) Configuration */
#define REG_PVP0_IPF0_PIPECTL           0xFFC1A184         /* PVP0 IPFn (Camera/Memory Pipe) Pipe Control */
#define REG_PVP0_IPF1_PIPECTL           0xFFC1A1C4         /* PVP0 IPFn (Camera/Memory Pipe) Pipe Control */
#define REG_PVP0_IPF0_CTL               0xFFC1A188         /* PVP0 IPFn (Camera/Memory Pipe) Control */
#define REG_PVP0_IPF1_CTL               0xFFC1A1C8         /* PVP0 IPFn (Camera/Memory Pipe) Control */
#define REG_PVP0_IPF0_TAG               0xFFC1A18C         /* PVP0 IPFn (Camera/Memory Pipe) TAG Value */
#define REG_PVP0_IPF1_TAG               0xFFC1A1CC         /* PVP0 IPFn (Camera/Memory Pipe) TAG Value */
#define REG_PVP0_IPF0_FCNT              0xFFC1A190         /* PVP0 IPFn (Camera/Memory Pipe) Frame Count */
#define REG_PVP0_IPF1_FCNT              0xFFC1A1D0         /* PVP0 IPFn (Camera/Memory Pipe) Frame Count */
#define REG_PVP0_IPF0_HCNT              0xFFC1A194         /* PVP0 IPFn (Camera/Memory Pipe) Horizontal Count */
#define REG_PVP0_IPF1_HCNT              0xFFC1A1D4         /* PVP0 IPFn (Camera/Memory Pipe) Horizontal Count */
#define REG_PVP0_IPF0_VCNT              0xFFC1A198         /* PVP0 IPFn (Camera/Memory Pipe) Vertical Count */
#define REG_PVP0_IPF1_VCNT              0xFFC1A1D8         /* PVP0 IPFn (Camera/Memory Pipe) Vertical Count */
#define REG_PVP0_IPF0_HPOS              0xFFC1A19C         /* PVP0 IPF0 (Camera Pipe) Horizontal Position */
#define REG_PVP0_IPF0_VPOS              0xFFC1A1A0         /* PVP0 IPF0 (Camera Pipe) Vertical Position */
#define REG_PVP0_IPF0_TAG_STAT          0xFFC1A1A4         /* PVP0 IPFn (Camera/Memory Pipe) TAG Status */
#define REG_PVP0_IPF1_TAG_STAT          0xFFC1A1E4         /* PVP0 IPFn (Camera/Memory Pipe) TAG Status */
#define REG_PVP0_IPF1_CFG               0xFFC1A1C0         /* PVP0 IPF1 (Memory Pipe) Configuration */
#define REG_PVP0_CNV0_CFG               0xFFC1A200         /* PVP0 CNVn Configuration */
#define REG_PVP0_CNV1_CFG               0xFFC1A280         /* PVP0 CNVn Configuration */
#define REG_PVP0_CNV2_CFG               0xFFC1A300         /* PVP0 CNVn Configuration */
#define REG_PVP0_CNV3_CFG               0xFFC1A380         /* PVP0 CNVn Configuration */
#define REG_PVP0_CNV0_CTL               0xFFC1A204         /* PVP0 CNVn Control */
#define REG_PVP0_CNV1_CTL               0xFFC1A284         /* PVP0 CNVn Control */
#define REG_PVP0_CNV2_CTL               0xFFC1A304         /* PVP0 CNVn Control */
#define REG_PVP0_CNV3_CTL               0xFFC1A384         /* PVP0 CNVn Control */
#define REG_PVP0_CNV0_C00C01            0xFFC1A208         /* PVP0 CNVn Coefficients 0,0 and 0,1 */
#define REG_PVP0_CNV1_C00C01            0xFFC1A288         /* PVP0 CNVn Coefficients 0,0 and 0,1 */
#define REG_PVP0_CNV2_C00C01            0xFFC1A308         /* PVP0 CNVn Coefficients 0,0 and 0,1 */
#define REG_PVP0_CNV3_C00C01            0xFFC1A388         /* PVP0 CNVn Coefficients 0,0 and 0,1 */
#define REG_PVP0_CNV0_C02C03            0xFFC1A20C         /* PVP0 CNVn Coefficients 0,2 and 0,3 */
#define REG_PVP0_CNV1_C02C03            0xFFC1A28C         /* PVP0 CNVn Coefficients 0,2 and 0,3 */
#define REG_PVP0_CNV2_C02C03            0xFFC1A30C         /* PVP0 CNVn Coefficients 0,2 and 0,3 */
#define REG_PVP0_CNV3_C02C03            0xFFC1A38C         /* PVP0 CNVn Coefficients 0,2 and 0,3 */
#define REG_PVP0_CNV0_C04               0xFFC1A210         /* PVP0 CNVn Coefficient 0,4 */
#define REG_PVP0_CNV1_C04               0xFFC1A290         /* PVP0 CNVn Coefficient 0,4 */
#define REG_PVP0_CNV2_C04               0xFFC1A310         /* PVP0 CNVn Coefficient 0,4 */
#define REG_PVP0_CNV3_C04               0xFFC1A390         /* PVP0 CNVn Coefficient 0,4 */
#define REG_PVP0_CNV0_C10C11            0xFFC1A214         /* PVP0 CNVn Coefficients 1,0 and 1,1 */
#define REG_PVP0_CNV1_C10C11            0xFFC1A294         /* PVP0 CNVn Coefficients 1,0 and 1,1 */
#define REG_PVP0_CNV2_C10C11            0xFFC1A314         /* PVP0 CNVn Coefficients 1,0 and 1,1 */
#define REG_PVP0_CNV3_C10C11            0xFFC1A394         /* PVP0 CNVn Coefficients 1,0 and 1,1 */
#define REG_PVP0_CNV0_C12C13            0xFFC1A218         /* PVP0 CNVn Coefficients 1,2 and 1,3 */
#define REG_PVP0_CNV1_C12C13            0xFFC1A298         /* PVP0 CNVn Coefficients 1,2 and 1,3 */
#define REG_PVP0_CNV2_C12C13            0xFFC1A318         /* PVP0 CNVn Coefficients 1,2 and 1,3 */
#define REG_PVP0_CNV3_C12C13            0xFFC1A398         /* PVP0 CNVn Coefficients 1,2 and 1,3 */
#define REG_PVP0_CNV0_C14               0xFFC1A21C         /* PVP0 CNVn Coefficient 1,4 */
#define REG_PVP0_CNV1_C14               0xFFC1A29C         /* PVP0 CNVn Coefficient 1,4 */
#define REG_PVP0_CNV2_C14               0xFFC1A31C         /* PVP0 CNVn Coefficient 1,4 */
#define REG_PVP0_CNV3_C14               0xFFC1A39C         /* PVP0 CNVn Coefficient 1,4 */
#define REG_PVP0_CNV0_C20C21            0xFFC1A220         /* PVP0 CNVn Coefficients 2,0 and 2,1 */
#define REG_PVP0_CNV1_C20C21            0xFFC1A2A0         /* PVP0 CNVn Coefficients 2,0 and 2,1 */
#define REG_PVP0_CNV2_C20C21            0xFFC1A320         /* PVP0 CNVn Coefficients 2,0 and 2,1 */
#define REG_PVP0_CNV3_C20C21            0xFFC1A3A0         /* PVP0 CNVn Coefficients 2,0 and 2,1 */
#define REG_PVP0_CNV0_C22C23            0xFFC1A224         /* PVP0 CNVn Coefficients 2,2 and 2,3 */
#define REG_PVP0_CNV1_C22C23            0xFFC1A2A4         /* PVP0 CNVn Coefficients 2,2 and 2,3 */
#define REG_PVP0_CNV2_C22C23            0xFFC1A324         /* PVP0 CNVn Coefficients 2,2 and 2,3 */
#define REG_PVP0_CNV3_C22C23            0xFFC1A3A4         /* PVP0 CNVn Coefficients 2,2 and 2,3 */
#define REG_PVP0_CNV0_C24               0xFFC1A228         /* PVP0 CNVn Coefficient 2,4 */
#define REG_PVP0_CNV1_C24               0xFFC1A2A8         /* PVP0 CNVn Coefficient 2,4 */
#define REG_PVP0_CNV2_C24               0xFFC1A328         /* PVP0 CNVn Coefficient 2,4 */
#define REG_PVP0_CNV3_C24               0xFFC1A3A8         /* PVP0 CNVn Coefficient 2,4 */
#define REG_PVP0_CNV0_C30C31            0xFFC1A22C         /* PVP0 CNVn Coefficients 3,0 and 3,1 */
#define REG_PVP0_CNV1_C30C31            0xFFC1A2AC         /* PVP0 CNVn Coefficients 3,0 and 3,1 */
#define REG_PVP0_CNV2_C30C31            0xFFC1A32C         /* PVP0 CNVn Coefficients 3,0 and 3,1 */
#define REG_PVP0_CNV3_C30C31            0xFFC1A3AC         /* PVP0 CNVn Coefficients 3,0 and 3,1 */
#define REG_PVP0_CNV0_C32C33            0xFFC1A230         /* PVP0 CNVn Coefficients 3,2 and 3,3 */
#define REG_PVP0_CNV1_C32C33            0xFFC1A2B0         /* PVP0 CNVn Coefficients 3,2 and 3,3 */
#define REG_PVP0_CNV2_C32C33            0xFFC1A330         /* PVP0 CNVn Coefficients 3,2 and 3,3 */
#define REG_PVP0_CNV3_C32C33            0xFFC1A3B0         /* PVP0 CNVn Coefficients 3,2 and 3,3 */
#define REG_PVP0_CNV0_C34               0xFFC1A234         /* PVP0 CNVn Coefficient 3,4 */
#define REG_PVP0_CNV1_C34               0xFFC1A2B4         /* PVP0 CNVn Coefficient 3,4 */
#define REG_PVP0_CNV2_C34               0xFFC1A334         /* PVP0 CNVn Coefficient 3,4 */
#define REG_PVP0_CNV3_C34               0xFFC1A3B4         /* PVP0 CNVn Coefficient 3,4 */
#define REG_PVP0_CNV0_C40C41            0xFFC1A238         /* PVP0 CNVn Coefficients 4,0 and 4,1 */
#define REG_PVP0_CNV1_C40C41            0xFFC1A2B8         /* PVP0 CNVn Coefficients 4,0 and 4,1 */
#define REG_PVP0_CNV2_C40C41            0xFFC1A338         /* PVP0 CNVn Coefficients 4,0 and 4,1 */
#define REG_PVP0_CNV3_C40C41            0xFFC1A3B8         /* PVP0 CNVn Coefficients 4,0 and 4,1 */
#define REG_PVP0_CNV0_C42C43            0xFFC1A23C         /* PVP0 CNVn Coefficients 4,2 and 4,3 */
#define REG_PVP0_CNV1_C42C43            0xFFC1A2BC         /* PVP0 CNVn Coefficients 4,2 and 4,3 */
#define REG_PVP0_CNV2_C42C43            0xFFC1A33C         /* PVP0 CNVn Coefficients 4,2 and 4,3 */
#define REG_PVP0_CNV3_C42C43            0xFFC1A3BC         /* PVP0 CNVn Coefficients 4,2 and 4,3 */
#define REG_PVP0_CNV0_C44               0xFFC1A240         /* PVP0 CNVn Coefficient 4,4 */
#define REG_PVP0_CNV1_C44               0xFFC1A2C0         /* PVP0 CNVn Coefficient 4,4 */
#define REG_PVP0_CNV2_C44               0xFFC1A340         /* PVP0 CNVn Coefficient 4,4 */
#define REG_PVP0_CNV3_C44               0xFFC1A3C0         /* PVP0 CNVn Coefficient 4,4 */
#define REG_PVP0_CNV0_SCALE             0xFFC1A244         /* PVP0 CNVn Scaling Factor */
#define REG_PVP0_CNV1_SCALE             0xFFC1A2C4         /* PVP0 CNVn Scaling Factor */
#define REG_PVP0_CNV2_SCALE             0xFFC1A344         /* PVP0 CNVn Scaling Factor */
#define REG_PVP0_CNV3_SCALE             0xFFC1A3C4         /* PVP0 CNVn Scaling Factor */
#define REG_PVP0_THC0_CFG               0xFFC1A400         /* PVP0 THCn Configuration */
#define REG_PVP0_THC1_CFG               0xFFC1A500         /* PVP0 THCn Configuration */
#define REG_PVP0_THC0_CTL               0xFFC1A404         /* PVP0 THCn Control */
#define REG_PVP0_THC1_CTL               0xFFC1A504         /* PVP0 THCn Control */
#define REG_PVP0_THC0_HFCNT             0xFFC1A408         /* PVP0 THCn Histogram Frame Count */
#define REG_PVP0_THC1_HFCNT             0xFFC1A508         /* PVP0 THCn Histogram Frame Count */
#define REG_PVP0_THC0_RMAXREP           0xFFC1A40C         /* PVP0 THCn Max RLE Reports */
#define REG_PVP0_THC1_RMAXREP           0xFFC1A50C         /* PVP0 THCn Max RLE Reports */
#define REG_PVP0_THC0_CMINVAL           0xFFC1A410         /* PVP0 THCn Min Clip Value */
#define REG_PVP0_THC1_CMINVAL           0xFFC1A510         /* PVP0 THCn Min Clip Value */
#define REG_PVP0_THC0_CMINTH            0xFFC1A414         /* PVP0 THCn Clip Min Threshold */
#define REG_PVP0_THC1_CMINTH            0xFFC1A514         /* PVP0 THCn Clip Min Threshold */
#define REG_PVP0_THC0_CMAXTH            0xFFC1A418         /* PVP0 THCn Clip Max Threshold */
#define REG_PVP0_THC1_CMAXTH            0xFFC1A518         /* PVP0 THCn Clip Max Threshold */
#define REG_PVP0_THC0_CMAXVAL           0xFFC1A41C         /* PVP0 THCn Max Clip Value */
#define REG_PVP0_THC1_CMAXVAL           0xFFC1A51C         /* PVP0 THCn Max Clip Value */
#define REG_PVP0_THC0_TH0               0xFFC1A420         /* PVP0 THCn Threshold Value 0 */
#define REG_PVP0_THC1_TH0               0xFFC1A520         /* PVP0 THCn Threshold Value 0 */
#define REG_PVP0_THC0_TH1               0xFFC1A424         /* PVP0 THCn Threshold Value 1 */
#define REG_PVP0_THC1_TH1               0xFFC1A524         /* PVP0 THCn Threshold Value 1 */
#define REG_PVP0_THC0_TH2               0xFFC1A428         /* PVP0 THCn Threshold Value 2 */
#define REG_PVP0_THC1_TH2               0xFFC1A528         /* PVP0 THCn Threshold Value 2 */
#define REG_PVP0_THC0_TH3               0xFFC1A42C         /* PVP0 THCn Threshold Value 3 */
#define REG_PVP0_THC1_TH3               0xFFC1A52C         /* PVP0 THCn Threshold Value 3 */
#define REG_PVP0_THC0_TH4               0xFFC1A430         /* PVP0 THCn Threshold Value 4 */
#define REG_PVP0_THC1_TH4               0xFFC1A530         /* PVP0 THCn Threshold Value 4 */
#define REG_PVP0_THC0_TH5               0xFFC1A434         /* PVP0 THCn Threshold Value 5 */
#define REG_PVP0_THC1_TH5               0xFFC1A534         /* PVP0 THCn Threshold Value 5 */
#define REG_PVP0_THC0_TH6               0xFFC1A438         /* PVP0 THCn Threshold Value 6 */
#define REG_PVP0_THC1_TH6               0xFFC1A538         /* PVP0 THCn Threshold Value 6 */
#define REG_PVP0_THC0_TH7               0xFFC1A43C         /* PVP0 THCn Threshold Value 7 */
#define REG_PVP0_THC1_TH7               0xFFC1A53C         /* PVP0 THCn Threshold Value 7 */
#define REG_PVP0_THC0_TH8               0xFFC1A440         /* PVP0 THCn Threshold Value 8 */
#define REG_PVP0_THC1_TH8               0xFFC1A540         /* PVP0 THCn Threshold Value 8 */
#define REG_PVP0_THC0_TH9               0xFFC1A444         /* PVP0 THCn Threshold Value 9 */
#define REG_PVP0_THC1_TH9               0xFFC1A544         /* PVP0 THCn Threshold Value 9 */
#define REG_PVP0_THC0_TH10              0xFFC1A448         /* PVP0 THCn Threshold Value 10 */
#define REG_PVP0_THC1_TH10              0xFFC1A548         /* PVP0 THCn Threshold Value 10 */
#define REG_PVP0_THC0_TH11              0xFFC1A44C         /* PVP0 THCn Threshold Value 11 */
#define REG_PVP0_THC1_TH11              0xFFC1A54C         /* PVP0 THCn Threshold Value 11 */
#define REG_PVP0_THC0_TH12              0xFFC1A450         /* PVP0 THCn Threshold Value 12 */
#define REG_PVP0_THC1_TH12              0xFFC1A550         /* PVP0 THCn Threshold Value 12 */
#define REG_PVP0_THC0_TH13              0xFFC1A454         /* PVP0 THCn Threshold Value 13 */
#define REG_PVP0_THC1_TH13              0xFFC1A554         /* PVP0 THCn Threshold Value 13 */
#define REG_PVP0_THC0_TH14              0xFFC1A458         /* PVP0 THCn Threshold Value 14 */
#define REG_PVP0_THC1_TH14              0xFFC1A558         /* PVP0 THCn Threshold Value 14 */
#define REG_PVP0_THC0_TH15              0xFFC1A45C         /* PVP0 THCn Threshold Value 15 */
#define REG_PVP0_THC1_TH15              0xFFC1A55C         /* PVP0 THCn Threshold Value 15 */
#define REG_PVP0_THC0_HHPOS             0xFFC1A460         /* PVP0 THCn Histogram Horizontal Position */
#define REG_PVP0_THC1_HHPOS             0xFFC1A560         /* PVP0 THCn Histogram Horizontal Position */
#define REG_PVP0_THC0_HVPOS             0xFFC1A464         /* PVP0 THCn Histogram Vertical Position */
#define REG_PVP0_THC1_HVPOS             0xFFC1A564         /* PVP0 THCn Histogram Vertical Position */
#define REG_PVP0_THC0_HHCNT             0xFFC1A468         /* PVP0 THCn Histogram Horizontal Count */
#define REG_PVP0_THC1_HHCNT             0xFFC1A568         /* PVP0 THCn Histogram Horizontal Count */
#define REG_PVP0_THC0_HVCNT             0xFFC1A46C         /* PVP0 THCn Histogram Vertical Count */
#define REG_PVP0_THC1_HVCNT             0xFFC1A56C         /* PVP0 THCn Histogram Vertical Count */
#define REG_PVP0_THC0_RHPOS             0xFFC1A470         /* PVP0 THCn RLE Horizontal Position */
#define REG_PVP0_THC1_RHPOS             0xFFC1A570         /* PVP0 THCn RLE Horizontal Position */
#define REG_PVP0_THC0_RVPOS             0xFFC1A474         /* PVP0 THCn RLE Vertical Position */
#define REG_PVP0_THC1_RVPOS             0xFFC1A574         /* PVP0 THCn RLE Vertical Position */
#define REG_PVP0_THC0_RHCNT             0xFFC1A478         /* PVP0 THCn RLE Horizontal Count */
#define REG_PVP0_THC1_RHCNT             0xFFC1A578         /* PVP0 THCn RLE Horizontal Count */
#define REG_PVP0_THC0_RVCNT             0xFFC1A47C         /* PVP0 THCn RLE Vertical Count */
#define REG_PVP0_THC1_RVCNT             0xFFC1A57C         /* PVP0 THCn RLE Vertical Count */
#define REG_PVP0_THC0_HFCNT_STAT        0xFFC1A480         /* PVP0 THCn Histogram Frame Count Status */
#define REG_PVP0_THC1_HFCNT_STAT        0xFFC1A580         /* PVP0 THCn Histogram Frame Count Status */
#define REG_PVP0_THC0_HCNT0_STAT        0xFFC1A484         /* PVP0 THCn Histogram Counter Value 0 */
#define REG_PVP0_THC1_HCNT0_STAT        0xFFC1A584         /* PVP0 THCn Histogram Counter Value 0 */
#define REG_PVP0_THC0_HCNT1_STAT        0xFFC1A488         /* PVP0 THCn Histogram Counter Value 1 */
#define REG_PVP0_THC1_HCNT1_STAT        0xFFC1A588         /* PVP0 THCn Histogram Counter Value 1 */
#define REG_PVP0_THC0_HCNT2_STAT        0xFFC1A48C         /* PVP0 THCn Histogram Counter Value 2 */
#define REG_PVP0_THC1_HCNT2_STAT        0xFFC1A58C         /* PVP0 THCn Histogram Counter Value 2 */
#define REG_PVP0_THC0_HCNT3_STAT        0xFFC1A490         /* PVP0 THCn Histogram Counter Value 3 */
#define REG_PVP0_THC1_HCNT3_STAT        0xFFC1A590         /* PVP0 THCn Histogram Counter Value 3 */
#define REG_PVP0_THC0_HCNT4_STAT        0xFFC1A494         /* PVP0 THCn Histogram Counter Value 4 */
#define REG_PVP0_THC1_HCNT4_STAT        0xFFC1A594         /* PVP0 THCn Histogram Counter Value 4 */
#define REG_PVP0_THC0_HCNT5_STAT        0xFFC1A498         /* PVP0 THCn Histogram Counter Value 5 */
#define REG_PVP0_THC1_HCNT5_STAT        0xFFC1A598         /* PVP0 THCn Histogram Counter Value 5 */
#define REG_PVP0_THC0_HCNT6_STAT        0xFFC1A49C         /* PVP0 THCn Histogram Counter Value 6 */
#define REG_PVP0_THC1_HCNT6_STAT        0xFFC1A59C         /* PVP0 THCn Histogram Counter Value 6 */
#define REG_PVP0_THC0_HCNT7_STAT        0xFFC1A4A0         /* PVP0 THCn Histogram Counter Value 7 */
#define REG_PVP0_THC1_HCNT7_STAT        0xFFC1A5A0         /* PVP0 THCn Histogram Counter Value 7 */
#define REG_PVP0_THC0_HCNT8_STAT        0xFFC1A4A4         /* PVP0 THCn Histogram Counter Value 8 */
#define REG_PVP0_THC1_HCNT8_STAT        0xFFC1A5A4         /* PVP0 THCn Histogram Counter Value 8 */
#define REG_PVP0_THC0_HCNT9_STAT        0xFFC1A4A8         /* PVP0 THCn Histogram Counter Value 9 */
#define REG_PVP0_THC1_HCNT9_STAT        0xFFC1A5A8         /* PVP0 THCn Histogram Counter Value 9 */
#define REG_PVP0_THC0_HCNT10_STAT       0xFFC1A4AC         /* PVP0 THCn Histogram Counter Value 10 */
#define REG_PVP0_THC1_HCNT10_STAT       0xFFC1A5AC         /* PVP0 THCn Histogram Counter Value 10 */
#define REG_PVP0_THC0_HCNT11_STAT       0xFFC1A4B0         /* PVP0 THCn Histogram Counter Value 11 */
#define REG_PVP0_THC1_HCNT11_STAT       0xFFC1A5B0         /* PVP0 THCn Histogram Counter Value 11 */
#define REG_PVP0_THC0_HCNT12_STAT       0xFFC1A4B4         /* PVP0 THCn Histogram Counter Value 12 */
#define REG_PVP0_THC1_HCNT12_STAT       0xFFC1A5B4         /* PVP0 THCn Histogram Counter Value 12 */
#define REG_PVP0_THC0_HCNT13_STAT       0xFFC1A4B8         /* PVP0 THCn Histogram Counter Value 13 */
#define REG_PVP0_THC1_HCNT13_STAT       0xFFC1A5B8         /* PVP0 THCn Histogram Counter Value 13 */
#define REG_PVP0_THC0_HCNT14_STAT       0xFFC1A4BC         /* PVP0 THCn Histogram Counter Value 14 */
#define REG_PVP0_THC1_HCNT14_STAT       0xFFC1A5BC         /* PVP0 THCn Histogram Counter Value 14 */
#define REG_PVP0_THC0_HCNT15_STAT       0xFFC1A4C0         /* PVP0 THCn Histogram Counter Value 15 */
#define REG_PVP0_THC1_HCNT15_STAT       0xFFC1A5C0         /* PVP0 THCn Histogram Counter Value 15 */
#define REG_PVP0_THC0_RREP_STAT         0xFFC1A4C4         /* PVP0 THCn Number of RLE Reports */
#define REG_PVP0_THC1_RREP_STAT         0xFFC1A5C4         /* PVP0 THCn Number of RLE Reports */
#define REG_PVP0_PMA_CFG                0xFFC1A600         /* PVP0 PMA Configuration */

/* =========================
        PVP
   ========================= */
/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CTL                              Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CTL_CLKDIV                   4                               /* Clock Divisor */
#define BITP_PVP_CTL_CPEN                     2                               /* Camera Pipe Enable */
#define BITP_PVP_CTL_MPEN                     1                               /* Memory Pipe Enable */
#define BITP_PVP_CTL_PVPEN                    0                               /* PVP Enable */

#define BITM_PVP_CTL_CLKDIV                  0x00000010  /* Clock Divisor */
#define ENUM_PVP_CTL_CLKDIV1                 0x00000000  /* CLKDIV: PVPCLK = SCLK */
#define ENUM_PVP_CTL_CLKDIV2                 0x00000010  /* CLKDIV: PVPCLK = SCLK/2 */

#define BITM_PVP_CTL_CPEN                    0x00000004  /* Camera Pipe Enable */
#define ENUM_PVP_CTL_CPDIS                   0x00000000  /* CPEN: Disable Camera Pipe */
#define ENUM_PVP_CTL_CPEN                    0x00000004  /* CPEN: Enable Camera Pipe */

#define BITM_PVP_CTL_MPEN                    0x00000002  /* Memory Pipe Enable */
#define ENUM_PVP_CTL_MPDIS                   0x00000000  /* MPEN: Disable Memory Pipe */
#define ENUM_PVP_CTL_MPEN                    0x00000002  /* MPEN: Enable Memory Pipe */

#define BITM_PVP_CTL_PVPEN                   0x00000001  /* PVP Enable */
#define ENUM_PVP_CTL_PVPDIS                  0x00000000  /* PVPEN: Disable PVP */
#define ENUM_PVP_CTL_PVPEN                   0x00000001  /* PVPEN: Enable PVP */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IMSK                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IMSK_ACUSUMSAT              27                               /* ACU SUM Saturate Unmask */
#define BITP_PVP_IMSK_ACUPRODSAT             26                               /* ACU PROD Saturate Unmask */
#define BITP_PVP_IMSK_ACUOUTSAT              25                               /* ACU MIN/MAX Saturate Unmask */
#define BITP_PVP_IMSK_ACUDIVERR              24                               /* ACU Divide By Zero Unmask */
#define BITP_PVP_IMSK_IIM1SOVF               23                               /* IIM1 Signed Overflow Unmask */
#define BITP_PVP_IMSK_IIM1UOVF               22                               /* IIM1 Unsigned Overflow Unmask */
#define BITP_PVP_IMSK_IIM0SOVF               21                               /* IIM0 Signed Overflow Unmask */
#define BITP_PVP_IMSK_IIM0UOVF               20                               /* IIM0 Unsigned Overflow Unmask */
#define BITP_PVP_IMSK_THC1RDY                18                               /* THC1 Report Ready Unmask */
#define BITP_PVP_IMSK_THC0RDY                16                               /* THC0 Report Ready Unmask */
#define BITP_PVP_IMSK_MPRDY                  15                               /* Memory Pipe Ready Unmask */
#define BITP_PVP_IMSK_CPRDY                  14                               /* Camera Pipe Ready Unmask */
#define BITP_PVP_IMSK_MPDRN                  13                               /* Memory Pipe Drain Done Unmask */
#define BITP_PVP_IMSK_CPDRN                  12                               /* Camera Pipe Drain Done Unmask */
#define BITP_PVP_IMSK_CPIPFOVF               10                               /* Camera Pipe Pixel Overrun Unmask */
#define BITP_PVP_IMSK_MPOPFDAT                9                               /* Memory Pipe First Pixel Unmask */
#define BITP_PVP_IMSK_CPOPFDAT                8                               /* Camera Pipe First Pixel Unmask */
#define BITP_PVP_IMSK_CPSTOVF                 7                               /* Status DDE Stall Error Unmask */
#define BITP_PVP_IMSK_OPF2OVF                 6                               /* OPF2 DDE Stall Error Unmask */
#define BITP_PVP_IMSK_OPF1OVF                 5                               /* OPF1 DDE Stall Error Unmask */
#define BITP_PVP_IMSK_OPF0OVF                 4                               /* OPF0 DDE Stall Error Unmask */
#define BITP_PVP_IMSK_MPWRERR                 3                               /* Memory Pipe MMR Write Error Unmask */
#define BITP_PVP_IMSK_CPWRERR                 2                               /* Camera Pipe MMR Write Error Unmask */
#define BITP_PVP_IMSK_MPDC                    1                               /* Memory Pipe DC Unmask */
#define BITP_PVP_IMSK_CPDC                    0                               /* Camera Pipe DC Unmask */

/* The fields and enumerations for PVP_IMSK are also in PVP - see the common set of ENUM_PVP_* #defines located with register PVP_STAT */

#define BITM_PVP_IMSK_ACUSUMSAT              0x08000000  /* ACU SUM Saturate Unmask */
#define BITM_PVP_IMSK_ACUPRODSAT             0x04000000  /* ACU PROD Saturate Unmask */
#define BITM_PVP_IMSK_ACUOUTSAT              0x02000000  /* ACU MIN/MAX Saturate Unmask */
#define BITM_PVP_IMSK_ACUDIVERR              0x01000000  /* ACU Divide By Zero Unmask */
#define BITM_PVP_IMSK_IIM1SOVF               0x00800000  /* IIM1 Signed Overflow Unmask */
#define BITM_PVP_IMSK_IIM1UOVF               0x00400000  /* IIM1 Unsigned Overflow Unmask */
#define BITM_PVP_IMSK_IIM0SOVF               0x00200000  /* IIM0 Signed Overflow Unmask */
#define BITM_PVP_IMSK_IIM0UOVF               0x00100000  /* IIM0 Unsigned Overflow Unmask */
#define BITM_PVP_IMSK_THC1RDY                0x00040000  /* THC1 Report Ready Unmask */
#define BITM_PVP_IMSK_THC0RDY                0x00010000  /* THC0 Report Ready Unmask */
#define BITM_PVP_IMSK_MPRDY                  0x00008000  /* Memory Pipe Ready Unmask */
#define BITM_PVP_IMSK_CPRDY                  0x00004000  /* Camera Pipe Ready Unmask */
#define BITM_PVP_IMSK_MPDRN                  0x00002000  /* Memory Pipe Drain Done Unmask */
#define BITM_PVP_IMSK_CPDRN                  0x00001000  /* Camera Pipe Drain Done Unmask */
#define BITM_PVP_IMSK_CPIPFOVF               0x00000400  /* Camera Pipe Pixel Overrun Unmask */
#define BITM_PVP_IMSK_MPOPFDAT               0x00000200  /* Memory Pipe First Pixel Unmask */
#define BITM_PVP_IMSK_CPOPFDAT               0x00000100  /* Camera Pipe First Pixel Unmask */
#define BITM_PVP_IMSK_CPSTOVF                0x00000080  /* Status DDE Stall Error Unmask */
#define BITM_PVP_IMSK_OPF2OVF                0x00000040  /* OPF2 DDE Stall Error Unmask */
#define BITM_PVP_IMSK_OPF1OVF                0x00000020  /* OPF1 DDE Stall Error Unmask */
#define BITM_PVP_IMSK_OPF0OVF                0x00000010  /* OPF0 DDE Stall Error Unmask */
#define BITM_PVP_IMSK_MPWRERR                0x00000008  /* Memory Pipe MMR Write Error Unmask */
#define BITM_PVP_IMSK_CPWRERR                0x00000004  /* Camera Pipe MMR Write Error Unmask */
#define BITM_PVP_IMSK_MPDC                   0x00000002  /* Memory Pipe DC Unmask */
#define BITM_PVP_IMSK_CPDC                   0x00000001  /* Camera Pipe DC Unmask */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_STAT                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_STAT_ACUSUMSAT              27                               /* ACU SUM Saturate Status */
#define BITP_PVP_STAT_ACUPRODSAT             26                               /* ACU PROD Saturate Status */
#define BITP_PVP_STAT_ACUOUTSAT              25                               /* ACU MIN/MAX Saturate Status */
#define BITP_PVP_STAT_ACUDIVERR              24                               /* ACU Divide By Zero Status */
#define BITP_PVP_STAT_IIM1SOVF               23                               /* IIM1 Signed Overflow Status */
#define BITP_PVP_STAT_IIM1UOVF               22                               /* IIM1 Unsigned Overflow Status */
#define BITP_PVP_STAT_IIM0SOVF               21                               /* IIM0 Signed Overflow Status */
#define BITP_PVP_STAT_IIM0UOVF               20                               /* IIM0 Unsigned Overflow Status */
#define BITP_PVP_STAT_THC1RDY                18                               /* THC1 Report Ready Status */
#define BITP_PVP_STAT_THC0RDY                16                               /* THC0 Report Ready Status */
#define BITP_PVP_STAT_MPRDY                  15                               /* Memory Pipe Ready Status */
#define BITP_PVP_STAT_CPRDY                  14                               /* Camera Pipe Ready Status */
#define BITP_PVP_STAT_MPDRN                  13                               /* Memory Pipe Drain Done Status */
#define BITP_PVP_STAT_CPDRN                  12                               /* Camera Pipe Drain Done Status */
#define BITP_PVP_STAT_CPIPFOVF               10                               /* Camera Pipe Pixel Overrun Status */
#define BITP_PVP_STAT_MPOPFDAT                9                               /* Memory Pipe First Pixel Status */
#define BITP_PVP_STAT_CPOPFDAT                8                               /* Camera Pipe First Pixel Status */
#define BITP_PVP_STAT_CPSTOVF                 7                               /* Camera Pipe DDE Stall Error Status */
#define BITP_PVP_STAT_OPF2OVF                 6                               /* OPF2 DDE Stall Error Status */
#define BITP_PVP_STAT_OPF1OVF                 5                               /* OPF1 DDE Stall Error Status */
#define BITP_PVP_STAT_OPF0OVF                 4                               /* OPF0 DDE Stall Error Status */
#define BITP_PVP_STAT_MPWRERR                 3                               /* Memory Pipe MMR Write Error Status */
#define BITP_PVP_STAT_CPWRERR                 2                               /* Camera Pipe MMR Write Error Status */
#define BITP_PVP_STAT_MPDC                    1                               /* Memory Pipe DC Status */
#define BITP_PVP_STAT_CPDC                    0                               /* Camera Pipe DC Status */

#define BITM_PVP_STAT_ACUSUMSAT              0x08000000  /* ACU SUM Saturate Status */
#define ENUM_PVP_ACUSUMSAT_LO                0x00000000  /* ACUSUMSAT: No Pending Interrupt */
#define ENUM_PVP_ACUSUMSAT_HI                0x08000000  /* ACUSUMSAT: Pending Interrupt */

#define BITM_PVP_STAT_ACUPRODSAT             0x04000000  /* ACU PROD Saturate Status */
#define ENUM_PVP_ACUPRODSAT_LO               0x00000000  /* ACUPRODSAT: No Pending Interrupt */
#define ENUM_PVP_ACUPRODSAT_HI               0x04000000  /* ACUPRODSAT: Pending Interrupt */

#define BITM_PVP_STAT_ACUOUTSAT              0x02000000  /* ACU MIN/MAX Saturate Status */
#define ENUM_PVP_ACUOUTSAT_LO                0x00000000  /* ACUOUTSAT: No Pending Interrupt */
#define ENUM_PVP_ACUOUTSAT_HI                0x02000000  /* ACUOUTSAT: Pending Interrupt */

#define BITM_PVP_STAT_ACUDIVERR              0x01000000  /* ACU Divide By Zero Status */
#define ENUM_PVP_ACUDIVERR_LO                0x00000000  /* ACUDIVERR: No Pending Interrupt */
#define ENUM_PVP_ACUDIVERR_HI                0x01000000  /* ACUDIVERR: Pending Interrupt */

#define BITM_PVP_STAT_IIM1SOVF               0x00800000  /* IIM1 Signed Overflow Status */
#define ENUM_PVP_IIM1SOVF_LO                 0x00000000  /* IIM1SOVF: No Pending Interrupt */
#define ENUM_PVP_IIM1SOVF_HI                 0x00800000  /* IIM1SOVF: Pending Interrupt */

#define BITM_PVP_STAT_IIM1UOVF               0x00400000  /* IIM1 Unsigned Overflow Status */
#define ENUM_PVP_IIM1UOVF_LO                 0x00000000  /* IIM1UOVF: No Pending Interrupt */
#define ENUM_PVP_IIM1UOVF_HI                 0x00400000  /* IIM1UOVF: Pending Interrupt */

#define BITM_PVP_STAT_IIM0SOVF               0x00200000  /* IIM0 Signed Overflow Status */
#define ENUM_PVP_IIM0SOVF_LO                 0x00000000  /* IIM0SOVF: No Pending Interrupt */
#define ENUM_PVP_IIM0SOVF_HI                 0x00200000  /* IIM0SOVF: Pending Interrupt */

#define BITM_PVP_STAT_IIM0UOVF               0x00100000  /* IIM0 Unsigned Overflow Status */
#define ENUM_PVP_IIM0UOVF_LO                 0x00000000  /* IIM0UOVF: No Pending Interrupt */
#define ENUM_PVP_IIM0UOVF_HI                 0x00100000  /* IIM0UOVF: Pending Interrupt */

#define BITM_PVP_STAT_THC1RDY                0x00040000  /* THC1 Report Ready Status */
#define ENUM_PVP_THC1RDY_LO                  0x00000000  /* THC1RDY: No Pending Interrupt */
#define ENUM_PVP_THC1RDY_HI                  0x00040000  /* THC1RDY: Pending Interrupt */

#define BITM_PVP_STAT_THC0RDY                0x00010000  /* THC0 Report Ready Status */
#define ENUM_PVP_THC0RDY_LO                  0x00000000  /* THC0RDY: No Pending Interrupt */
#define ENUM_PVP_THC0RDY_HI                  0x00010000  /* THC0RDY: Pending Interrupt */

#define BITM_PVP_STAT_MPRDY                  0x00008000  /* Memory Pipe Ready Status */
#define ENUM_PVP_MPRDY_LO                    0x00000000  /* MPRDY: No Pending Interrupt */
#define ENUM_PVP_MPRDY_HI                    0x00008000  /* MPRDY: Pending Interrupt */

#define BITM_PVP_STAT_CPRDY                  0x00004000  /* Camera Pipe Ready Status */
#define ENUM_PVP_CPRDY_LO                    0x00000000  /* CPRDY: No Pending Interrupt */
#define ENUM_PVP_CPRDY_HI                    0x00004000  /* CPRDY: Pending Interrupt */

#define BITM_PVP_STAT_MPDRN                  0x00002000  /* Memory Pipe Drain Done Status */
#define ENUM_PVP_MPDRN_LO                    0x00000000  /* MPDRN: No Pending Interrupt */
#define ENUM_PVP_MPDRN_HI                    0x00002000  /* MPDRN: Pending Interrupt */

#define BITM_PVP_STAT_CPDRN                  0x00001000  /* Camera Pipe Drain Done Status */
#define ENUM_PVP_CPDRN_LO                    0x00000000  /* CPDRN: No Pending Interrupt */
#define ENUM_PVP_CPDRN_HI                    0x00001000  /* CPDRN: Pending Interrupt */

#define BITM_PVP_STAT_CPIPFOVF               0x00000400  /* Camera Pipe Pixel Overrun Status */
#define ENUM_PVP_CPIPFOVF_LO                 0x00000000  /* CPIPFOVF: No Pending Interrupt */
#define ENUM_PVP_CPIPFOVF_HI                 0x00000400  /* CPIPFOVF: Pending Interrupt */

#define BITM_PVP_STAT_MPOPFDAT               0x00000200  /* Memory Pipe First Pixel Status */
#define ENUM_PVP_MPOPFDAT_LO                 0x00000000  /* MPOPFDAT: No Pending Interrupt */
#define ENUM_PVP_MPOPFDAT_HI                 0x00000200  /* MPOPFDAT: Pending Interrupt */

#define BITM_PVP_STAT_CPOPFDAT               0x00000100  /* Camera Pipe First Pixel Status */
#define ENUM_PVP_CPOPFDAT_LO                 0x00000000  /* CPOPFDAT: No Pending Interrupt */
#define ENUM_PVP_CPOPFDAT_HI                 0x00000100  /* CPOPFDAT: Pending Interrupt */

#define BITM_PVP_STAT_CPSTOVF                0x00000080  /* Camera Pipe DDE Stall Error Status */
#define ENUM_PVP_CPSTOVF_LO                  0x00000000  /* CPSTOVF: No Pending Interrupt */
#define ENUM_PVP_CPSTOVF_HI                  0x00000080  /* CPSTOVF: Pending Interrupt */

#define BITM_PVP_STAT_OPF2OVF                0x00000040  /* OPF2 DDE Stall Error Status */
#define ENUM_PVP_OPF2OVF_LO                  0x00000000  /* OPF2OVF: No Pending Interrupt */
#define ENUM_PVP_OPF2OVF_HI                  0x00000040  /* OPF2OVF: Pending Interrupt */

#define BITM_PVP_STAT_OPF1OVF                0x00000020  /* OPF1 DDE Stall Error Status */
#define ENUM_PVP_OPF1OVF_LO                  0x00000000  /* OPF1OVF: No Pending Interrupt */
#define ENUM_PVP_OPF1OVF_HI                  0x00000020  /* OPF1OVF: Pending Interrupt */

#define BITM_PVP_STAT_OPF0OVF                0x00000010  /* OPF0 DDE Stall Error Status */
#define ENUM_PVP_OPF0OVF_LO                  0x00000000  /* OPF0OVF: No Pending Interrupt */
#define ENUM_PVP_OPF0OVF_HI                  0x00000010  /* OPF0OVF: Pending Interrupt */

#define BITM_PVP_STAT_MPWRERR                0x00000008  /* Memory Pipe MMR Write Error Status */
#define ENUM_PVP_MPWRERR_LO                  0x00000000  /* MPWRERR: No Pending Interrupt */
#define ENUM_PVP_MPWRERR_HI                  0x00000008  /* MPWRERR: Pending Interrupt */

#define BITM_PVP_STAT_CPWRERR                0x00000004  /* Camera Pipe MMR Write Error Status */
#define ENUM_PVP_CPWRERR_LO                  0x00000000  /* CPWRERR: No Pending Interrupt */
#define ENUM_PVP_CPWRERR_HI                  0x00000004  /* CPWRERR: Pending Interrupt */

#define BITM_PVP_STAT_MPDC                   0x00000002  /* Memory Pipe DC Status */
#define ENUM_PVP_MPDC_LO                     0x00000000  /* MPDC: No Pending Interrupt */
#define ENUM_PVP_MPDC_HI                     0x00000002  /* MPDC: Pending Interrupt */

#define BITM_PVP_STAT_CPDC                   0x00000001  /* Camera Pipe DC Status */
#define ENUM_PVP_CPDC_LO                     0x00000000  /* CPDC: No Pending Interrupt */
#define ENUM_PVP_CPDC_HI                     0x00000001  /* CPDC: Pending Interrupt */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_ILAT                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_ILAT_ACUSUMSAT              27                               /* ACU SUM Saturate Latch */
#define BITP_PVP_ILAT_ACUPRODSAT             26                               /* ACU PROD Saturate Latch */
#define BITP_PVP_ILAT_ACUOUTSAT              25                               /* ACU MIN/MAX Saturate Latch */
#define BITP_PVP_ILAT_ACUDIVERR              24                               /* ACU Divide By Zero Latch */
#define BITP_PVP_ILAT_IIM1SOVF               23                               /* IIM1 Signed Overflow Latch */
#define BITP_PVP_ILAT_IIM1UOVF               22                               /* IIM1 Unsigned Overflow Latch */
#define BITP_PVP_ILAT_IIM0SOVF               21                               /* IIM0 Signed Overflow Latch */
#define BITP_PVP_ILAT_IIM0UOVF               20                               /* IIM0 Unsigned Overflow Latch */
#define BITP_PVP_ILAT_THC1RDY                18                               /* THC1 Report Ready Latch */
#define BITP_PVP_ILAT_THC0RDY                16                               /* THC0 Report Ready Latch */
#define BITP_PVP_ILAT_MPRDY                  15                               /* Memory Pipe Ready Latch */
#define BITP_PVP_ILAT_CPRDY                  14                               /* Camera Pipe Ready Latch */
#define BITP_PVP_ILAT_MPDRN                  13                               /* Memory Pipe Drain Done Latch */
#define BITP_PVP_ILAT_CPDRN                  12                               /* Camera Pipe Drain Done Latch */
#define BITP_PVP_ILAT_CPIPFOVF               10                               /* Camera Pipe Pixel Overrun Latch */
#define BITP_PVP_ILAT_MPOPFDAT                9                               /* Memory Pipe First Pixel Latch */
#define BITP_PVP_ILAT_CPOPFDAT                8                               /* Camera Pipe First Pixel Latch */
#define BITP_PVP_ILAT_CPSTOVF                 7                               /* Status DDE Stall Error Latch */
#define BITP_PVP_ILAT_OPF2OVF                 6                               /* OPF2 DDE Stall Error Latch */
#define BITP_PVP_ILAT_OPF1OVF                 5                               /* OPF1 DDE Stall Error Latch */
#define BITP_PVP_ILAT_OPF0OVF                 4                               /* OPF0 DDE Stall Error Latch */
#define BITP_PVP_ILAT_MPWRERR                 3                               /* Memory Pipe MMR Write Error Latch */
#define BITP_PVP_ILAT_CPWRERR                 2                               /* Camera Pipe MMR Write Error Latch */
#define BITP_PVP_ILAT_MPDC                    1                               /* Memory Pipe DC Mask */
#define BITP_PVP_ILAT_CPDC                    0                               /* Camera Pipe DC Latch */

/* The fields and enumerations for PVP_ILAT are also in PVP - see the common set of ENUM_PVP_* #defines located with register PVP_STAT */

#define BITM_PVP_ILAT_ACUSUMSAT              0x08000000  /* ACU SUM Saturate Latch */
#define BITM_PVP_ILAT_ACUPRODSAT             0x04000000  /* ACU PROD Saturate Latch */
#define BITM_PVP_ILAT_ACUOUTSAT              0x02000000  /* ACU MIN/MAX Saturate Latch */
#define BITM_PVP_ILAT_ACUDIVERR              0x01000000  /* ACU Divide By Zero Latch */
#define BITM_PVP_ILAT_IIM1SOVF               0x00800000  /* IIM1 Signed Overflow Latch */
#define BITM_PVP_ILAT_IIM1UOVF               0x00400000  /* IIM1 Unsigned Overflow Latch */
#define BITM_PVP_ILAT_IIM0SOVF               0x00200000  /* IIM0 Signed Overflow Latch */
#define BITM_PVP_ILAT_IIM0UOVF               0x00100000  /* IIM0 Unsigned Overflow Latch */
#define BITM_PVP_ILAT_THC1RDY                0x00040000  /* THC1 Report Ready Latch */
#define BITM_PVP_ILAT_THC0RDY                0x00010000  /* THC0 Report Ready Latch */
#define BITM_PVP_ILAT_MPRDY                  0x00008000  /* Memory Pipe Ready Latch */
#define BITM_PVP_ILAT_CPRDY                  0x00004000  /* Camera Pipe Ready Latch */
#define BITM_PVP_ILAT_MPDRN                  0x00002000  /* Memory Pipe Drain Done Latch */
#define BITM_PVP_ILAT_CPDRN                  0x00001000  /* Camera Pipe Drain Done Latch */
#define BITM_PVP_ILAT_CPIPFOVF               0x00000400  /* Camera Pipe Pixel Overrun Latch */
#define BITM_PVP_ILAT_MPOPFDAT               0x00000200  /* Memory Pipe First Pixel Latch */
#define BITM_PVP_ILAT_CPOPFDAT               0x00000100  /* Camera Pipe First Pixel Latch */
#define BITM_PVP_ILAT_CPSTOVF                0x00000080  /* Status DDE Stall Error Latch */
#define BITM_PVP_ILAT_OPF2OVF                0x00000040  /* OPF2 DDE Stall Error Latch */
#define BITM_PVP_ILAT_OPF1OVF                0x00000020  /* OPF1 DDE Stall Error Latch */
#define BITM_PVP_ILAT_OPF0OVF                0x00000010  /* OPF0 DDE Stall Error Latch */
#define BITM_PVP_ILAT_MPWRERR                0x00000008  /* Memory Pipe MMR Write Error Latch */
#define BITM_PVP_ILAT_CPWRERR                0x00000004  /* Camera Pipe MMR Write Error Latch */
#define BITM_PVP_ILAT_MPDC                   0x00000002  /* Memory Pipe DC Mask */
#define BITM_PVP_ILAT_CPDC                   0x00000001  /* Camera Pipe DC Latch */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IREQ                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IREQ_ACUSUMSAT              27                               /* ACU SUM Saturate Request */
#define BITP_PVP_IREQ_ACUPRODSAT             26                               /* ACU PROD Saturate Request */
#define BITP_PVP_IREQ_ACUOUTSAT              25                               /* ACU MIN/MAX Saturate Request */
#define BITP_PVP_IREQ_ACUDIVERR              24                               /* ACU Divide By Zero Request */
#define BITP_PVP_IREQ_IIM1SOVF               23                               /* IIM1 Signed Overflow Request */
#define BITP_PVP_IREQ_IIM1UOVF               22                               /* IIM1 Unsigned Overflow Request */
#define BITP_PVP_IREQ_IIM0SOVF               21                               /* IIM0 Signed Overflow Request */
#define BITP_PVP_IREQ_IIM0UOVF               20                               /* IIM0 Unsigned Overflow Request */
#define BITP_PVP_IREQ_THC1RDY                18                               /* THC1 Report Ready Request */
#define BITP_PVP_IREQ_THC0RDY                16                               /* THC0 Report Ready Request */
#define BITP_PVP_IREQ_MPRDY                  15                               /* Memory Pipe Ready Request */
#define BITP_PVP_IREQ_CPRDY                  14                               /* Camera Pipe Ready Request */
#define BITP_PVP_IREQ_MPDRN                  13                               /* Memory Pipe Drain Done Request */
#define BITP_PVP_IREQ_CPDRN                  12                               /* Camera Pipe Drain Done Request */
#define BITP_PVP_IREQ_CPIPFOVF               10                               /* Camera Pipe Pixel Overrun Request */
#define BITP_PVP_IREQ_MPOPFDAT                9                               /* Memory Pipe First Pixel Request */
#define BITP_PVP_IREQ_CPOPFDAT                8                               /* Camera Pipe First Pixel Request */
#define BITP_PVP_IREQ_CPSTOVF                 7                               /* Status DDE Stall Error Request */
#define BITP_PVP_IREQ_OPF2OVF                 6                               /* OPF2 DDE Stall Error Request */
#define BITP_PVP_IREQ_OPF1OVF                 5                               /* OPF1 DDE Stall Error Request */
#define BITP_PVP_IREQ_OPF0OVF                 4                               /* OPF0 DDE Stall Error Request */
#define BITP_PVP_IREQ_MPWRERR                 3                               /* Memory Pipe MMR Write Error Request */
#define BITP_PVP_IREQ_CPWRERR                 2                               /* Camera Pipe MMR Write Error Request */
#define BITP_PVP_IREQ_MPDC                    1                               /* Memory Pipe DC Request */
#define BITP_PVP_IREQ_CPDC                    0                               /* Camera Pipe DC Request */

/* The fields and enumerations for PVP_IREQ are also in PVP - see the common set of ENUM_PVP_* #defines located with register PVP_STAT */

#define BITM_PVP_IREQ_ACUSUMSAT              0x08000000  /* ACU SUM Saturate Request */
#define BITM_PVP_IREQ_ACUPRODSAT             0x04000000  /* ACU PROD Saturate Request */
#define BITM_PVP_IREQ_ACUOUTSAT              0x02000000  /* ACU MIN/MAX Saturate Request */
#define BITM_PVP_IREQ_ACUDIVERR              0x01000000  /* ACU Divide By Zero Request */
#define BITM_PVP_IREQ_IIM1SOVF               0x00800000  /* IIM1 Signed Overflow Request */
#define BITM_PVP_IREQ_IIM1UOVF               0x00400000  /* IIM1 Unsigned Overflow Request */
#define BITM_PVP_IREQ_IIM0SOVF               0x00200000  /* IIM0 Signed Overflow Request */
#define BITM_PVP_IREQ_IIM0UOVF               0x00100000  /* IIM0 Unsigned Overflow Request */
#define BITM_PVP_IREQ_THC1RDY                0x00040000  /* THC1 Report Ready Request */
#define BITM_PVP_IREQ_THC0RDY                0x00010000  /* THC0 Report Ready Request */
#define BITM_PVP_IREQ_MPRDY                  0x00008000  /* Memory Pipe Ready Request */
#define BITM_PVP_IREQ_CPRDY                  0x00004000  /* Camera Pipe Ready Request */
#define BITM_PVP_IREQ_MPDRN                  0x00002000  /* Memory Pipe Drain Done Request */
#define BITM_PVP_IREQ_CPDRN                  0x00001000  /* Camera Pipe Drain Done Request */
#define BITM_PVP_IREQ_CPIPFOVF               0x00000400  /* Camera Pipe Pixel Overrun Request */
#define BITM_PVP_IREQ_MPOPFDAT               0x00000200  /* Memory Pipe First Pixel Request */
#define BITM_PVP_IREQ_CPOPFDAT               0x00000100  /* Camera Pipe First Pixel Request */
#define BITM_PVP_IREQ_CPSTOVF                0x00000080  /* Status DDE Stall Error Request */
#define BITM_PVP_IREQ_OPF2OVF                0x00000040  /* OPF2 DDE Stall Error Request */
#define BITM_PVP_IREQ_OPF1OVF                0x00000020  /* OPF1 DDE Stall Error Request */
#define BITM_PVP_IREQ_OPF0OVF                0x00000010  /* OPF0 DDE Stall Error Request */
#define BITM_PVP_IREQ_MPWRERR                0x00000008  /* Memory Pipe MMR Write Error Request */
#define BITM_PVP_IREQ_CPWRERR                0x00000004  /* Camera Pipe MMR Write Error Request */
#define BITM_PVP_IREQ_MPDC                   0x00000002  /* Memory Pipe DC Request */
#define BITM_PVP_IREQ_CPDC                   0x00000001  /* Camera Pipe DC Request */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_OPF_CFG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_OPF_CFG_IBLOCK0              8                               /* Input Block ID */
#define BITP_PVP_OPF_CFG_IPORT0               4                               /* Input Port ID */
#define BITP_PVP_OPF_CFG_MPIPE                2                               /* Memory Pipe */
#define BITP_PVP_OPF_CFG_START                0                               /* Start */

#define BITM_PVP_OPF_CFG_IBLOCK0             0x0000FF00  					   /* Input Block ID */

#define BITM_PVP_OPF_CFG_IPORT0              0x00000030  /* Input Port ID */
#define BITM_PVP_OPF_CFG_MPIPE               0x00000004  /* Memory Pipe */

#define BITM_PVP_OPF_CFG_START               0x00000001  /* Start */
#define ENUM_PVP_OPF_CFG_START               0x00000001  /* Start */


/* ------------------------------------------------------------------------------------------------------------------------
        PVP_OPF_CTL                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_OPF_CTL_FINISH              12                               /* Finish Enable */
#define BITP_PVP_OPF_CTL_OSIZE                8                               /* Output Data Size */
#define BITP_PVP_OPF_CTL_QFRMT                5                               /* Q Format Correction */
#define BITP_PVP_OPF_CTL_IUP16                4                               /* Input Upper 16-Bit Data */
#define BITP_PVP_OPF_CTL_ISIZE                0                               /* Input Data Size */

#define BITM_PVP_OPF_CTL_FINISH              0x00001000  /* Finish Enable */
#define ENUM_PVP_OPF_CTL_NOFINISH            0x00000000  /* FINISH: Disable Finish Signal */
#define ENUM_PVP_OPF_CTL_FINISH              0x00001000  /* FINISH: Enable Finish Signal */

#define BITM_PVP_OPF_CTL_OSIZE               0x00000300  /* Output Data Size */
#define ENUM_PVP_OPF_CTL_OSIZE32             0x00000000  /* OSIZE: 32-Bit Output Data Size */
#define ENUM_PVP_OPF_CTL_OSIZE16             0x00000100  /* OSIZE: 16-Bit Output Data Size */
#define ENUM_PVP_OPF_CTL_OSIZE8              0x00000200  /* OSIZE: 8-Bit Output Data Size */

#define BITM_PVP_OPF_CTL_QFRMT               0x00000020  /* Q Format Correction */
#define ENUM_PVP_OPF_CTL_NOQFRMT             0x00000000  /* QFRMT: Disable Q Format Correction */
#define ENUM_PVP_OPF_CTL_QFRMT               0x00000020  /* QFRMT: Enable Q Format Correction */

#define BITM_PVP_OPF_CTL_IUP16               0x00000010  /* Input Upper 16-Bit Data */
#define ENUM_PVP_OPF_CTL_LOWER16             0x00000000  /* IUP16: Lower 16 Bits */
#define ENUM_PVP_OPF_CTL_UPPER16             0x00000010  /* IUP16: Upper 16 Bits */

#define BITM_PVP_OPF_CTL_ISIZE               0x00000003  /* Input Data Size */
#define ENUM_PVP_OPF_CTL_ISIZE32             0x00000000  /* ISIZE: 32-Bit Input Data Size */
#define ENUM_PVP_OPF_CTL_ISIZE16             0x00000001  /* ISIZE: 16-Bit Input Data Size */
#define ENUM_PVP_OPF_CTL_ISIZE8              0x00000002  /* ISIZE: 8-Bit Input Data Size */
#define ENUM_PVP_OPF_CTL_ISIZE4              0x00000003  /* ISIZE: 4-Bit Input Data Size */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_OPF3_CFG                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_OPF3_CFG_IBLOCK0             8                               /* Input Block ID */
#define BITP_PVP_OPF3_CFG_IPORT0              4                               /* Input Port ID */
#define BITP_PVP_OPF3_CFG_MPIPE               2                               /* Memory Pipe */
#define BITP_PVP_OPF3_CFG_START               0                               /* Start */
#define BITM_PVP_OPF3_CFG_IBLOCK0            0x0000FF00  /* Input Block ID */
#define BITM_PVP_OPF3_CFG_IPORT0             0x00000030  /* Input Port ID */
#define BITM_PVP_OPF3_CFG_MPIPE              0x00000004  /* Memory Pipe */
#define BITM_PVP_OPF3_CFG_START              0x00000001  /* Start */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_OPF3_CTL                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_OPF3_CTL_FINISH             12                               /* Finish Enable */
#define BITP_PVP_OPF3_CTL_OSIZE               8                               /* Output Data Size */
#define BITP_PVP_OPF3_CTL_QFRMT               5                               /* Q Format Correction */
#define BITP_PVP_OPF3_CTL_IUP16               4                               /* Input Upper 16-Bit Data */
#define BITP_PVP_OPF3_CTL_ISIZE               0                               /* Input Data Size */

#define BITM_PVP_OPF3_CTL_FINISH             0x00001000  /* Finish Enable */
#define ENUM_PVP_OPF3_CTL_NOFINISH           0x00000000  /* FINISH: Disable Finish Signal */
#define ENUM_PVP_OPF3_CTL_FINISH             0x00001000  /* FINISH: Enable Finish Signal */

#define BITM_PVP_OPF3_CTL_OSIZE              0x00000300  /* Output Data Size */
#define ENUM_PVP_OPF3_CTL_OSIZE32            0x00000000  /* OSIZE: 32-Bit Output Data Size */
#define ENUM_PVP_OPF3_CTL_OSIZE16            0x00000100  /* OSIZE: 16-Bit Output Data Size */
#define ENUM_PVP_OPF3_CTL_OSIZE8             0x00000200  /* OSIZE: 8-Bit Output Data Size */

#define BITM_PVP_OPF3_CTL_QFRMT              0x00000020  /* Q Format Correction */
#define ENUM_PVP_OPF3_CTL_NOQFRMT            0x00000000  /* QFRMT: Disable Q Format Correction */
#define ENUM_PVP_OPF3_CTL_QFRMT              0x00000020  /* QFRMT: Enable Q Format Correction */

#define BITM_PVP_OPF3_CTL_IUP16              0x00000010  /* Input Upper 16-Bit Data */
#define ENUM_PVP_OPF3_CTL_LOWER16            0x00000000  /* IUP16: Lower 16 Bits */
#define ENUM_PVP_OPF3_CTL_UPPER16            0x00000010  /* IUP16: Upper 16 Bits */

#define BITM_PVP_OPF3_CTL_ISIZE              0x00000003  /* Input Data Size */
#define ENUM_PVP_OPF3_CTL_ISIZE32            0x00000000  /* ISIZE: 32-Bit Input Data Size */
#define ENUM_PVP_OPF3_CTL_ISIZE16            0x00000001  /* ISIZE: 16-Bit Input Data Size */
#define ENUM_PVP_OPF3_CTL_ISIZE8             0x00000002  /* ISIZE: 8-Bit Input Data Size */
#define ENUM_PVP_OPF3_CTL_ISIZE4             0x00000003  /* ISIZE: 4-Bit Input Data Size */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_PEC_CFG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_PEC_CFG_IBLOCK0              8                               /* Input Block ID */
#define BITP_PVP_PEC_CFG_IPORT0               4                               /* Input Port ID */
#define BITP_PVP_PEC_CFG_MPIPE                2                               /* Memory Pipe */
#define BITP_PVP_PEC_CFG_START                0                               /* Start */
#define BITM_PVP_PEC_CFG_IBLOCK0             0x0000FF00  /* Input Block ID */
#define BITM_PVP_PEC_CFG_IPORT0              0x00000030  /* Input Port ID */

#define BITM_PVP_PEC_CFG_MPIPE               0x00000004  /* Memory Pipe */
#define ENUM_PVP_PEC_CFG_CAMPIPE             0x00000000  /* MPIPE: Camera Pipe */
#define ENUM_PVP_PEC_CFG_MEMPIPE             0x00000004  /* MPIPE: Memory Pipe */
#define BITM_PVP_PEC_CFG_START               0x00000001  /* Start */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_PEC_CTL                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_PEC_CTL_IGNTH1               3                               /* Ignore TH1 Threshold for Encoding */
#define BITP_PVP_PEC_CTL_OSIZE                2                               /* Output Data Size per Bin */
#define BITP_PVP_PEC_CTL_ZCRSS                1                               /* Zero Cross */
#define BITP_PVP_PEC_CTL_MODE                 0                               /* Derivative Mode Select */

#define BITM_PVP_PEC_CTL_IGNTH1              0x00000008  /* Ignore TH1 Threshold for Encoding */
#define ENUM_PVP_PEC_CTL_ENCODEDIFF          0x00000000  /* IGNTH1: Different Strong/Weak Edge Encoding */
#define ENUM_PVP_PEC_CTL_ENCODESAME          0x00000008  /* IGNTH1: Identical Strong/Weak Edge Encoding */

#define BITM_PVP_PEC_CTL_OSIZE               0x00000004  /* Output Data Size per Bin */
#define ENUM_PVP_PEC_CTL_BIN8                0x00000000  /* OSIZE: 8 Bits Per Bin PEC Output Data Size */
#define ENUM_PVP_PEC_CTL_BIN16               0x00000004  /* OSIZE: 16 Bits Per Bin PEC Output Data Size */

#define BITM_PVP_PEC_CTL_ZCRSS               0x00000002  /* Zero Cross */
#define ENUM_PVP_PEC_CTL_ANGLE               0x00000000  /* ZCRSS: Angle Indices and Sub-Pixel Values */
#define ENUM_PVP_PEC_CTL_ZEROCROSS           0x00000002  /* ZCRSS: Zero Crossing Codes and Sub-Pixel Values */

#define BITM_PVP_PEC_CTL_MODE                0x00000001  /* Derivative Mode Select */
#define ENUM_PVP_PEC_CTL_DERIV1              0x00000000  /* MODE: 1st Derivative Mode */
#define ENUM_PVP_PEC_CTL_DERIV2              0x00000001  /* MODE: 2nd Derivative Mode */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_PEC_D1TH0                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_PEC_D1TH0_VALUE              0                               /* Lower Hysteresis Threshold */
#define BITM_PVP_PEC_D1TH0_VALUE             0x0000FFFF  /* Lower Hysteresis Threshold */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_PEC_D1TH1                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_PEC_D1TH1_VALUE              0                               /* Upper Hysteresis Threshold */
#define BITM_PVP_PEC_D1TH1_VALUE             0x0000FFFF  /* Upper Hysteresis Threshold */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_PEC_D2TH0                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_PEC_D2TH0_VALUE              0                               /* Weak Zero Crossing Threshold */
#define BITM_PVP_PEC_D2TH0_VALUE             0x00007FFF  /* Weak Zero Crossing Threshold */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_PEC_D2TH1                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_PEC_D2TH1_VALUE              0                               /* Strong Zero Crossing Threshold */
#define BITM_PVP_PEC_D2TH1_VALUE             0x00007FFF  /* Strong Zero Crossing Threshold */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IIM_CFG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IIM_CFG_IBLOCK0              8                               /* Input Block ID */
#define BITP_PVP_IIM_CFG_IPORT0               4                               /* Input Port ID */
#define BITP_PVP_IIM_CFG_MPIPE                2                               /* Memory Pipe */
#define BITP_PVP_IIM_CFG_START                0                               /* Start */
#define BITM_PVP_IIM_CFG_IBLOCK0             0x0000FF00  /* Input Block ID */
#define BITM_PVP_IIM_CFG_IPORT0              0x00000030  /* Input Port ID */

#define BITM_PVP_IIM_CFG_MPIPE               0x00000004  /* Memory Pipe */
#define ENUM_PVP_IIM_CFG_CAMPIPE             0x00000000  /* MPIPE: Camera Pipe */
#define ENUM_PVP_IIM_CFG_MEMPIPE             0x00000004  /* MPIPE: Memory Pipe */
#define BITM_PVP_IIM_CFG_START               0x00000001  /* Start */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IIM_CTL                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IIM_CTL_SHIFT                8                               /* Shift Select */
#define BITP_PVP_IIM_CTL_WIDTH                2                               /* Width Select */
#define BITP_PVP_IIM_CTL_MODE                 0                               /* Mode Select */
#define BITM_PVP_IIM_CTL_SHIFT               0x00001F00  /* Shift Select */

#define BITM_PVP_IIM_CTL_WIDTH               0x0000000C  /* Width Select */
#define ENUM_PVP_IIM_CTL_SINGLE32            0x00000000  /* WIDTH: Single 32 Bit */
#define ENUM_PVP_IIM_CTL_DUAL16              0x00000004  /* WIDTH: Dual 16 Bit */
#define ENUM_PVP_IIM_CTL_QUAD8               0x0000000C  /* WIDTH: Quad 8 Bit */

#define BITM_PVP_IIM_CTL_MODE                0x00000003  /* Mode Select */
#define ENUM_PVP_IIM_CTL_RECTMODE            0x00000000  /* MODE: Rectangular Mode ( SAT) */
#define ENUM_PVP_IIM_CTL_DIAGMODE            0x00000001  /* MODE: Diagonal Mode ( RSAT -45) */
#define ENUM_PVP_IIM_CTL_ROWMODE             0x00000002  /* MODE: Row Mode */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IIM_SCALE                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IIM_SCALE_VSCL              16                               /* Vertical Scaling Factor */
#define BITP_PVP_IIM_SCALE_HSCL               0                               /* Horizontal Scaling Factor */
#define BITM_PVP_IIM_SCALE_VSCL              0x01FF0000  /* Vertical Scaling Factor */
#define BITM_PVP_IIM_SCALE_HSCL              0x000003FF  /* Horizontal Scaling Factor */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IIM_SOVF_STAT                    Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IIM_SOVF_STAT_VPOS          16                               /* Vertical Pixel Coordinate */
#define BITP_PVP_IIM_SOVF_STAT_HPOS           0                               /* Horizontal Pixel Coordinate */
#define BITM_PVP_IIM_SOVF_STAT_VPOS          0x03FF0000  /* Vertical Pixel Coordinate */
#define BITM_PVP_IIM_SOVF_STAT_HPOS          0x000007FF  /* Horizontal Pixel Coordinate */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IIM_UOVF_STAT                    Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IIM_UOVF_STAT_VPOS          16                               /* Vertical Pixel Coordinate */
#define BITP_PVP_IIM_UOVF_STAT_HPOS           0                               /* Horizontal Pixel Coordinate */
#define BITM_PVP_IIM_UOVF_STAT_VPOS          0x03FF0000  /* Vertical Pixel Coordinate */
#define BITM_PVP_IIM_UOVF_STAT_HPOS          0x000007FF  /* Horizontal Pixel Coordinate */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_ACU_CFG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_ACU_CFG_IBLOCK1             16                               /* Input Block 1 ID */
#define BITP_PVP_ACU_CFG_IBLOCK0              8                               /* Input Block 0 ID */
#define BITP_PVP_ACU_CFG_IPORT1               6                               /* Input Port 1 ID */
#define BITP_PVP_ACU_CFG_IPORT0               4                               /* Input Port 0 ID */
#define BITP_PVP_ACU_CFG_MPIPE                2                               /* Memory Pipe */
#define BITP_PVP_ACU_CFG_START                0                               /* Start */
#define BITM_PVP_ACU_CFG_IBLOCK1             0x00FF0000  /* Input Block 1 ID */
#define BITM_PVP_ACU_CFG_IBLOCK0             0x0000FF00  /* Input Block 0 ID */
#define BITM_PVP_ACU_CFG_IPORT1              0x000000C0  /* Input Port 1 ID */
#define BITM_PVP_ACU_CFG_IPORT0              0x00000030  /* Input Port 0 ID */

#define BITM_PVP_ACU_CFG_MPIPE               0x00000004  /* Memory Pipe */
#define ENUM_PVP_ACU_CFG_CAMPIPE             0x00000000  /* MPIPE: Camera Pipe */
#define ENUM_PVP_ACU_CFG_MEMPIPE             0x00000004  /* MPIPE: Memory Pipe */
#define BITM_PVP_ACU_CFG_START               0x00000001  /* Start */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_ACU_CTL                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_ACU_CTL_SUMOP               28                               /* Sum Operation */
#define BITP_PVP_ACU_CTL_SUMISW              27                               /* Sum Input Swap */
#define BITP_PVP_ACU_CTL_SUMINP              24                               /* Sum Inputs for Adder */
#define BITP_PVP_ACU_CTL_PRDOP               20                               /* Prod Operation */
#define BITP_PVP_ACU_CTL_PRDISW              19                               /* Prod Input Swap */
#define BITP_PVP_ACU_CTL_PRDINP              16                               /* Prod Inputs for Mult/Div */
#define BITP_PVP_ACU_CTL_ACCFRAME            15                               /* Accumulator Frame */
#define BITP_PVP_ACU_CTL_ACCINP               8                               /* Accumulator Input */
#define BITP_PVP_ACU_CTL_SFTINP               0                               /* Shift Input */

#define BITM_PVP_ACU_CTL_SUMOP               0x10000000  /* Sum Operation */
#define ENUM_PVP_ACU_CTL_ADD                 0x00000000  /* SUMOP: Add */
#define ENUM_PVP_ACU_CTL_SUBTRACT            0x10000000  /* SUMOP: Subtract */

#define BITM_PVP_ACU_CTL_SUMISW              0x08000000  /* Sum Input Swap */
#define ENUM_PVP_ACU_CTL_NOSWAPSUM           0x00000000  /* SUMISW: Do Not Swap Operands */
#define ENUM_PVP_ACU_CTL_SWAPSUM             0x08000000  /* SUMISW: Swap Operands */

#define BITM_PVP_ACU_CTL_SUMINP              0x03000000  /* Sum Inputs for Adder */
#define ENUM_PVP_ACU_CTL_SUMXY               0x00000000  /* SUMINP: X,Y Inputs */
#define ENUM_PVP_ACU_CTL_SUMXOFF             0x01000000  /* SUMINP: X,OFFSET Inputs */
#define ENUM_PVP_ACU_CTL_SUMYOFF             0x02000000  /* SUMINP: Y,OFFSET Inputs */

#define BITM_PVP_ACU_CTL_PRDOP               0x00300000  /* Prod Operation */
#define ENUM_PVP_ACU_CTL_MULTIPLY            0x00000000  /* PRDOP: Multiply */
#define ENUM_PVP_ACU_CTL_DIVQUOTIENT         0x00100000  /* PRDOP: Divide with Quotient */
#define ENUM_PVP_ACU_CTL_DIVMODULUS          0x00200000  /* PRDOP: Divide with Modulus */

#define BITM_PVP_ACU_CTL_PRDISW              0x00080000  /* Prod Input Swap */
#define ENUM_PVP_ACU_CTL_NOSWAPPROD          0x00000000  /* PRDISW: Do Not Swap Operands */
#define ENUM_PVP_ACU_CTL_SWAPPROD            0x00080000  /* PRDISW: Swap Operands */

#define BITM_PVP_ACU_CTL_PRDINP              0x00030000  /* Prod Inputs for Mult/Div */
#define ENUM_PVP_ACU_CTL_PRODXY              0x00000000  /* PRDINP: X,Y Inputs */
#define ENUM_PVP_ACU_CTL_PRODXFACT           0x00010000  /* PRDINP: X,FACTOR Inputs */
#define ENUM_PVP_ACU_CTL_PRODYFACT           0x00020000  /* PRDINP: Y,FACTOR Inputs */
#define ENUM_PVP_ACU_CTL_PRODSUMFACT         0x00030000  /* PRDINP: SUM,FACTOR Inputs */

#define BITM_PVP_ACU_CTL_ACCFRAME            0x00008000  /* Accumulator Frame */
#define ENUM_PVP_ACU_CTL_ACCUMROW            0x00000000  /* ACCFRAME: Clear ACC After Row */
#define ENUM_PVP_ACU_CTL_ACCUMFRAME          0x00008000  /* ACCFRAME: Clear ACC After Frame */

#define BITM_PVP_ACU_CTL_ACCINP              0x00000300  /* Accumulator Input */
#define ENUM_PVP_ACU_CTL_ACCUMX              0x00000000  /* ACCINP: X Input */
#define ENUM_PVP_ACU_CTL_ACCUMSUM            0x00000100  /* ACCINP: SUM Input */
#define ENUM_PVP_ACU_CTL_ACCUMPROD           0x00000200  /* ACCINP: PROD Input */

#define BITM_PVP_ACU_CTL_SFTINP              0x00000003  /* Shift Input */
#define ENUM_PVP_ACU_CTL_SHIFTXIN            0x00000000  /* SFTINP: X Input */
#define ENUM_PVP_ACU_CTL_SHIFTSUM            0x00000001  /* SFTINP: SUM Result Input */
#define ENUM_PVP_ACU_CTL_SHIFTPROD           0x00000002  /* SFTINP: PROD Result Input */
#define ENUM_PVP_ACU_CTL_SHIFTACC            0x00000003  /* SFTINP: ACC Result Input */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_ACU_SHIFT                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_ACU_SHIFT_VALUE              0                               /* SHIFT Constant */
#define BITM_PVP_ACU_SHIFT_VALUE             0x0000003F  /* SHIFT Constant */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_UDS_CFG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_UDS_CFG_IBLOCK0              8                               /* Input Block ID */
#define BITP_PVP_UDS_CFG_IPORT0               4                               /* Input Port ID */
#define BITP_PVP_UDS_CFG_MPIPE                2                               /* Memory Pipe */
#define BITP_PVP_UDS_CFG_START                0                               /* Start */
#define BITM_PVP_UDS_CFG_IBLOCK0             0x0000FF00  /* Input Block ID */
#define BITM_PVP_UDS_CFG_IPORT0              0x00000030  /* Input Port ID */
#define BITM_PVP_UDS_CFG_MPIPE               0x00000004  /* Memory Pipe */
#define BITM_PVP_UDS_CFG_START               0x00000001  /* Start */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_UDS_CTL                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_UDS_CTL_AAVG                 0                               /* Automatic Averaging */

#define BITM_PVP_UDS_CTL_AAVG                0x00000001  /* Automatic Averaging */
#define ENUM_PVP_UDS_CTL_MANTAPS             0x00000000  /* AAVG: Manual Filter Tap Selection */
#define ENUM_PVP_UDS_CTL_AUTOTAPS            0x00000001  /* AAVG: Auto Filter Tap Selection */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_UDS_OHCNT                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_UDS_OHCNT_VALUE              4                               /* H Dimension of Output Frame */
#define BITM_PVP_UDS_OHCNT_VALUE             0x000000F0  /* H Dimension of Output Frame */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_UDS_OVCNT                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_UDS_OVCNT_VALUE              4                               /* V Dimension of Output Frame */
#define BITM_PVP_UDS_OVCNT_VALUE             0x000000F0  /* V Dimension of Output Frame */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_UDS_HAVG                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_UDS_HAVG_VALUE               0                               /* H Filter Taps */
#define BITM_PVP_UDS_HAVG_VALUE              0x000000FF  /* H Filter Taps */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_UDS_VAVG                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_UDS_VAVG_VALUE               0                               /* V Filter Taps */
#define BITM_PVP_UDS_VAVG_VALUE              0x0000007F  /* V Filter Taps */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF0_CFG                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF0_CFG_STATWCNT           24                               /* Camera Pipe DMA Status */
#define BITP_PVP_IPF0_CFG_MPIPE               2                               /* Memory Pipe */
#define BITP_PVP_IPF0_CFG_START               0                               /* Start */
#define BITM_PVP_IPF0_CFG_STATWCNT           0xFF000000  /* Camera Pipe DMA Status */
#define BITM_PVP_IPF0_CFG_MPIPE              0x00000004  /* Memory Pipe */
#define BITM_PVP_IPF0_CFG_START              0x00000001  /* Start */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF_PIPECTL                      Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF_PIPECTL_STATEN           4                               /* DMA Status Enable */
#define BITP_PVP_IPF_PIPECTL_DRAIN            0                               /* Drain Enable */
#define BITM_PVP_IPF_PIPECTL_STATEN          0x00000010  /* DMA Status Enable */
#define BITM_PVP_IPF_PIPECTL_DRAIN           0x00000001  /* Drain Enable */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF_CTL                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF_CTL_BP0                 30                               /* Blue on Port 0 */
#define BITP_PVP_IPF_CTL_BFRMT1              29                               /* Bayer special format, type select */
#define BITP_PVP_IPF_CTL_BFRMT0              28                               /* Bayer special format, extraction enable */
#define BITP_PVP_IPF_CTL_QFRMT               27                               /* Q Format Correction */
#define BITP_PVP_IPF_CTL_SIGNEXT             26                               /* Sign Extend */
#define BITP_PVP_IPF_CTL_EXTRED              25                               /* Extract Red/Green */
#define BITP_PVP_IPF_CTL_UNPACK              24                               /* Unpack Incoming */
#define BITP_PVP_IPF_CTL_CFRMT               16                               /* Color Space Format */
#define BITP_PVP_IPF_CTL_OPORT2EN            12                               /* Output Port 2 Enable */
#define BITP_PVP_IPF_CTL_OPORT1EN             8                               /* Output Port 1 Enable */
#define BITP_PVP_IPF_CTL_OPORT0EN             4                               /* Output Port 0 Enable */
#define BITM_PVP_IPF_CTL_BP0                 0x40000000  /* Blue on Port 0 */
#define BITM_PVP_IPF_CTL_BFRMT1              0x20000000  /* Bayer special format, type select */
#define PVP_IPF_CTL_BFRMT1_TYPE1             0x00000000  /* Selects Type-1 Bayer format */
#define PVP_IPF_CTL_BFRMT1_TYPE2             0x20000000  /* Selects Type-2 Bayer format */
#define BITM_PVP_IPF_CTL_BFRMT0              0x10000000  /* Bayer special format, extraction enable */

#define BITM_PVP_IPF_CTL_QFRMT               0x08000000  /* Q Format Correction */
#define ENUM_PVP_IPF_CTL_NOQFRMT             0x00000000  /* QFRMT: Disable Q Format Correction */
#define ENUM_PVP_IPF_CTL_QFRMT               0x08000000  /* QFRMT: Enable Q Format Correction */

#define BITM_PVP_IPF_CTL_SIGNEXT             0x04000000  /* Sign Extend */
#define ENUM_PVP_IPF_CTL_ZEROEXT             0x00000000  /* SIGNEXT: Zero Extend */
#define ENUM_PVP_IPF_CTL_SIGNEXT             0x04000000  /* SIGNEXT: Sign Extend */

#define BITM_PVP_IPF_CTL_EXTRED              0x02000000  /* Extract Red/Green */
#define ENUM_PVP_IPF_CTL_EXTGREEN            0x00000000  /* EXTRED: Extract Green */
#define ENUM_PVP_IPF_CTL_EXTRED              0x02000000  /* EXTRED: Extract Red */

#define BITM_PVP_IPF_CTL_UNPACK              0x01000000  /* Unpack Incoming */
#define ENUM_PVP_IPF_CTL_UNPACKDIS           0x00000000  /* UNPACK: No Unpacking */
#define ENUM_PVP_IPF_CTL_UNPACKEN            0x01000000  /* UNPACK: Unpack Data */

#define BITM_PVP_IPF_CTL_CFRMT               0x001F0000  /* Color Space Format */
#define ENUM_PVP_IPF_CTL_RGB8                0x00000000  /* CFRMT: RGB 8-Bit */
#define ENUM_PVP_IPF_CTL_RGB888              0x00010000  /* CFRMT: RGB 888 */
#define ENUM_PVP_IPF_CTL_YUV8                0x00100000  /* CFRMT: YUV 4:2:2 8-Bit Type 1 */
#define ENUM_PVP_IPF_CTL_YUV8SPLT            0x00110000  /* CFRMT: YUV 4:2:2 8-Bit Type 2 */
#define ENUM_PVP_IPF_CTL_YUV8SUBSPLT         0x00120000  /* CFRMT: YUV 4:2:2 8-Bit Type 3 */
#define ENUM_PVP_IPF_CTL_YUV8IN16            0x00130000  /* CFRMT: YUV 4:2:2 8-Bit Pair 16-Bit */
#define ENUM_PVP_IPF_CTL_RGB565              0x00020000  /* CFRMT: RGB 565 */
#define ENUM_PVP_IPF_CTL_YUV16               0x00140000  /* CFRMT: YUV 4:2:2 16-Bit Type 1 */
#define ENUM_PVP_IPF_CTL_YUV16SPLT           0x00150000  /* CFRMT: YUV 4:2:2 16-Bit Type 2 */
#define ENUM_PVP_IPF_CTL_YUV16SUBSPLT        0x00160000  /* CFRMT: YUV 4:2:2 16-Bit Type 3 */
#define ENUM_PVP_IPF_CTL_Y8                  0x00180000  /* CFRMT: Y Alone 8-Bit */
#define ENUM_PVP_IPF_CTL_Y16                 0x00190000  /* CFRMT: Y Alone 16-Bit */
#define ENUM_PVP_IPF_CTL_Y24                 0x001A0000  /* CFRMT: Y Alone 24-Bit */
#define ENUM_PVP_IPF_CTL_WORD32              0x001B0000  /* CFRMT: 32 Bit */
#define ENUM_PVP_IPF_CTL_RGB666              0x00030000  /* CFRMT: RGB 666 */
#define ENUM_PVP_IPF_CTL_RGB16               0x00040000  /* CFRMT: RGB 16-Bit */
#define ENUM_PVP_IPF_CTL_BAYER1              0x00050000  /* CFRMT: RGB Bayer Format Type-1 */
#define ENUM_PVP_IPF_CTL_BAYER2              0x00060000  /* CFRMT: RGB Bayer Format Type-2 */

#define BITM_PVP_IPF_CTL_OPORT2EN            0x00001000  /* Output Port 2 Enable */
#define ENUM_PVP_IPF_CTL_OPORT2DIS           0x00000000  /* OPORT2EN: Disable OPORT2 */
#define ENUM_PVP_IPF_CTL_OPORT2EVEN          0x00001000  /* OPORT2EN: Enable OPORT2 (full resolution) */

#define BITM_PVP_IPF_CTL_OPORT1EN            0x00000300  /* Output Port 1 Enable */
#define ENUM_PVP_IPF_CTL_OPORT1DIS           0x00000000  /* OPORT1EN: Disable OPORT1 */
#define ENUM_PVP_IPF_CTL_OPORT1ODD           0x00000100  /* OPORT1EN: Enable OPORT1 (full resolution) */
#define ENUM_PVP_IPF_CTL_OPORT1WIN           0x00000200  /* OPORT1EN: Enable OPORT1 (windowed resolution) */

#define BITM_PVP_IPF_CTL_OPORT0EN            0x00000010  /* Output Port 0 Enable */
#define ENUM_PVP_IPF_CTL_OPORT0DIS           0x00000000  /* OPORT0EN: Disable OPORT0 */
#define ENUM_PVP_IPF_CTL_OPORT0EN            0x00000010  /* OPORT0EN: Enable OPORT0 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF_TAG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF_TAG_VALUE                0                               /* TAG Value */
#define BITM_PVP_IPF_TAG_VALUE               0x0000FFFF  /* TAG Value */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF_HCNT                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF_HCNT_VALUE               0                               /* Effective Width of ROI */
#define BITM_PVP_IPF_HCNT_VALUE              0x0000FFFF  /* Effective Width of ROI */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF_VCNT                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF_VCNT_VALUE               0                               /* Effective Height of ROI */
#define BITM_PVP_IPF_VCNT_VALUE              0x0000FFFF  /* Effective Height of ROI */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF0_HPOS                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF0_HPOS_VALUE              0                               /* Horizontal Delay of ROI */
#define BITM_PVP_IPF0_HPOS_VALUE             0x0000FFFF  /* Horizontal Delay of ROI */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF0_VPOS                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF0_VPOS_VALUE              0                               /* Vertical Delay of ROI */
#define BITM_PVP_IPF0_VPOS_VALUE             0x0000FFFF  /* Vertical Delay of ROI */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF_TAG_STAT                     Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF_TAG_STAT_VALUE           0                               /* TAG Value */
#define BITM_PVP_IPF_TAG_STAT_VALUE          0x0000FFFF  /* TAG Value */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_IPF1_CFG                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_IPF1_CFG_STATWCNT           24                               /* Status Word Count */
#define BITP_PVP_IPF1_CFG_MPIPE               2                               /* Memory Pipe */
#define BITP_PVP_IPF1_CFG_START               0                               /* Start */
#define BITM_PVP_IPF1_CFG_STATWCNT           0xFF000000  /* Status Word Count */
#define BITM_PVP_IPF1_CFG_MPIPE              0x00000004  /* Memory Pipe */
#define BITM_PVP_IPF1_CFG_START              0x00000001  /* Start */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_CFG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_CFG_IBLOCK0              8                               /* Input Block ID */
#define BITP_PVP_CNV_CFG_IPORT0               4                               /* Input Port ID */
#define BITP_PVP_CNV_CFG_MPIPE                2                               /* Memory Pipe */
#define BITP_PVP_CNV_CFG_START                0                               /* Start */
#define BITM_PVP_CNV_CFG_IBLOCK0             0x0000FF00  /* Input Block ID */
#define BITM_PVP_CNV_CFG_IPORT0              0x00000030  /* Input Port ID */

#define BITM_PVP_CNV_CFG_MPIPE               0x00000004  /* Memory Pipe */
#define ENUM_PVP_CNV_CFG_CAMPIPE             0x00000000  /* MPIPE: Camera Pipe */
#define ENUM_PVP_CNV_CFG_MEMPIPE             0x00000004  /* MPIPE: Memory Pipe */
#define BITM_PVP_CNV_CFG_START               0x00000001  /* Start */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_CTL                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#if defined(__SILICON_REVISION__) && (__SILICON_REVISION__ >= 1)
#define BITP_PVP_CNV_CTL_RFRMT1              13                               /* RCCC format, Bayer type select */
#endif
#if defined(__SILICON_REVISION__) && (__SILICON_REVISION__ >= 1)
#define BITP_PVP_CNV_CTL_RFRMT0              12                               /* RCCC format, convolution enable */
#endif
#define BITP_PVP_CNV_CTL_SHIFT                4                               /* Shift Right */
#define BITP_PVP_CNV_CTL_ZEROFILL             1                               /* Zero Fill */
#define BITP_PVP_CNV_CTL_SAT32                0                               /* Saturate Output to 32 Bits */
#if defined(__SILICON_REVISION__) && (__SILICON_REVISION__ >= 1)
#define BITM_PVP_CNV_CTL_RFRMT1              0x00002000  /* RCCC format, Bayer type select */
#endif
#if defined(__SILICON_REVISION__) && (__SILICON_REVISION__ >= 1)
#define BITM_PVP_CNV_CTL_RFRMT0              0x00001000  /* RCCC format, convolution enable */
#endif
#define BITM_PVP_CNV_CTL_SHIFT               0x000001F0  /* Shift Right */

#define BITM_PVP_CNV_CTL_ZEROFILL            0x00000002  /* Zero Fill */
#define ENUM_PVP_CNV_CTL_EDGEDUP             0x00000000  /* ZEROFILL: Duplicated Data Fill */
#define ENUM_PVP_CNV_CTL_EDGEZFILL           0x00000002  /* ZEROFILL: Zero Fill */

#define BITM_PVP_CNV_CTL_SAT32               0x00000001  /* Saturate Output to 32 Bits */
#define ENUM_PVP_CNV_CTL_SIGNEXT             0x00000000  /* SAT32: 16-Bit Saturate of Output */
#define ENUM_PVP_CNV_CTL_SAT32               0x00000001  /* SAT32: 32-Bit Saturate of Output */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C00C01                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C00C01_C01              16                               /* Coefficient 0, 1 */
#define BITP_PVP_CNV_C00C01_C00               0                               /* Coefficient 0, 0 */
#define BITM_PVP_CNV_C00C01_C01              0xFFFF0000  /* Coefficient 0, 1 */
#define BITM_PVP_CNV_C00C01_C00              0x0000FFFF  /* Coefficient 0, 0 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C02C03                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C02C03_C03              16                               /* Coefficient 0, 3 */
#define BITP_PVP_CNV_C02C03_C02               0                               /* Coefficient 0, 2 */
#define BITM_PVP_CNV_C02C03_C03              0xFFFF0000  /* Coefficient 0, 3 */
#define BITM_PVP_CNV_C02C03_C02              0x0000FFFF  /* Coefficient 0, 2 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C04                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C04_C04                  0                               /* Coefficient 0, 4 */
#define BITM_PVP_CNV_C04_C04                 0x0000FFFF  /* Coefficient 0, 4 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C10C11                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C10C11_C11              16                               /* Coefficient 1, 1 */
#define BITP_PVP_CNV_C10C11_C10               0                               /* Coefficient 1, 0 */
#define BITM_PVP_CNV_C10C11_C11              0xFFFF0000  /* Coefficient 1, 1 */
#define BITM_PVP_CNV_C10C11_C10              0x0000FFFF  /* Coefficient 1, 0 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C12C13                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C12C13_C13              16                               /* Coefficient 1, 3 */
#define BITP_PVP_CNV_C12C13_C12               0                               /* Coefficient 1, 2 */
#define BITM_PVP_CNV_C12C13_C13              0xFFFF0000  /* Coefficient 1, 3 */
#define BITM_PVP_CNV_C12C13_C12              0x0000FFFF  /* Coefficient 1, 2 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C14                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C14_C14                  0                               /* Coefficient 1, 4 */
#define BITM_PVP_CNV_C14_C14                 0x0000FFFF  /* Coefficient 1, 4 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C20C21                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C20C21_C21              16                               /* Coefficient 2, 1 */
#define BITP_PVP_CNV_C20C21_C20               0                               /* Coefficient 2, 0 */
#define BITM_PVP_CNV_C20C21_C21              0xFFFF0000  /* Coefficient 2, 1 */
#define BITM_PVP_CNV_C20C21_C20              0x0000FFFF  /* Coefficient 2, 0 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C22C23                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C22C23_C23              16                               /* Coefficient 2, 3 */
#define BITP_PVP_CNV_C22C23_C22               0                               /* Coefficient 2, 2 */
#define BITM_PVP_CNV_C22C23_C23              0xFFFF0000  /* Coefficient 2, 3 */
#define BITM_PVP_CNV_C22C23_C22              0x0000FFFF  /* Coefficient 2, 2 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C24                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C24_C24                  0                               /* Coefficient 2, 4 */
#define BITM_PVP_CNV_C24_C24                 0x0000FFFF  /* Coefficient 2, 4 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C30C31                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C30C31_C31              16                               /* Coefficient 3, 1 */
#define BITP_PVP_CNV_C30C31_C30               0                               /* Coefficient 3, 0 */
#define BITM_PVP_CNV_C30C31_C31              0xFFFF0000  /* Coefficient 3, 1 */
#define BITM_PVP_CNV_C30C31_C30              0x0000FFFF  /* Coefficient 3, 0 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C32C33                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C32C33_C33              16                               /* Coefficient 3, 3 */
#define BITP_PVP_CNV_C32C33_C32               0                               /* Coefficient 3, 2 */
#define BITM_PVP_CNV_C32C33_C33              0xFFFF0000  /* Coefficient 3, 3 */
#define BITM_PVP_CNV_C32C33_C32              0x0000FFFF  /* Coefficient 3, 2 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C34                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C34_C34                  0                               /* Coefficient 3, 4 */
#define BITM_PVP_CNV_C34_C34                 0x0000FFFF  /* Coefficient 3, 4 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C40C41                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C40C41_C41              16                               /* Coefficient 4, 1 */
#define BITP_PVP_CNV_C40C41_C40               0                               /* Coefficient 4, 0 */
#define BITM_PVP_CNV_C40C41_C41              0xFFFF0000  /* Coefficient 4, 1 */
#define BITM_PVP_CNV_C40C41_C40              0x0000FFFF  /* Coefficient 4, 0 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C42C43                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C42C43_C43              16                               /* Coefficient 4, 3 */
#define BITP_PVP_CNV_C42C43_C42               0                               /* Coefficient 4, 2 */
#define BITM_PVP_CNV_C42C43_C43              0xFFFF0000  /* Coefficient 4, 3 */
#define BITM_PVP_CNV_C42C43_C42              0x0000FFFF  /* Coefficient 4, 2 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_C44                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_C44_C44                  0                               /* Coefficient 4, 4 */
#define BITM_PVP_CNV_C44_C44                 0x0000FFFF  /* Coefficient 4, 4 */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_CNV_SCALE                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_CNV_SCALE_VSCL              16                               /* Vertical Scaling factor */
#define BITP_PVP_CNV_SCALE_HSCL               0                               /* Horizontal Scaling factor */
#define BITM_PVP_CNV_SCALE_VSCL              0x01FF0000  /* Vertical Scaling factor */
#define BITM_PVP_CNV_SCALE_HSCL              0x000003FF  /* Horizontal Scaling factor */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_THC_CFG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_THC_CFG_STATWCNT            24                               /* Status Word Count */
#define BITP_PVP_THC_CFG_IBLOCK0              8                               /* Input Block ID */
#define BITP_PVP_THC_CFG_IPORT0               4                               /* Input Port ID */
#define BITP_PVP_THC_CFG_MPIPE                2                               /* Memory Pipe */
#define BITP_PVP_THC_CFG_START                0                               /* Start */
#define BITM_PVP_THC_CFG_STATWCNT            0xFF000000  /* Status Word Count */
#define BITM_PVP_THC_CFG_IBLOCK0             0x0000FF00  /* Input Block ID */
#define BITM_PVP_THC_CFG_IPORT0              0x00000030  /* Input Port ID */

#define BITM_PVP_THC_CFG_MPIPE               0x00000004  /* Memory Pipe */
#define ENUM_PVP_THC_CFG_CAMPIPE             0x00000000  /* MPIPE: Camera Pipe */
#define ENUM_PVP_THC_CFG_MEMPIPE             0x00000004  /* MPIPE: Memory Pipe */
#define BITM_PVP_THC_CFG_START               0x00000001  /* Start */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_THC_CTL                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_THC_CTL_HISTEN              16                               /* Histogram Counters Enable */
#define BITP_PVP_THC_CTL_RLEWM               11                               /* Run-length Encoding Window Mode */
#define BITP_PVP_THC_CTL_HISTWM               9                               /* Histogram Window Mode */
#define BITP_PVP_THC_CTL_RLEFRAME             8                               /* Run-Length-Encode Frame */
#define BITP_PVP_THC_CTL_OFRMT                4                               /* Output Format */
#define BITP_PVP_THC_CTL_ZEXT                 2                               /* Zero Extend */
#define BITP_PVP_THC_CTL_MODE                 0                               /* Mode */

#define BITM_PVP_THC_CTL_HISTEN              0x00010000  /* Histogram Counters Enable */
#define ENUM_PVP_THC_CTL_HISTDIS             0x00000000  /* HISTEN: Disable */
#define ENUM_PVP_THC_CTL_HISTEN              0x00010000  /* HISTEN: Enable */

#define BITM_PVP_THC_CTL_RLEWM               0x00001800  /* Run-length Encoding Window Mode */
#define ENUM_PVP_THC_CTL_COMPFRAME           0x00000000  /* RLEWM: Frame Compression */
#define ENUM_PVP_THC_CTL_COMPWIN             0x00000800  /* RLEWM: Window Compression */

#define BITM_PVP_THC_CTL_HISTWM              0x00000600  /* Histogram Window Mode */
#define ENUM_PVP_THC_CTL_HISTFRAME           0x00000000  /* HISTWM: Frame Histogram */
#define ENUM_PVP_THC_CTL_HISTWIN             0x00000200  /* HISTWM: Inside-Window Histogram */
#define ENUM_PVP_THC_CTL_HISTOUTWIN          0x00000400  /* HISTWM: Outside-Window Histogram */

#define BITM_PVP_THC_CTL_RLEFRAME            0x00000100  /* Run-Length-Encode Frame */
#define ENUM_PVP_THC_CTL_RLELINE             0x00000000  /* RLEFRAME: Row (Line) Compression */
#define ENUM_PVP_THC_CTL_RLEFRAME            0x00000100  /* RLEFRAME: Frame Compression */

#define BITM_PVP_THC_CTL_OFRMT               0x000000F0  /* Output Format */
#define ENUM_PVP_THC_CTL_WORD32              0x00000000  /* OFRMT: 32-Bit Word ( No Compression ) */
#define ENUM_PVP_THC_CTL_NODATA              0x000000A0  /* OFRMT: Disable Output/RLE */
#define ENUM_PVP_THC_CTL_INDX4               0x00000020  /* OFRMT: 4-Bit Index ( No Compression) */
#define ENUM_PVP_THC_CTL_INDX4RL4            0x00000030  /* OFRMT: 4-Bit Index / 4-Bit Run Length */
#define ENUM_PVP_THC_CTL_INDX4ANGL4          0x00000040  /* OFRMT: 4-Bit Index / 4-Bit angle ( No Compression) */
#define ENUM_PVP_THC_CTL_INDX3RL5            0x00000050  /* OFRMT: 3-Bit Index / 5-Bit Run Length */
#define ENUM_PVP_THC_CTL_INDX4RL12           0x00000060  /* OFRMT: 4-Bit Index / 12-Bit Run Length */
#define ENUM_PVP_THC_CTL_INDX3RL13           0x00000070  /* OFRMT: 3-Bit Index / 13-Bit Run Length */
#define ENUM_PVP_THC_CTL_INDX4RL21           0x00000080  /* OFRMT: 4-Bit Index / 21-Bit Run Length */
#define ENUM_PVP_THC_CTL_WORD16RL16          0x00000090  /* OFRMT: 16-Bit Word / 16-Bit Run Length */

#define BITM_PVP_THC_CTL_ZEXT                0x00000004  /* Zero Extend */
#define ENUM_PVP_THC_CTL_ZEXTDIS             0x00000000  /* ZEXT: No Zero Extension */
#define ENUM_PVP_THC_CTL_ZEXTEN              0x00000004  /* ZEXT: Zero Extend */

#define BITM_PVP_THC_CTL_MODE                0x00000003  /* Mode */
#define ENUM_PVP_THC_CTL_CLIPMODE            0x00000000  /* MODE: Clipping/Saturation Mode */
#define ENUM_PVP_THC_CTL_QUANTMODE           0x00000001  /* MODE: Quantization Mode */
#define ENUM_PVP_THC_CTL_HYSTMODE            0x00000002  /* MODE: Hysteresis Mode */

/* ------------------------------------------------------------------------------------------------------------------------
        PVP_PMA_CFG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_PVP_PMA_CFG_IBLOCK1             16                               /* Input Block 1 ID */
#define BITP_PVP_PMA_CFG_IBLOCK0              8                               /* Input Block 0 ID */
#define BITP_PVP_PMA_CFG_IPORT1               6                               /* Input Port 1 ID */
#define BITP_PVP_PMA_CFG_IPORT0               4                               /* Input Port 0 ID */
#define BITP_PVP_PMA_CFG_MPIPE                2                               /* Memory Pipe */
#define BITP_PVP_PMA_CFG_START                0                               /* Start */
#define BITM_PVP_PMA_CFG_IBLOCK1             0x00FF0000  /* Input Block 1 ID */
#define BITM_PVP_PMA_CFG_IBLOCK0             0x0000FF00  /* Input Block 0 ID */
#define BITM_PVP_PMA_CFG_IPORT1              0x000000C0  /* Input Port 1 ID */
#define BITM_PVP_PMA_CFG_IPORT0              0x00000030  /* Input Port 0 ID */

#define BITM_PVP_PMA_CFG_MPIPE               0x00000004  /* Memory Pipe */
#define ENUM_PVP_PMA_CFG_CAMPIPE             0x00000000  /* MPIPE: Camera Pipe */
#define ENUM_PVP_PMA_CFG_MEMPIPE             0x00000004  /* MPIPE: Memory Pipe */
#define BITM_PVP_PMA_CFG_START               0x00000001  /* Start */

/* ==================================================
        System Debug Unit Registers
   ================================================== */

/* =========================
        SDU0
   ========================= */
#define REG_SDU0_IDCODE                 0xFFC1F020         /* SDU0 ID Code Register */
#define REG_SDU0_CTL                    0xFFC1F050         /* SDU0 Control Register */
#define REG_SDU0_STAT                   0xFFC1F054         /* SDU0 Status Register */
#define REG_SDU0_MACCTL                 0xFFC1F058         /* SDU0 Memory Access Control Register */
#define REG_SDU0_MACADDR                0xFFC1F05C         /* SDU0 Memory Access Address Register */
#define REG_SDU0_MACDATA                0xFFC1F060         /* SDU0 Memory Access Data Register */
#define REG_SDU0_DMARD                  0xFFC1F064         /* SDU0 DMA Read Data Register */
#define REG_SDU0_DMAWD                  0xFFC1F068         /* SDU0 DMA Write Data Register */
#define REG_SDU0_MSG                    0xFFC1F080         /* SDU0 Message Register */
#define REG_SDU0_MSG_SET                0xFFC1F084         /* SDU0 Message Set Register */
#define REG_SDU0_MSG_CLR                0xFFC1F088         /* SDU0 Message Clear Register */
#define REG_SDU0_GHLT                   0xFFC1F08C         /* SDU0 Group Halt Register */

/* =========================
        SDU
   ========================= */
/* ------------------------------------------------------------------------------------------------------------------------
        SDU_IDCODE                           Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_SDU_IDCODE_REVID                28                               /* Revision ID */
#define BITP_SDU_IDCODE_PRID                 12                               /* Product ID */
#define BITP_SDU_IDCODE_MFID                  1                               /* Manufacturer ID */
#define BITM_SDU_IDCODE_REVID                (_ADI_MSK(0xF0000000,uint32_t))  /* Revision ID */
#define BITM_SDU_IDCODE_PRID                 (_ADI_MSK(0x0FFFF000,uint32_t))  /* Product ID */
#define BITM_SDU_IDCODE_MFID                 (_ADI_MSK(0x00000FFE,uint32_t))  /* Manufacturer ID */

/* ------------------------------------------------------------------------------------------------------------------------
        SDU_CTL                              Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_SDU_CTL_EHLT                     8                               /* Emulator Halt Select */
#define BITP_SDU_CTL_EMEEN                    4                               /* Emulation Event Enable */
#define BITP_SDU_CTL_DMAEN                    2                               /* DMA Enable */
#define BITP_SDU_CTL_CSPEN                    1                               /* Core Scan Path Enable */
#define BITP_SDU_CTL_SYSRST                   0                               /* System Reset */
#define BITM_SDU_CTL_EHLT                    (_ADI_MSK(0x0000FF00,uint32_t))  /* Emulator Halt Select */
#define BITM_SDU_CTL_EMEEN                   (_ADI_MSK(0x00000010,uint32_t))  /* Emulation Event Enable */
#define BITM_SDU_CTL_DMAEN                   (_ADI_MSK(0x00000004,uint32_t))  /* DMA Enable */
#define BITM_SDU_CTL_CSPEN                   (_ADI_MSK(0x00000002,uint32_t))  /* Core Scan Path Enable */
#define BITM_SDU_CTL_SYSRST                  (_ADI_MSK(0x00000001,uint32_t))  /* System Reset */

/* ------------------------------------------------------------------------------------------------------------------------
        SDU_STAT                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_SDU_STAT_CRST                   22                               /* Core Reset */
#define BITP_SDU_STAT_CHLT                   21                               /* Core Halt */
#define BITP_SDU_STAT_EME                    20                               /* Emulation Event */
#define BITP_SDU_STAT_GHLTC                  17                               /* Group Halt Cause */
#define BITP_SDU_STAT_GHLT                   16                               /* Group Halt */
#define BITP_SDU_STAT_DMAFIFO                12                               /* DMA FIFO */
#define BITP_SDU_STAT_ADDRERR                11                               /* Address Error */
#define BITP_SDU_STAT_DMAWDRDY               10                               /* DMAWD Ready */
#define BITP_SDU_STAT_DMARDRDY                9                               /* DMARD Ready */
#define BITP_SDU_STAT_MACRDY                  8                               /* MAC Ready */
#define BITP_SDU_STAT_ERRC                    4                               /* Error Cause */
#define BITP_SDU_STAT_SECURE                  3                               /* Secure Mode */
#define BITP_SDU_STAT_DEEPSLEEP               2                               /* Deep Sleep Mode */
#define BITP_SDU_STAT_ERR                     1                               /* Error */
#define BITP_SDU_STAT_SYSRST                  0                               /* System Reset */
#define BITM_SDU_STAT_CRST                   (_ADI_MSK(0x00400000,uint32_t))  /* Core Reset */
#define BITM_SDU_STAT_CHLT                   (_ADI_MSK(0x00200000,uint32_t))  /* Core Halt */
#define BITM_SDU_STAT_EME                    (_ADI_MSK(0x00100000,uint32_t))  /* Emulation Event */
#define BITM_SDU_STAT_GHLTC                  (_ADI_MSK(0x000E0000,uint32_t))  /* Group Halt Cause */
#define BITM_SDU_STAT_GHLT                   (_ADI_MSK(0x00010000,uint32_t))  /* Group Halt */
#define BITM_SDU_STAT_DMAFIFO                (_ADI_MSK(0x00007000,uint32_t))  /* DMA FIFO */
#define BITM_SDU_STAT_ADDRERR                (_ADI_MSK(0x00000800,uint32_t))  /* Address Error */
#define BITM_SDU_STAT_DMAWDRDY               (_ADI_MSK(0x00000400,uint32_t))  /* DMAWD Ready */
#define BITM_SDU_STAT_DMARDRDY               (_ADI_MSK(0x00000200,uint32_t))  /* DMARD Ready */
#define BITM_SDU_STAT_MACRDY                 (_ADI_MSK(0x00000100,uint32_t))  /* MAC Ready */
#define BITM_SDU_STAT_ERRC                   (_ADI_MSK(0x000000F0,uint32_t))  /* Error Cause */
#define BITM_SDU_STAT_SECURE                 (_ADI_MSK(0x00000008,uint32_t))  /* Secure Mode */
#define BITM_SDU_STAT_DEEPSLEEP              (_ADI_MSK(0x00000004,uint32_t))  /* Deep Sleep Mode */
#define BITM_SDU_STAT_ERR                    (_ADI_MSK(0x00000002,uint32_t))  /* Error */
#define BITM_SDU_STAT_SYSRST                 (_ADI_MSK(0x00000001,uint32_t))  /* System Reset */

/* ------------------------------------------------------------------------------------------------------------------------
        SDU_MACCTL                           Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_SDU_MACCTL_AUTOINC               4                               /* Auto (Post) Increment MACADDR (by SIZE) */
#define BITP_SDU_MACCTL_RNW                   3                               /* Read Not Write */
#define BITP_SDU_MACCTL_SIZE                  0                               /* Transfer Data Size */
#define BITM_SDU_MACCTL_AUTOINC              (_ADI_MSK(0x00000010,uint32_t))  /* Auto (Post) Increment MACADDR (by SIZE) */
#define BITM_SDU_MACCTL_RNW                  (_ADI_MSK(0x00000008,uint32_t))  /* Read Not Write */
#define BITM_SDU_MACCTL_SIZE                 (_ADI_MSK(0x00000007,uint32_t))  /* Transfer Data Size */

/* ------------------------------------------------------------------------------------------------------------------------
        SDU_MSG                              Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_SDU_MSG_CALLERR                 31                               /* Flag Set by the Boot Code Prior to an Error Call */
#define BITP_SDU_MSG_CALLBACK                30                               /* Flag Set by the Boot Code Prior to a Callback Call */
#define BITP_SDU_MSG_CALLINIT                29                               /* Flag Set by the Boot Code Prior to an Initcode Call */
#define BITP_SDU_MSG_CALLAPP                 28                               /* Flag Set by the Boot Code Prior to an Application Call */
#define BITP_SDU_MSG_HALTONERR               27                               /* Generate an Emulation Exception Prior to an Error Call */
#define BITP_SDU_MSG_HALTONCALL              26                               /* Generate an Emulation Exception Prior to a Callback Call */
#define BITP_SDU_MSG_HALTONINIT              25                               /* Generate an Emulation Exception Prior to an Initcode Call */
#define BITP_SDU_MSG_HALTONAPP               24                               /* Generate an Emulation Exception Prior to an Application Call */
#define BITP_SDU_MSG_L3INIT                  23                               /* Indicates that the L3 Resource is Initialized */
#define BITP_SDU_MSG_L2INIT                  22                               /* Indicates that the L2 Resource is Initialized */
#define BITP_SDU_MSG_C1L1INIT                17                               /* Indicates that the Core 1 L1 Resource is Initialized */
#define BITP_SDU_MSG_C0L1INIT                16                               /* Indicates that the Core 0 L1 Resource is Initialized */
#define BITM_SDU_MSG_CALLERR                 (_ADI_MSK(0x80000000,uint32_t))  /* Flag Set by the Boot Code Prior to an Error Call */
#define BITM_SDU_MSG_CALLBACK                (_ADI_MSK(0x40000000,uint32_t))  /* Flag Set by the Boot Code Prior to a Callback Call */
#define BITM_SDU_MSG_CALLINIT                (_ADI_MSK(0x20000000,uint32_t))  /* Flag Set by the Boot Code Prior to an Initcode Call */
#define BITM_SDU_MSG_CALLAPP                 (_ADI_MSK(0x10000000,uint32_t))  /* Flag Set by the Boot Code Prior to an Application Call */
#define BITM_SDU_MSG_HALTONERR               (_ADI_MSK(0x08000000,uint32_t))  /* Generate an Emulation Exception Prior to an Error Call */
#define BITM_SDU_MSG_HALTONCALL              (_ADI_MSK(0x04000000,uint32_t))  /* Generate an Emulation Exception Prior to a Callback Call */
#define BITM_SDU_MSG_HALTONINIT              (_ADI_MSK(0x02000000,uint32_t))  /* Generate an Emulation Exception Prior to an Initcode Call */
#define BITM_SDU_MSG_HALTONAPP               (_ADI_MSK(0x01000000,uint32_t))  /* Generate an Emulation Exception Prior to an Application Call */
#define BITM_SDU_MSG_L3INIT                  (_ADI_MSK(0x00800000,uint32_t))  /* Indicates that the L3 Resource is Initialized */
#define BITM_SDU_MSG_L2INIT                  (_ADI_MSK(0x00400000,uint32_t))  /* Indicates that the L2 Resource is Initialized */
#define BITM_SDU_MSG_C1L1INIT                (_ADI_MSK(0x00020000,uint32_t))  /* Indicates that the Core 1 L1 Resource is Initialized */
#define BITM_SDU_MSG_C0L1INIT                (_ADI_MSK(0x00010000,uint32_t))  /* Indicates that the Core 0 L1 Resource is Initialized */

/* ------------------------------------------------------------------------------------------------------------------------
        SDU_GHLT                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_SDU_GHLT_SS2                    18                               /* Slave Select 2 */
#define BITP_SDU_GHLT_SS1                    17                               /* Slave Select 1 */
#define BITP_SDU_GHLT_SS0                    16                               /* Slave Select 0 */
#define BITP_SDU_GHLT_MS2                     2                               /* Master Select 2 */
#define BITP_SDU_GHLT_MS1                     1                               /* Master Select 1 */
#define BITP_SDU_GHLT_MS0                     0                               /* Master Select 0 */
#define BITM_SDU_GHLT_SS2                    (_ADI_MSK(0x00040000,uint32_t))  /* Slave Select 2 */
#define BITM_SDU_GHLT_SS1                    (_ADI_MSK(0x00020000,uint32_t))  /* Slave Select 1 */
#define BITM_SDU_GHLT_SS0                    (_ADI_MSK(0x00010000,uint32_t))  /* Slave Select 0 */
#define BITM_SDU_GHLT_MS2                    (_ADI_MSK(0x00000004,uint32_t))  /* Master Select 2 */
#define BITM_SDU_GHLT_MS1                    (_ADI_MSK(0x00000002,uint32_t))  /* Master Select 1 */
#define BITM_SDU_GHLT_MS0                    (_ADI_MSK(0x00000001,uint32_t))  /* Master Select 0 */

/* ==================================================
        System Cross Bar Registers
   ================================================== */

/* =========================
        SCB0
   ========================= */
#define REG_SCB0_ARBR0                  0xFFCA2408         /* SCB0 Arbitration Read Channel Master Interface n Register */
#define REG_SCB0_ARBR1                  0xFFCA2428         /* SCB0 Arbitration Read Channel Master Interface n Register */
#define REG_SCB0_ARBR2                  0xFFCA2448         /* SCB0 Arbitration Read Channel Master Interface n Register */
#define REG_SCB0_ARBR3                  0xFFCA2468         /* SCB0 Arbitration Read Channel Master Interface n Register */
#define REG_SCB0_ARBR4                  0xFFCA2488         /* SCB0 Arbitration Read Channel Master Interface n Register */
#define REG_SCB0_ARBR5                  0xFFCA24A8         /* SCB0 Arbitration Read Channel Master Interface n Register */
#define REG_SCB0_ARBW0                  0xFFCA240C         /* SCB0 Arbitration Write Channel Master Interface n Register */
#define REG_SCB0_ARBW1                  0xFFCA242C         /* SCB0 Arbitration Write Channel Master Interface n Register */
#define REG_SCB0_ARBW2                  0xFFCA244C         /* SCB0 Arbitration Write Channel Master Interface n Register */
#define REG_SCB0_ARBW3                  0xFFCA246C         /* SCB0 Arbitration Write Channel Master Interface n Register */
#define REG_SCB0_ARBW4                  0xFFCA248C         /* SCB0 Arbitration Write Channel Master Interface n Register */
#define REG_SCB0_ARBW5                  0xFFCA24AC         /* SCB0 Arbitration Write Channel Master Interface n Register */
#define REG_SCB0_SLAVES                 0xFFCA2FC0         /* SCB0 Slave Interfaces Number Register */
#define REG_SCB0_MASTERS                0xFFCA2FC4         /* SCB0 Master Interfaces Number Register */

/* =========================
        SCB1
   ========================= */
#define REG_SCB1_ARBR0                  0xFFC42408         /* SCB1 Arbitration Read Channel Master Interface n Register */
#define REG_SCB1_ARBW0                  0xFFC4240C         /* SCB1 Arbitration Write Channel Master Interface n Register */
#define REG_SCB1_SLAVES                 0xFFC42FC0         /* SCB1 Slave Interfaces Number Register */
#define REG_SCB1_MASTERS                0xFFC42FC4         /* SCB1 Master Interfaces Number Register */

/* =========================
        SCB2
   ========================= */
#define REG_SCB2_ARBR0                  0xFFC06408         /* SCB2 Arbitration Read Channel Master Interface n Register */
#define REG_SCB2_ARBW0                  0xFFC0640C         /* SCB2 Arbitration Write Channel Master Interface n Register */
#define REG_SCB2_SLAVES                 0xFFC06FC0         /* SCB2 Slave Interfaces Number Register */
#define REG_SCB2_MASTERS                0xFFC06FC4         /* SCB2 Master Interfaces Number Register */

/* =========================
        SCB3
   ========================= */
#define REG_SCB3_ARBR0                  0xFFC08408         /* SCB3 Arbitration Read Channel Master Interface n Register */
#define REG_SCB3_ARBW0                  0xFFC0840C         /* SCB3 Arbitration Write Channel Master Interface n Register */
#define REG_SCB3_SLAVES                 0xFFC08FC0         /* SCB3 Slave Interfaces Number Register */
#define REG_SCB3_MASTERS                0xFFC08FC4         /* SCB3 Master Interfaces Number Register */

/* =========================
        SCB4
   ========================= */
#define REG_SCB4_ARBR0                  0xFFC0A408         /* SCB4 Arbitration Read Channel Master Interface n Register */
#define REG_SCB4_ARBW0                  0xFFC0A40C         /* SCB4 Arbitration Write Channel Master Interface n Register */
#define REG_SCB4_SLAVES                 0xFFC0AFC0         /* SCB4 Slave Interfaces Number Register */
#define REG_SCB4_MASTERS                0xFFC0AFC4         /* SCB4 Master Interfaces Number Register */

/* =========================
        SCB5
   ========================= */
#define REG_SCB5_ARBR0                  0xFFC0C408         /* SCB5 Arbitration Read Channel Master Interface n Register */
#define REG_SCB5_ARBW0                  0xFFC0C40C         /* SCB5 Arbitration Write Channel Master Interface n Register */
#define REG_SCB5_SLAVES                 0xFFC0CFC0         /* SCB5 Slave Interfaces Number Register */
#define REG_SCB5_MASTERS                0xFFC0CFC4         /* SCB5 Master Interfaces Number Register */

/* =========================
        SCB6
   ========================= */
#define REG_SCB6_ARBR0                  0xFFC0E408         /* SCB6 Arbitration Read Channel Master Interface n Register */
#define REG_SCB6_ARBW0                  0xFFC0E40C         /* SCB6 Arbitration Write Channel Master Interface n Register */
#define REG_SCB6_SLAVES                 0xFFC0EFC0         /* SCB6 Slave Interfaces Number Register */
#define REG_SCB6_MASTERS                0xFFC0EFC4         /* SCB6 Master Interfaces Number Register */

/* =========================
        SCB7
   ========================= */
#define REG_SCB7_ARBR0                  0xFFC11408         /* SCB7 Arbitration Read Channel Master Interface n Register */
#define REG_SCB7_ARBW0                  0xFFC1140C         /* SCB7 Arbitration Write Channel Master Interface n Register */
#define REG_SCB7_SLAVES                 0xFFC11FC0         /* SCB7 Slave Interfaces Number Register */
#define REG_SCB7_MASTERS                0xFFC11FC4         /* SCB7 Master Interfaces Number Register */

/* =========================
        SCB8
   ========================= */
#define REG_SCB8_ARBR0                  0xFFC13408         /* SCB8 Arbitration Read Channel Master Interface n Register */
#define REG_SCB8_ARBW0                  0xFFC1340C         /* SCB8 Arbitration Write Channel Master Interface n Register */
#define REG_SCB8_SLAVES                 0xFFC13FC0         /* SCB8 Slave Interfaces Number Register */
#define REG_SCB8_MASTERS                0xFFC13FC4         /* SCB8 Master Interfaces Number Register */

/* =========================
        SCB9
   ========================= */
#define REG_SCB9_ARBR0                  0xFFC15408         /* SCB9 Arbitration Read Channel Master Interface n Register */
#define REG_SCB9_ARBW0                  0xFFC1540C         /* SCB9 Arbitration Write Channel Master Interface n Register */
#define REG_SCB9_SLAVES                 0xFFC15FC0         /* SCB9 Slave Interfaces Number Register */
#define REG_SCB9_MASTERS                0xFFC15FC4         /* SCB9 Master Interfaces Number Register */

/* =========================
        SCB10
   ========================= */
#define REG_SCB10_ARBR0                 0xFFCA1408         /* SCB10 Arbitration Read Channel Master Interface n Register */
#define REG_SCB10_ARBR1                 0xFFCA1428         /* SCB10 Arbitration Read Channel Master Interface n Register */
#define REG_SCB10_ARBR2                 0xFFCA1448         /* SCB10 Arbitration Read Channel Master Interface n Register */
#define REG_SCB10_ARBW0                 0xFFCA140C         /* SCB10 Arbitration Write Channel Master Interface n Register */
#define REG_SCB10_ARBW1                 0xFFCA142C         /* SCB10 Arbitration Write Channel Master Interface n Register */
#define REG_SCB10_ARBW2                 0xFFCA144C         /* SCB10 Arbitration Write Channel Master Interface n Register */
#define REG_SCB10_SLAVES                0xFFCA1FC0         /* SCB10 Slave Interfaces Number Register */
#define REG_SCB10_MASTERS               0xFFCA1FC4         /* SCB10 Master Interfaces Number Register */

/* =========================
        SCB11
   ========================= */
#define REG_SCB11_ARBR0                 0xFFCA0408         /* SCB11 Arbitration Read Channel Master Interface n Register */
#define REG_SCB11_ARBR1                 0xFFCA0428         /* SCB11 Arbitration Read Channel Master Interface n Register */
#define REG_SCB11_ARBR2                 0xFFCA0448         /* SCB11 Arbitration Read Channel Master Interface n Register */
#define REG_SCB11_ARBR3                 0xFFCA0468         /* SCB11 Arbitration Read Channel Master Interface n Register */
#define REG_SCB11_ARBR4                 0xFFCA0488         /* SCB11 Arbitration Read Channel Master Interface n Register */
#define REG_SCB11_ARBR5                 0xFFCA04A8         /* SCB11 Arbitration Read Channel Master Interface n Register */
#define REG_SCB11_ARBR6                 0xFFCA04C8         /* SCB11 Arbitration Read Channel Master Interface n Register */
#define REG_SCB11_ARBW0                 0xFFCA040C         /* SCB11 Arbitration Write Channel Master Interface n Register */
#define REG_SCB11_ARBW1                 0xFFCA042C         /* SCB11 Arbitration Write Channel Master Interface n Register */
#define REG_SCB11_ARBW2                 0xFFCA044C         /* SCB11 Arbitration Write Channel Master Interface n Register */
#define REG_SCB11_ARBW3                 0xFFCA046C         /* SCB11 Arbitration Write Channel Master Interface n Register */
#define REG_SCB11_ARBW4                 0xFFCA048C         /* SCB11 Arbitration Write Channel Master Interface n Register */
#define REG_SCB11_ARBW5                 0xFFCA04AC         /* SCB11 Arbitration Write Channel Master Interface n Register */
#define REG_SCB11_ARBW6                 0xFFCA04CC         /* SCB11 Arbitration Write Channel Master Interface n Register */
#define REG_SCB11_SLAVES                0xFFCA0FC0         /* SCB11 Slave Interfaces Number Register */
#define REG_SCB11_MASTERS               0xFFCA0FC4         /* SCB11 Master Interfaces Number Register */

#endif // ifndef CYGONCE_HAL_BFIN_REGS_H
