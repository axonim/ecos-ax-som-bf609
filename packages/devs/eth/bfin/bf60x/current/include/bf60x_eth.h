#ifndef CYGONCE_HAL_BF60X_H
#define CYGONCE_HAL_BF60X_H

/** @file i2c_bfin_bf60x.h
 *  @brief Analog Devices BF60x (Blackfin) MAC driver defines
*/

//========================================================================
//
//      bf60x_eth.h
//
//      Analog Devices BF60x (Blackfin) MAC driver defines
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
// Author(s):     Siarhei Vasiliuk
// Date:          2013-10-11
// Purpose:       
// Description:   Analog Devices BF60x (Blackfin) MAC driver defines
// Usage:         
//
//####DESCRIPTIONEND####
//
//========================================================================

#define MAC_LOOP_COUNT              (100000)            /**< counter used as timeout for MAC operations */
#define PHY_LOOP_COUNT              (100000)            /** Loop count to check PHY busy status */
#define ETH_PACKET_SIZE             1540                /**< Size of an ethernet packet */

#define EMAC_DMAOWN                 (1UL << 31)         /**< Set when DMA owns the descriptor */     
#define EMAC_RCH                    (1UL << 14)         /**< RDES1: Second Address Chained */
#define EMAC_RX_SIZE(x)             ((x >> 16) & 0x00007FFF )/**< Receive Frame Length*/
#define EMAC_VLAN(x)                ((x >> 10) & 0x1)        /**< VLAN bit */


/**
 * Macros to set GEMAC MII ADDR register values
 */
#define SET_PHYAD(x) ( ((x) & 0x1F) << 11 )  /**< Set PHY address          */
#define SET_REGAD(x) ( ((x) & 0x1F) << 6  )  /**< Set PHY register address */
#define SET_CR(x)    ( ((x) & 0x0F) << 2  )  /**< Set PHY clock range      */

#define PHY1_ADDRESS     (1)                 /**< PHY 1 address                   */
#define PHY2_ADDRESS     (2)                 /**< PHY 2 address                   */
#define NUM_PHY_REGS     (0x20)              /**< Number of registers             */



/** DP83848 phy regsiter address */

#define REG_PHY_MODECTL             (0)      /**<   PHY Mode control register      */

#define PHY_MODECTL_RESET          (0x8000)  /**<RW Reset phy, self clearing       */    
#define PHY_MODECTL_LOOPBACK       (0x4000)  /**<RW 1:enable loopback 0:disable    */   
#define PHY_MODECTL_SPEED_SELECT   (0x2000)  /**<RW Speed, 1:100Mbps, 0:10Mbps     */ 
#define PHY_MODECTL_AUTO_NEGO_ENA  (0x1000)  /**<RW 1: Auto-negotiation on, 0: off */
#define PHY_MODECTL_POWER_DOWN     (0x0800)  /**<RW 1: power down, 0: normal       */
#define PHY_MODECTL_ISOLATE        (0x0400)  /**<RW not supported                  */
#define PHY_MODECTL_RESTART_A_NEGO (0x0200)  /**<RW 1: restart Auto-negotiation, 0: normal */
#define PHY_MODECTL_DUPLEX_MODE    (0x0100)  /**<RW 1: full duplex, 0: half duplex */
#define PHY_MODECTL_COLLISION_TEST (0x0080)  /**<RO not supported                  */

#define REG_PHY_MODESTAT            (1)      /**< Basic Status Register           */
#define PHY_MODESTAT_100BASE_T4    (0x8000)  /**< Status 100Base full T4          */
#define PHY_MODESTAT_100BASE_FULL  (0x4000)  /**< Status 100Base full duplex      */       
#define PHY_MODESTAT_100BASE_HALF  (0x2000)  /**< Status 100Base half duplex      */ 
#define PHY_MODESTAT_10BASE_FULL   (0x1000)  /**< Status 10Base full duplex      */
#define PHY_MODESTAT_10BASE_HALF   (0x0800)  /**< Status 10Base half duplex      */ 
#define PHY_MODESTAT_MF_PREAMBLE   (0x0400)  /**< Status preamble                */   
#define PHY_MODESTAT_AUTO_NEGO_COM (0x0020)  /**< Status Autonegotiation complete */  
#define PHY_MODESTAT_REMOTE_FAULT  (0x0010)  /**< Remote fault detected          */  
#define PHY_MODESTAT_AUTO_NEGO_ABILITY (0x0008) /**< Autoneotiation capable      */   
#define PHY_MODESTAT_LINK_STATUS   (0x0004)     /**< Link status                 */
#define PHY_MODESTAT_JABBER_DETECT (0x0002)     /**< Jabber detected             */
#define PHY_MODESTAT_EXT_CAPABILIT (0x0001)     /**< Extended capabilities       */

#define REG_PHY_PHYID1              (2)      /**< PHY Identifier High           */
#define REG_PHY_PHYID2              (3)      /**< PHY Identifier Low            */
#define REG_PHY_ANAR                (4)      /**< Auto-Negotiation Advertisement Register */
#define PHY_ANAR_NEXT_PAGE         (0x8000)  /**< Autonegotiation next page register */ 
#define PHY_ANAR_REMOTE_FAULT      (0x2000)  /**< Remote fault */  
#define PHY_ANAR_ASM_DIR           (0x0800)  /**<Asymmetric PAUSE Support for Full Duplex Links */
#define PHY_ANAR_PAUSE_OPERATION   (0x0400)  /**< Supports Pause operations */
#define PHY_ANAR_100BASE_T4        (0x0200)  /**<TX 100BASE-T4 Support */
#define PHY_ANAR_100BASE_FULL      (0x0100)  /**< Autonegotiation adv 100 Base full */  
#define PHY_ANAR_100BASE_HALF      (0x0080)  /**< Autonegotiation adv 100 Base half */ 
#define PHY_ANAR_10BASE_FULL       (0x0040)  /**< Autonegotiation adv 10 Base full  */
#define PHY_ANAR_10BASE_HALF       (0x0020)  /**< Autonegotiation adv 10 Base half  */


#define REG_PHY_ANLPAR              (5)      /**< Auto-Negotiation Link Partner Ability Register */
#define PHY_ANLPAR_NP              (0x8000)  /**< Next page indication                           */
#define PHY_ANLPAR_ACK             (0x4000)  /**< Acknowledge                                    */
#define PHY_ANLPAR_RF              (0x2000)  /**< Remote Fault                                   */
#define PHY_ANLPAR_ASM_DIR         (0x0800)  /**< Asymmetric PAUSE Support for Full Duplex Links:*/
#define PHY_ANLPAR_PAUSE_OPERATION (0x0400)  /**< Pause operation is supported by remote MAC     */
#define PHY_ANLPAR_100BASE_T4      (0x0200)  /**< TX 100BASE-T4 Support                          */
#define PHY_ANLPAR_100BASE_DUPLEX  (0x0100)  /**< TX with full duplex                            */
#define PHY_ANLPAR_100BASE_HALF    (0x0080)  /**< TX with half duplex                            */
#define PHY_ANLPAR_10BASE_DUPLEX   (0x0040)  /**< 10Mbps with full duplex                        */
#define PHY_ANLPAR_10BASE_HALF     (0x0020)  /**<  10Mbps with half duplex                       */

#define REG_PHY_ANER                 (6)      /**< Auto_negotiation Expansion Register           */
#define PHY_ANER_PARALLEL_DETECTION (0x0010)  /**< fault dected by parallel detction logic       */
#define PHY_ANER_LP_NEXT_PAGE       (0x0008)  /**< link partner has next page ability            */
#define PHY_ANER_NEXT_PAGE_ABLE     (0x0004)  /**< local device has next page ability            */
#define PHY_ANER_PAGE_RECEIVED      (0x0002)  /**< new page received                             */
#define PHY_ANER_LP_ANG_ABLE        (0x0001)  /**< link partner has auto-negotiation ability     */

#define REG_PHY_STS                  (16)     /**< PHY Status Register                           */
#define PHY_STS_MDI_X               (0x4000)  /**< MDI-X mode as reported by auto-nego logic     */
#define PHY_STS_REV_ERROR_LATCH     (0x2000)  /**< Receive Error Latch                          */
#define PHY_STS_POLARITY_STATUS     (0x1000)  /**< Polarity Status                               */
#define PHY_STS_FALSE_CSL           (0x0800)  /**<  False Carrier Sense Latch                    */
#define PHY_STS_SIGNAL_DETECT       (0x0400)  /**< 100Base-TX unconditional Signal DetectfromPMD */
#define PHY_STS_DESCRAMBLER_LOCK    (0x0200)  /**< 100Base-TX Descrambler Lock from PMD.         */
#define PHY_STS_PAGE_RECEIVED       (0x0100)  /**< Link Code Word Page Received                  */
#define PHY_STS_REMOTE_FAULT        (0x0040)  /**< Remote Fault:                                 */
#define PHY_STS_JAVVER_DETECT       (0x0020)  /**< Jabber Detect:only applicable in 10 Mb/s mode */
#define PHY_STS_AUTO_NEG_COMPLETE   (0x0010)  /**< Auto-Negotiation Complete                    */
#define PHY_STS_LOOPBACK_STATUS     (0x0008)  /**< Loopback                                     */
#define PHY_STS_DUPLEX_STATUS       (0x0004)  /**< Duplex Status                                */
#define PHY_STS_SPEED_STATUS        (0x0002)  /**< Speed Status                                 */
#define PHY_STS_LINK_STATUS         (0x0001)  /**< Link Status                                  */

#define REG_PHY_MICR                 (17)     /**< MII Interrupt Control Register               */
#define PHY_MICR_TINT               (0x0004)  /**< Test Interrupt                               */
#define PHY_MICR_INTEN              (0x0002)  /**< Interrupt Enable                             */
#define PHY_MICR_INT_OE             (0x0001)  /**< Interrupt Output Enable                      */

#define REG_PHY_MISR                 (18)     /**< MII Interrupt Status Register                */
#define PHY_MISR_RHFINT_EN          (0x0001)  /**< Enable Interrupt on RECR half full event     */
#define PHY_MISR_FHFINT_EN          (0x0002)  /**< Enable Interrupt on FCCR half full event     */
#define PHY_MISR_ANCINT_EN          (0x0004)  /**< Enable Interrupt on AN complete              */
#define PHY_MISR_SPDINT_EN          (0x0010)  /**< Enable Speed Status Change Interrupt         */
#define PHY_MISR_LINKINT_EN         (0x0020)  /**< Enable Link Status Change Interrupt          */
#define PHY_MISR_EDINT_EN           (0x0040)  /**< Enable Energy Detect Interrupt               */
#define PHY_MISR_RHFINT             (0x0100)  /**< RECR half full event                         */
#define PHY_MISR_FHFINT             (0x0200)  /**< FCCR half full event                         */
#define PHY_MISR_ANCINT             (0x0400)  /**< Autonegotiation completed                    */
#define PHY_MISR_SPDINT             (0x1000)  /**< Speed Status interrupt                       */
#define PHY_MISR_LINKINT            (0x2000)  /**< Link Status Change Interrupt                 */
#define PHY_MISR_EDINT              (0x4000)  /**< Energy Detect Interrupt                      */

#define REG_PHY_CR                   (25)     /**< PHY Control Register                         */
#define PHY_CR_MDIX_EN              (0x8000)  /**< Auto-MDIX Enable                             */
#define PHY_CR_FORCE_MDIX           (0x4000)  /**< Force MDIX                                   */
#define PHY_CR_PAUSE_RX             (0x2000)  /**< Pause Receive Negotiated                     */
#define PHY_CR_PAUSE_TX             (0x1000)  /**< Pause Transmit Negotiated                    */
#define PHY_CR_BIST_FE              (0x0800)  /**< BIST Force Error                             */
#define PHY_CR_PSR_15               (0x0400)  /**< BIST Sequence select                         */
#define PHY_CR_BIST_STATUS          (0x0200)  /**< BIST Test Status                             */
#define PHY_CR_BIST_START           (0x0100)  /**< BIST BIST Start                              */
#define PHY_CR_BP_STRETCH           (0x0080)  /**< Bypass LED Stretching                        */
#define PHY_CR_LED_CNFG             (0x0020)  /**< LED Configuration                            */
#define PHY_CR_PHYADDR              (0x001f)  /**< PHY Address                                  */

/**< =========================
        EMAC REGISTERS OFFSET
     ========================= */
