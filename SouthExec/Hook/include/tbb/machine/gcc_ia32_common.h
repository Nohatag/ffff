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

#ifndef __TBB_machine_gcc_ia32_common_H
#define __TBB_machine_gcc_ia32_common_H

#ifndef __TBB_Log2

template <typename T>
static inline intptr_t __TBB_machine_lg( T x ) {
    __TBB_ASSERT(x>0, "The logarithm of a non-positive value is undefined.");
    uintptr_t j, i = x;
    __asm__("bsr %1,%0" : "=r"(j) : "r"(i));
    return j;
}
#define __TBB_Log2(V)  __TBB_machine_lg(V)
#endif /* !__TBB_Log2 */

#ifndef __TBB_Pause

static inline void __TBB_machine_pause( int32_t delay ) {
    for (int32_t i = 0; i < delay; i++) {
       __asm__ __volatile__("pause;");
    }
    return;
}
#define __TBB_Pause(V) __TBB_machine_pause(V)
#endif /* !__TBB_Pause */

namespace tbb { namespace internal { typedef uint64_t machine_tsc_t; } }
static inline tbb::internal::machine_tsc_t __TBB_machine_time_stamp() {
#if __INTEL_COMPILER
    return _rdtsc();
#else
    tbb::internal::uint32_t hi, lo;
    __asm__ __volatile__("rdtsc" : "=d"(hi), "=a"(lo));
    return (tbb::internal::machine_tsc_t( hi ) << 32) | lo;
#endif
}
#define __TBB_time_stamp() __TBB_machine_time_stamp()

#ifndef __TBB_CPU_CTL_ENV_PRESENT
#define __TBB_CPU_CTL_ENV_PRESENT 1
namespace tbb {
namespace internal {
class cpu_ctl_env {
private:
    int     mxcsr;
    short   x87cw;
    static const int MXCSR_CONTROL_MASK = ~0x3f; /* all except last six status bits */
public:
    bool operator!=( const cpu_ctl_env& ctl ) const { return mxcsr != ctl.mxcsr || x87cw != ctl.x87cw; }
    void get_env() {
    #if __TBB_ICC_12_0_INL_ASM_FSTCW_BROKEN
        cpu_ctl_env loc_ctl;
        __asm__ __volatile__ (
                "stmxcsr %0\n\t"
                "fstcw %1"
                : "=m"(loc_ctl.mxcsr), "=m"(loc_ctl.x87cw)
        );
        *this = loc_ctl;
    #else
        __asm__ __volatile__ (
                "stmxcsr %0\n\t"
                "fstcw %1"
                : "=m"(mxcsr), "=m"(x87cw)
        );
    #endif
        mxcsr &= MXCSR_CONTROL_MASK;
    }
    void set_env() const {
        __asm__ __volatile__ (
                "ldmxcsr %0\n\t"
                "fldcw %1"
                : : "m"(mxcsr), "m"(x87cw)
        );
    }
};
} 
} 
#endif /* !__TBB_CPU_CTL_ENV_PRESENT */

#include "gcc_itsx.h"

#endif /* __TBB_machine_gcc_ia32_common_H */
