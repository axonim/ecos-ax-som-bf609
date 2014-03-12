/** @file i2c_bfin_bf60x.c
 *  @brief BF60x MAC driver
 *
///==========================================================================
//
//      if_bf60x.c
//
//      Blackfin BF60x MAC driver
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2006 Free Software Foundation, Inc.
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
// Author(s):    Siarhei Vasiliuk
// Date:         2013-09-22
// Purpose:
// Description:  Blackfin BF60x MAC driver
//
//####DESCRIPTIONEND####
//
//========================================================================*/

/** \file if_bf60x.c
    * Ethernet driver for Blackfin bf60x core.
 */

#include <pkgconf/system.h>
#include <pkgconf/hal.h>
#include <pkgconf/devs_eth_bf60x.h>
#include <pkgconf/io_eth_drivers.h>
#if defined(CYGPKG_REDBOOT)
   #include <pkgconf/redboot.h>
#endif

#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/hal/drv_api.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_ass.h>
#include <cyg/infra/diag.h>
#include <cyg/io/eth/netdev.h>
#include <cyg/io/eth/eth_drv.h>
#include <cyg/io/eth/eth_drv_stats.h>
#include <cyg/io/eth_phy.h>
#include <errno.h>
#include <string.h>
#include <net/bf60x_eth.h>
#include <cyg/hal/hal_if.h>             // interface API

#if defined(CYGPKG_HAL_BFIN_ADSP_BF609EZ)
#include <cyg/io/io.h>
#include <cyg/io/i2c.h>
#include <cyg/io/i2c_bfin_bf60x.h>
#endif

//Driver interface callbacks
#define _eth_drv_init(sc,mac) \
  (sc->funs->eth_drv->init)(sc,(unsigned char *)mac)
#define _eth_drv_tx_done(sc,key,status) \
  (sc->funs->eth_drv->tx_done)(sc,key,status)
#define _eth_drv_recv(sc,len) \
  (sc->funs->eth_drv->recv)(sc,len)
      
  
// Set up the level of debug output

#if CYGPKG_DEVS_ETH_BF60X_DEBUG_LEVEL > 10
   #define debug1_printf(args...) diag_printf(args)
   #define DBG_READ_UINT32(a, b)    HAL_READ_UINT32(a, b)
#else
   #define debug1_printf(args...) 
   #define DBG_READ_UINT32(a, b)
#endif

#ifndef CYGPKG_REDBOOT		// Commented
    static cyg_uint32 bf60x_eth_isr(cyg_vector_t vector, cyg_addrword_t data);
#endif


//============================================================================

// Private Data structures


/** 
 *  @brief: Transmit Descriptor structure
*/
typedef struct _tdd
{ 
    cyg_uint32          Status;          /**<TDES0 OR RDES0 */
    cyg_uint32          ControlDesc;     /**<TDES1 OR RDES1 */
    cyg_uint32          StartAddr1;      /**<TDES2 OR RDES2 */
//    cyg_uint32          StartAddr2;      /**<TDES3 OR RDES3: ring structure */
    void                *pNextDesc;      /**<TDES3 OR RDES3: chain structure*/ 
    cyg_uint32          CheckSumStat;
    cyg_uint32          Reserved;       /**< reserved                */
    cyg_uint32          RxTimeStampLo;  /**< ieee-1588 timestamp low */
    cyg_uint32          RxTimeStampHi;  /**< ieee-1588 timestamp high */

} tdd; 


/** Emac channel */
typedef struct _emac_chan
{
    tdd                  *pDmaDesc;
    bool                 Recv;                 /**< receive channel identifier */   
    cyg_uint8            *pDataBuffer;         /**< Driver memory used for operations.     */
    cyg_uint32           BufCount;             /**< Supplied memory in bytes - 1 .     */
    cyg_uint32           IdxIn;
    cyg_uint32           IdxOut;
    bool                 iGotFrame ;

} emac_chan;

typedef struct _tx_frame_info
{
    cyg_uint16 ind;
    cyg_uint16 num;
} tx_frame_info;
/** 
 *  @brief: bf60x Ethernet private data
*/
typedef struct bf60x_eth_priv_s
{
    cyg_uint32        intr_vector;
    cyg_interrupt     intr;
    cyg_handle_t      intr_handle;

    cyg_uint32        base;      // Base address of device
    eth_phy_access_t  *phy;
  
    /* Channels */
    emac_chan         Rx;            /**< receive channel              */
    emac_chan         Tx;            /**< transmit channel             */
    bool              AutoNegotiate; /**< AutoNegotiation enabled or not */
    bool              Port10;        /**< 10Mbps port or 100Mbps port   */
    bool              FullDuplex;    /**< full-duplex or half-duplex    */
    bool              LoopBackMode;  /**< loopback mode on              */
    cyg_uint32        MDCClockRange; /**< MDC clock range               */
    cyg_uint32        tx_FreeDesc;             /**< number of available dma descriptors */
     
    cyg_uint32        *tx_keys;
    tx_frame_info     *tx_fsize;
    cyg_uint32        isr;
    cyg_uint8         EMAC_num;

} bf60x_eth_priv_t;

// bf60x multicast hash mask (64-bit hash in two 32-bit words)
typedef struct mc_hash_mask_s
{
    cyg_uint32 hi;
    cyg_uint32 lo;
} bf60x_mc_hash_mask_t;



//#define CHAIN_MODE

//============================================================================
// Structures init
#ifdef CYGPKG_DEVS_ETH_BF60X_EMAC0

    #define ETH0_TX_BUFFER_LEN              CYGNUM_DEVS_ETH_BF60X_EMAC0_TX_BUFS * ETH_PACKET_SIZE /**< Total size of an ethernet tx packet */
    #define ETH0_RX_BUFFER_LEN              CYGNUM_DEVS_ETH_BF60X_EMAC0_RX_BUFS * ETH_PACKET_SIZE /**< Total size of an ethernet tx packet */
    #define HAL_DCACHE_LINE_SIZE    32

    
// #if defined(CYGPKG_REDBOOT)
// #else
#ifdef CHAIN_MODE
    static cyg_int8   txDataBuffer_eth0[ETH0_TX_BUFFER_LEN]                     __attribute__ ((aligned (HAL_DCACHE_LINE_SIZE))) ;//__attribute__ ((section(".ethernet_buf")));
#endif
    static cyg_int8   rxDataBuffer_eth0[ETH0_RX_BUFFER_LEN]                     __attribute__ ((aligned (HAL_DCACHE_LINE_SIZE))) __attribute__ ((section(".ramdata_buf1")));
    volatile static tdd txDmaDesc_eth0[CYGNUM_DEVS_ETH_BF60X_EMAC0_TX_BUFS]      __attribute__ ((aligned (HAL_DCACHE_LINE_SIZE)))  __attribute__ ((section(".ramdata_buf1")));
    volatile static tdd rxDmaDesc_eth0[CYGNUM_DEVS_ETH_BF60X_EMAC0_RX_BUFS]      __attribute__ ((aligned (HAL_DCACHE_LINE_SIZE))) __attribute__ ((section(".ramdata_buf1")));
    static cyg_uint32 tx_keys_eth0[CYGNUM_DEVS_ETH_BF60X_EMAC0_TX_BUFS]      __attribute__ ((aligned (HAL_DCACHE_LINE_SIZE))) __attribute__ ((section(".ramdata_buf1")));
    static bf60x_mc_hash_mask_t tx_fsize_eth0[CYGNUM_DEVS_ETH_BF60X_EMAC0_TX_BUFS]      __attribute__ ((aligned (HAL_DCACHE_LINE_SIZE))) __attribute__ ((section(".ramdata_buf1")));