#define EMAC_MACCFG                     0x0000         /**< EMAC MAC Configuration Register */
#define EMAC_MACFRMFILT                 0x0004         /**< EMAC MAC Rx Frame Filter Register */
#define EMAC_HASHTBL_HI                 0x0008         /**< EMAC Hash Table High Register */
#define EMAC_HASHTBL_LO                 0x000C         /**< EMAC Hash Table Low Register */
#define EMAC_SMI_ADDR                   0x0010         /**< EMAC SMI Address Register */
#define EMAC_SMI_DATA                   0x0014         /**< EMAC SMI Data Register */
#define EMAC_FLOWCTL                    0x0018         /**< EMAC FLow Control Register */
#define EMAC_VLANTAG                    0x001C         /**< EMAC VLAN Tag Register */
#define EMAC_DBG                        0x0024         /**< EMAC Debug Register */
#define EMAC_ISTAT                      0x0038         /**< EMAC Interrupt Status Register */
#define EMAC_IMSK                       0x003C         /**< EMAC Interrupt Mask Register */
#define EMAC_ADDR0_HI                   0x0040         /**< EMAC MAC Address 0 High Register */
#define EMAC_ADDR0_LO                   0x0044         /**< EMAC MAC Address 0 Low Register */
#define EMAC_MMC_CTL                    0x0100         /**< EMAC MMC Control Register */
#define EMAC_MMC_RXINT                  0x0104         /**< EMAC MMC Rx Interrupt Register */
#define EMAC_MMC_TXINT                  0x0108         /**< EMAC MMC Tx Interrupt Register */
#define EMAC_MMC_RXIMSK                 0x010C         /**< EMAC MMC Rx Interrupt Mask Register */
#define EMAC_MMC_TXIMSK                 0x0110         /**< EMAC MMC TX Interrupt Mask Register */
#define EMAC_TXOCTCNT_GB                0x0114         /**< EMAC Tx OCT Count (Good/Bad) Register */
#define EMAC_TXFRMCNT_GB                0x0118         /**< EMAC Tx Frame Count (Good/Bad) Register */
#define EMAC_TXBCASTFRM_G               0x011C         /**< EMAC Tx Broadcast Frames (Good) Register */
#define EMAC_TXMCASTFRM_G               0x0120         /**< EMAC Tx Multicast Frames (Good) Register */
#define EMAC_TX64_GB                    0x0124         /**< EMAC Tx 64-Byte Frames (Good/Bad) Register */
#define EMAC_TX65TO127_GB               0x0128         /**< EMAC Tx 65- to 127-Byte Frames (Good/Bad) Register */
#define EMAC_TX128TO255_GB              0x012C         /**< EMAC Tx 128- to 255-Byte Frames (Good/Bad) Register */
#define EMAC_TX256TO511_GB              0x0130         /**< EMAC Tx 256- to 511-Byte Frames (Good/Bad) Register */
#define EMAC_TX512TO1023_GB             0x0134         /**< EMAC Tx 512- to 1023-Byte Frames (Good/Bad) Register */
#define EMAC_TX1024TOMAX_GB             0x0138         /**< EMAC Tx 1024- to Max-Byte Frames (Good/Bad) Register */
#define EMAC_TXUCASTFRM_GB              0x013C         /**< EMAC Tx Unicast Frames (Good/Bad) Register */
#define EMAC_TXMCASTFRM_GB              0x0140         /**< EMAC Tx Multicast Frames (Good/Bad) Register */
#define EMAC_TXBCASTFRM_GB              0x0144         /**< EMAC Tx Broadcast Frames (Good/Bad) Register */
#define EMAC_TXUNDR_ERR                 0x0148         /**< EMAC Tx Underflow Error Register */
#define EMAC_TXSNGCOL_G                 0x014C         /**< EMAC Tx Single Collision (Good) Register */
#define EMAC_TXMULTCOL_G                0x0150         /**< EMAC Tx Multiple Collision (Good) Register */
#define EMAC_TXDEFERRED                 0x0154         /**< EMAC Tx Deferred Register */
#define EMAC_TXLATECOL                  0x0158         /**< EMAC Tx Late Collision Register */
#define EMAC_TXEXCESSCOL                0x015C         /**< EMAC Tx Excess Collision Register */
#define EMAC_TXCARR_ERR                 0x0160         /**< EMAC Tx Carrier Error Register */
#define EMAC_TXOCTCNT_G                 0x0164         /**< EMAC Tx Octet Count (Good) Register */
#define EMAC_TXFRMCNT_G                 0x0168         /**< EMAC Tx Frame Count (Good) Register */
#define EMAC_TXEXCESSDEF                0x016C         /**< EMAC Tx Excess Deferral Register */
#define EMAC_TXPAUSEFRM                 0x0170         /**< EMAC Tx Pause Frame Register */
#define EMAC_TXVLANFRM_G                0x0174         /**< EMAC Tx VLAN Frames (Good) Register */
#define EMAC_RXFRMCNT_GB                0x0180         /**< EMAC Rx Frame Count (Good/Bad) Register */
#define EMAC_RXOCTCNT_GB                0x0184         /**< EMAC Rx Octet Count (Good/Bad) Register */
#define EMAC_RXOCTCNT_G                 0x0188         /**< EMAC Rx Octet Count (Good) Register */
#define EMAC_RXBCASTFRM_G               0x018C         /**< EMAC Rx Broadcast Frames (Good) Register */
#define EMAC_RXMCASTFRM_G               0x0190         /**< EMAC Rx Multicast Frames (Good) Register */
#define EMAC_RXCRC_ERR                  0x0194         /**< EMAC Rx CRC Error Register */
#define EMAC_RXALIGN_ERR                0x0198         /**< EMAC Rx alignment Error Register */
#define EMAC_RXRUNT_ERR                 0x019C         /**< EMAC Rx Runt Error Register */
#define EMAC_RXJAB_ERR                  0x01A0         /**< EMAC Rx Jab Error Register */
#define EMAC_RXUSIZE_G                  0x01A4         /**< EMAC Rx Undersize (Good) Register */
#define EMAC_RXOSIZE_G                  0x01A8         /**< EMAC Rx Oversize (Good) Register */
#define EMAC_RX64_GB                    0x01AC         /**< EMAC Rx 64-Byte Frames (Good/Bad) Register */
#define EMAC_RX65TO127_GB               0x01B0         /**< EMAC Rx 65- to 127-Byte Frames (Good/Bad) Register */
#define EMAC_RX128TO255_GB              0x01B4         /**< EMAC Rx 128- to 255-Byte Frames (Good/Bad) Register */
#define EMAC_RX256TO511_GB              0x01B8         /**< EMAC Rx 256- to 511-Byte Frames (Good/Bad) Register */
#define EMAC_RX512TO1023_GB             0x01BC         /**< EMAC Rx 512- to 1023-Byte Frames (Good/Bad) Register */
#define EMAC_RX1024TOMAX_GB             0x01C0         /**< EMAC Rx 1024- to Max-Byte Frames (Good/Bad) Register */
#define EMAC_RXUCASTFRM_G               0x01C4         /**< EMAC Rx Unicast Frames (Good) Register */
#define EMAC_RXLEN_ERR                  0x01C8         /**< EMAC Rx Length Error Register */
#define EMAC_RXOORTYPE                  0x01CC         /**< EMAC Rx Out Of Range Type Register */
#define EMAC_RXPAUSEFRM                 0x01D0         /**< EMAC Rx Pause Frames Register */
#define EMAC_RXFIFO_OVF                 0x01D4         /**< EMAC Rx FIFO Overflow Register */
#define EMAC_RXVLANFRM_GB               0x01D8         /**< EMAC Rx VLAN Frames (Good/Bad) Register */
#define EMAC_RXWDOG_ERR                 0x01DC         /**< EMAC Rx Watch Dog Error Register */
#define EMAC_IPC_RXIMSK                 0x0200         /**< EMAC MMC IPC Rx Interrupt Mask Register */
#define EMAC_IPC_RXINT                  0x0208         /**< EMAC MMC IPC Rx Interrupt Register */
#define EMAC_RXIPV4_GD_FRM              0x0210         /**< EMAC Rx IPv4 Datagrams (Good) Register */
#define EMAC_RXIPV4_HDR_ERR_FRM         0x0214         /**< EMAC Rx IPv4 Datagrams Header Errors Register */
#define EMAC_RXIPV4_NOPAY_FRM           0x0218         /**< EMAC Rx IPv4 Datagrams No Payload Frame Register */
#define EMAC_RXIPV4_FRAG_FRM            0x021C         /**< EMAC Rx IPv4 Datagrams Fragmented Frames Register */
#define EMAC_RXIPV4_UDSBL_FRM           0x0220         /**< EMAC Rx IPv4 UDP Disabled Frames Register */
#define EMAC_RXIPV6_GD_FRM              0x0224         /**< EMAC Rx IPv6 Datagrams Good Frames Register */
#define EMAC_RXIPV6_HDR_ERR_FRM         0x0228         /**< EMAC Rx IPv6 Datagrams Header Error Frames Register */
#define EMAC_RXIPV6_NOPAY_FRM           0x022C         /**< EMAC Rx IPv6 Datagrams No Payload Frames Register */
#define EMAC_RXUDP_GD_FRM               0x0230         /**< EMAC Rx UDP Good Frames Register */
#define EMAC_RXUDP_ERR_FRM              0x0234         /**< EMAC Rx UDP Error Frames Register */
#define EMAC_RXTCP_GD_FRM               0x0238         /**< EMAC Rx TCP Good Frames Register */
#define EMAC_RXTCP_ERR_FRM              0x023C         /**< EMAC Rx TCP Error Frames Register */
#define EMAC_RXICMP_GD_FRM              0x0240         /**< EMAC Rx ICMP Good Frames Register */
#define EMAC_RXICMP_ERR_FRM             0x0244         /**< EMAC Rx ICMP Error Frames Register */
#define EMAC_RXIPV4_GD_OCT              0x0250         /**< EMAC Rx IPv4 Datagrams Good Octets Register */
#define EMAC_RXIPV4_HDR_ERR_OCT         0x0254         /**< EMAC Rx IPv4 Datagrams Header Errors Register */
#define EMAC_RXIPV4_NOPAY_OCT           0x0258         /**< EMAC Rx IPv4 Datagrams No Payload Octets Register */
#define EMAC_RXIPV4_FRAG_OCT            0x025C         /**< EMAC Rx IPv4 Datagrams Fragmented Octets Register */
#define EMAC_RXIPV4_UDSBL_OCT           0x0260         /**< EMAC Rx IPv4 UDP Disabled Octets Register */
#define EMAC_RXIPV6_GD_OCT              0x0264         /**< EMAC Rx IPv6 Good Octets Register */
#define EMAC_RXIPV6_HDR_ERR_OCT         0x0268         /**< EMAC Rx IPv6 Header Errors Register */
#define EMAC_RXIPV6_NOPAY_OCT           0x026C         /**< EMAC Rx IPv6 No Payload Octets Register */
#define EMAC_RXUDP_GD_OCT               0x0270         /**< EMAC Rx UDP Good Octets Register */
#define EMAC_RXUDP_ERR_OCT              0x0274         /**< EMAC Rx UDP Error Octets Register */
#define EMAC_RXTCP_GD_OCT               0x0278         /**< EMAC Rx TCP Good Octets Register */
#define EMAC_RXTCP_ERR_OCT              0x027C         /**< EMAC Rx TCP Error Octets Register */
#define EMAC_RXICMP_GD_OCT              0x0280         /**< EMAC Rx ICMP Good Octets Register */
#define EMAC_RXICMP_ERR_OCT             0x0284         /**< EMAC Rx ICMP Error Octets Register */
#define EMAC_TM_CTL                     0x0700         /**< EMAC Time Stamp Control Register */
#define EMAC_TM_SUBSEC                  0x0704         /**< EMAC Time Stamp Sub Second Increment Register */
#define EMAC_TM_SEC                     0x0708         /**< EMAC Time Stamp Low Seconds Register */
#define EMAC_TM_NSEC                    0x070C         /**< EMAC Time Stamp Nanoseconds Register */
#define EMAC_TM_SECUPDT                 0x0710         /**< EMAC Time Stamp Seconds Update Register */
#define EMAC_TM_NSECUPDT                0x0714         /**< EMAC Time Stamp Nanoseconds Update Register */
#define EMAC_TM_ADDEND                  0x0718         /**< EMAC Time Stamp Addend Register */
#define EMAC_TM_TGTM                    0x071C         /**< EMAC Time Stamp Target Time Seconds Register */
#define EMAC_TM_NTGTM                   0x0720         /**< EMAC Time Stamp Target Time Nanoseconds Register */
#define EMAC_TM_HISEC                   0x0724         /**< EMAC Time Stamp High Second Register */
#define EMAC_TM_STMPSTAT                0x0728         /**< EMAC Time Stamp Status Register */
#define EMAC_TM_PPSCTL                  0x072C         /**< EMAC PPS Control Register */
#define EMAC_TM_AUXSTMP_NSEC            0x0730         /**< EMAC Time Stamp Auxiliary TS Nano Seconds Register */
#define EMAC_TM_AUXSTMP_SEC             0x0734         /**< EMAC Time Stamp Auxiliary TM Seconds Register */
#define EMAC_TM_PPSINTVL                0x0760         /**< EMAC Time Stamp PPS Interval Register */
#define EMAC_TM_PPSWIDTH                0x0764         /**< EMAC PPS Width Register */
#define EMAC_DMA_BUSMODE                0x1000         /**< EMAC DMA Bus Mode Register */
#define EMAC_DMA_TXPOLL                 0x1004         /**< EMAC DMA Tx Poll Demand Register */
#define EMAC_DMA_RXPOLL                 0x1008         /**< EMAC DMA Rx Poll Demand register */
#define EMAC_DMA_RXDSC_ADDR             0x100C         /**< EMAC DMA Rx Descriptor List Address Register */
#define EMAC_DMA_TXDSC_ADDR             0x1010         /**< EMAC DMA Tx Descriptor List Address Register */
#define EMAC_DMA_STAT                   0x1014         /**< EMAC DMA Status Register */
#define EMAC_DMA_OPMODE                 0x1018         /**< EMAC DMA Operation Mode Register */
#define EMAC_DMA_IEN                    0x101C         /**< EMAC DMA Interrupt Enable Register */
#define EMAC_DMA_MISS_FRM               0x1020         /**< EMAC DMA Missed Frame Register */
#define EMAC_DMA_RXIWDOG                0x1024         /**< EMAC DMA Rx Interrupt Watch Dog Register */
#define EMAC_DMA_BMMODE                 0x1028         /**< EMAC DMA SCB Bus Mode Register */
#define EMAC_DMA_BMSTAT                 0x102C         /**< EMAC DMA SCB Status Register */
#define EMAC_DMA_TXDSC_CUR              0x1048         /**< EMAC DMA Tx Descriptor Current Register */
#define EMAC_DMA_RXDSC_CUR              0x104C         /**< EMAC DMA Rx Descriptor Current Register */
#define EMAC_DMA_TXBUF_CUR              0x1050         /**< EMAC DMA Tx Buffer Current Register */
#define EMAC_DMA_RXBUF_CUR              0x1054         /**< EMAC DMA Rx Buffer Current Register */        
/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_MACCFG                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_MACCFG_CST                 25                               /**< CRC Stripping */
#define BITP_EMAC_MACCFG_WD                  23                               /**< Watch Dog Disable */
#define BITP_EMAC_MACCFG_JB                  22                               /**< Jabber Disable */
#define BITP_EMAC_MACCFG_JE                  20                               /**< Jumbo Frame Enable */
#define BITP_EMAC_MACCFG_IFG                 17                               /**< Inter-Frame Gap */
#define BITP_EMAC_MACCFG_DCRS                16                               /**< Disable Carrier Sense */
#define BITP_EMAC_MACCFG_FES                 14                               /**< Speed of Operation */
#define BITP_EMAC_MACCFG_DO                  13                               /**< Disable Receive Own */
#define BITP_EMAC_MACCFG_LM                  12                               /**< Loopback Mode */
#define BITP_EMAC_MACCFG_DM                  11                               /**< Duplex Mode */
#define BITP_EMAC_MACCFG_IPC                 10                               /**< IP Checksum */
#define BITP_EMAC_MACCFG_DR                   9                               /**< Disable Retry */
#define BITP_EMAC_MACCFG_ACS                  7                               /**< Automatic Pad/CRC Stripping */
#define BITP_EMAC_MACCFG_BL                   5                               /**< Back Off Limit */
#define BITP_EMAC_MACCFG_DC                   4                               /**< Deferral Check */
#define BITP_EMAC_MACCFG_TE                   3                               /**< Transmitter Enable */
#define BITP_EMAC_MACCFG_RE                   2                               /**< Receiver Enable */
#define BITM_EMAC_MACCFG_CST                 0x02000000                       /**< CRC Stripping */
#define BITM_EMAC_MACCFG_WD                  0x00800000                       /**< Watch Dog Disable */
#define BITM_EMAC_MACCFG_JB                  0x00400000                       /**< Jabber Disable */
#define BITM_EMAC_MACCFG_JE                  0x00100000                       /**< Jumbo Frame Enable */

