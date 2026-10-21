// Leaked By Cry Service & Siraj
/*
    Copyright (c) 2005-2018 Intel Corporation

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/

#ifndef __TBB_runtime_loader_H
#define __TBB_runtime_loader_H

#if ! TBB_PREVIEW_RUNTIME_LOADER
    #error Set TBB_PREVIEW_RUNTIME_LOADER to include runtime_loader.h
#endif

#include "tbb_stddef.h"
#include <climits>

#if _MSC_VER
    #if ! __TBB_NO_IMPLICIT_LINKAGE
        #ifdef _DEBUG
            #pragma comment( linker, "/nodefaultlib:tbb_debug.lib" )
            #pragma comment( linker, "/defaultlib:tbbproxy_debug.lib" )
        #else
            #pragma comment( linker, "/nodefaultlib:tbb.lib" )
            #pragma comment( linker, "/defaultlib:tbbproxy.lib" )
        #endif
    #endif
#endif

namespace tbb {

namespace interface6 {

/*!

\b Usage:

In source code:

\code
#include "tbb/runtime_loader.h"

char const * path[] = { "<install dir>/lib/ia32", NULL };
tbb::runtime_loader loader( path );

// Now use TBB.
\endcode

Link with \c tbbproxy.lib (or \c libtbbproxy.a) instead of \c tbb.lib (\c libtbb.dylib,
\c libtbb.so).

TBB library will be loaded at runtime from \c <install dir>/lib/ia32 directory.

\b Attention:

All \c runtime_loader objects (in the same module, i.e. exe or dll) share some global state.
The most noticeable piece of global state is loaded TBB library.
There are some implications:

    -   Only one TBB library can be loaded per module.

    -   If one object has already loaded TBB library, another object will not load TBB.
        If the loaded TBB library is suitable for the second object, both will use TBB
        cooperatively, otherwise the second object will report an error.

    -   \c runtime_loader objects will not work (correctly) in parallel due to absence of
        synchronization.

*/

class runtime_loader : tbb::internal::no_copy {

    public:

        enum error_mode {
            em_status,     
            em_throw,      
            em_abort       
        }; 

        enum error_code {
            ec_ok,         
            ec_bad_call,   
            ec_bad_arg,    
            ec_bad_lib,    
            ec_bad_ver,    
            ec_no_lib      
        }; 

        runtime_loader( error_mode mode = em_abort );

        /*!
            See load() for details.

            If error mode is \c em_status, call status() to check whether TBB was loaded or not.
        */
        runtime_loader(
            char const * path[],                           
            int          min_ver = TBB_INTERFACE_VERSION,  
            int          max_ver = INT_MAX,                
            error_mode   mode    = em_abort                
        );

        ~runtime_loader();

        /*!
            The method searches the directories specified in \c path[] array for the TBB library.
            When the library is found, it is loaded and its version is checked. If the version is
            not suitable, the library is unloaded, and the search continues.

            \b Note:

            For security reasons, avoid using relative directory names. For example, never load
            TBB from current (\c "."), parent (\c "..") or any other relative directory (like
            \c "lib" ). Use only absolute directory names (e. g. "/usr/local/lib").

            For the same security reasons, avoid using system default directories (\c "") on
            Windows. (See http://www.microsoft.com/technet/security/advisory/2269637.mspx for
            details.)

            Neglecting these rules may cause your program to execute 3-rd party malicious code.

            \b Errors:
                -   \c ec_bad_call - TBB already loaded by this object.
                -   \c ec_bad_arg - \p min_ver and/or \p max_ver negative or zero,
                    or \p min_ver > \p max_ver.
                -   \c ec_bad_ver - TBB of unsuitable version already loaded by another object.
                -   \c ec_no_lib - No suitable library found.
        */
        error_code
        load(
            char const * path[],                           
            int          min_ver = TBB_INTERFACE_VERSION,  
            int          max_ver = INT_MAX                 

        );

        /*!
            If error mode is \c em_status, the function returns status of the last operation.
        */
        error_code status();

    private:

        error_mode const my_mode;
        error_code       my_status;
        bool             my_loaded;

}; 

} 

using interface6::runtime_loader;

} 

#endif /* __TBB_runtime_loader_H */

