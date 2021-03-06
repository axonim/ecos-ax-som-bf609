 /*
 * uClinux
 * File:        arch/blackfin/mach-common/flush.S
 * Based on:
 * Author:      LG SOft India
 *              COPYRIGHT 2005 Analog Devices
 * Created:     ?
 * Description: This is an external function being called by the user
 * application through __flush_cache_all. Currently this function
 * serves the purpose of flushing all the pending writes in
 * in the instruction cache.
 *
 * Rev:
 *
 * Modified:
 *
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.
 * If not, write to the Free Software Foundation,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <cyg/hal/bfin-regs.h>

    .section ".interrupt_vectors","ax"

#define ENTRY(name)    \
        .type name,@function;   \
        .globl name;        \
name:



ENTRY(_flush_instruction_cache)
	[--SP] = ( R7:6, P5:4 );
	LINK 12;
	SP += -12;
	P5.H = (ICPLB_ADDR0 >> 16);
	P5.L = (ICPLB_ADDR0 & 0xFFFF);
	P4.H = (ICPLB_DATA0 >> 16);
	P4.L = (ICPLB_DATA0 & 0xFFFF);
	R7 = HAL_ICACHE_SET_VALID | HAL_ICACHE_SET_CACHABLE;
	R6 = 16;
inext:	R0 = [P5++];
	R1 = [P4++];
	[--SP] =  RETS;
	CALL _icplb_flush;	/* R0 = page, R1 = data*/
	RETS = [SP++];
iskip:	R6 += -1;
	CC = R6;
	IF CC JUMP inext (bp);
	SSYNC;
	SP += 12;
	UNLINK;
	( R7:6, P5:4 ) = [SP++];
	RTS;

/* This is an internal function to flush all pending
 * writes in the cache associated with a particular ICPLB.
 *
 * R0 -  page's start address
 * R1 -  CPLB's data field.
 */

.align 2
ENTRY(_icplb_flush)
	[--SP] = ( R7:4, P5:3 );

	/* If it's a 1K or 4K page, then it's quickest to
	 * just systematically flush all the addresses in
	 * the page, regardless of whether they're in the
	 * cache, or dirty. If it's a 1M or 4M page, there
	 * are too many addresses, and we have to search the
	 * cache for lines corresponding to the page.
	 */

	CC = BITTST(R1, 17);	/* 1MB or 4MB */
	IF !CC JUMP iflush_whole_page;

	/* We're only interested in the page's size, so extract
	 * this from the CPLB (bits 17:16), and scale to give an
	 * offset into the page_size and page_prefix tables.
	 */

	R1 <<= 14;
	R1 >>= 30;
	R1 <<= 2;

	/* We can also determine the sub-bank used, because this is
	 * taken from bits 13:12 of the address.
	 */

	R3 = ((12<<8)|2);		/* Extraction pattern */
	nop;				/*Anamoly 05000209*/
	R4 = EXTRACT(R0, R3.L) (Z);	/* Extract bits*/
	R3.H = R4.L << 0 ;		/* Save in extraction pattern for
					 * later deposit.
					 */


	/* So:
	 * R0 = Page start
	 * R1 = Page length (actually, offset into size/prefix tables)
	 * R3 = sub-bank deposit values
  	 *
	 * The cache has 2 Ways, and 64 sets, so we iterate through
	 * the sets, accessing the tag for each Way, for our Bank and
	 * sub-bank, looking for dirty, valid tags that match our
	 * address prefix.
	 */

	P5.L = (ITEST_COMMAND & 0xFFFF);
	P5.H = (ITEST_COMMAND >> 16);
	P4.L = (ITEST_DATA0 & 0xFFFF);
	P4.H = (ITEST_DATA0 >> 16);

	P0.L = page_prefix_table;
	P0.H = page_prefix_table;
	P1 = R1;
	R5 = 0;			/* Set counter*/
	P0 = P1 + P0;
	R4 = [P0];		/* This is the address prefix*/

	/* We're reading (bit 1==0) the tag (bit 2==0), and we
	 * don't care about which double-word, since we're only
	 * fetching tags, so we only have to set Set, Bank,
	 * Sub-bank and Way.
	 */

	P2 = 4;
	LSETUP (ifs1, ife1) LC1 = P2;
ifs1:	P0 = 32;		/* iterate over all sets*/
	LSETUP (ifs0, ife0) LC0 = P0;