#define BITM_EMAC_MACCFG_IFG                 0x000E0000                       /**< Inter-Frame Gap */
#define ENUM_EMAC_MACCFG_BIT_TIMES_96        0x00000000                       /**< IFG: 96 bit times */
#define ENUM_EMAC_MACCFG_BIT_TIMES_88        0x00020000                       /**< IFG: 88 bit times */
#define ENUM_EMAC_MACCFG_BIT_TIMES_80        0x00040000                       /**< IFG: 80 bit times */
#define ENUM_EMAC_MACCFG_BIT_TIMES_72        0x00060000                       /**< IFG: 72 bit times */
#define ENUM_EMAC_MACCFG_BIT_TIMES_64        0x00080000                       /**< IFG: 64 bit times */
#define ENUM_EMAC_MACCFG_BIT_TIMES_56        0x000A0000                       /**< IFG: 56 bit times */
#define ENUM_EMAC_MACCFG_BIT_TIMES_48        0x000C0000                       /**< IFG: 48 bit times */
#define ENUM_EMAC_MACCFG_BIT_TIMES_40        0x000E0000                       /**< IFG: 40 bit times */
#define BITM_EMAC_MACCFG_DCRS                0x00010000                       /**< Disable Carrier Sense */
#define BITM_EMAC_MACCFG_FES                 0x00004000                       /**< Speed of Operation */
#define BITM_EMAC_MACCFG_DO                  0x00002000                       /**< Disable Receive Own */
#define BITM_EMAC_MACCFG_LM                  0x00001000                       /**< Loopback Mode */
#define BITM_EMAC_MACCFG_DM                  0x00000800                       /**< Duplex Mode */
#define BITM_EMAC_MACCFG_IPC                 0x00000400                       /**< IP Checksum */

#define BITM_EMAC_MACCFG_DR                  0x00000200                       /**< Disable Retry */
#define ENUM_EMAC_MACCFG_RETRY_ENABLED       0x00000000                       /**< DR: Retry enabled */
#define ENUM_EMAC_MACCFG_RETRY_DISABLED      0x00000200                       /**< DR: Retry disabled */
#define BITM_EMAC_MACCFG_ACS                 0x00000080                       /**< Automatic Pad/CRC Stripping */

#define BITM_EMAC_MACCFG_BL                  0x00000060                       /**< Back Off Limit */
#define ENUM_EMAC_MACCFG_BL_10               0x00000000                       /**< BL: k = min (n, 10) */
#define ENUM_EMAC_MACCFG_BL_8                0x00000020                       /**< BL: k = min (n, 8) */
#define ENUM_EMAC_MACCFG_BL_4                0x00000040                       /**< BL: k = min (n, 4) */
#define ENUM_EMAC_MACCFG_BL_1                0x00000060                       /**< BL: k = min (n, 1) */
#define BITM_EMAC_MACCFG_DC                  0x00000010                       /**< Deferral Check */
#define BITM_EMAC_MACCFG_TE                  0x00000008                       /**< Transmitter Enable */
#define BITM_EMAC_MACCFG_RE                  0x00000004                       /**< Receiver Enable */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_MACFRMFILT                      Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_MACFRMFILT_RA              31                               /**< Receive All Frames */
#define BITP_EMAC_MACFRMFILT_HPF             10                               /**< Hash or Perfect Filter */
#define BITP_EMAC_MACFRMFILT_PCF              6                               /**< Pass Control Frames */
#define BITP_EMAC_MACFRMFILT_DBF              5                               /**< Disable Broadcast Frames */
#define BITP_EMAC_MACFRMFILT_PM               4                               /**< Pass All Multicast Frames */
#define BITP_EMAC_MACFRMFILT_DAIF             3                               /**< Destination Address Inverse Filtering */
#define BITP_EMAC_MACFRMFILT_HMC              2                               /**< Hash Multicast */
#define BITP_EMAC_MACFRMFILT_HUC              1                               /**< Hash Unicast */
#define BITP_EMAC_MACFRMFILT_PR               0                               /**< Promiscuous Mode */
#define BITM_EMAC_MACFRMFILT_RA              0x80000000                       /**< Receive All Frames */
#define BITM_EMAC_MACFRMFILT_HPF             0x00000400                       /**< Hash or Perfect Filter */

#define BITM_EMAC_MACFRMFILT_PCF             0x000000C0                       /**< Pass Control Frames */
#define ENUM_EMAC_MACFRMFILT_FILT_ALL        0x00000000                       /**< PCF: Pass no control frames */
#define ENUM_EMAC_MACFRMFILT_NO_PAUSE        0x00000040                       /**< PCF: Pass no PAUSE frames */
#define ENUM_EMAC_MACFRMFILT_FWD_ALL         0x00000080                       /**< PCF: Pass all control frames */
#define ENUM_EMAC_MACFRMFILT_PADR_FILT       0x000000C0                       /**< PCF: Pass address filtered control frames */

#define BITM_EMAC_MACFRMFILT_DBF             0x00000020                       /**< Disable Broadcast Frames */
#define ENUM_EMAC_MACFRMFILT_DIS_BCAST       0x00000000                       /**< DBF: AFM module passes all received broadcast frames */
#define ENUM_EMAC_MACFRMFILT_EN_BCAST        0x00000020                       /**< DBF: AFM module filters all incoming broadcast frames */
#define BITM_EMAC_MACFRMFILT_PM              0x00000010                       /**< Pass All Multicast Frames */
#define BITM_EMAC_MACFRMFILT_DAIF            0x00000008                       /**< Destination Address Inverse Filtering */
#define BITM_EMAC_MACFRMFILT_HMC             0x00000004                       /**< Hash Multicast */
#define BITM_EMAC_MACFRMFILT_HUC             0x00000002                       /**< Hash Unicast */
#define BITM_EMAC_MACFRMFILT_PR              0x00000001                       /**< Promiscuous Mode */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_SMI_ADDR                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_SMI_ADDR_PA                11                               /**< Physical Layer Address */
#define BITP_EMAC_SMI_ADDR_SMIR               6                               /**< SMI Register Address */
#define BITP_EMAC_SMI_ADDR_CR                 2                               /**< Clock Range */
#define BITP_EMAC_SMI_ADDR_SMIW               1                               /**< SMI Write */
#define BITP_EMAC_SMI_ADDR_SMIB               0                               /**< SMI Busy */
#define BITM_EMAC_SMI_ADDR_PA                0x0000F800                       /**< Physical Layer Address */
#define BITM_EMAC_SMI_ADDR_SMIR              0x000007C0                       /**< SMI Register Address */
#define BITM_EMAC_SMI_ADDR_CR                0x0000003C                       /**< Clock Range */
#define BITM_EMAC_SMI_ADDR_SMIW              0x00000002                       /**< SMI Write */
#define BITM_EMAC_SMI_ADDR_SMIB              0x00000001                       /**< SMI Busy */

#define SET_EMAC_SMI_ADDR_PA(x)              (((x)& 0x1F) <<  BITP_EMAC_SMI_ADDR_PA)    /**< Set Physical Layer Address */
#define SET_EMAC_SMI_ADDR_SMIR(x)            (((x)& 0x1F) <<  BITP_EMAC_SMI_ADDR_SMIR)  /**< Set SMI Register Address */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_SMI_DATA                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_SMI_DATA_SMID               0                               /**< SMI Data */
#define BITM_EMAC_SMI_DATA_SMID              0x0000FFFF                       /**< SMI Data */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_FLOWCTL                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_FLOWCTL_PT                 16                               /**< Pause Time */
#define BITP_EMAC_FLOWCTL_UP                  3                               /**< Unicast Pause Frame Detect */
#define BITP_EMAC_FLOWCTL_RFE                 2                               /**< Receive Flow Control Enable */
#define BITP_EMAC_FLOWCTL_TFE                 1                               /**< Transmit Flow Control Enable */
#define BITP_EMAC_FLOWCTL_FCBBPA              0                               /**< Initiate Pause Control Frame */
#define BITM_EMAC_FLOWCTL_PT                 0xFFFF0000                       /**< Pause Time */
#define BITM_EMAC_FLOWCTL_UP                 0x00000008                       /**< Unicast Pause Frame Detect */
#define BITM_EMAC_FLOWCTL_RFE                0x00000004                       /**< Receive Flow Control Enable */
#define BITM_EMAC_FLOWCTL_TFE                0x00000002                       /**< Transmit Flow Control Enable */
#define BITM_EMAC_FLOWCTL_FCBBPA             0x00000001                       /**< Initiate Pause Control Frame */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_VLANTAG                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_VLANTAG_ETV                16                               /**< Enable Tag VLAN Comparison */
#define BITP_EMAC_VLANTAG_VL                  0                               /**< VLAN Tag Id Receive Frames */
#define BITM_EMAC_VLANTAG_ETV                0x00010000                       /**< Enable Tag VLAN Comparison */
#define BITM_EMAC_VLANTAG_VL                 0x0000FFFF                       /**< VLAN Tag Id Receive Frames */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_DBG                             Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_DBG_TXFIFOFULL             25                               /**< Tx FIFO Full */
#define BITP_EMAC_DBG_TXFIFONE               24                               /**< Tx FIFO Not Empty */
#define BITP_EMAC_DBG_TXFIFOACT              22                               /**< Tx FIFO Active */
#define BITP_EMAC_DBG_TXFIFOCTLST            20                               /**< Tx FIFO Controller State */
#define BITP_EMAC_DBG_TXPAUSE                19                               /**< Tx Paused */
#define BITP_EMAC_DBG_TXFRCTL                17                               /**< Tx Frame Controller State */
#define BITP_EMAC_DBG_MMTEA                  16                               /**< MM Tx Engine Active */
#define BITP_EMAC_DBG_RXFIFOST                8                               /**< Rx FIFO State */
#define BITP_EMAC_DBG_RXFIFOCTLST             5                               /**< Rx FIFO Controller State */
#define BITP_EMAC_DBG_RXFIFOACT               4                               /**< Rx FIFO Active */
#define BITP_EMAC_DBG_SFIFOST                 1                               /**< Small FIFO State */
#define BITP_EMAC_DBG_MMREA                   0                               /**< MM Rx Engine Active */
#define BITM_EMAC_DBG_TXFIFOFULL             0x02000000                       /**< Tx FIFO Full */
#define BITM_EMAC_DBG_TXFIFONE               0x01000000                       /**< Tx FIFO Not Empty */
#define BITM_EMAC_DBG_TXFIFOACT              0x00400000                       /**< Tx FIFO Active */
#define BITM_EMAC_DBG_TXFIFOCTLST            0x00300000                       /**< Tx FIFO Controller State */
#define BITM_EMAC_DBG_TXPAUSE                0x00080000                       /**< Tx Paused */

