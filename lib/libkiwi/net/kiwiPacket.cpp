#include <libkiwi.h>

namespace kiwi {

/**
 * Prepares packet for new message
 *
 * @param header Packet header
 * @param dest Packet recipient (optional)
 */
void Packet::Set(const Header& header, const SOSockAddr* dest) {
    K_ASSERT(header.capacity <= MAX_SIZE);

    // Allocate for capacity
    mHeader = header;
    Alloc();

    // Destination address is optional
    if (dest != NULL) {
        std::memcpy(&mAddress, dest, dest->len);
    } else {
        std::memset(&mAddress, 0, sizeof(SOSockAddr));
    }
}

/**
 * Allocates packet buffer
 */
void Packet::Alloc() {
    // Free existing buffer
    if (mpBuffer != NULL) {
        Free();
    }

    mpBuffer = new u8[mHeader.capacity];
    K_ASSERT(mpBuffer != NULL);

    mReadOffset = 0;
    mWriteOffset = 0;
}

/**
 * Releases packet buffer
 */
void Packet::Free() {
    delete mpBuffer;
    mpBuffer = NULL;
}

/**
 * Reads data from packet buffer
 *
 * @param dst Data destination
 * @param n Data size
 *
 * @returns Bytes read
 */
u16 Packet::Read(void* dst, u16 n) {
    K_ASSERT(mpBuffer != NULL);
    K_ASSERT(n <= MAX_SIZE);

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
 * @returns Bytes written
 */

u16 Packet::Write(const void* src, u16 n) {
    K_ASSERT(mpBuffer != NULL);
    K_ASSERT(n <= MAX_SIZE);

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
 * @returns Bytes received, or -1 if blocking
 */
s32 Packet::Recv(SOSocket socket) {
    K_ASSERT(mpBuffer != NULL);

    // Read from socket (try to complete packet)
    s32 result = LibSO::RecvFrom(socket, mpBuffer + mWriteOffset, WriteRemain(),
                                 0, mAddress);

    // > 0 means bytes read from socket
    if (result >= 0) {
        mWriteOffset += result;
        return result;
    }

    // SO error: Nothing received, but -1 to signal blocking
    return result == SO_EWOULDBLOCK ? -1 : 0;
}

/**
 * Writes data to socket
 *
 * @param socket Socket descriptor
 *
 * @returns Bytes written, or -1 if blocking
 */
s32 Packet::Send(SOSocket socket) {
    K_ASSERT(mpBuffer != NULL);

    // Try to send header first
    if (!mSentHeader) {
        s32 result =
            LibSO::SendTo(socket, &mHeader, sizeof(Header), 0, mAddress);

        if (result < 0) {
            return result == SO_EWOULDBLOCK ? -1 : 0;
        }

        mSentHeader = true;
    }

    // Send through socket (try to complete packet)
    s32 result = LibSO::SendTo(socket, mpBuffer + mReadOffset, ReadRemain(), 0,
                               mAddress);

    // > 0 means bytes written to socket
    if (result >= 0) {
        mReadOffset += result;
        return result;
    }

    // SO error: Nothing written, but -1 to signal blocking
    return result == SO_EWOULDBLOCK ? -1 : 0;
}

} // namespace kiwi