//#endif
#endif
        
/**
 * @brief       Wait until busy bit is cleared
 * 
 * @param [in]  mac_ba    EMAC base address
 *
 * @return      void
 */ 
static cyg_int32 poll_mii_done(unsigned long mac_ba)
{

    cyg_int32  loop_count = PHY_LOOP_COUNT;
    volatile cyg_uint32  val32;
    debug1_printf( " Func: %s \n",   __FUNCTION__);

    /// poll for the GMII busy bit 
    do 
    {
        HAL_READ_UINT32(mac_ba + EMAC_SMI_ADDR, val32);
        if (!(val32 &  BITM_EMAC_SMI_ADDR_SMIB) )
               break;
    } while (--loop_count > 0);

    return ( (loop_count == 0) ? -1 : 0);
}

/**
*
* @brief  Write one of the PHY registers via the MII bus
*
* @param  reg_addr  -   address of PHY register
*
* @param  phy_addr  -   number PHY
*
* @param  data  -   data for write
*
* @param  mac_ba  -   MAC base address
*
* @return None
*
*/
static void bf60x_write_phy(int reg_addr, int phy_addr, unsigned short data, unsigned long mac_ba)
{
    cyg_uint32 val;
    debug1_printf( " Func: %s \n",   __FUNCTION__);

    /// set write data
    HAL_WRITE_UINT32(mac_ba + EMAC_SMI_DATA, (cyg_uint32)(data & BITM_EMAC_SMI_DATA_SMID));
    asm volatile ("SSYNC;");
    val = ( BITM_EMAC_SMI_ADDR_SMIB | 
            BITM_EMAC_SMI_ADDR_SMIW | 
            SET_EMAC_SMI_ADDR_PA(phy_addr) |
            SET_EMAC_SMI_ADDR_SMIR(reg_addr)|
            SET_CR(4)
           );
        
    HAL_WRITE_UINT32(mac_ba + EMAC_SMI_ADDR, val);
    asm volatile ("SSYNC;");
    /// Wait for operation to complete
    poll_mii_done(mac_ba);
}

/**
*
* @brief   Read one of the PHY registers via the MII bus
*
* @param  reg_addr  -   address of PHY register
*
* @param  phy_addr  -   number PHY
*
* @param  data  -   data pointer for read
*
* @param  mac_ba  -   MAC base address
*
* @return true if the operation is successful
*
*/
static bool bf60x_read_phy(int reg_addr, int phy_addr, unsigned short *data, unsigned long mac_ba)
{
    cyg_uint32 val;

    poll_mii_done(mac_ba);

    val = ( BITM_EMAC_SMI_ADDR_SMIB |
            SET_EMAC_SMI_ADDR_PA(phy_addr) |
            SET_EMAC_SMI_ADDR_SMIR(reg_addr)|
            SET_CR(4)
          );

    /// set parameters for read data
    HAL_WRITE_UINT32(mac_ba + EMAC_SMI_ADDR, val);
    asm volatile ("SSYNC;");

    /// Wait for operation to complete
    poll_mii_done(mac_ba);

    HAL_READ_UINT32(mac_ba + EMAC_SMI_DATA, val);
    *data = (cyg_uint16) (val & BITM_EMAC_SMI_DATA_SMID);

    return (true);
}


/**
*
* @brief  Initialize PHY level
*
* @return  none
*
*/
static void bf60x_init_phy(void)
{
    cyg_uint32 phydat;
    cyg_int32  loop_count = PHY_LOOP_COUNT;
    debug1_printf( " Func: %s \n",   __FUNCTION__);
    
    bf60x_write_phy(REG_PHY_MODECTL, 1, PHY_MODECTL_RESET/*|PHY_MODECTL_AUTO_NEGO_ENA|PHY_MODECTL_RESTART_A_NEGO*/, EMAC0_REGBASE);    //RESET PHY;
    CYGACC_CALL_IF_DELAY_US(100000);

    bf60x_read_phy(REG_PHY_MODECTL, 1, &phydat, EMAC0_REGBASE);
    if ( phydat & PHY_MODECTL_RESET)
    {
        debug1_printf("RESET PHY FAILED\n");
    }
    
    bf60x_write_phy(REG_PHY_MODECTL, 1, PHY_MODECTL_AUTO_NEGO_ENA|PHY_MODECTL_RESTART_A_NEGO, EMAC0_REGBASE);    //RESET PHY;
    CYGACC_CALL_IF_DELAY_US(100000);
    do{
    
        bf60x_read_phy(REG_PHY_MODESTAT, 1, &phydat, EMAC0_REGBASE);
        if (( phydat & PHY_MODESTAT_AUTO_NEGO_COM)== PHY_MODESTAT_AUTO_NEGO_COM)
        {
            break;
        }

    }while(--loop_count > 0);
    
    if (( phydat & PHY_MODESTAT_AUTO_NEGO_COM)!= PHY_MODESTAT_AUTO_NEGO_COM)
    {
        diag_printf("Auto-Negotiation PHY FAILED\n");
    }
    

   
}


ETH_PHY_REG_LEVEL_ACCESS_FUNS(bf60x_phy0, 
                              bf60x_init_phy,
                              NULL,
                              bf60x_write_phy,
                              bf60x_read_phy);
                              

ETH_PHY_REG_LEVEL_ACCESS_FUNS(bf60x_phy1, 
                              bf60x_init_phy,
                              NULL,
                              bf60x_write_phy,
                              bf60x_read_phy);


/// ======================================================================
///Initialization code
  

/**
*
* @brief   Set interface MAC address
*
* @param priv - driver private data
*
* @param mac_addr - pointer to MAC address 
* 
* @return none
*
*/
static void bf60x_set_mac(bf60x_eth_priv_t *priv, cyg_uint8 *mac_addr)
{
    cyg_uint32 mac_hi, mac_lo;
    debug1_printf( " Func: %s \n",   __FUNCTION__);

    /// Split MAC address into two words.
    mac_lo = (mac_addr[3] << 24) |
             (mac_addr[2] << 16) |
             (mac_addr[1] <<  8) |
              mac_addr[0];

    mac_hi = (mac_addr[5] <<  8) |
              mac_addr[4];

    /// Write MAC address to the specific-address register.
    HAL_WRITE_UINT32(priv->base + EMAC_ADDR0_LO , mac_lo);
    asm volatile ("SSYNC;");
    HAL_WRITE_UINT32(priv->base + EMAC_ADDR0_HI , mac_hi);
    asm volatile ("SSYNC;");
    HAL_READ_UINT32(priv->base + EMAC_ADDR0_LO , mac_lo);
    HAL_READ_UINT32(priv->base + EMAC_ADDR0_HI , mac_hi);

    debug1_printf( " Set mac: mac_lo %x mac_hi %x \n",   mac_lo, mac_hi);

}


