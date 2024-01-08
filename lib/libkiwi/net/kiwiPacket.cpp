#include <libkiwi.h>

namespace kiwi {

/**
 * Prepares packet for new message
 *
 * @param header Packet header
 * @param dest Packet recipient (optional)
 */
void Packet::Set(const Header& header, const SOSockAddr* dest) {
    // Allocate data buffer
    Alloc(header.length);

    // Copy in destination address
    if (dest != NULL) {
        std::memcpy(&mAddress, dest, dest->len);
    } else {
        std::memset(&mAddress, 0, sizeof(SOSockAddr));
    }
}

/**
 * Allocates packet buffer
 *
 * @param capacity Buffer capacity
 */
void Packet::Alloc(size_type capacity) {
    Free();

    // New buffer
    mpBuffer = new u8[capacity];
    mCapacity = capacity;

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
 * Erase packet contents
 */
void Packet::Clear() {
    std::memset(mpBuffer, 0, mCapacity);
    mReadOffset = 0;
    mWriteOffset = 0;
}

/**
 * Reads data from packet buffer
 *
 * @param dst Data destination
 * @param n Data size
 *
 * @returns Bytes read
 */
std::size_t Packet::Read(void* dst, std::size_t n) {
    // Clamp size to avoid overflow
    n = Min(n, mCapacity - mReadOffset);

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
std::size_t Packet::Write(const void* src, std::size_t n) {
    // Clamp size to avoid overflow
    n = Min(n, mCapacity - mWriteOffset);

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
std::size_t Packet::Receive(SOSocket socket) {
    s32 result = LibSO::RecvFrom(socket, mpBuffer + mWriteOffset, WriteRemain(),
                                 0, mAddress);

    if (result >= 0) {
        mWriteOffset += result;
        return result;
    }

    return result == SO_EWOULDBLOCK ? -1 : 0;
}

/**
 * Writes data to socket
 *
 * @param socket Socket descriptor
 *
 * @returns Bytes written, or -1 if blocking
 */
std::size_t Packet::Send(SOSocket socket) {
    s32 result;

    // Send header before data
    if (mReadOffset == 0) {
        Header header;
        header.length = mCapacity;
        result = LibSO::SendTo(socket, &header, sizeof(Header), 0, mAddress);

        if (result < 0) {
            return result == SO_EWOULDBLOCK ? -1 : 0;
        }
    }

    // Send packet data
    result = LibSO::SendTo(socket, mpBuffer + mReadOffset, ReadRemain(), 0,
                           mAddress);

    if (result >= 0) {
        mReadOffset += result;
        return result;
    }

    return result == SO_EWOULDBLOCK ? -1 : 0;
}

} // namespace kiwi
