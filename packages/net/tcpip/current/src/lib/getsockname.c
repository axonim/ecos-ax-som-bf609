//==========================================================================
//
//      lib/getsockname.c
//
//      getsockname() system call
//
//==========================================================================
// ####BSDALTCOPYRIGHTBEGIN####                                             
// -------------------------------------------                              
// Portions of this software may have been derived from OpenBSD             
// or other sources, and if so are covered by the appropriate copyright     
// and license included herein.                                             
// -------------------------------------------                              
// ####BSDALTCOPYRIGHTEND####                                               
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    gthomas
// Contributors: gthomas
// Date:         2000-01-10
// Purpose:      
// Description:  
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================


#include <sys/param.h>
#include <cyg/io/file.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <sys/syscallargs.h>

int 
getsockname(int s, const struct sockaddr *name, socklen_t *namelen)
{
    struct sys_getsockname_args args;
    int res, error;
    SYSCALLARG(args,fdes) = s;
    SYSCALLARG(args,asa) = (struct sockaddr *)name;
    SYSCALLARG(args,alen) = namelen;
    error = sys_getsockname(&args, &res);
    if (error) {
        errno = error;
        return -1;
    } else {
        return 0;
    }
}