#define BITM_EMAC_DBG_TXFRCTL                0x00060000                       /**< Tx Frame Controller State */
#define ENUM_EMAC_DBG_TXFRCTL_IDLE           0x00000000                       /**< TXFRCTL: Idle */
#define ENUM_EMAC_DBG_TXFRCTL_WT_STATUS      0x00020000                       /**< TXFRCTL: Wait */
#define ENUM_EMAC_DBG_TXFRCTL_PAUSE          0x00040000                       /**< TXFRCTL: Pause */
#define ENUM_EMAC_DBG_TXFRCTL_TXFRAME        0x00060000                       /**< TXFRCTL: Transmit */
#define BITM_EMAC_DBG_MMTEA                  0x00010000                       /**< MM Tx Engine Active */

#define BITM_EMAC_DBG_RXFIFOST               0x00000300                       /**< Rx FIFO State */
#define ENUM_EMAC_DBG_FIFO_EMPTY             0x00000000                       /**< RXFIFOST: Rx FIFO Empty */
#define ENUM_EMAC_DBG_FIFO_BEL_THERSHLD      0x00000100                       /**< RXFIFOST: Rx FIFO Below De-activate FCT */
#define ENUM_EMAC_DBG_FIFO_ABV_THERSHLD      0x00000200                       /**< RXFIFOST: Rx FIFO Above De-activate FCT */
#define ENUM_EMAC_DBG_FIFO_FULL              0x00000300                       /**< RXFIFOST: Rx FIFO Full */