/**
 * @brief       Initializes EMAC
 *
 * @details     This function configures the EMAC Configuration register, filter register
 *              and flow control register using the default configuration supplied by the
 *              user.
 *
* @param priv - driver private data
 *
 * @return      none
 *
 * @note        Internal Driver function.
 */
static void emac_init(bf60x_eth_priv_t *priv)
{
    cyg_uint32 val32;
    debug1_printf( " Func: %s \n",   __FUNCTION__);
     
    HAL_READ_UINT32(priv->base + EMAC_MACCFG, val32);

    /// initialize mac configuration register
    if (priv->LoopBackMode)
        val32 = BITM_EMAC_MACCFG_LM;

    if (priv->FullDuplex)
        val32 |= BITM_EMAC_MACCFG_DM;

    if(priv->Port10)
        val32 &= ~BITM_EMAC_MACCFG_FES;
    else
        val32 |= BITM_EMAC_MACCFG_FES;

    val32|= ( BITM_EMAC_MACCFG_TE 
            | BITM_EMAC_MACCFG_RE 
            | BITM_EMAC_MACCFG_CST
//            | BITM_EMAC_MACCFG_JB 
            //| BITM_EMAC_MACCFG_WD
            //| BITM_EMAC_MACCFG_DC
//            | ENUM_EMAC_MACCFG_BIT_TIMES_96
            /*|BITM_EMAC_MACCFG_JE*/
            );

    HAL_WRITE_UINT32(priv->base + EMAC_MACCFG, val32);
    asm volatile ("SSYNC;");

    /// initialize frame filter configuration

    HAL_WRITE_UINT32(priv->base + EMAC_MACFRMFILT ,
                                    BITM_EMAC_MACFRMFILT_RA  | 
                                    BITM_EMAC_MACFRMFILT_PR  |
                                    BITM_EMAC_MACFRMFILT_PM  |
                                    BITM_EMAC_MACFRMFILT_HMC) ;
    asm volatile ("SSYNC;");
    
    /// setup flow control options
    if(priv->FullDuplex)
    {
        HAL_WRITE_UINT32(priv->base + EMAC_FLOWCTL,
                                    BITM_EMAC_FLOWCTL_TFE |
                                    BITM_EMAC_FLOWCTL_RFE);
        asm volatile ("SSYNC;");
    }                                
}
/**
 * @brief       Initializes the descriptor list for a channel
 *
 * @details     This functions constructs the descriptor linked list from the supplied memory.
 *
 * @param [in]  chan      Pointer to the channel
 *
 * @return      none                  
*/
static void init_descriptor_list(emac_chan *chan)
{
    cyg_int32 i;
    emac_chan *pChan = (emac_chan *)chan;
    cyg_int8 *pPacket = (cyg_int8*)pChan->pDataBuffer;
    debug1_printf( " Func: %s \n",   __FUNCTION__);
    
    
    /// Initialize buffer
	for (i = 0; i <= pChan->BufCount; i++) 
    {
#ifndef CHAIN_MODE
        if (pChan->Recv)
#endif
        {

            pChan->pDmaDesc[i].StartAddr1 =  &pPacket[i * ETH_PACKET_SIZE];   
            if (pChan->Recv)
            {
                pChan->pDmaDesc[i].Status = (EMAC_DMAOWN );
                pChan->pDmaDesc[i].ControlDesc = (ETH_PACKET_SIZE|
                                                  EMAC_RCH );
            }
            if (i > 0) {
                pChan->pDmaDesc[i-1].pNextDesc = &pChan->pDmaDesc[i];   
                if (i == (pChan->BufCount))
                    pChan->pDmaDesc[i].pNextDesc = pChan->pDmaDesc;
            }
        }
#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
        HAL_DCACHE_FLUSH(&pChan->pDmaDesc[i]);                                                                    
#endif  

	}    

}