ifs0:	R6 = R5 << 5;		/* Combine set*/
	R6.H = R3.H << 0 ;	/* and sub-bank*/
	[P5] = R6;		/* Issue Command*/
	SSYNC;			/* CSYNC will not work here :(*/
	R7 = [P4];		/* and read Tag.*/
	CC = BITTST(R7, 0);	/* Check if valid*/
	IF !CC JUMP ifskip;	/* and skip if not.*/

	/* Compare against the page address. First, plant bits 13:12
	 * into the tag, since those aren't part of the returned data.
	 */

	R7 = DEPOSIT(R7, R3);	/* set 13:12*/
	R1 = R7 & R4;		/* Mask off lower bits*/
	CC = R1 == R0;		/* Compare against page start.*/
	IF !CC JUMP ifskip;	/* Skip it if it doesn't match.*/

	/* Tag address matches against page, so this is an entry
	 * we must flush.
	 */

	R7 >>= 10;		/* Mask off the non-address bits*/
	R7 <<= 10;
	P3 = R7;
	IFLUSH [P3];		/* And flush the entry*/
ifskip:
ife0:	R5 += 1;		/* Advance to next Set*/
ife1:	NOP;

ifinished:
	SSYNC;			/* Ensure the data gets out to mem.*/

	/*Finished. Restore context.*/
	( R7:4, P5:3 ) = [SP++];
	RTS;

iflush_whole_page:
	/* It's a 1K or 4K page, so quicker to just flush the
	 * entire page.
	 */

	P1 = 32;		/* For 1K pages*/
	P2 = P1 << 2;		/* For 4K pages*/
	P0 = R0;		/* Start of page*/
	CC = BITTST(R1, 16);	/* Whether 1K or 4K*/
	IF CC P1 = P2;
	P1 += -1;		/* Unroll one iteration*/
	SSYNC;
	IFLUSH [P0++];		/* because CSYNC can't end loops.*/
	LSETUP (isall, ieall) LC0 = P1;
isall:IFLUSH [P0++];
ieall: NOP;
	SSYNC;
	JUMP ifinished;

/* This is an external function being called by the user
 * application through __flush_cache_all. Currently this function
 * serves the purpose of flushing all the pending writes in
 * in the data cache.
 */

ENTRY(_flush_data_cache)
	[--SP] = ( R7:6, P5:4 );
	LINK 12;
	SP += -12;
	P5.H = (DCPLB_ADDR0 >> 16);
	P5.L = (DCPLB_ADDR0 & 0xFFFF);
	P4.H = (DCPLB_DATA0 >> 16);
	P4.L = (DCPLB_DATA0 & 0xFFFF);
	R7 = HAL_DCACHE_SET_VALID | HAL_DCACHE_SET_CACHABLE | HAL_DCACHE_SET_DIRTY (Z);
	R6 = 16;
next:	R0 = [P5++];
	R1 = [P4++];
	CC = BITTST(R1, 14);	/* Is it write-through?*/
	IF CC JUMP skip;	/* If so, ignore it.*/
	R2 = R1 & R7;		/* Is it a dirty, cached page?*/
	CC = R2;
	IF !CC JUMP skip;	/* If not, ignore it.*/
	[--SP] = RETS;
	CALL _dcplb_flush;	/* R0 = page, R1 = data*/
	RETS = [SP++];
skip:	R6 += -1;
	CC = R6;
	IF CC JUMP next (bp);
	SSYNC;
	SP += 12;
	UNLINK;
	( R7:6, P5:4 ) = [SP++];
	RTS;

/* This is an internal function to flush all pending
 * writes in the cache associated with a particular DCPLB.
 *
 * R0 -  page's start address
 * R1 -  CPLB's data field.
 */

.align 2
ENTRY(_dcplb_flush)
	[--SP] = ( R7:4, P5:3 );

	/* If it's a 1K or 4K page, then it's quickest to
	 * just systematically flush all the addresses in
	 * the page, regardless of whether they're in the
	 * cache, or dirty. If it's a 1M or 4M page, there
	 * are too many addresses, and we have to search the
	 * cache for lines corresponding to the page.
	 */

	CC = BITTST(R1, 17);	/* 1MB or 4MB */
	IF !CC JUMP dflush_whole_page;

	/* We're only interested in the page's size, so extract
	 * this from the CPLB (bits 17:16), and scale to give an
	 * offset into the page_size and page_prefix tables.
	 */

	R1 <<= 14;
	R1 >>= 30;
	R1 <<= 2;

	/* The page could be mapped into Bank A or Bank B, depending
	 * on (a) whether both banks are configured as cache, and
	 * (b) on whether address bit A[x] is set. x is determined
	 * by DCBS in DMEM_CONTROL
	 */

	R2 = 0;			/* Default to Bank A (Bank B would be 1)*/

	P0.L = (DMEM_CONTROL & 0xFFFF);
	P0.H = (DMEM_CONTROL >> 16);

	R3 = [P0];		/* If Bank B is not enabled as cache*/
	CC = BITTST(R3, 2);	/* then Bank A is our only option.*/
	IF CC JUMP bank_chosen;

	R4 = 1<<14;		/* If DCBS==0, use A[14].*/
	R5 = R4 << 7;		/* If DCBS==1, use A[23];*/
	CC = BITTST(R3, 4);
	IF CC R4 = R5;		/* R4 now has either bit 14 or bit 23 set.*/
	R5 = R0 & R4;		/* Use it to test the Page address*/
	CC = R5;		/* and if that bit is set, we use Bank B,*/
	R2 = CC;		/* else we use Bank A.*/
	R2 <<= 23;		/* The Bank selection's at posn 23.*/

