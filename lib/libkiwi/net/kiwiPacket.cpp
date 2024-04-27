#include <libkiwi.h>

namespace kiwi {

/**
 * Allocates packet buffer
 *
 * @param size Packet size
 */
void Packet::Alloc(u32 size) {
    K_ASSERT(size > 0);
    K_ASSERT_EX(size < GetMaxContent(), "Must be fragmented!");

    // Free existing message
    if (mpBuffer != NULL) {
        Free();
    }

    // Protocol may have memory overhead
    mBufferSize = size + GetOverhead();
    mpBuffer = new u8[mBufferSize];
    K_ASSERT(mpBuffer != NULL);

    Clear();
}

/**
 * Releases packet buffer
 */
void Packet::Free() {
    AutoMutexLock lock(mBufferMutex);

    delete mpBuffer;
    mpBuffer = NULL;

    Clear();
}

/**
 * Clear existing state
 */
void Packet::Clear() {
    AutoMutexLock lock(mBufferMutex);

    mReadOffset = 0;
    mWriteOffset = 0;
}

/**
 * Reads data from packet buffer
 *
 * @param dst Data destination
 * @param n Data size
 *
 * @return Number of bytes read
 */
u32 Packet::Read(void* dst, u32 n) {
    K_ASSERT(mpBuffer != NULL);
    K_ASSERT(n <= GetMaxContent());

    AutoMutexLock lock(mBufferMutex);

    // Clamp size to avoid overflow
    n = Min(n, ReadRemain());

    // Copy data from buffer
    std::memcpy(dst, mpBuffer + mReadOffset, n);
    mReadOffset += n;

    return n;
}

/**
 * Writes data to packet buffer
 *
 * @param src Data source
 * @param n Data size
 *
 * @return Number of bytes written
 */

u32 Packet::Write(const void* src, u32 n) {
    K_ASSERT(mpBuffer != NULL);
    K_ASSERT(n <= GetMaxContent());

    AutoMutexLock lock(mBufferMutex);

    // Clamp size to avoid overflow
    n = Min(n, WriteRemain());

    // Copy data to buffer
    std::memcpy(mpBuffer + mWriteOffset, src, n);
    mWriteOffset += n;

    return n;
}

/**
 * Receives data from socket
 *
 * @param socket Socket descriptor
 *
 * @return Number of bytes received
 */
Optional<u32> Packet::Recv(SOSocket socket) {
    K_ASSERT(mpBuffer != NULL);

    AutoMutexLock lock(mBufferMutex);

    // Read from socket (try to complete packet)
    s32 result = LibSO::RecvFrom(socket, mpBuffer + mWriteOffset, WriteRemain(),
                                 0, mAddress);

    // Blocking, just say zero
    if (result == SO_EWOULDBLOCK) {
        return 0;
    }

    // > 0 means bytes read from socket
    if (result >= 0) {
        mWriteOffset += result;
        return result;
    }

    return kiwi::nullopt;
}

/**
 * Writes data to socket
 *
 * @param socket Socket descriptor
 *
 * @return Number of bytes sent
 */
Optional<u32> Packet::Send(SOSocket socket) {
    K_ASSERT(mpBuffer != NULL);

    AutoMutexLock lock(mBufferMutex);

    // Send through socket (try to complete packet)
    s32 result = LibSO::SendTo(socket, mpBuffer + mReadOffset, ReadRemain(), 0,
                               mAddress);

    // Blocking, just say zero
    if (result == SO_EWOULDBLOCK) {
        return 0;
    }

    // > 0 means bytes written to socket
    if (result >= 0) {
        mReadOffset += result;
        return result;
    }

    return kiwi::nullopt;
}

} // namespace kiwi