/**
*
* @brief  Emac initialization
*
* @param  tab - pointer to the struct that  contains all functions which allows stack to control our hardware
* 
* @return  true if the operation is successful
*
*/
static bool bf60x_eth_init(struct cyg_netdevtab_entry *tab)
{
    struct eth_drv_sc *sc = (struct eth_drv_sc *)tab->device_instance;
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    emac_chan *TxChan = (emac_chan *)&priv->Tx;
    emac_chan *RxChan = (emac_chan *)&priv->Rx;
    eth_phy_access_t* pPhy = (eth_phy_access_t* )priv->phy;
    cyg_uint32 EmacBase = (cyg_uint32) priv->base;
    cyg_uint32 IntrVect = (cyg_uint32) priv->intr_vector;

    int phy_state;
    cyg_uint32  val32;
    cyg_int32  loop_count = MAC_LOOP_COUNT;
    cyg_uint8 tx_data[2];
    cyg_uint32 result;
    
#ifdef CYGPKG_DEVS_ETH_BF60X_EMAC0
    unsigned char mac_addr_eth0[6] = {CYGPKG_DEVS_ETH_BF60X_EMAC0_MACADDR};
#else
    unsigned char mac_addr_eth0[6] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
#endif    
#ifdef CYGPKG_DEVS_ETH_BF60X_EMAC1
    unsigned char mac_addr_eth1[6] = {CYGPKG_DEVS_ETH_BF60X_EMAC1_MACADDR};
#else
    unsigned char mac_addr_eth1[6] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
#endif    

    debug1_printf( " Func: %s \n",   __FUNCTION__);

#if defined(CYGPKG_ADSP_BF609_EZ_KIT)
    
    ///Init RMII_CLK_EN via MCP23017 IO Expander

    tx_data[0] = 0x00; // IODIRA
    tx_data[1] = 0x00; // Setup pins out


    cyg_i2c_transaction_begin(&cyg_i2c_adsp_bf609ez_dev_mcp23017);
    result = cyg_i2c_transaction_tx(&cyg_i2c_adsp_bf609ez_dev_mcp23017, true, &tx_data[0], 2, true);
    cyg_i2c_transaction_end(&cyg_i2c_adsp_bf609ez_dev_mcp23017);
 
    tx_data[0] = 0x14; // OLATA
    tx_data[1] = 0x80; // RMII_CLK_EN

    cyg_i2c_transaction_begin(&cyg_i2c_adsp_bf609ez_dev_mcp23017);
    result = cyg_i2c_transaction_tx(&cyg_i2c_adsp_bf609ez_dev_mcp23017, true, &tx_data[0], 2, true);
    cyg_i2c_transaction_end(&cyg_i2c_adsp_bf609ez_dev_mcp23017);
#endif
    /// Perform a software reset by setting the EMAC_DMA_BUSMODE.SWRbit.
    
    HAL_WRITE_UINT32(priv->base + EMAC_DMA_BUSMODE, BITM_EMAC_DMA_BUSMODE_SWR);
    asm volatile ("SSYNC;");

    /// Wait for the completion of the reset process by polling 
    do 
    {
        HAL_READ_UINT32(priv->base + EMAC_DMA_BUSMODE, val32);
        if(!(val32 &  BITM_EMAC_DMA_BUSMODE_SWR))
            break;
    } while (--loop_count > 0);


    if (loop_count == 0)
    {
        debug1_printf("SWR reset failed.\n");
        return (false);
    }    
    
    if (!_eth_phy_init(pPhy, EmacBase, CYGPKG_DEVS_ETH_BF60X_EMAC0_PHYADDR))
    {
        debug1_printf("Init phy failed\n" );
        return (false);
    } else
    {
        cyg_uint32 anar,anlpar,res;

        bf60x_read_phy(REG_PHY_ANAR, 1, &anar, EMAC0_REGBASE);
        bf60x_read_phy(REG_PHY_ANLPAR, 1, &anlpar, EMAC0_REGBASE);
        res = anar & anlpar;
        if(res & PHY_ANAR_100BASE_FULL){
            priv->FullDuplex = 1;
            priv->Port10 = 0;
            diag_printf(" 100BASE_FULL\n");

        }
        else if(res & PHY_ANAR_100BASE_HALF){
            priv->FullDuplex = 0;
            priv->Port10 = 0;
            diag_printf(" 100BASE_HALF\n");
        }    
        else if(res & PHY_ANAR_10BASE_FULL){
            priv->FullDuplex = 1;
            priv->Port10 = 1;
            diag_printf(" 10BASE_FULL\n");
        }
        else if(res & PHY_ANAR_10BASE_HALF){
            priv->FullDuplex = 0;
            priv->Port10 = 1;
            diag_printf(" 10BASE_HALF\n");
            
        }
    }
#ifdef CYGHWR_DEVS_ETH_PHY_KSZ808863
//    _eth_phy_state(priv->phy);
    // bf60x_read_phy(0x00 , 1, &phy_state, EMAC0_REGBASE);
    // debug1_printf("  0x00 : %x\n", phy_state);
    // bf60x_read_phy(0x01 , 1, &phy_state, EMAC0_REGBASE);
    // debug1_printf("  0x01 : %x\n", phy_state);
    // bf60x_read_phy(0x04 , 1, &phy_state, EMAC0_REGBASE);
    // debug1_printf("  0x04 : %x\n", phy_state);
    // bf60x_read_phy(0x05 , 1, &phy_state, EMAC0_REGBASE);
    // debug1_printf("  0x05 : %x\n", phy_state);
    // bf60x_read_phy(0x19 , 1, &phy_state, EMAC0_REGBASE);
    // debug1_printf("  0x19 : %x\n", phy_state);
    // bf60x_read_phy(0x1f , 1, &phy_state, EMAC0_REGBASE);
    // debug1_printf("  0x1f : %x\n", phy_state);
    // bf60x_read_phy(0x1e , 1, &phy_state, EMAC0_REGBASE);
    // debug1_printf("  REG_PHY_100BASE_CTRL : %x\n", phy_state);

#endif
    
    /// clear any pending interrupts - write to clear bits in status reg
    /// only lower 16-bits has interrupts and w1c 
    HAL_WRITE_UINT32(priv->base + EMAC_DMA_STAT,  0x1FFFF);
    asm volatile ("SSYNC;");
    HAL_WRITE_UINT32(priv->base + EMAC_IPC_RXIMSK,0x3fff3fff);//mask counter interrupts
    asm volatile ("SSYNC;");
    HAL_WRITE_UINT32(priv->base + EMAC_MMC_RXIMSK,0x00ffffff);//mask counter interrupts
    asm volatile ("SSYNC;");
    HAL_WRITE_UINT32(priv->base + EMAC_MMC_TXIMSK,0x00ffffff);//mask counter interrupts
    asm volatile ("SSYNC;");

    HAL_WRITE_UINT32(priv->base + EMAC_MMC_CTL, BITM_EMAC_MMC_CTL_RDRST/*| BITM_EMAC_MMC_CTL_CNTRPSET*/);//reset counters to 0 after read
    asm volatile ("SSYNC;");
    HAL_READ_UINT32(priv->base + EMAC_TXFRMCNT_GB, val32); //set to 0
   
    
    HAL_WRITE_UINT32(priv->base + EMAC_DMA_BUSMODE, BITM_EMAC_DMA_BUSMODE_ATDS);    
    asm volatile ("SSYNC;");

    /// initialize dma interrupt mask register 
    /// normal mode enables the following interrupts
    /// - Transmit interrupt
    /// - Transmit buffer unavailable
    /// - Receive interrupt
    /// - Early receive interrupt
    
    HAL_WRITE_UINT32(priv->base + EMAC_DMA_IEN,   BITM_EMAC_DMA_IEN_NIS | 
//                                                  BITM_EMAC_DMA_IEN_AIS | 
//                                                  BITM_EMAC_DMA_IEN_RU  | 
                                                  BITM_EMAC_DMA_IEN_RI  | 
//                                                  BITM_EMAC_DMA_IEN_UNF | 
                                                  BITM_EMAC_DMA_IEN_TU  | 
//                                                  BITM_EMAC_DMA_IEN_ETI | 
                                                  BITM_EMAC_DMA_IEN_TI
                                                  );
    asm volatile ("SSYNC;");
    
    /// setup appropriate filtering options GEMAC 1,2,3 
    /// enable GEMAC register 0 for trnamsit and receive modes 
    emac_init(priv);   

    /// Enable DMA for rx and tx if pakcets are present 

    HAL_WRITE_UINT32(priv->base +   EMAC_DMA_OPMODE, BITM_EMAC_DMA_OPMODE_OSF    |
                                                     BITM_EMAC_DMA_OPMODE_FEF    |
                                                     BITM_EMAC_DMA_OPMODE_FUF    |
                                                    // BITM_EMAC_DMA_OPMODE_DT |
                                                    // BITM_EMAC_DMA_OPMODE_TSF |
                                                    // BITM_EMAC_DMA_OPMODE_RSF |  
                                                     ENUM_EMAC_DMA_OPMODE_TTC_64|
                                                     ENUM_EMAC_DMA_OPMODE_RTC_64);
    asm volatile ("SSYNC;");   

#ifndef CYGPKG_REDBOOT
    cyg_drv_interrupt_create( IntrVect,
                            4,
                            (cyg_addrword_t)sc,
                            bf60x_eth_isr,
                            eth_drv_dsr,
                            &priv->intr_handle,
                            &priv->intr);

    cyg_drv_interrupt_attach(priv->intr_handle);
#endif


    if (priv->EMAC_num == 1)
    {
       bf60x_set_mac(priv, mac_addr_eth1);
    }
    else if (priv->EMAC_num == 0)
    {
       bf60x_set_mac(priv, mac_addr_eth0);
    }

    /// setup rx/tx descriptor lists 
    init_descriptor_list(RxChan);
    init_descriptor_list(TxChan);


    HAL_WRITE_UINT32(priv->base + EMAC_DMA_RXDSC_ADDR, (cyg_uint32)RxChan->pDmaDesc);
    asm volatile ("SSYNC;");   
    HAL_WRITE_UINT32(priv->base + EMAC_DMA_TXDSC_ADDR, (cyg_uint32)TxChan->pDmaDesc);
    asm volatile ("SSYNC;");   
    
    
    if (priv->EMAC_num == 1)
    {
       _eth_drv_init(sc, mac_addr_eth1);
    }
    else if (priv->EMAC_num == 0)
    {
       _eth_drv_init(sc, mac_addr_eth0); 
    }
    cyg_drv_interrupt_unmask(priv->intr_vector);
   
    return (true);
}

