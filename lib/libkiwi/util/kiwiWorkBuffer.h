#ifndef LIBKIWI_UTIL_WORK_BUFFER_H
#define LIBKIWI_UTIL_WORK_BUFFER_H
#include <libkiwi/core/kiwiMemoryMgr.h>
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Work buffer parameters
 */
struct WorkBufferArg {
    u32 size;       // Buffer size
    u32 sizeAlign;  // Size alignment
    u32 memAlign;   // Memory alignment
    EMemory region; // Allocation region

    /**
     * @brief Constructor
     */
    WorkBufferArg()
        : size(0), sizeAlign(32), memAlign(32), region(EMemory_MEM1) {}
};

/**
 * @brief Byte/size aligned work buffer
 */
class WorkBuffer {
public:
    /**
     * @brief Constructor
     *
     * @param arg Buffer parameters
     */
    WorkBuffer(const WorkBufferArg& arg)
        : mpBuffer(nullptr),
          mBufferSize(arg.size),
          mBufferAlignSize(0),
          mMemRegion(arg.region),
          mSizeAlign(arg.sizeAlign),
          mMemAlign(arg.memAlign) {

        mBufferAlignSize = ROUND_UP(mBufferSize, mSizeAlign);
        mpBuffer = new (mMemAlign, mMemRegion) u8[mBufferAlignSize];
        K_ASSERT(mpBuffer != nullptr);
    }

    /**
     * @brief Destructor
     */
    ~WorkBuffer() {
        delete[] mpBuffer;
    }

    /**
     * @brief Access buffer contents
     */
    u8* Contents() const {
        return mpBuffer;
    }

    /**
     * @brief Access buffer size before alignment
     */
    u32 Size() const {
        return mBufferSize;
    }

    /**
     * @brief Access buffer size after alignment
     */
    u32 AlignedSize() const {
        return mBufferAlignSize;
    }

    // clang-format off
    operator       void*()       { return mpBuffer; }
    operator const void*() const { return mpBuffer; }
    // clang-format on

private:
    u8* mpBuffer;         // Buffer memory
    u32 mBufferSize;      // Buffer unaligned size
    u32 mBufferAlignSize; // Buffer aligned size

    EMemory mMemRegion; // Requested memory region
    u32 mSizeAlign;     // Requested size alignment
    u32 mMemAlign;      // Requested address alignment
};

//! @}
} // namespace kiwi

#endif