#define BITM_EMAC_DBG_RXFIFOCTLST            0x00000060                       /**< Rx FIFO Controller State */
#define ENUM_EMAC_DBG_IDLE_FIFO              0x00000000                       /**< RXFIFOCTLST: Idle */
#define ENUM_EMAC_DBG_RD_DATA_FIFO           0x00000020                       /**< RXFIFOCTLST: Read Data */
#define ENUM_EMAC_DBG_RD_STS_FIFO            0x00000040                       /**< RXFIFOCTLST: Read Status */
#define ENUM_EMAC_DBG_FLUSH_FIFO             0x00000060                       /**< RXFIFOCTLST: Flush */
#define BITM_EMAC_DBG_RXFIFOACT              0x00000010                       /**< Rx FIFO Active */
#define BITM_EMAC_DBG_SFIFOST                0x00000006                       /**< Small FIFO State */
#define BITM_EMAC_DBG_MMREA                  0x00000001                       /**< MM Rx Engine Active */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_ISTAT                           Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_ISTAT_TS                    9                               /**< Time Stamp Interrupt Status */
#define BITP_EMAC_ISTAT_MMCRC                 7                               /**< MMC Receive Checksum Offload Interrupt Status */
#define BITP_EMAC_ISTAT_MMCTX                 6                               /**< MMC Transmit Interrupt Status */
#define BITP_EMAC_ISTAT_MMCRX                 5                               /**< MMC Receive Interrupt Status */
#define BITP_EMAC_ISTAT_MMC                   4                               /**< MMC Interrupt Status */
#define BITM_EMAC_ISTAT_TS                   0x00000200                       /**< Time Stamp Interrupt Status */
#define BITM_EMAC_ISTAT_MMCRC                0x00000080                       /**< MMC Receive Checksum Offload Interrupt Status */
#define BITM_EMAC_ISTAT_MMCTX                0x00000040                       /**< MMC Transmit Interrupt Status */
#define BITM_EMAC_ISTAT_MMCRX                0x00000020                       /**< MMC Receive Interrupt Status */
#define BITM_EMAC_ISTAT_MMC                  0x00000010                       /**< MMC Interrupt Status */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_IMSK                            Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_IMSK_TS                     9                               /**< Time Stamp Interrupt Mask */
#define BITM_EMAC_IMSK_TS                    0x00000200                       /**< Time Stamp Interrupt Mask */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_ADDR0_HI                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_ADDR0_HI_ADDR               0                               /**< Address */
#define BITM_EMAC_ADDR0_HI_ADDR              0x0000FFFF                       /**< Address */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_MMC_CTL                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_MMC_CTL_FULLPSET            5                               /**< Full Preset */
#define BITP_EMAC_MMC_CTL_CNTRPSET            4                               /**< Counter Reset/Preset */
#define BITP_EMAC_MMC_CTL_CNTRFRZ             3                               /**< Counter Freeze */
#define BITP_EMAC_MMC_CTL_RDRST               2                               /**< Read Reset */
#define BITP_EMAC_MMC_CTL_NOROLL              1                               /**< No Rollover */
#define BITP_EMAC_MMC_CTL_RST                 0                               /**< Reset */
#define BITM_EMAC_MMC_CTL_FULLPSET           0x00000020                       /**< Full Preset */
#define BITM_EMAC_MMC_CTL_CNTRPSET           0x00000010                       /**< Counter Reset/Preset */
#define BITM_EMAC_MMC_CTL_CNTRFRZ            0x00000008                       /**< Counter Freeze */
#define BITM_EMAC_MMC_CTL_RDRST              0x00000004                       /**< Read Reset */
#define BITM_EMAC_MMC_CTL_NOROLL             0x00000002                       /**< No Rollover */
#define BITM_EMAC_MMC_CTL_RST                0x00000001                       /**< Reset */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_MMC_RXINT                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_MMC_RXINT_WDOGERR          23                               /**< Rx Watch Dog Error Count Half/Full */
#define BITP_EMAC_MMC_RXINT_VLANFRGB         22                               /**< Rx VLAN Frames (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_FIFOOVF          21                               /**< Rx FIFO Overflow Count Half/Full */
#define BITP_EMAC_MMC_RXINT_PAUSEFR          20                               /**< Rx Pause Frames Count Half/Full */
#define BITP_EMAC_MMC_RXINT_OUTRANGE         19                               /**< Rx Out Of Range Type Count Half/Full */
#define BITP_EMAC_MMC_RXINT_LENERR           18                               /**< Rx Length Error Count Half/Full */
#define BITP_EMAC_MMC_RXINT_UCASTG           17                               /**< Rx Unicast Frames (Good) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_R1024TOMAX       16                               /**< Rx 1024-to-max Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_R512TO1023       15                               /**< Rx 512-to-1023 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_R256TO511        14                               /**< Rx 255-to-511 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_R128TO255        13                               /**< Rx 128-to-255 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_R65TO127         12                               /**< Rx 65-to-127 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_R64              11                               /**< Rx 64 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_OSIZEG           10                               /**< Rx Oversize (Good) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_USIZEG            9                               /**< Rx Undersize (Good) Count Half/Full */
#define BITP_EMAC_MMC_RXINT_JABERR            8                               /**< Rx Jabber Error Count Half/Full */
#define BITP_EMAC_MMC_RXINT_RUNTERR           7                               /**< Rx Runt Error Count Half/Full */
#define BITP_EMAC_MMC_RXINT_ALIGNERR          6                               /**< Rx Alignment Error Count Half/Full */
#define BITP_EMAC_MMC_RXINT_CRCERR            5                               /**< Rx CRC Error Counter Half/Full */
#define BITP_EMAC_MMC_RXINT_MCASTG            4                               /**< Rx Multicast Count (Good) Half/Full */
#define BITP_EMAC_MMC_RXINT_BCASTG            3                               /**< Rx Broadcast Count (Good) Half/Full */
#define BITP_EMAC_MMC_RXINT_OCTCNTG           2                               /**< Octet Count (Good) Half/Full */
#define BITP_EMAC_MMC_RXINT_OCTCNTGB          1                               /**< Octet Count (Good/Bad) Half/Full */
#define BITP_EMAC_MMC_RXINT_FRCNTGB           0                               /**< Frame Count (Good/Bad) Half/Full */
#define BITM_EMAC_MMC_RXINT_WDOGERR          0x00800000                       /**< Rx Watch Dog Error Count Half/Full */
#define BITM_EMAC_MMC_RXINT_VLANFRGB         0x00400000                       /**< Rx VLAN Frames (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_FIFOOVF          0x00200000                       /**< Rx FIFO Overflow Count Half/Full */
#define BITM_EMAC_MMC_RXINT_PAUSEFR          0x00100000                       /**< Rx Pause Frames Count Half/Full */
#define BITM_EMAC_MMC_RXINT_OUTRANGE         0x00080000                       /**< Rx Out Of Range Type Count Half/Full */
#define BITM_EMAC_MMC_RXINT_LENERR           0x00040000                       /**< Rx Length Error Count Half/Full */
#define BITM_EMAC_MMC_RXINT_UCASTG           0x00020000                       /**< Rx Unicast Frames (Good) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_R1024TOMAX       0x00010000                       /**< Rx 1024-to-max Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_R512TO1023       0x00008000                       /**< Rx 512-to-1023 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_R256TO511        0x00004000                       /**< Rx 255-to-511 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_R128TO255        0x00002000                       /**< Rx 128-to-255 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_R65TO127         0x00001000                       /**< Rx 65-to-127 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_R64              0x00000800                       /**< Rx 64 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_OSIZEG           0x00000400                       /**< Rx Oversize (Good) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_USIZEG           0x00000200                       /**< Rx Undersize (Good) Count Half/Full */
#define BITM_EMAC_MMC_RXINT_JABERR           0x00000100                       /**< Rx Jabber Error Count Half/Full */
#define BITM_EMAC_MMC_RXINT_RUNTERR          0x00000080                       /**< Rx Runt Error Count Half/Full */
#define BITM_EMAC_MMC_RXINT_ALIGNERR         0x00000040                       /**< Rx Alignment Error Count Half/Full */
#define BITM_EMAC_MMC_RXINT_CRCERR           0x00000020                       /**< Rx CRC Error Counter Half/Full */
#define BITM_EMAC_MMC_RXINT_MCASTG           0x00000010                       /**< Rx Multicast Count (Good) Half/Full */
#define BITM_EMAC_MMC_RXINT_BCASTG           0x00000008                       /**< Rx Broadcast Count (Good) Half/Full */
#define BITM_EMAC_MMC_RXINT_OCTCNTG          0x00000004                       /**< Octet Count (Good) Half/Full */
#define BITM_EMAC_MMC_RXINT_OCTCNTGB         0x00000002                       /**< Octet Count (Good/Bad) Half/Full */
#define BITM_EMAC_MMC_RXINT_FRCNTGB          0x00000001                       /**< Frame Count (Good/Bad) Half/Full */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_MMC_TXINT                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_MMC_TXINT_VLANFRGB         24                               /**< Tx VLAN Frames (Good) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_PAUSEFRM         23                               /**< Tx Pause Frames Count Half/Full */
#define BITP_EMAC_MMC_TXINT_EXCESSDEF        22                               /**< Tx Excess Deferred Count Half/Full */
#define BITP_EMAC_MMC_TXINT_FRCNTG           21                               /**< Tx Frame Count (Good) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_OCTCNTG          20                               /**< Tx Octet Count (Good) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_CARRERR          19                               /**< Tx Carrier Error Count Half/Full */
#define BITP_EMAC_MMC_TXINT_EXCESSCOL        18                               /**< Tx Exess Collision Count Half/Full */
#define BITP_EMAC_MMC_TXINT_LATECOL          17                               /**< Tx Late Collision Count Half/Full */
#define BITP_EMAC_MMC_TXINT_DEFERRED         16                               /**< Tx Deffered Count Half/Full */
#define BITP_EMAC_MMC_TXINT_MULTCOLG         15                               /**< Tx Multiple collision (Good) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_SNGCOLG          14                               /**< Tx Single Collision (Good) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_UNDERR           13                               /**< Tx Underflow Error Count Half/Full */
#define BITP_EMAC_MMC_TXINT_BCASTGB          12                               /**< Tx Broadcast Frames (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_MCASTGB          11                               /**< Tx Multicast Frames (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_UCASTGB          10                               /**< Tx Unicast Frames (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_T1024TOMAX        9                               /**< Tx 1024-to-max Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_T512TO1023        8                               /**< Tx 512-to-1023 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_T256TO511         7                               /**< Tx 256-to-511 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_T128TO255         6                               /**< Tx 128-to-255 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_T65TO127          5                               /**< Tx 65-to-127 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_T64               4                               /**< Tx 64 Octets (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_MCASTG            3                               /**< Tx Multicast Frames (Good) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_BCASTG            2                               /**< Tx Broadcast Frames (Good) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_FRCNTGB           1                               /**< Tx Frame Count (Good/Bad) Count Half/Full */
#define BITP_EMAC_MMC_TXINT_OCTCNTGB          0                               /**< Tx Octet Count (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_VLANFRGB         0x01000000                       /**< Tx VLAN Frames (Good) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_PAUSEFRM         0x00800000                       /**< Tx Pause Frames Count Half/Full */
#define BITM_EMAC_MMC_TXINT_EXCESSDEF        0x00400000                       /**< Tx Excess Deferred Count Half/Full */
#define BITM_EMAC_MMC_TXINT_FRCNTG           0x00200000                       /**< Tx Frame Count (Good) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_OCTCNTG          0x00100000                       /**< Tx Octet Count (Good) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_CARRERR          0x00080000                       /**< Tx Carrier Error Count Half/Full */
#define BITM_EMAC_MMC_TXINT_EXCESSCOL        0x00040000                       /**< Tx Exess Collision Count Half/Full */
#define BITM_EMAC_MMC_TXINT_LATECOL          0x00020000                       /**< Tx Late Collision Count Half/Full */
#define BITM_EMAC_MMC_TXINT_DEFERRED         0x00010000                       /**< Tx Deffered Count Half/Full */
#define BITM_EMAC_MMC_TXINT_MULTCOLG         0x00008000                       /**< Tx Multiple collision (Good) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_SNGCOLG          0x00004000                       /**< Tx Single Collision (Good) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_UNDERR           0x00002000                       /**< Tx Underflow Error Count Half/Full */
#define BITM_EMAC_MMC_TXINT_BCASTGB          0x00001000                       /**< Tx Broadcast Frames (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_MCASTGB          0x00000800                       /**< Tx Multicast Frames (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_UCASTGB          0x00000400                       /**< Tx Unicast Frames (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_T1024TOMAX       0x00000200                       /**< Tx 1024-to-max Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_T512TO1023       0x00000100                       /**< Tx 512-to-1023 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_T256TO511        0x00000080                       /**< Tx 256-to-511 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_T128TO255        0x00000040                       /**< Tx 128-to-255 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_T65TO127         0x00000020                       /**< Tx 65-to-127 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_T64              0x00000010                       /**< Tx 64 Octets (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_MCASTG           0x00000008                       /**< Tx Multicast Frames (Good) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_BCASTG           0x00000004                       /**< Tx Broadcast Frames (Good) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_FRCNTGB          0x00000002                       /**< Tx Frame Count (Good/Bad) Count Half/Full */
#define BITM_EMAC_MMC_TXINT_OCTCNTGB         0x00000001                       /**< Tx Octet Count (Good/Bad) Count Half/Full */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_MMC_RXIMSK                      Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_MMC_RXIMSK_WATCHERR        23                               /**< Rx Watch Dog Error Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_VLANFRGB        22                               /**< Rx VLAN Frames (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_FIFOOV          21                               /**< Rx FIFO Overflow Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_PAUSEFRM        20                               /**< Rx Pause Frames Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_OUTRANGE        19                               /**< Rx Out Of Range Type Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_LENERR          18                               /**< Rx Length Error Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_UCASTG          17                               /**< Rx Unicast Frames (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_R1024TOMAX      16                               /**< Rx 1024-to-max Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_R512TO1023      15                               /**< Rx 512-to-1023 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_R256TO511       14                               /**< Rx 255-to-511 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_R128TO255       13                               /**< Rx 128-to-255 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_R65TO127        12                               /**< Rx 65-to-127 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_R64             11                               /**< Rx 64 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_OSIZEG          10                               /**< Rx Oversize (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_USIZEG           9                               /**< Rx Undersize (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_JABERR           8                               /**< Rx Jabber Error Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_RUNTERR          7                               /**< Rx Runt Error Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_ALIGNERR         6                               /**< Rx Alignment Error Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_CRCERR           5                               /**< Rx CRC Error Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_MCASTG           4                               /**< Rx Multicast Frames (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_BCASTG           3                               /**< Rx Broadcast Frames (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_OCTCNTG          2                               /**< Rx Octet Count (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_OCTCNTGB         1                               /**< Rx Octet Count (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_RXIMSK_FRCNTGB          0                               /**< Rx Frame Count (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_WATCHERR        0x00800000                       /**< Rx Watch Dog Error Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_VLANFRGB        0x00400000                       /**< Rx VLAN Frames (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_FIFOOV          0x00200000                       /**< Rx FIFO Overflow Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_PAUSEFRM        0x00100000                       /**< Rx Pause Frames Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_OUTRANGE        0x00080000                       /**< Rx Out Of Range Type Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_LENERR          0x00040000                       /**< Rx Length Error Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_UCASTG          0x00020000                       /**< Rx Unicast Frames (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_R1024TOMAX      0x00010000                       /**< Rx 1024-to-max Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_R512TO1023      0x00008000                       /**< Rx 512-to-1023 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_R256TO511       0x00004000                       /**< Rx 255-to-511 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_R128TO255       0x00002000                       /**< Rx 128-to-255 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_R65TO127        0x00001000                       /**< Rx 65-to-127 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_R64             0x00000800                       /**< Rx 64 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_OSIZEG          0x00000400                       /**< Rx Oversize (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_USIZEG          0x00000200                       /**< Rx Undersize (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_JABERR          0x00000100                       /**< Rx Jabber Error Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_RUNTERR         0x00000080                       /**< Rx Runt Error Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_ALIGNERR        0x00000040                       /**< Rx Alignment Error Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_CRCERR          0x00000020                       /**< Rx CRC Error Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_MCASTG          0x00000010                       /**< Rx Multicast Frames (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_BCASTG          0x00000008                       /**< Rx Broadcast Frames (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_OCTCNTG         0x00000004                       /**< Rx Octet Count (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_OCTCNTGB        0x00000002                       /**< Rx Octet Count (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_RXIMSK_FRCNTGB         0x00000001                       /**< Rx Frame Count (Good/Bad) Count Half/Full Mask */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_MMC_TXIMSK                      Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_MMC_TXIMSK_VLANFRG         24                               /**< Tx VLAN Frames (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_PAUSEFRM        23                               /**< Tx Pause Frames Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_EXCESSDEF       22                               /**< Tx Excess Deferred Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_FRCNTG          21                               /**< Tx Frame Count (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_OCTCNTG         20                               /**< Tx Octet Count (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_CARRERR         19                               /**< Tx Carrier Error Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_EXCESSCOL       18                               /**< Tx Exess collision Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_LATECOL         17                               /**< Tx Late Collision Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_DEFERRED        16                               /**< Tx Deferred Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_MULTCOLG        15                               /**< Tx Multiple Collisions (Good) Count Mask */
#define BITP_EMAC_MMC_TXIMSK_SNGCOLG         14                               /**< Tx Single Collision (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_UNDERR          13                               /**< Tx Underflow Error Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_BCASTGB         12                               /**< Tx Broadcast Frames (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_MCASTGB         11                               /**< Tx Multicast Frames (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_UCASTGB         10                               /**< Tx Unicast Frames (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_T1024TOMAX       9                               /**< Tx 1024-to-max Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_T512TO1023       8                               /**< Tx 512-to-1023 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_T256TO511        7                               /**< Tx 256-to-511 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_T128TO255        6                               /**< Tx 128-to-255 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_T65TO127         5                               /**< Tx 65-to-127 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_T64              4                               /**< Tx 64 Octets (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_MCASTG           3                               /**< Tx Multicast Frames (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_BCASTG           2                               /**< Tx Broadcast Frames (Good) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_FRCNTGB          1                               /**< Tx Frame Count (Good/Bad) Count Half/Full Mask */
#define BITP_EMAC_MMC_TXIMSK_OCTCNTGB         0                               /**< Tx Octet Count (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_VLANFRG         0x01000000                       /**< Tx VLAN Frames (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_PAUSEFRM        0x00800000                       /**< Tx Pause Frames Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_EXCESSDEF       0x00400000                       /**< Tx Excess Deferred Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_FRCNTG          0x00200000                       /**< Tx Frame Count (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_OCTCNTG         0x00100000                       /**< Tx Octet Count (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_CARRERR         0x00080000                       /**< Tx Carrier Error Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_EXCESSCOL       0x00040000                       /**< Tx Exess collision Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_LATECOL         0x00020000                       /**< Tx Late Collision Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_DEFERRED        0x00010000                       /**< Tx Deferred Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_MULTCOLG        0x00008000                       /**< Tx Multiple Collisions (Good) Count Mask */
#define BITM_EMAC_MMC_TXIMSK_SNGCOLG         0x00004000                       /**< Tx Single Collision (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_UNDERR          0x00002000                       /**< Tx Underflow Error Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_BCASTGB         0x00001000                       /**< Tx Broadcast Frames (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_MCASTGB         0x00000800                       /**< Tx Multicast Frames (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_UCASTGB         0x00000400                       /**< Tx Unicast Frames (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_T1024TOMAX      0x00000200                       /**< Tx 1024-to-max Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_T512TO1023      0x00000100                       /**< Tx 512-to-1023 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_T256TO511       0x00000080                       /**< Tx 256-to-511 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_T128TO255       0x00000040                       /**< Tx 128-to-255 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_T65TO127        0x00000020                       /**< Tx 65-to-127 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_T64             0x00000010                       /**< Tx 64 Octets (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_MCASTG          0x00000008                       /**< Tx Multicast Frames (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_BCASTG          0x00000004                       /**< Tx Broadcast Frames (Good) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_FRCNTGB         0x00000002                       /**< Tx Frame Count (Good/Bad) Count Half/Full Mask */
#define BITM_EMAC_MMC_TXIMSK_OCTCNTGB        0x00000001                       /**< Tx Octet Count (Good/Bad) Count Half/Full Mask */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_IPC_RXIMSK                      Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_IPC_RXIMSK_ICMPERROCT      29                               /**< Rx ICMP Error Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_ICMPGOCT        28                               /**< Rx ICMP (Good) Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_TCPERROCT       27                               /**< Rx TCP Error Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_TCPGOCT         26                               /**< Rx TCP (Good) Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_UDPERROCT       25                               /**< Rx UDP Error Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_UDPGOCT         24                               /**< Rx UDP (Good) Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V6NOPAYOCT      23                               /**< Rx IPv6 No Payload Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V6HDERROCT      22                               /**< Rx IPv6 Header Error Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V6GOCT          21                               /**< Rx IPv6 (Good) Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4UDSBLOCT      20                               /**< Rx IPv4 UDS Disable Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4FRAGOCT       19                               /**< Rx IPv4 Fragmented Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4NOPAYOCT      18                               /**< Rx IPv4 No Payload Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4HDERROCT      17                               /**< Rx IPv4 Header Error Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4GOCT          16                               /**< Rx IPv4 (Good) Octets Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_ICMPERRFRM      13                               /**< Rx ICMP Error Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_ICMPGFRM        12                               /**< Rx ICMP (Good) Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_TCPERRFRM       11                               /**< Rx TCP Error Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_TCPGFRM         10                               /**< Rx TCP (Good) Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_UDPERRFRM        9                               /**< Rx UDP Error Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_UDPGFRM          8                               /**< Rx UDP (Good) Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V6NOPAYFRM       7                               /**< Rx IPv6 No Payload Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V6HDERRFRM       6                               /**< Rx IPv6 Header Error Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V6GFRM           5                               /**< Rx IPv6 (Good) Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4UDSBLFRM       4                               /**< Rx IPv4 UDS Disable Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4FRAGFRM        3                               /**< Rx IPv4 Fragmented Frames Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4NOPAYFRM       2                               /**< Rx IPv4 No Payload Frame Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4HDERRFRM       1                               /**< Rx IPv4 Header Error Frame Count Half/Full Mask */
#define BITP_EMAC_IPC_RXIMSK_V4GFRM           0                               /**< Rx IPv4 (Good) Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_ICMPERROCT      0x20000000                       /**< Rx ICMP Error Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_ICMPGOCT        0x10000000                       /**< Rx ICMP (Good) Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_TCPERROCT       0x08000000                       /**< Rx TCP Error Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_TCPGOCT         0x04000000                       /**< Rx TCP (Good) Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_UDPERROCT       0x02000000                       /**< Rx UDP Error Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_UDPGOCT         0x01000000                       /**< Rx UDP (Good) Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V6NOPAYOCT      0x00800000                       /**< Rx IPv6 No Payload Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V6HDERROCT      0x00400000                       /**< Rx IPv6 Header Error Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V6GOCT          0x00200000                       /**< Rx IPv6 (Good) Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4UDSBLOCT      0x00100000                       /**< Rx IPv4 UDS Disable Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4FRAGOCT       0x00080000                       /**< Rx IPv4 Fragmented Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4NOPAYOCT      0x00040000                       /**< Rx IPv4 No Payload Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4HDERROCT      0x00020000                       /**< Rx IPv4 Header Error Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4GOCT          0x00010000                       /**< Rx IPv4 (Good) Octets Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_ICMPERRFRM      0x00002000                       /**< Rx ICMP Error Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_ICMPGFRM        0x00001000                       /**< Rx ICMP (Good) Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_TCPERRFRM       0x00000800                       /**< Rx TCP Error Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_TCPGFRM         0x00000400                       /**< Rx TCP (Good) Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_UDPERRFRM       0x00000200                       /**< Rx UDP Error Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_UDPGFRM         0x00000100                       /**< Rx UDP (Good) Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V6NOPAYFRM      0x00000080                       /**< Rx IPv6 No Payload Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V6HDERRFRM      0x00000040                       /**< Rx IPv6 Header Error Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V6GFRM          0x00000020                       /**< Rx IPv6 (Good) Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4UDSBLFRM      0x00000010                       /**< Rx IPv4 UDS Disable Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4FRAGFRM       0x00000008                       /**< Rx IPv4 Fragmented Frames Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4NOPAYFRM      0x00000004                       /**< Rx IPv4 No Payload Frame Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4HDERRFRM      0x00000002                       /**< Rx IPv4 Header Error Frame Count Half/Full Mask */
#define BITM_EMAC_IPC_RXIMSK_V4GFRM          0x00000001                       /**< Rx IPv4 (Good) Frames Count Half/Full Mask */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_IPC_RXINT                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_IPC_RXINT_ICMPERROCT       29                               /**< Rx ICMP Error Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_ICMPGOCT         28                               /**< Rx ICMP (Good) Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_TCPERROCT        27                               /**< Rx TCP Error Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_TCPGOCT          26                               /**< Rx TCP (Good) Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_UDPERROCT        25                               /**< Rx UDP Error Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_UDPGOCT          24                               /**< Rx UDP (Good) Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V6NOPAYOCT       23                               /**< Rx IPv6 No Payload Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V6HDERROCT       22                               /**< Rx IPv6 Header Error Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V6GOCT           21                               /**< Rx IPv6 (Good) Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4UDSBLOCT       20                               /**< Rx IPv4 UDS Disable Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4FRAGOCT        19                               /**< Rx IPv4 Fragmented Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4NOPAYOCT       18                               /**< Rx IPv4 No Payload Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4HDERROCT       17                               /**< Rx IPv4 Header Error Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4GOCT           16                               /**< Rx IPv4 (Good) Octets Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_ICMPERRFRM       13                               /**< Rx ICMP Error Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_ICMPGFRM         12                               /**< Rx ICMP (Good) Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_TCPERRFRM        11                               /**< Rx TCP Error Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_TCPGFRM          10                               /**< Rx TCP (Good) Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_UDPERRFRM         9                               /**< Rx IDP Error Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_UDPGFRM           8                               /**< Rx UDP (Good) Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V6NOPAYFRM        7                               /**< Rx IPv6 No Payload Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V6HDERRFRM        6                               /**< Rx IPv6 Header Error Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V6GFRM            5                               /**< Rx IPv6 (Good) Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4UDSBLFRM        4                               /**< Rx IPv4 UDS Disable Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4FRAGFRM         3                               /**< Rx IPv4 Fragmented Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4NOPAYFRM        2                               /**< Rx IPv4 No Payload Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4HDERRFRM        1                               /**< Rx IPv4 Header Error Frames Count Half/Full Interrupt */
#define BITP_EMAC_IPC_RXINT_V4GFRM            0                               /**< Rx IPv4 (Good) Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_ICMPERROCT       0x20000000                       /**< Rx ICMP Error Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_ICMPGOCT         0x10000000                       /**< Rx ICMP (Good) Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_TCPERROCT        0x08000000                       /**< Rx TCP Error Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_TCPGOCT          0x04000000                       /**< Rx TCP (Good) Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_UDPERROCT        0x02000000                       /**< Rx UDP Error Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_UDPGOCT          0x01000000                       /**< Rx UDP (Good) Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V6NOPAYOCT       0x00800000                       /**< Rx IPv6 No Payload Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V6HDERROCT       0x00400000                       /**< Rx IPv6 Header Error Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V6GOCT           0x00200000                       /**< Rx IPv6 (Good) Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4UDSBLOCT       0x00100000                       /**< Rx IPv4 UDS Disable Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4FRAGOCT        0x00080000                       /**< Rx IPv4 Fragmented Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4NOPAYOCT       0x00040000                       /**< Rx IPv4 No Payload Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4HDERROCT       0x00020000                       /**< Rx IPv4 Header Error Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4GOCT           0x00010000                       /**< Rx IPv4 (Good) Octets Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_ICMPERRFRM       0x00002000                       /**< Rx ICMP Error Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_ICMPGFRM         0x00001000                       /**< Rx ICMP (Good) Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_TCPERRFRM        0x00000800                       /**< Rx TCP Error Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_TCPGFRM          0x00000400                       /**< Rx TCP (Good) Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_UDPERRFRM        0x00000200                       /**< Rx IDP Error Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_UDPGFRM          0x00000100                       /**< Rx UDP (Good) Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V6NOPAYFRM       0x00000080                       /**< Rx IPv6 No Payload Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V6HDERRFRM       0x00000040                       /**< Rx IPv6 Header Error Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V6GFRM           0x00000020                       /**< Rx IPv6 (Good) Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4UDSBLFRM       0x00000010                       /**< Rx IPv4 UDS Disable Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4FRAGFRM        0x00000008                       /**< Rx IPv4 Fragmented Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4NOPAYFRM       0x00000004                       /**< Rx IPv4 No Payload Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4HDERRFRM       0x00000002                       /**< Rx IPv4 Header Error Frames Count Half/Full Interrupt */
#define BITM_EMAC_IPC_RXINT_V4GFRM           0x00000001                       /**< Rx IPv4 (Good) Frames Count Half/Full Interrupt */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_TM_CTL                          Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_TM_CTL_ATSFC               24                               /**< Auxiliary Time Stamp FIFO Clear */
#define BITP_EMAC_TM_CTL_TSENMACADDR         18                               /**< Time Stamp Enable MAC Address */
#define BITP_EMAC_TM_CTL_SNAPTYPSEL          16                               /**< Snapshot Type Select */
#define BITP_EMAC_TM_CTL_TSMSTRENA           15                               /**< Time Stamp Master (Frames) Enable */
#define BITP_EMAC_TM_CTL_TSEVNTENA           14                               /**< Time Stamp Event (PTP Frames) Enable */
#define BITP_EMAC_TM_CTL_TSIPV4ENA           13                               /**< Time Stamp IPV4 (PTP Frames) Enable */
#define BITP_EMAC_TM_CTL_TSIPV6ENA           12                               /**< Time Stamp IPV6 (PTP Frames) Enable */
#define BITP_EMAC_TM_CTL_TSIPENA             11                               /**< Time Stamp IP Enable */
#define BITP_EMAC_TM_CTL_TSVER2ENA           10                               /**< Time Stamp VER2 (Snooping) Enable */
#define BITP_EMAC_TM_CTL_TSCTRLSSR            9                               /**< Time Stamp Control Nanosecond Rollover */
#define BITP_EMAC_TM_CTL_TSENALL              8                               /**< Time Stamp Enable All (Frames) */
#define BITP_EMAC_TM_CTL_TSADDREG             5                               /**< Time Stamp Addend Register Update */
#define BITP_EMAC_TM_CTL_TSTRIG               4                               /**< Time Stamp (Target Time) Trigger Enable */
#define BITP_EMAC_TM_CTL_TSUPDT               3                               /**< Time Stamp (System Time) Update */
#define BITP_EMAC_TM_CTL_TSINIT               2                               /**< Time Stamp (System Time) Initialize */
#define BITP_EMAC_TM_CTL_TSCFUPDT             1                               /**< Time Stamp (System Time) Fine/Coarse Update */
#define BITP_EMAC_TM_CTL_TSENA                0                               /**< Time Stamp (PTP) Enable */
#define BITM_EMAC_TM_CTL_ATSFC               0x01000000                       /**< Auxiliary Time Stamp FIFO Clear */