/**
*
* @brief  Stop Interface
*
* @param  sc - pointer to the driver instance
* 
* @return  none
*
*/
static void bf60x_eth_stop(struct eth_drv_sc *sc)
{
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    cyg_uint32  val32;;
    debug1_printf( " Func: %s \n",   __FUNCTION__);
   
    /// disabled RX \ TX 
    HAL_READ_UINT32(priv->base + EMAC_DMA_OPMODE, val32 );
    val32 &= ~(BITM_EMAC_DMA_OPMODE_SR | BITM_EMAC_DMA_OPMODE_ST);
    HAL_WRITE_UINT32(priv->base + EMAC_DMA_OPMODE, val32);
    asm volatile ("SSYNC;");
}

/**
*
* @brief  Start Interface
*
* @param  sc - pointer to the driver instance
*
* @return  none
*
*/
static void bf60x_eth_start(struct eth_drv_sc *sc, unsigned char *enaddr, int flags)
{
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    int val32;
    debug1_printf( " Func: %s \n",   __FUNCTION__);
   
    /// enabled RX \ TX
    HAL_READ_UINT32(priv->base + EMAC_DMA_OPMODE, val32 );
    val32 |= (BITM_EMAC_DMA_OPMODE_SR | BITM_EMAC_DMA_OPMODE_ST);
    HAL_WRITE_UINT32(priv->base + EMAC_DMA_OPMODE, val32);
    asm volatile ("SSYNC;");
    HAL_READ_UINT32(priv->base + EMAC_DMA_STAT, val32);
    if(( val32& BITM_EMAC_DMA_STAT_TS) == ENUM_EMAC_DMA_STAT_TS_STOPPED)
    {
        debug1_printf( " STOPPED\n" );
        
    } else if (( val32& BITM_EMAC_DMA_STAT_TS) == ENUM_EMAC_DMA_STAT_TS_SUSPENDED)
    {
        debug1_printf( " SUSPENDED\n");
    }    
    
}

/**
*
* @brief  Function which allows stack to control hardware
*
* @param  sc - pointer to the driver instance
* 
* @param  key - indicates what stack wanted to do
*
* @return  false if the operation is successful
*
*/
static int bf60x_eth_control(struct eth_drv_sc *sc, unsigned long key,
                             void *data, int length)
{
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    cyg_uint32 val;
    cyg_uint8 *mac_addr;
    cyg_int32 ret = 0;

    debug1_printf( " Func: %s \n",   __FUNCTION__);

    switch (key) 
    {
        /// Set Ethernet MAC address
        case ETH_DRV_SET_MAC_ADDRESS:
        
            mac_addr = data;

            if (mac_addr == NULL || length < ETHER_ADDR_LEN)
            {
                return 1;
            }
            bf60x_eth_stop(sc);
            bf60x_set_mac(priv, mac_addr);
            bf60x_eth_start(sc, mac_addr, 0);
        break;
        /// Set specified multicast MAC addresses
        case ETH_DRV_SET_MC_LIST:
        /// Enable all multicast MAC addresses
        case ETH_DRV_SET_MC_ALL:
          
            HAL_READ_UINT32(priv->base + EMAC_MACFRMFILT, val);
            val |= BITP_EMAC_MACFRMFILT_PM;
            HAL_WRITE_UINT32(priv->base + EMAC_MACFRMFILT, val);
            asm volatile ("SSYNC;");
        break; 
        /// Unknown or unhandled key
        default:
            ret = 1;
        break; 
    } // switch
    return ret;
    
}

/**
*
* @brief  Gives buffer number of free buffer
*
* @param  sc - pointer to the driver instance
*
* @return number of free buffer
*
*/
static int bf60x_eth_can_send(struct eth_drv_sc *sc)
{
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;

    debug1_printf( " Func: %s : free: %d\n",   __FUNCTION__, priv->tx_FreeDesc);

    return priv->tx_FreeDesc;
}



/**
*
* @brief  Fill DMA descriptors function (Ring mode)  
*
* @param  sc -        pointer to the driver instance
* @param  sg_list -   Pointer to the buffers that contains packet
* @param  sg_len -    Number of buffers in sg_list
* @param  total_len - length of all packet
* @param  key       - key of packet
*
* @return none
*
*/
#ifdef CHAIN_MODE
static void fill_tx_desc(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, 
                            int sg_len, int total_len, unsigned long key)
{

    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    emac_chan* TxChan = (emac_chan*) &priv->Tx;
    cyg_uint32 ind = TxChan->IdxIn;
    int i;
    int len = 0;
    cyg_uint8 *BaseAddr = &TxChan->pDataBuffer[ind * ETH_PACKET_SIZE];
    priv->tx_keys[ind] = key;


    
 
    for(i = 0; i < sg_len; i++) 
    {
       
        memcpy(&BaseAddr[len], (cyg_uint8*) sg_list[i].buf, sg_list[i].len);
        len += sg_list[i].len;
    }
#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
    hal_flush_area(BaseAddr, len);
#endif

    ///  Set the transmit descriptor


    TxChan->pDmaDesc[TxChan->IdxIn].ControlDesc = len;
    TxChan->pDmaDesc[TxChan->IdxIn].Status = (  (1UL << 30) | //IC Interrupt on Completion.
                                                (1UL << 29) | //LS Last Segment. 
                                                (1UL << 28) | //FS First Segment. 
                                                (1UL << 20) | //TCH Second Address Chained. 
                                                 EMAC_DMAOWN   //
                                                            );
    TxChan->pDmaDesc[TxChan->IdxIn].Status |= (  EMAC_DMAOWN   //
                                                            );
#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
    HAL_DCACHE_FLUSH(&TxChan->pDmaDesc[TxChan->IdxIn]);                                                                    
#endif

    debug1_printf( " TX_CUR: %x: TxChan->IdxIn: %x len: %d\n",(cyg_uint32)&TxChan->pDmaDesc[TxChan->IdxIn], TxChan->IdxIn, len);
   
    if(TxChan->IdxIn == TxChan->BufCount)
    {
        TxChan->IdxIn = 0;
        
    } else 
    {
        ++TxChan->IdxIn;
    }
    priv->tx_fsize[ind].ind = TxChan->IdxIn; //index for next frame
    priv->tx_fsize[ind].num = 1; //num descriptors in frame 
    --priv->tx_FreeDesc;

}
#else //!CHAIN_MODE

