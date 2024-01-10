#ifndef LIBKIWI_NET_PACKET_H
#define LIBKIWI_NET_PACKET_H
#include <cstring>
#include <libkiwi/math/kiwiAlgorithm.hpp>
#include <libkiwi/rvl/kiwiLibSO.hpp>
#include <revolution/SO.h>
#include <types.h>

namespace kiwi {

/**
 * Network packet wrapper
 */
class Packet {
public:
    /**
     * Packet header
     */
    struct Header {
        Header() : capacity(0) {}

        // Packet capacity
        u16 capacity;
    };

    /**
     * Maximum packet content size, in bytes
     */
    static const u16 MAX_SIZE = 0xFFFF - sizeof(Header);

public:
    /**
     * Constructor
     */
    Packet()
        : mpBuffer(NULL), mSentHeader(false), mReadOffset(0), mWriteOffset(0) {}

    /**
     * Constructor
     *
     * @param header Packet header
     * @param dest Packet recipient (optional)
     */
    Packet(const Header& header, const SOSockAddr* dest = NULL)
        : mpBuffer(NULL), mReadOffset(0), mWriteOffset(0) {
        Set(header, dest);
    }

    /**
     * Destructor
     */
    ~Packet() {
        Free();
    }

    void Set(const Header& header, const SOSockAddr* dest = NULL);

    /**
     * Whether the packet contains no data
     */
    bool IsEmpty() const {
        return mpBuffer == NULL || mHeader.capacity == 0;
    }

    u16 Read(void* dst, u16 n);

    /**
     * The number of bytes that must be read to complete the packet
     */
    u16 ReadRemain() const {
        return Max(mHeader.capacity - mReadOffset, 0);
    }

    /**
     * Whether read operation has been completed
     */
    bool IsReadComplete() const {
        return ReadRemain() == 0;
    }

    u16 Write(const void* src, u16 n);

    /**
     * The number of bytes that must be written to complete the packet
     */
    u16 WriteRemain() const {
        return Max(mHeader.capacity - mWriteOffset, 0);
    }

    /**
     * Whether write operation has been completed
     */
    bool IsWriteComplete() const {
        return WriteRemain() == 0;
    }

    s32 Send(SOSocket socket);
    s32 Receive(SOSocket socket);

private:
    void Alloc();
    void Free();

private:
    // Packet header
    Header mHeader;
    // Packet data
    u8* mpBuffer;

    // Whether packet header has been sent
    bool mSentHeader;

    // Packet read offset
    int mReadOffset;
    // Packet write offset
    int mWriteOffset;

    // Packet sender (RecvFrom) or recipient (SendTo)
    SOSockAddr mAddress;
};

} // namespace kiwi

#endif
