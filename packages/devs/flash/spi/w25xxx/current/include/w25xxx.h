#ifndef CYGONCE_DEVS_FLASH_SPI_W25XXX_H
#define CYGONCE_DEVS_FLASH_SPI_W25XXX_H

//=============================================================================
//
//      w25xxx.h
//
//      SPI flash driver for Winbond W25xxx devices and compatibles.
//
//=============================================================================
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   Daniel Helgason
// Date:        2011-01-01
// Purpose:     Windbond W25Xxx SPI flash driver implementation
//
//####DESCRIPTIONEND####
//
//=============================================================================

// Required data structures.
#include <cyg/io/flash_dev.h>

// Exported handle on the driver function table.
externC struct cyg_flash_dev_funs cyg_devs_flash_spi_w25xxx_funs;

//-----------------------------------------------------------------------------
// Macro used to generate a flash device object with the default W25XXX
// settings.  Even though the block info data structure is declared here, the
// details are not filled in until the device type is inferred during
// initialisation.  This also applies to the 'end' field which is calculated
// using the _start_ address and the inferred size of the device.
// _name_   is the root name of the instantiated data structures.
// _start_  is the base address of the device - for SPI based devices this can
//          have an arbitrary value, since the device is not memory mapped.
// _spidev_ is a pointer to a SPI device object of type cyg_spi_device.  This
//          is not typechecked during compilation so be careful!

#define CYG_DEVS_FLASH_SPI_W25XXX_DRIVER(_name_, _start_, _spidev_) \
struct cyg_flash_block_info _name_ ##_block_info; \
CYG_FLASH_DRIVER(_name_, &cyg_devs_flash_spi_w25xxx_funs, 0, \
  _start_, _start_, 1, & _name_ ##_block_info, (void*) _spidev_)

//=============================================================================

#endif // CYGONCE_DEVS_FLASH_SPI_W25XXX_H