static void fill_tx_desc(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, 
                            int sg_len, int total_len, unsigned long key)
{

    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    emac_chan* TxChan = (emac_chan*) &priv->Tx;
    cyg_uint32 ind = TxChan->IdxIn;
    cyg_uint32 start = ind;
    int i;
    int next,desc = 0;
    
    priv->tx_keys[ind] = key;
    TxChan->pDmaDesc[ind].Status =  (1UL << 28)     ///FS First Segment.
                                   |(1UL << 30) ;   ///IC Interrupt on Completion.
    
    for(i = 0; i < sg_len; i+=2) 
    {
        next = i+1;
        // ring buff 1
        TxChan->pDmaDesc[ind].StartAddr1 = sg_list[i].buf;
        TxChan->pDmaDesc[ind].ControlDesc = sg_list[i].len;
#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
        hal_flush_area( sg_list[i].buf, sg_list[i].len);
#endif            
        if(next != sg_len) 
        {
            // ring buff 2
            TxChan->pDmaDesc[ind].pNextDesc = sg_list[next].buf;
            TxChan->pDmaDesc[ind].ControlDesc |= (sg_list[next].len << 16);
#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
            hal_flush_area( sg_list[next].buf, sg_list[next].len);
#endif            
        }
        if (i >= sg_len - 2)
        {
            TxChan->pDmaDesc[ind].Status |= (1UL << 29);  ///LS Last Segment.

        }        

        if (ind == TxChan->BufCount)
        {
            TxChan->pDmaDesc[ind].Status |= (1UL << 21) ; ///Transmit End of Ring.
            TxChan->IdxIn = 0;
        } else
        {
            TxChan->IdxIn++;
        }

        TxChan->pDmaDesc[ind].Status |= EMAC_DMAOWN   ;

#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
        HAL_DCACHE_FLUSH(&TxChan->pDmaDesc[ind]);                                                                    
#endif            
        ind = TxChan->IdxIn;
        TxChan->pDmaDesc[ind].Status  = (1UL << 30);      ///IC Interrupt on Completion.      
        desc++;
    }

    priv->tx_fsize[start].ind = TxChan->IdxIn; //index for next frame
    priv->tx_fsize[start].num = desc; //index for next frame

    priv->tx_FreeDesc -= desc;

}
#endif
/**
*
* @brief  Send function 
*
* @param  sc -        pointer to the driver instance
* @param  sg_list -   Pointer to the buffers that contains packet
* @param  sg_len -    Number of buffers in sg_list
* @param  total_len - length of all packet
* @param  key       - key of packet
*
* @return none
*
*/
static void bf60x_eth_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, 
                            int sg_len, int total_len, unsigned long key)
{
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    emac_chan* TxChan = (emac_chan*) &priv->Tx;
    cyg_uint32 val, val32;

    debug1_printf( " Func: %s \n",   __FUNCTION__);

    // 2 descriptors for 1 send is possible
    if ( priv->tx_FreeDesc < 2) {
         debug1_printf( " No free descriptors\n");
        (sc->funs->eth_drv->tx_done)(sc, key, 1);
        return;
    }

    /// Disable EMAC interrupts to assure the buffer consistence
#ifndef CYGPKG_REDBOOT
    cyg_interrupt_disable();
#endif
    cyg_drv_interrupt_mask(priv->intr_vector);
//    cyg_drv_dsr_lock();

    fill_tx_desc(sc, sg_list, sg_len, total_len, key);

    /// Allow EMAC interrupts again
     
#ifndef CYGPKG_REDBOOT
    cyg_interrupt_enable();
#endif
    cyg_drv_interrupt_unmask(priv->intr_vector);
//    cyg_drv_dsr_unlock();

    HAL_READ_UINT32(priv->base + EMAC_DMA_STAT, val32);
    if (( val32& BITM_EMAC_DMA_STAT_TS) == ENUM_EMAC_DMA_STAT_TS_SUSPENDED)
    {
        HAL_READ_UINT32(priv->base + EMAC_DMA_TXDSC_CUR, val);
        debug1_printf( "SUSPENDED DMA_TXDSC_CUR: %x\n", val );
        HAL_WRITE_UINT32(priv->base + EMAC_DMA_TXPOLL , 0x1);
        asm volatile ("SSYNC;");
    } else  if(( val32& BITM_EMAC_DMA_STAT_TS) == ENUM_EMAC_DMA_STAT_TS_STOPPED)
    {
        HAL_WRITE_UINT32(priv->base + EMAC_DMA_TXDSC_ADDR, (cyg_uint32)&TxChan->pDmaDesc[TxChan->IdxIn]);
        asm volatile ("SSYNC;");
        HAL_READ_UINT32(priv->base + EMAC_DMA_OPMODE, val);
        val |=BITM_EMAC_DMA_OPMODE_ST;
        HAL_WRITE_UINT32(priv->base + EMAC_DMA_OPMODE, val);
        asm volatile ("SSYNC;");
        debug1_printf( "STOPPED\n" );
        
    }
     
    return;
}



/**
*
* @brief  Handler to packet transmit operation
*   
* @param  sc - pointer to the driver instance
*
* @return none
*
*/

static void bf60x_eth_tx(struct eth_drv_sc *sc)
{
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    emac_chan* TxChan = (emac_chan*) &priv->Tx;
    cyg_uint32 frameCount,i ;
    debug1_printf( " Func: %s \n",   __FUNCTION__);

    // cyg_uint32 freedesc = 0;
    // cyg_uint32 idxout = TxChan->IdxOut;
    
    // Count of transmitted frames 
    HAL_READ_UINT32(priv->base + EMAC_TXFRMCNT_GB, frameCount);
    // if(frameCount > 200 || priv->tx_FreeDesc < 90 ||( frameCount > 320 - priv->tx_FreeDesc))
    // diag_printf("tx_FreeDesc = %d, frameCount = %d\n",priv->tx_FreeDesc, frameCount);
    cyg_drv_dsr_lock();
 
    for (i = 0; i < frameCount; i++)
    {
        (sc->funs->eth_drv->tx_done)(sc, priv->tx_keys[TxChan->IdxOut], 0);
        priv->tx_FreeDesc += priv->tx_fsize[TxChan->IdxOut].num;
        TxChan->IdxOut = priv->tx_fsize[TxChan->IdxOut].ind;
    }
//    cyg_drv_dsr_lock();
    
    // priv->tx_FreeDesc += freedesc;
    // TxChan->IdxOut = idxout;
    
    cyg_drv_dsr_unlock();

}

/**
*
* @brief  Handler to packet receive operation
*   
* @param  sc - pointer to the driver instance
*
* @return none
*
*/
static void bf60x_eth_rx(struct eth_drv_sc *sc)
{
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    emac_chan* RxChan = (emac_chan*) &priv->Rx;
    
    debug1_printf( " Func: %s \n",   __FUNCTION__);

    RxChan->iGotFrame = 1;
    
    while(RxChan->iGotFrame)
    {
        cyg_uint32 rcvBufferControl;
      
        RxChan->iGotFrame = 0;
        HAL_DCACHE_INVALIDATE(&RxChan->pDmaDesc[RxChan->IdxIn])
        
        rcvBufferControl = RxChan->pDmaDesc[RxChan->IdxIn].Status;
        debug1_printf("RxStatus :: %x ",rcvBufferControl);
        debug1_printf("RxControl:: %x ",RxChan->pDmaDesc[RxChan->IdxIn].ControlDesc);

        if (0 == (rcvBufferControl & EMAC_DMAOWN))
        {
            cyg_int32 len;

            len = EMAC_RX_SIZE(rcvBufferControl);
            if (0 == len)
            {
               debug1_printf(" RxEvent:: rx_size == 0\n");
            }
            else
            {
               debug1_printf(" RxEvent:: rx_size == %x\n", len);
                (sc->funs->eth_drv->recv)(sc, len);
            }
        }
    }
}