bank_chosen:

	/* We can also determine the sub-bank used, because this is
	 * taken from bits 13:12 of the address.
	 */

	R3 = ((12<<8)|2);		/* Extraction pattern */
	nop;				/*Anamoly 05000209*/
	R4 = EXTRACT(R0, R3.L) (Z);	/* Extract bits*/
	R3.H = R4.L << 0 ;		/* Save in extraction pattern for
					 * later deposit.
					 */

	/* So:
	 * R0 = Page start
	 * R1 = Page length (actually, offset into size/prefix tables)
	 * R2 = Bank select mask
	 * R3 = sub-bank deposit values
  	 *
	 * The cache has 2 Ways, and 64 sets, so we iterate through
	 * the sets, accessing the tag for each Way, for our Bank and
	 * sub-bank, looking for dirty, valid tags that match our
	 * address prefix.
	 */

	P5.L = (DTEST_COMMAND & 0xFFFF);
	P5.H = (DTEST_COMMAND >> 16);
	P4.L = (DTEST_DATA0 & 0xFFFF);
	P4.H = (DTEST_DATA0 >> 16);

	P0.L = page_prefix_table;
	P0.H = page_prefix_table;
	P1 = R1;
	R5 = 0;			/* Set counter*/
	P0 = P1 + P0;
	R4 = [P0];		/* This is the address prefix*/


	/* We're reading (bit 1==0) the tag (bit 2==0), and we
	 * don't care about which double-word, since we're only
	 * fetching tags, so we only have to set Set, Bank,
	 * Sub-bank and Way.
	 */

	P2 = 2;
	LSETUP (fs1, fe1) LC1 = P2;
fs1:	P0 = 64;		/* iterate over all sets*/
	LSETUP (fs0, fe0) LC0 = P0;
fs0:	R6 = R5 << 5;		/* Combine set*/
	R6.H = R3.H << 0 ;	/* and sub-bank*/
	R6 = R6 | R2;		/* and Bank. Leave Way==0 at first.*/
	BITSET(R6,14);
	[P5] = R6;		/* Issue Command*/
	SSYNC;
	R7 = [P4];		/* and read Tag.*/
	CC = BITTST(R7, 0);	/* Check if valid*/
	IF !CC JUMP fskip;	/* and skip if not.*/
	CC = BITTST(R7, 1);	/* Check if dirty*/
	IF !CC JUMP fskip;	/* and skip if not.*/

	/* Compare against the page address. First, plant bits 13:12
	 * into the tag, since those aren't part of the returned data.
	 */

	R7 = DEPOSIT(R7, R3);	/* set 13:12*/
	R1 = R7 & R4;		/* Mask off lower bits*/
	CC = R1 == R0;		/* Compare against page start.*/
	IF !CC JUMP fskip;	/* Skip it if it doesn't match.*/

	/* Tag address matches against page, so this is an entry
	 * we must flush.
	 */

	R7 >>= 10;		/* Mask off the non-address bits*/
	R7 <<= 10;
	P3 = R7;
	SSYNC;
	FLUSHINV [P3];		/* And flush the entry*/
fskip:
fe0:	R5 += 1;		/* Advance to next Set*/
fe1:	BITSET(R2, 26);		/* Go to next Way.*/

dfinished:
	SSYNC;			/* Ensure the data gets out to mem.*/

	/*Finished. Restore context.*/
	( R7:4, P5:3 ) = [SP++];
	RTS;

dflush_whole_page:

	/* It's a 1K or 4K page, so quicker to just flush the
	 * entire page.
	 */

	P1 = 32;		/* For 1K pages*/
	P2 = P1 << 2;		/* For 4K pages*/
	P0 = R0;		/* Start of page*/
	CC = BITTST(R1, 16);	/* Whether 1K or 4K*/
	IF CC P1 = P2;
	P1 += -1;		/* Unroll one iteration*/
    SSYNC;
	FLUSHINV [P0++];	/* because CSYNC can't end loops.*/
	LSETUP (eall, eall) LC0 = P1;
eall:	FLUSHINV [P0++];
	SSYNC;
	JUMP dfinished;

.align 4;
page_prefix_table:
.byte4 	0xFFFFFC00;	/* 1K */
.byte4	0xFFFFF000;	/* 4K */
.byte4	0xFFF00000;	/* 1M */
.byte4	0xFFC00000;	/* 4M */
.page_prefix_table.end:
