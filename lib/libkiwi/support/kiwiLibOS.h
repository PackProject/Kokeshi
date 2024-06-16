#ifndef LIBKIWI_SUPPORT_LIBOS_H
#define LIBKIWI_SUPPORT_LIBOS_H
#include <libkiwi/k_types.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_support
//! @{

/**
 * @brief OS library wrapper/extension
 */
class LibOS {
public:
    static void FillFPUContext(OSContext* ctx);
};

//! @}
} // namespace kiwi

#endif