#define BITM_EMAC_TM_CTL_TSENMACADDR         0x00040000                       /**< Time Stamp Enable MAC Address */
#define ENUM_EMAC_TM_CTL_D_PTP_ADDRFILT      0x00000000                       /**< TSENMACADDR: Disable PTP MAC address filter */
#define ENUM_EMAC_TM_CTL_E_PTP_ADDRFILT      0x00040000                       /**< TSENMACADDR: Enable PTP MAC address filter */
#define BITM_EMAC_TM_CTL_SNAPTYPSEL          0x00030000                       /**< Snapshot Type Select */

#define BITM_EMAC_TM_CTL_TSMSTRENA           0x00008000                       /**< Time Stamp Master (Frames) Enable */
#define ENUM_EMAC_TM_CTL_E_SLVSNPT_MSGS      0x00000000                       /**< TSMSTRENA: Enable Snapshot for Slave Messages */
#define ENUM_EMAC_TM_CTL_E_MSSNPST_MSGS      0x00008000                       /**< TSMSTRENA: Enable Snapshot for Master Messages */

#define BITM_EMAC_TM_CTL_TSEVNTENA           0x00004000                       /**< Time Stamp Event (PTP Frames) Enable */
#define ENUM_EMAC_TM_CTL_E_ATSTMP_MSGS       0x00000000                       /**< TSEVNTENA: Enable Time Stamp for All Messages */
#define ENUM_EMAC_TM_CTL_E_ETSTMP_MSGS       0x00004000                       /**< TSEVNTENA: Enable Time Stamp for Event Messages Only */

#define BITM_EMAC_TM_CTL_TSIPV4ENA           0x00002000                       /**< Time Stamp IPV4 (PTP Frames) Enable */
#define ENUM_EMAC_TM_CTL_D_TSTMP_IPV4        0x00000000                       /**< TSIPV4ENA: Disable Time Stamp for PTP Over IPv4 Frames */
#define ENUM_EMAC_TM_CTL_E_TSTMP_IPV4        0x00002000                       /**< TSIPV4ENA: Enable Time Stamp for PTP Over IPv4 Frames */

#define BITM_EMAC_TM_CTL_TSIPV6ENA           0x00001000                       /**< Time Stamp IPV6 (PTP Frames) Enable */
#define ENUM_EMAC_TM_CTL_D_TSTMP_IPV6        0x00000000                       /**< TSIPV6ENA: Disable Time Stamp for PTP Over IPv6 frames */
#define ENUM_EMAC_TM_CTL_E_TSTMP_IPV6        0x00001000                       /**< TSIPV6ENA: Enable Time Stamp for PTP Over IPv6 Frames */

#define BITM_EMAC_TM_CTL_TSIPENA             0x00000800                       /**< Time Stamp IP Enable */
#define ENUM_EMAC_TM_CTL_D_PTP_OV_ETHER      0x00000000                       /**< TSIPENA: Disable PTP Over Ethernet Frames */
#define ENUM_EMAC_TM_CTL_E_PTP_OV_ETHER      0x00000800                       /**< TSIPENA: Enable PTP Over Ethernet Frames */

#define BITM_EMAC_TM_CTL_TSVER2ENA           0x00000400                       /**< Time Stamp VER2 (Snooping) Enable */
#define ENUM_EMAC_TM_CTL_D_PKT_SNOOP_V2      0x00000000                       /**< TSVER2ENA: Disable packet snooping for V2 frames */
#define ENUM_EMAC_TM_CTL_E_PKT_SNOOP_V2      0x00000400                       /**< TSVER2ENA: Enable packet snooping for V2 frames */

#define BITM_EMAC_TM_CTL_TSCTRLSSR           0x00000200                       /**< Time Stamp Control Nanosecond Rollover */
#define ENUM_EMAC_TM_CTL_RO_SUBSEC_RES       0x00000000                       /**< TSCTRLSSR: Roll Over Nanosecond After 0x7FFFFFFF */
#define ENUM_EMAC_TM_CTL_RO_NANO_RES         0x00000200                       /**< TSCTRLSSR: Roll Over Nanosecond After 0x3B9AC9FF */

#define BITM_EMAC_TM_CTL_TSENALL             0x00000100                       /**< Time Stamp Enable All (Frames) */
#define ENUM_EMAC_TM_CTL_D_TSALL_FRAMES      0x00000000                       /**< TSENALL: Disable timestamp for all frames */
#define ENUM_EMAC_TM_CTL_E_TSALL_FRAMES      0x00000100                       /**< TSENALL: Enable timestamp for all frames */
#define BITM_EMAC_TM_CTL_TSADDREG            0x00000020                       /**< Time Stamp Addend Register Update */

