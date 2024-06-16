#ifndef LIBKIWI_UTIL_PTR_UTIL_H
#define LIBKIWI_UTIL_PTR_UTIL_H
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Memory utilities
 */
class PtrUtil {
public:
    /**
     * @name Validity
     */
    /**@{*/
    /**
     * @brief Tests whether an address is a valid pointer
     *
     * @param addr Address
     */
    static bool IsPointer(const void* addr);

    /**
     * @brief Tests whether an address is a valid pointer, and is aligned to the
     * specified number of bytes
     *
     * @param addr Address
     * @param align Byte alignment
     */
    static bool IsAlignedPointer(const void* addr, u32 align);
    /**@}*/

    /**
     * @name Domain
     */
    /**@{*/
    /**
     * @brief Tests whether an address points to libkiwi memory
     *
     * @param addr Address
     */
    static bool IsLibKiwi(const void* addr);

    /**
     * @brief Tests whether an address points to stack memory
     *
     * @param addr Address
     */
    static bool IsStack(const void* addr);
    /**@}*/

    /**
     * @name Type heuristics
     */
    /**@{*/
    /**
     * @brief Tests whether an address likely points to a floating-point value
     *
     * @param addr Address
     */
    static bool IsFloat(const void* addr);

    /**
     * @brief Tests whether an address likely points to or falls within a string
     *
     * @param addr Address
     */
    static bool IsString(const void* addr);

    /**
     * @brief Tests whether an address likely points to or falls within a PTMF
     *
     * @param addr Address
     */
    static bool IsPtmf(const void* addr);

    /**
     * @brief Tests whether the specified address points to a heap block tag
     *
     * @param addr Address
     */
    static bool IsMBlockTag(const void* addr);
    /**@}*/
};

//! @}
} // namespace kiwi

#endif