/**
*
* @brief Recieve function
*
* @param  sc -        pointer to the driver instance
* @param  sg_list -   Pointer to the buffers to store the packet
* @param  sg_len -    Number of buffers in sg_list
*
* @return none
*
*/
static void
bf60x_eth_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, int sg_len)
{
    cyg_int32 i;
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    emac_chan* RxChan = (emac_chan*) &priv->Rx;
    cyg_addrword_t srcBufferAddr;
    cyg_uint32 val, val32;
    debug1_printf( " Func: %s \n",   __FUNCTION__);

    /// Copy the frames in the scatter-gather-list of the TCP/IP Stack
    if (NULL == sg_list)
    {
        return;
    }
 
    /// Read the buffer base address from the emac controller

#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
    hal_flushinv_area(RxChan->pDmaDesc[RxChan->IdxIn].StartAddr1, EMAC_RX_SIZE(RxChan->pDmaDesc[RxChan->IdxIn].Status));
#endif            

    srcBufferAddr = (cyg_addrword_t)RxChan->pDmaDesc[RxChan->IdxIn].StartAddr1;

    for (i = 0; i < sg_len && sg_list[i].buf; i++)
    {
        /// Copy the received frame to the next net stack level
        memcpy((void *)sg_list[i].buf, (void *)srcBufferAddr, sg_list[i].len);
        srcBufferAddr += sg_list[i].len;

    }

 
    /// Increase the buffer index and mark the read entry as free again

    RxChan->pDmaDesc[RxChan->IdxIn].ControlDesc |= EMAC_RCH;
    RxChan->pDmaDesc[RxChan->IdxIn].Status = (  EMAC_DMAOWN  );

#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
    HAL_DCACHE_FLUSH(&RxChan->pDmaDesc[RxChan->IdxIn]);                                                                    
#endif            

    /// Check if it is the last entry of the ring buffer
    if (RxChan->IdxIn == RxChan->BufCount)
    {
        /// start again from the beginning of the ring buffer
        RxChan->IdxIn = 0;
    }
    else
    {
        /// point to next entry
        RxChan->IdxIn ++;
    }

    RxChan->iGotFrame = 1;

 
    DBG_READ_UINT32(priv->base + EMAC_DMA_RXDSC_CUR, val);
    debug1_printf( " RXDSC_CUR: %x: IdxIn: %d ",val,RxChan->IdxIn );
    DBG_READ_UINT32(priv->base + EMAC_RXFRMCNT_GB, val);
    debug1_printf( " RXFRMCNT_GB: %d: \n",val);
        
   
    HAL_READ_UINT32(priv->base + EMAC_DMA_STAT, val32);
    if (( val32& BITM_EMAC_DMA_STAT_RS) == ENUM_EMAC_DMA_STAT_RS_SUSPENDED)
    {
        HAL_WRITE_UINT32(priv->base + EMAC_DMA_RXPOLL , 0x1);
        asm volatile ("SSYNC;");
        debug1_printf( " RS_SUSPENDED\n");
    }  else if(( val32& BITM_EMAC_DMA_STAT_RS) == ENUM_EMAC_DMA_STAT_RS_STOPPED)
    {
        HAL_WRITE_UINT32(priv->base + EMAC_DMA_RXDSC_ADDR, (cyg_uint32)&RxChan->pDmaDesc[RxChan->IdxIn]);
        asm volatile ("SSYNC;");
        HAL_READ_UINT32(priv->base + EMAC_DMA_OPMODE, val);
        val|=BITM_EMAC_DMA_OPMODE_SR;
        HAL_WRITE_UINT32(priv->base + EMAC_DMA_OPMODE, val);
        asm volatile ("SSYNC;");
        debug1_printf( " RS_STOPPED\n");
    }   
    
}
/**
*
* @brief  delivered service routine (second interrupt handler)
*   
* @param  sc - pointer to the driver instance
*
* @return none
*
*/
static void
bf60x_eth_deliver(struct eth_drv_sc *sc)
{
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    cyg_uint32 dma_status;
    debug1_printf( " Func: %s \n",   __FUNCTION__);
  
    HAL_READ_UINT32(priv->base + EMAC_DMA_STAT, dma_status);
    debug1_printf( " DMA_STATUS: %x \n",   dma_status);
 
    if ( dma_status )
    {      

        /* receive frame interrupt */
        if (dma_status & BITM_EMAC_DMA_STAT_RI)
        {
            //debug1_printf( " EMAC_DMA_STAT_RI \n");
            bf60x_eth_rx(sc);
        }

        /* transmit complete interrupt */
        if (dma_status &  BITM_EMAC_DMA_STAT_TI)
        {
          //  debug1_printf( " EMAC_DMA_STAT_TI \n");
            bf60x_eth_tx(sc);
        }

        /* abnormal interrupts - errors */
        if (dma_status & BITM_EMAC_DMA_STAT_AIS)
        {
            debug1_printf( " %x \n",   dma_status);
           // debug1_printf( " EMAC_DMA_STAT_AIS \n");
            // /* transmit process stopped */
            // if (dma_status & BITM_EMAC_DMA_STAT_TPS)
            // {
                // debug1_printf( " BITM_EMAC_DMA_STAT_TPS \n");
           // }
            
            // /* transmit jabber timeout */
            // if (dma_status & BITM_EMAC_DMA_STAT_TJT)
            // {
                // debug1_printf( " BITM_EMAC_DMA_STAT_TJT \n");
            // }

            // /* receive buffer unavailable */
            // if (dma_status & BITM_EMAC_DMA_STAT_RU) 
            // {
                // debug1_printf( " BITM_EMAC_DMA_STAT_RU \n");
            // }
            
            // /* receiver process stopped */
            // if (dma_status & BITM_EMAC_DMA_STAT_RPS) 
            // {    
                // debug1_printf( " BITM_EMAC_DMA_STAT_RPS \n");
            // }
            
            // /* transmit buffer unavailable */
            // if (dma_status & BITM_EMAC_DMA_STAT_UNF)
            // {
                // debug1_printf( " BITM_EMAC_DMA_STAT_UNF \n");
            // }
            
            // /* received watchdog timeout */
            // if (dma_status & BITM_EMAC_DMA_STAT_RWT) 
            // {
                // debug1_printf( " BITM_EMAC_DMA_STAT_RWT \n");
            // }
            
            // /* early transmit interrupt */
            // if (dma_status & BITM_EMAC_DMA_STAT_ETI) 
            // {
                // debug1_printf( " BITM_EMAC_DMA_STAT_ETI \n");
            // }
            
            // /* early transmit interrupt */
            // if (dma_status & BITM_EMAC_DMA_STAT_ERI) 
            // {
                // debug1_printf( " BITM_EMAC_DMA_STAT_ERI \n");
            // }
            
            // /* fatal buss error */
            // if (dma_status & BITM_EMAC_DMA_STAT_FBI) 
            // {
                // debug1_printf( " BITM_EMAC_DMA_STAT_FBI \n");
            // }

            // /* mmc interrupt */
            // if (dma_status & BITM_EMAC_DMA_STAT_MCI)
            // {
                // debug1_printf( " BITM_EMAC_DMA_STAT_MCI \n");
            // }

        // //           handle_abnormal_interrupts(pDev,dma_status);
        }
        /* IEEE-1588 time stamp trigger interrupt */
        if (dma_status & BITM_EMAC_DMA_STAT_TTI)
        {
            debug1_printf( " EMAC_DMA_STAT_TTI \n");
        }

        // /* memory management counter interrupts */
        if (dma_status & BITM_EMAC_DMA_STAT_MCI)
        {
            cyg_uint32 emac_istat;
            cyg_uint32 ipc_rxint;
            cyg_uint32 mmc_rxint;
            cyg_uint32 mmc_txint;
            HAL_READ_UINT32(priv->base + EMAC_ISTAT, emac_istat);
            debug1_printf( " EMAC_ISTAT: %x \n",   emac_istat);
            HAL_WRITE_UINT32(priv->base + EMAC_ISTAT,  0);
            asm volatile ("SSYNC;");
            HAL_WRITE_UINT32(priv->base + EMAC_MMC_CTL, 0x1);
            asm volatile ("SSYNC;");
            
            HAL_READ_UINT32(priv->base + EMAC_IPC_RXINT, ipc_rxint);
            debug1_printf( " EMAC_IPC_RXINT: %x \n", ipc_rxint);
            HAL_READ_UINT32(priv->base + EMAC_MMC_RXINT, mmc_rxint);
            debug1_printf( " EMAC_MMC_RXINT: %x \n", mmc_rxint);
            HAL_READ_UINT32(priv->base + EMAC_MMC_TXINT, mmc_txint);
            debug1_printf( " EMAC_MMC_TXINT: %x \n", mmc_txint);

        }

  
  
    }
    
    HAL_WRITE_UINT32(priv->base + EMAC_DMA_STAT,  dma_status & 0x1FFFF);
    asm volatile ("SSYNC;");
    
#ifndef CYGPKG_REDBOOT	// Commented
    cyg_interrupt_acknowledge(priv->intr_vector);
    cyg_drv_interrupt_unmask(priv->intr_vector);
#endif
}