#define BITM_EMAC_TM_CTL_TSTRIG              0x00000010                       /**< Time Stamp (Target Time) Trigger Enable */
#define ENUM_EMAC_TM_CTL_EN_TRIGGER          0x00000010                       /**< TSTRIG: Interrupt (TS) if system time is greater than target time register */

#define BITM_EMAC_TM_CTL_TSUPDT              0x00000008                       /**< Time Stamp (System Time) Update */
#define ENUM_EMAC_TM_CTL_EN_UPDATE           0x00000008                       /**< TSUPDT: System time updated with Time stamp register values */

#define BITM_EMAC_TM_CTL_TSINIT              0x00000004                       /**< Time Stamp (System Time) Initialize */
#define ENUM_EMAC_TM_CTL_EN_TS_INIT          0x00000004                       /**< TSINIT: System time initialized with Time stamp register values */

#define BITM_EMAC_TM_CTL_TSCFUPDT            0x00000002                       /**< Time Stamp (System Time) Fine/Coarse Update */
#define ENUM_EMAC_TM_CTL_EN_COARSE_UPDT      0x00000000                       /**< TSCFUPDT: Use Coarse Correction Method for System Time Update */
#define ENUM_EMAC_TM_CTL_EN_FINE_UPDT        0x00000002                       /**< TSCFUPDT: Use Fine Correction Method for System Time Update */

#define BITM_EMAC_TM_CTL_TSENA               0x00000001                       /**< Time Stamp (PTP) Enable */
#define ENUM_EMAC_TM_CTL_DTS                 0x00000000                       /**< TSENA: Disable PTP Module */
#define ENUM_EMAC_TM_CTL_TS                  0x00000001                       /**< TSENA: Enable PTP Module */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_TM_SUBSEC                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_TM_SUBSEC_SSINC             0                               /**< Sub-Second Increment Value */
#define BITM_EMAC_TM_SUBSEC_SSINC            0x000000FF                       /**< Sub-Second Increment Value */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_TM_NSEC                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_TM_NSEC_TSSS                0                               /**< Time Stamp Nanoseconds */
#define BITM_EMAC_TM_NSEC_TSSS               0x7FFFFFFF                       /**< Time Stamp Nanoseconds */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_TM_NSECUPDT                     Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_TM_NSECUPDT_ADDSUB         31                               /**< Add or Subtract the Time */
#define BITP_EMAC_TM_NSECUPDT_TSSS            0                               /**< Time Stamp Sub Second Initialize/Increment */
#define BITM_EMAC_TM_NSECUPDT_ADDSUB         0x80000000                       /**< Add or Subtract the Time */
#define BITM_EMAC_TM_NSECUPDT_TSSS           0x7FFFFFFF                       /**< Time Stamp Sub Second Initialize/Increment */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_TM_NTGTM                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_TM_NTGTM_TSTRBUSY          31                               /**< Target Time Register Busy */
#define BITP_EMAC_TM_NTGTM_TSTR               0                               /**< Target Time Nano Seconds */
#define BITM_EMAC_TM_NTGTM_TSTRBUSY          0x80000000                       /**< Target Time Register Busy */
#define BITM_EMAC_TM_NTGTM_TSTR              0x7FFFFFFF                       /**< Target Time Nano Seconds */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_TM_HISEC                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_TM_HISEC_TSHWR              0                               /**< Time Stamp Higher Word Seconds Register */
#define BITM_EMAC_TM_HISEC_TSHWR             0x0000FFFF                       /**< Time Stamp Higher Word Seconds Register */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_TM_STMPSTAT                     Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_TM_STMPSTAT_ATSNS          25                               /**< Auxiliary Time Stamp Number of Snapshots */
#define BITP_EMAC_TM_STMPSTAT_ATSSTM         24                               /**< Auxiliary Time Stamp Snapshot Trigger Missed */
#define BITP_EMAC_TM_STMPSTAT_TSTRGTERR       3                               /**< Time Stamp Target Time Programming Error */
#define BITP_EMAC_TM_STMPSTAT_ATSTS           2                               /**< Auxiliary Time Stamp Trigger Snapshot */
#define BITP_EMAC_TM_STMPSTAT_TSTARGT         1                               /**< Time Stamp Target Time Reached */
#define BITP_EMAC_TM_STMPSTAT_TSSOVF          0                               /**< Time Stamp Seconds Overflow */
#define BITM_EMAC_TM_STMPSTAT_ATSNS          0x0E000000                       /**< Auxiliary Time Stamp Number of Snapshots */
#define BITM_EMAC_TM_STMPSTAT_ATSSTM         0x01000000                       /**< Auxiliary Time Stamp Snapshot Trigger Missed */
#define BITM_EMAC_TM_STMPSTAT_TSTRGTERR      0x00000008                       /**< Time Stamp Target Time Programming Error */
#define BITM_EMAC_TM_STMPSTAT_ATSTS          0x00000004                       /**< Auxiliary Time Stamp Trigger Snapshot */
#define BITM_EMAC_TM_STMPSTAT_TSTARGT        0x00000002                       /**< Time Stamp Target Time Reached */
#define BITM_EMAC_TM_STMPSTAT_TSSOVF         0x00000001                       /**< Time Stamp Seconds Overflow */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_TM_PPSCTL                       Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_TM_PPSCTL_TRGTMODSEL        5                               /**< Target Time Register Mode */
#define BITP_EMAC_TM_PPSCTL_PPSEN             4                               /**< Enable the flexible PPS output mode */
#define BITP_EMAC_TM_PPSCTL_PPSCTL            0                               /**< PPS Frequency Control */
#define BITM_EMAC_TM_PPSCTL_TRGTMODSEL       0x00000060                       /**< Target Time Register Mode */
#define BITM_EMAC_TM_PPSCTL_PPSEN            0x00000010                       /**< Enable the flexible PPS output mode */
#define BITM_EMAC_TM_PPSCTL_PPSCTL           0x0000000F                       /**< PPS Frequency Control */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_DMA_BUSMODE                     Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_DMA_BUSMODE_AAL            25                               /**< Address Aligned Bursts */
#define BITP_EMAC_DMA_BUSMODE_PBL8           24                               /**< PBL * 8 */
#define BITP_EMAC_DMA_BUSMODE_USP            23                               /**< Use Separate PBL */
#define BITP_EMAC_DMA_BUSMODE_RPBL           17                               /**< Receive Programmable Burst Length */
#define BITP_EMAC_DMA_BUSMODE_FB             16                               /**< Fixed Burst */
#define BITP_EMAC_DMA_BUSMODE_PBL             8                               /**< Programmable Burst Length */
#define BITP_EMAC_DMA_BUSMODE_ATDS            7                               /**< Alternate Descriptor Size */
#define BITP_EMAC_DMA_BUSMODE_DSL             2                               /**< Descriptor Skip Length */
#define BITP_EMAC_DMA_BUSMODE_SWR             0                               /**< Software Reset */
#define BITM_EMAC_DMA_BUSMODE_AAL            0x02000000                       /**< Address Aligned Bursts */
#define BITM_EMAC_DMA_BUSMODE_PBL8           0x01000000                       /**< PBL * 8 */
#define BITM_EMAC_DMA_BUSMODE_USP            0x00800000                       /**< Use Separate PBL */
#define BITM_EMAC_DMA_BUSMODE_RPBL           0x007E0000                       /**< Receive Programmable Burst Length */
#define BITM_EMAC_DMA_BUSMODE_FB             0x00010000                       /**< Fixed Burst */
#define BITM_EMAC_DMA_BUSMODE_PBL            0x00003F00                       /**< Programmable Burst Length */
#define BITM_EMAC_DMA_BUSMODE_ATDS           0x00000080                       /**< Alternate Descriptor Size */
#define BITM_EMAC_DMA_BUSMODE_DSL            0x0000007C                       /**< Descriptor Skip Length */
#define BITM_EMAC_DMA_BUSMODE_SWR            0x00000001                       /**< Software Reset */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_DMA_STAT                        Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_DMA_STAT_TTI               29                               /**< Time Stamp Trigger Interrupt */
#define BITP_EMAC_DMA_STAT_MCI               27                               /**< MAC MMC Interrupt */
#define BITP_EMAC_DMA_STAT_EB                23                               /**< Error Bits */
#define BITP_EMAC_DMA_STAT_TS                20                               /**< Tx Process State */
#define BITP_EMAC_DMA_STAT_RS                17                               /**< Rx Process State */
#define BITP_EMAC_DMA_STAT_NIS               16                               /**< Normal Interrupt Summary */
#define BITP_EMAC_DMA_STAT_AIS               15                               /**< Abnormal Interrupt Summary */
#define BITP_EMAC_DMA_STAT_ERI               14                               /**< Early Receive Interrupt */
#define BITP_EMAC_DMA_STAT_FBI               13                               /**< Fatal Bus Error Interrupt */
#define BITP_EMAC_DMA_STAT_ETI               10                               /**< Early Transmit Interrupt */
#define BITP_EMAC_DMA_STAT_RWT                9                               /**< Receive WatchDog Timeout */
#define BITP_EMAC_DMA_STAT_RPS                8                               /**< Receive Process Stopped */
#define BITP_EMAC_DMA_STAT_RU                 7                               /**< Receive Buffer Unavailable */
#define BITP_EMAC_DMA_STAT_RI                 6                               /**< Receive Interrupt */
#define BITP_EMAC_DMA_STAT_UNF                5                               /**< Transmit Buffer Underflow */
#define BITP_EMAC_DMA_STAT_OVF                4                               /**< Receive Buffer Overflow */
#define BITP_EMAC_DMA_STAT_TJT                3                               /**< Transmit Jabber Timeout */
#define BITP_EMAC_DMA_STAT_TU                 2                               /**< Transmit Buffer Unavailable */
#define BITP_EMAC_DMA_STAT_TPS                1                               /**< Transmit Process Stopped */
#define BITP_EMAC_DMA_STAT_TI                 0                               /**< Transmit Interrupt */
#define BITM_EMAC_DMA_STAT_TTI               0x20000000                       /**< Time Stamp Trigger Interrupt */
#define BITM_EMAC_DMA_STAT_MCI               0x08000000                       /**< MAC MMC Interrupt */
#define BITM_EMAC_DMA_STAT_EB                0x03800000                       /**< Error Bits */

#define BITM_EMAC_DMA_STAT_TS                0x00700000                       /**< Tx Process State */
#define ENUM_EMAC_DMA_STAT_TS_STOPPED        0x00000000                       /**< TS: Stopped; Reset or Stop Tx Command Issued */
#define ENUM_EMAC_DMA_STAT_TS_R_FTD          0x00100000                       /**< TS: Running; Fetching Tx Transfer Descriptor */
#define ENUM_EMAC_DMA_STAT_TS_R_WSTAT        0x00200000                       /**< TS: Running; Waiting for Status */
#define ENUM_EMAC_DMA_STAT_TS_R_TXHMBUF      0x00300000                       /**< TS: Reading Data from Host Memory Buffer and Queuing It to Tx Buffer */
#define ENUM_EMAC_DMA_STAT_TS_WR_TSTMP       0x00400000                       /**< TS: TIME_STAMP Write State */
#define ENUM_EMAC_DMA_STAT_TS_SUSPENDED      0x00600000                       /**< TS: Suspended; Tx Descriptor Unavailable or Tx Buffer Underflow */
#define ENUM_EMAC_DMA_STAT_TS_R_CLSTD        0x00700000                       /**< TS: Closing Tx Descriptor */

