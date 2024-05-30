#ifndef LIBKIWI_UTIL_WORK_BUFFER_H
#define LIBKIWI_UTIL_WORK_BUFFER_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {

/**
 * @brief Byte/size aligned work buffer
 */
class WorkBuffer {
public:
    /**
     * @brief Constructor
     *
     * @param size Buffer size
     * @param memAlign Requested buffer alignment
     * @param sizeAlign Requested size alignment
     */
    WorkBuffer(u32 size, u32 memAlign = 32, u32 sizeAlign = 32)
        : mpBuffer(NULL),
          mBufferSize(size),
          mBufferAlignSize(0),
          mMemAlign(memAlign),
          mSizeAlign(sizeAlign) {
        mBufferAlignSize = ROUND_UP(size, mSizeAlign);
        mpBuffer = new (mMemAlign) u8[mBufferAlignSize];
        K_ASSERT(mpBuffer != NULL);
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

    operator void*() {
        return mpBuffer;
    }
    operator const void*() const {
        return mpBuffer;
    }

private:
    u8* mpBuffer;         // Buffer memory
    u32 mBufferSize;      // Buffer unaligned size
    u32 mBufferAlignSize; // Buffer aligned size

    u32 mMemAlign;  // Requested address alignment
    u32 mSizeAlign; // Requested size alignment
};

} // namespace kiwi

#endif
