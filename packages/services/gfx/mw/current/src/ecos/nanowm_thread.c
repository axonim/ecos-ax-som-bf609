#include <pkgconf/system.h>     /* which packages are enabled/disabled */
#ifdef CYGPKG_KERNEL
# include <pkgconf/kernel.h>
#endif
#ifdef CYGPKG_LIBC
# include <pkgconf/libc.h>
#endif
#ifdef CYGPKG_IO_SERIAL
# include <pkgconf/io_serial.h>
#endif

#ifndef CYGFUN_KERNEL_API_C
# error Kernel API must be enabled to build this application
#endif

#ifndef CYGPKG_LIBC_STDIO
# error C library standard I/O must be enabled to build this application
#endif

#ifndef CYGPKG_IO_SERIAL_HALDIAG
# error I/O HALDIAG pseudo-device driver must be enabled to build this application
#endif

/* INCLUDES */

#include <stdio.h>                      /* printf */
#include <stdlib.h>                      /* printf */
#include <string.h>                     /* strlen */
#include <cyg/kernel/kapi.h>            /* All the kernel specific stuff */
#include <cyg/infra/diag.h>        

#define MWINCLUDECOLORS
#include "nano-X.h"

#include <microwin/ecos_mw_app.h>

extern void ecos_nx_init(CYG_ADDRWORD data);
_mw_app("Nano-WM", nanowm, ECOS_MW_NANOX_PRIORITY, ecos_nx_init);

//
// Component interfaces
//

externC int nanowm_main(int argc, char *argv[]);

static void
nanowm_thread(CYG_ADDRWORD data)
{
    int argc = 0;
    char **argv;

    INIT_PER_THREAD_DATA();
    nanowm_main(argc, argv);
}
