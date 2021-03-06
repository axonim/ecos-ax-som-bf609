// eCos memory layout

#ifndef __ASSEMBLER__
#include <cyg/infra/cyg_type.h>
#include <stddef.h>

#endif
#define CYGMEM_REGION_sram (0x20000000 - CYGHWR_HAL_KINETIS_SRAM_BANK_SIZE)
#define CYGMEM_REGION_sram_SIZE (CYGHWR_HAL_KINETIS_SRAM_SIZE-CYGNUM_HAL_COMMON_INTERRUPTS_STACK_SIZE)
#define CYGMEM_REGION_sram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)

#define CYGMEM_REGION_flash (0x00000000)
#define CYGMEM_REGION_flash_SIZE (CYGHWR_HAL_KINETIS_FLASH_SIZE)
#define CYGMEM_REGION_flash_ATTR (CYGMEM_REGION_ATTR_R)

#define CYGMEM_REGION_ramcod (CYGHWR_HAL_KINETIS_DDR_CODE_BASE)
#define CYGMEM_REGION_ramcod_SIZE (CYGHWR_HAL_KINETIS_DDR_CODE_SIZE)
#define CYGMEM_REGION_ramcod_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)

#define CYGMEM_REGION_ram (CYGHWR_HAL_KINETIS_DDR_CACHED_BASE)
#define CYGMEM_REGION_ram_SIZE (CYGHWR_HAL_KINETIS_DDR_CACHED_SIZE)
#define CYGMEM_REGION_ram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)

#define CYGMEM_REGION_ramnc (CYGHWR_HAL_KINETIS_DDR_NON_CACHED_BASE)
#define CYGMEM_REGION_ramnc_SIZE (CYGHWR_HAL_KINETIS_DDR_NON_CACHED_SIZE)
#define CYGMEM_REGION_ramnc_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)

#ifndef __ASSEMBLER__
extern char CYG_LABEL_NAME (__heap1) [];
#endif
#define CYGMEM_SECTION_heap1 (CYG_LABEL_NAME (__heap1))
#define CYGMEM_SECTION_heap1_SIZE (CYGMEM_REGION_ram+CYGMEM_REGION_ram_SIZE - (size_t) CYG_LABEL_NAME (__heap1))