#define BITM_EMAC_DMA_STAT_RS                0x000E0000                       /**< Rx Process State */
#define ENUM_EMAC_DMA_STAT_RS_STOPPED        0x00000000                       /**< RS: Stopped: Reset or Stop Rx Command Issued. */
#define ENUM_EMAC_DMA_STAT_RS_R_FRD          0x00020000                       /**< RS: Running: Fetching Rx Transfer Descriptor. */
#define ENUM_EMAC_DMA_STAT_RS_R_WTRX         0x00060000                       /**< RS: Running: Waiting for Rx Packet */
#define ENUM_EMAC_DMA_STAT_RS_SUSPENDED      0x00080000                       /**< RS: Suspended: Rx Descriptor Unavailable */
#define ENUM_EMAC_DMA_STAT_RS_R_CLSRD        0x000A0000                       /**< RS: Running: Closing Rx Descriptor */
#define ENUM_EMAC_DMA_STAT_RS_WR_TSTMP       0x000C0000                       /**< RS: TIME_STAMP Write State */
#define ENUM_EMAC_DMA_STAT_RS_R_RXWRHM       0x000E0000                       /**< RS: Running: Transferring Rx Packet Data from Rx Buffer to Host Memory */
#define BITM_EMAC_DMA_STAT_NIS               0x00010000                       /**< Normal Interrupt Summary */
#define BITM_EMAC_DMA_STAT_AIS               0x00008000                       /**< Abnormal Interrupt Summary */
#define BITM_EMAC_DMA_STAT_ERI               0x00004000                       /**< Early Receive Interrupt */
#define BITM_EMAC_DMA_STAT_FBI               0x00002000                       /**< Fatal Bus Error Interrupt */
#define BITM_EMAC_DMA_STAT_ETI               0x00000400                       /**< Early Transmit Interrupt */
#define BITM_EMAC_DMA_STAT_RWT               0x00000200                       /**< Receive WatchDog Timeout */
#define BITM_EMAC_DMA_STAT_RPS               0x00000100                       /**< Receive Process Stopped */
#define BITM_EMAC_DMA_STAT_RU                0x00000080                       /**< Receive Buffer Unavailable */
#define BITM_EMAC_DMA_STAT_RI                0x00000040                       /**< Receive Interrupt */
#define BITM_EMAC_DMA_STAT_UNF               0x00000020                       /**< Transmit Buffer Underflow */
#define BITM_EMAC_DMA_STAT_OVF               0x00000010                       /**< Receive Buffer Overflow */
#define BITM_EMAC_DMA_STAT_TJT               0x00000008                       /**< Transmit Jabber Timeout */
#define BITM_EMAC_DMA_STAT_TU                0x00000004                       /**< Transmit Buffer Unavailable */
#define BITM_EMAC_DMA_STAT_TPS               0x00000002                       /**< Transmit Process Stopped */
#define BITM_EMAC_DMA_STAT_TI                0x00000001                       /**< Transmit Interrupt */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_DMA_OPMODE                      Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_DMA_OPMODE_DT              26                               /**< Disable Dropping TCP/IP Errors */
#define BITP_EMAC_DMA_OPMODE_RSF             25                               /**< Receive Store and Forward */
#define BITP_EMAC_DMA_OPMODE_DFF             24                               /**< Disable Flushing of received Frames */
#define BITP_EMAC_DMA_OPMODE_TSF             21                               /**< Transmit Store and Forward */
#define BITP_EMAC_DMA_OPMODE_FTF             20                               /**< Flush Transmit FIFO */
#define BITP_EMAC_DMA_OPMODE_TTC             14                               /**< Transmit Threshold Control */
#define BITP_EMAC_DMA_OPMODE_ST              13                               /**< Start/Stop Transmission */
#define BITP_EMAC_DMA_OPMODE_FEF              7                               /**< Forward Error Frames */
#define BITP_EMAC_DMA_OPMODE_FUF              6                               /**< Forward Undersized good Frames */
#define BITP_EMAC_DMA_OPMODE_RTC              3                               /**< Receive Threshold Control */
#define BITP_EMAC_DMA_OPMODE_OSF              2                               /**< Operate on Second Frame */
#define BITP_EMAC_DMA_OPMODE_SR               1                               /**< Start/Stop Receive */
#define BITM_EMAC_DMA_OPMODE_DT              0x04000000                       /**< Disable Dropping TCP/IP Errors */
#define BITM_EMAC_DMA_OPMODE_RSF             0x02000000                       /**< Receive Store and Forward */
#define BITM_EMAC_DMA_OPMODE_DFF             0x01000000                       /**< Disable Flushing of received Frames */
#define BITM_EMAC_DMA_OPMODE_TSF             0x00200000                       /**< Transmit Store and Forward */
#define BITM_EMAC_DMA_OPMODE_FTF             0x00100000                       /**< Flush Transmit FIFO */

#define BITM_EMAC_DMA_OPMODE_TTC             0x0001C000                       /**< Transmit Threshold Control */
#define ENUM_EMAC_DMA_OPMODE_TTC_64          0x00000000                       /**< TTC: 64 */
#define ENUM_EMAC_DMA_OPMODE_TTC_128         0x00004000                       /**< TTC: 128 */
#define ENUM_EMAC_DMA_OPMODE_TTC_192         0x00008000                       /**< TTC: 192 */
#define ENUM_EMAC_DMA_OPMODE_TTC_256         0x0000C000                       /**< TTC: 256 */
#define ENUM_EMAC_DMA_OPMODE_TTC_40          0x00010000                       /**< TTC: 40 */
#define ENUM_EMAC_DMA_OPMODE_TTC_32          0x00014000                       /**< TTC: 32 */
#define ENUM_EMAC_DMA_OPMODE_TTC_24          0x00018000                       /**< TTC: 24 */
#define ENUM_EMAC_DMA_OPMODE_TTC_16          0x0001C000                       /**< TTC: 16 */
#define BITM_EMAC_DMA_OPMODE_ST              0x00002000                       /**< Start/Stop Transmission */
#define BITM_EMAC_DMA_OPMODE_FEF             0x00000080                       /**< Forward Error Frames */
#define BITM_EMAC_DMA_OPMODE_FUF             0x00000040                       /**< Forward Undersized good Frames */

#define BITM_EMAC_DMA_OPMODE_RTC             0x00000018                       /**< Receive Threshold Control */
#define ENUM_EMAC_DMA_OPMODE_RTC_64          0x00000000                       /**< RTC: 64 */
#define ENUM_EMAC_DMA_OPMODE_RTC_32          0x00000008                       /**< RTC: 32 */
#define ENUM_EMAC_DMA_OPMODE_RTC_96          0x00000010                       /**< RTC: 96 */
#define ENUM_EMAC_DMA_OPMODE_RTC_128         0x00000018                       /**< RTC: 128 */
#define BITM_EMAC_DMA_OPMODE_OSF             0x00000004                       /**< Operate on Second Frame */
#define BITM_EMAC_DMA_OPMODE_SR              0x00000002                       /**< Start/Stop Receive */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_DMA_IEN                         Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_DMA_IEN_NIS                16                               /**< Normal Interrupt Summary Enable */
#define BITP_EMAC_DMA_IEN_AIS                15                               /**< Abnormal Interrupt Summary Enable */
#define BITP_EMAC_DMA_IEN_ERI                14                               /**< Early Receive Interrupt Enable */
#define BITP_EMAC_DMA_IEN_FBI                13                               /**< Fatal Bus Error Enable */
#define BITP_EMAC_DMA_IEN_ETI                10                               /**< Early Transmit Interrupt Enable */
#define BITP_EMAC_DMA_IEN_RWT                 9                               /**< Receive WatchdogTimeout Enable */
#define BITP_EMAC_DMA_IEN_RPS                 8                               /**< Receive Stopped Enable */
#define BITP_EMAC_DMA_IEN_RU                  7                               /**< Receive Buffer Unavailable Enable */
#define BITP_EMAC_DMA_IEN_RI                  6                               /**< Receive Interrupt Enable */
#define BITP_EMAC_DMA_IEN_UNF                 5                               /**< Underflow Interrupt Enable */
#define BITP_EMAC_DMA_IEN_OVF                 4                               /**< Overflow Interrupt Enable */
#define BITP_EMAC_DMA_IEN_TJT                 3                               /**< Transmit Jabber Timeout Enable */
#define BITP_EMAC_DMA_IEN_TU                  2                               /**< Transmit Buffer Unavailable Enable */
#define BITP_EMAC_DMA_IEN_TPS                 1                               /**< Transmit Stopped Enable */
#define BITP_EMAC_DMA_IEN_TI                  0                               /**< Transmit Interrupt Enable */
#define BITM_EMAC_DMA_IEN_NIS                0x00010000                       /**< Normal Interrupt Summary Enable */
#define BITM_EMAC_DMA_IEN_AIS                0x00008000                       /**< Abnormal Interrupt Summary Enable */
#define BITM_EMAC_DMA_IEN_ERI                0x00004000                       /**< Early Receive Interrupt Enable */
#define BITM_EMAC_DMA_IEN_FBI                0x00002000                       /**< Fatal Bus Error Enable */
#define BITM_EMAC_DMA_IEN_ETI                0x00000400                       /**< Early Transmit Interrupt Enable */
#define BITM_EMAC_DMA_IEN_RWT                0x00000200                       /**< Receive WatchdogTimeout Enable */
#define BITM_EMAC_DMA_IEN_RPS                0x00000100                       /**< Receive Stopped Enable */
#define BITM_EMAC_DMA_IEN_RU                 0x00000080                       /**< Receive Buffer Unavailable Enable */
#define BITM_EMAC_DMA_IEN_RI                 0x00000040                       /**< Receive Interrupt Enable */
#define BITM_EMAC_DMA_IEN_UNF                0x00000020                       /**< Underflow Interrupt Enable */
#define BITM_EMAC_DMA_IEN_OVF                0x00000010                       /**< Overflow Interrupt Enable */
#define BITM_EMAC_DMA_IEN_TJT                0x00000008                       /**< Transmit Jabber Timeout Enable */
#define BITM_EMAC_DMA_IEN_TU                 0x00000004                       /**< Transmit Buffer Unavailable Enable */
#define BITM_EMAC_DMA_IEN_TPS                0x00000002                       /**< Transmit Stopped Enable */
#define BITM_EMAC_DMA_IEN_TI                 0x00000001                       /**< Transmit Interrupt Enable */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_DMA_MISS_FRM                    Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_DMA_MISS_FRM_OVFFIFO       28                               /**< Overflow bit for FIFO Overflow Counter */
#define BITP_EMAC_DMA_MISS_FRM_MISSFROV      17                               /**< Missed Frames Buffer Overflow */
#define BITP_EMAC_DMA_MISS_FRM_OVFMISS       16                               /**< Overflow bit for Missed Frame Counter */
#define BITP_EMAC_DMA_MISS_FRM_MISSFRUN       0                               /**< Missed Frames Unavailable Buffer */
#define BITM_EMAC_DMA_MISS_FRM_OVFFIFO       0x10000000                       /**< Overflow bit for FIFO Overflow Counter */
#define BITM_EMAC_DMA_MISS_FRM_MISSFROV      0x0FFE0000                       /**< Missed Frames Buffer Overflow */
#define BITM_EMAC_DMA_MISS_FRM_OVFMISS       0x00010000                       /**< Overflow bit for Missed Frame Counter */
#define BITM_EMAC_DMA_MISS_FRM_MISSFRUN      0x0000FFFF                       /**< Missed Frames Unavailable Buffer */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_DMA_RXIWDOG                     Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_DMA_RXIWDOG_RIWT            0                               /**< RI WatchDog Timer Count */
#define BITM_EMAC_DMA_RXIWDOG_RIWT           0x000000FF                       /**< RI WatchDog Timer Count */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_DMA_BMMODE                      Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_DMA_BMMODE_WROSRLMT        20                               /**< SCB Maximum Write Outstanding Request */
#define BITP_EMAC_DMA_BMMODE_RDOSRLMT        16                               /**< SCB Maximum Read Outstanding Request */
#define BITP_EMAC_DMA_BMMODE_AAL             12                               /**< Address Aligned Beats */
#define BITP_EMAC_DMA_BMMODE_BLEN16           3                               /**< SCB Burst Length 16 */
#define BITP_EMAC_DMA_BMMODE_BLEN8            2                               /**< SCB Burst Length 8 */
#define BITP_EMAC_DMA_BMMODE_BLEN4            1                               /**< SCB Burst Length 4 */
#define BITP_EMAC_DMA_BMMODE_UNDEF            0                               /**< SCB Undefined Burst Length */
#define BITM_EMAC_DMA_BMMODE_WROSRLMT        0x00700000                       /**< SCB Maximum Write Outstanding Request */
#define BITM_EMAC_DMA_BMMODE_RDOSRLMT        0x00070000                       /**< SCB Maximum Read Outstanding Request */
#define BITM_EMAC_DMA_BMMODE_AAL             0x00001000                       /**< Address Aligned Beats */
#define BITM_EMAC_DMA_BMMODE_BLEN16          0x00000008                       /**< SCB Burst Length 16 */
#define BITM_EMAC_DMA_BMMODE_BLEN8           0x00000004                       /**< SCB Burst Length 8 */
#define BITM_EMAC_DMA_BMMODE_BLEN4           0x00000002                       /**< SCB Burst Length 4 */
#define BITM_EMAC_DMA_BMMODE_UNDEF           0x00000001                       /**< SCB Undefined Burst Length */

/**< ------------------------------------------------------------------------------------------------------------------------
        EMAC_DMA_BMSTAT                      Pos/Masks                        Description
   ------------------------------------------------------------------------------------------------------------------------ */
#define BITP_EMAC_DMA_BMSTAT_BUSRD            1                               /**< Bus (SCB master) Read Active */
#define BITP_EMAC_DMA_BMSTAT_BUSWR            0                               /**< Bus (SCB master) Write Active */
#define BITM_EMAC_DMA_BMSTAT_BUSRD           0x00000002                       /**< Bus (SCB master) Read Active */
#define BITM_EMAC_DMA_BMSTAT_BUSWR           0x00000001                       /**< Bus (SCB master) Write Active */

#endif // ifndef CYGONCE_HAL_BF60X_ETH_H