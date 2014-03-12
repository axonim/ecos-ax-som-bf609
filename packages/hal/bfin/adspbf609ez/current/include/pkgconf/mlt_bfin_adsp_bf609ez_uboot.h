// eCos memory layout - Fri Oct 20 06:11:28 2000

// This is a generated file - do not edit

#ifndef __ASSEMBLER__
#include <cyg/infra/cyg_type.h>
#include <stddef.h>
#include <pkgconf/hal_bfin_bf60x.h>

#endif
#define CYGMEM_REGION_ram (0x00000000)
#ifdef CYGPKG_HAL_BFIN_BF537
#define CYGMEM_REGION_ram_SIZE (0x04000000)
#elif defined(CYGPKG_HAL_BFIN_BF533)
    #define CYGMEM_REGION_ram_SIZE (0x08000000)
#else
    #error please select a valid processor type
#endif
#define CYGMEM_REGION_ram_INVALID (CYGMEM_REGION_ram + CYGMEM_REGION_ram_SIZE)
#define CYGMEM_REGION_ram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)
#define CYGMEM_REGION_rom (0x20000000)
#define CYGMEM_REGION_rom_SIZE (0x00400000)
#define CYGMEM_REGION_rom_ATTR (CYGMEM_REGION_ATTR_R)
#ifndef __ASSEMBLER__
extern char CYG_LABEL_NAME (__heap1) [];
#endif
#define CYGMEM_SECTION_heap1 (CYG_LABEL_NAME (__heap1))
#define CYGMEM_SECTION_heap1_SIZE (0x01000000 - (size_t) CYG_LABEL_NAME (__heap1))
