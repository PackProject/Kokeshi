#include <libkiwi.h>

#include <cctype>

namespace kiwi {

/**
 * @brief Tests whether an address is a valid pointer
 *
 * @param addr Address
 */
bool PtrUtil::IsPointer(const void* addr) {
    uintptr_t bits = reinterpret_cast<uintptr_t>(addr);

    // MEM1
    if (((bits & 0xFF000000) == 0x80000000) ||
        ((bits & 0xFF000000) == 0x81000000)) {
        return true;
    }

    // MEM2
    if ((bits & 0xF8000000) == 0x90000000) {
        return true;
    }

    return false;
}

/**
 * @brief Tests whether an address is aligned to the specified number of
 * bytes
 *
 * @param addr Address
 * @param align Byte alignment
 */
bool PtrUtil::IsAlignedPointer(const void* addr, u32 align) {
    if (!IsPointer(addr)) {
        return false;
    }

    return reinterpret_cast<uintptr_t>(addr) % align == 0;
}

/**
 * @brief Tests whether an address points to libkiwi memory
 *
 * @param addr Address
 */
bool PtrUtil::IsLibKiwi(const void* addr) {
    if (!IsPointer(addr)) {
        return false;
    }

    // In memory allocated by libkiwi
    if (MemoryMgr::GetInstance().IsHeapMemory(addr)) {
        return true;
    }

    // In libkiwi module
    if (addr >= GetModuleStart() && addr < GetModuleEnd()) {
        return true;
    }

    return false;
}

/**
 * @brief Tests whether an address points to stack memory
 *
 * @param addr Address
 */
bool PtrUtil::IsStack(const void* addr) {
    return IsPointer(addr) && addr >= GetStackEnd() && addr < GetStackStart();
}

/**
 * @brief Tests whether an address likely points to a floating-point value
 *
 * @param addr Address
 */
bool PtrUtil::IsFloat(const void* addr) {
    if (!IsAlignedPointer(addr, sizeof(f32))) {
        return false;
    }

    f32 value = *reinterpret_cast<const f32*>(addr);

    // Exact match
    if (value == 0.0f || value == -0.0f) {
        return true;
    }

    // Close to zero may actually be some integer instead
    if (Abs(value) <= FLT_EPSILON) {
        return false;
    }

    // Reasonable bounds checking
    return value >= -100000000.0f && value <= 100000000.0f;
}

/**
 * @brief Tests whether an address likely points to a string
 *
 * @param addr Address
 */
bool PtrUtil::IsString(const void* addr) {
    if (!IsPointer(addr)) {
        return false;
    }

    char buffer[128];
    std::memcpy(buffer, addr, sizeof(buffer));

    // Count number of "string-like" characters
    int chars = 0;
    int length = 0;

    for (; length < LENGTHOF(buffer); length++) {
        char c = buffer[length];

        // "String-like" characters
        if (std::isalpha(c) || std::isdigit(c) || c == ' ' || c == '_' ||
            c == '-') {
            chars++;
        }

        // Probably null terminator
        if (c == 0x00) {
            break;
        }
    }

    // At least 50% good characters
    return chars > (length / 2);
}

/**
 * @brief Tests whether an address likely points to or falls within a PTMF
 *
 * @param addr Address
 */
bool PtrUtil::IsPtmf(const void* addr) {
    if (!IsAlignedPointer(addr, sizeof(u32))) {
        return false;
    }

    // 'addr' may point to any one of the MemberFunction fields
    for (int i = 0; i < sizeof(detail::MemberFunction) / sizeof(u32); i++) {
        // Interpret as PTMF from each possible position in the structure
        const detail::MemberFunction* func =
            static_cast<const detail::MemberFunction*>(
                AddToPtr(addr, -(sizeof(u32) * i)));

        // Reasonable alignment/bounds for object/vtable offset
        if (func->toff % sizeof(u32) != 0 || func->toff < -0x10000 ||
            func->toff > 0x10000) {
            continue;
        }

        // Non-virtual function
        if (func->voff == -1) {
            if (IsPointer(func->pAddr)) {
                return true;
            }

            continue;
        }

        // Virtual function
        if (func->foff % sizeof(u32) == 0        // Word-aligned
            && func->foff >= (2 * sizeof(void*)) // After RTTI/destructor slots
            && func->foff <= ((128 + 2) * sizeof(void*)) // Assume at most 128
        ) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Tests whether the specified address points to a heap block tag
 *
 * @param addr Address
 */
bool PtrUtil::IsMBlockTag(const void* addr) {
    if (!IsAlignedPointer(addr, sizeof(u16))) {
        return false;
    }

    u16 tag = *static_cast<const u16*>(addr);
    return tag == 'UD' || tag == 'FR';
}

} // namespace kiwi