// routine called to handle ethernet controller in polled mode
static void
bf60x_eth_poll(struct eth_drv_sc *sc)
{
    debug1_printf( " Func: %s \n",   __FUNCTION__);
  /* Service the buffers */
    bf60x_eth_deliver(sc);
}


static int
bf60x_eth_int_vector(struct eth_drv_sc *sc)
{
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    debug1_printf( " Func: %s \n",   __FUNCTION__);
    return(priv->intr_vector);
}
//======================================================================

#ifndef CYGPKG_REDBOOT
/**
*
* @brief  Interrupt handler of emac interrupt
*
* @param  vector  -  number of vector that calls this handler       
* @param  sc -        pointer to the driver instance
*
* @return information about handling an interrupt
*
*/
static cyg_uint32 bf60x_eth_isr (cyg_vector_t vector, cyg_addrword_t data)
{   
    struct eth_drv_sc *sc = (struct eth_drv_sc *)data;
    bf60x_eth_priv_t *priv = (bf60x_eth_priv_t *)sc->driver_private;
    debug1_printf( " Func: %s \n",   __FUNCTION__);
        
    cyg_drv_interrupt_mask(priv->intr_vector);
//    cyg_drv_interrupt_acknowledge(priv->intr_vector);

    return(CYG_ISR_HANDLED | CYG_ISR_CALL_DSR);
}
#endif

#ifdef CYGPKG_DEVS_ETH_BF60X_EMAC0

static bf60x_eth_priv_t bf60x_priv_data_eth0 = {
  .intr_vector = CYGNUM_HAL_INTERRUPT_EMAC0,
  .base = EMAC0_REGBASE,
  .phy = &bf60x_phy0,
  .isr = 0,
  .tx_keys = tx_keys_eth0,  
  .tx_fsize = tx_fsize_eth0,  
  .tx_FreeDesc  = CYGNUM_DEVS_ETH_BF60X_EMAC0_TX_BUFS,

  .Rx.pDataBuffer  = rxDataBuffer_eth0,
#ifdef CHAIN_MODE
  .Tx.pDataBuffer  = txDataBuffer_eth0,
#endif
  .Rx.pDmaDesc  = rxDmaDesc_eth0 ,
  .Tx.pDmaDesc  = txDmaDesc_eth0 ,
 
  .Rx.BufCount  = CYGNUM_DEVS_ETH_BF60X_EMAC0_RX_BUFS - 1,
  .Tx.BufCount  = CYGNUM_DEVS_ETH_BF60X_EMAC0_TX_BUFS - 1 ,
  .Rx.Recv  = 1,
  .Tx.Recv  = 0,
  .EMAC_num =  0,
  .FullDuplex = 1,
  .Port10 = 0,
  
};

 

ETH_DRV_SC(bf60x_sc_eth0,
		   &bf60x_priv_data_eth0,       // Driver specific data
           "eth0",                      // Name for this interface
           bf60x_eth_start,
           bf60x_eth_stop,
           bf60x_eth_control,
           bf60x_eth_can_send,
           bf60x_eth_send,
           bf60x_eth_recv,
           bf60x_eth_deliver,
           bf60x_eth_poll,
           bf60x_eth_int_vector);

NETDEVTAB_ENTRY(bf60x_netdev,
                "bf60x",
                bf60x_eth_init,
                &bf60x_sc_eth0);
#endif //CYGPKG_DEVS_ETH_BF60X_EMAC0
                
#ifdef CYGPKG_DEVS_ETH_BF60X_EMAC1
static bf60x_eth_priv_t bf60x_priv_data_eth1 = {
  .intr_vector = CYGNUM_HAL_INTERRUPT_EMAC1,
  .base = EMAC1_REGBASE,
  .phy = &bf60x_phy1
};

ETH_DRV_SC(bf60x_sc_eth1,
		   &bf60x_priv_data_eth1,       // Driver specific data
           "eth1",                      // Name for this interface
           bf60x_eth_start,
           bf60x_eth_stop,
           bf60x_eth_control,
           bf60x_eth_can_send,
           bf60x_eth_send,
           bf60x_eth_recv,
           bf60x_eth_deliver,
           bf60x_eth_poll,
           bf60x_eth_int_vector);

NETDEVTAB_ENTRY(bf60x_netdev,
                "bf60x",
                bf60x_eth_init,
                &bf60x_sc_eth1);
#endif // CYGPKG_DEVS_ETH_BF60X_EMAC1

// EOF if_bf60x.c

