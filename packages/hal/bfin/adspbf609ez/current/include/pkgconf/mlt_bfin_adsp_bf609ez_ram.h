// eCos memory layout - Fri Oct 20 06:11:28 2000

// This is a generated file - do not edit

#ifndef __ASSEMBLER__
#include <cyg/infra/cyg_type.h>
#include <stddef.h>

#endif
#define CYGMEM_REGION_ram (0x00000000)
#define CYGMEM_REGION_ram_SIZE (0x10000000)
#define CYGMEM_REGION_ram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)

#define CYGMEM_REGION_async (0xB0000000)
#define CYGMEM_REGION_async_SIZE (0x10000000)
#define CYGMEM_REGION_async_ATTR (CYGMEM_REGION_ATTR_R)

#ifndef __ASSEMBLER__
extern char CYG_LABEL_NAME (__heap1) [];
#endif

#define CYGMEM_SECTION_heap1 (CYG_LABEL_NAME (__heap1))
//#define CYGMEM_SECTION_heap1_SIZE (0x00010000)
#define CYGMEM_SECTION_heap1_SIZE (0x01000000 - (size_t) CYG_LABEL_NAME (__heap1))