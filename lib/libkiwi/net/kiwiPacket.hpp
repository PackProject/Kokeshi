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
    typedef u16 size_type;
    // Largest packet size
    static const size_type MAX_SIZE = 2048;

    /**
     * Packet header
     */
    struct Header {
        size_type length;
    };

public:
    /**
     * Constructor
     */
    Packet() : mpBuffer(NULL), mReadOffset(0), mWriteOffset(0), mCapacity(0) {}

    /**
     * Constructor
     *
     * @param header Packet header
     * @param dest Packet recipient (optional)
     */
    Packet(const Header& header, const SOSockAddr* dest = NULL)
        : mpBuffer(NULL), mReadOffset(0), mWriteOffset(0), mCapacity(0) {
        Set(header, dest);
    }

    /**
     * Destructor
     */
    ~Packet() {
        Free();
    }

    void Set(const Header& header, const SOSockAddr* dest = NULL);
    void Alloc(size_type capacity = MAX_SIZE);
    void Free();
    void Clear();

    std::size_t Read(void* dst, std::size_t n);
    std::size_t Write(const void* src, std::size_t n);

    std::size_t Send(SOSocket socket);
    std::size_t Receive(SOSocket socket);

    /**
     * Gets the number of bytes that must be read to complete the packet
     */
    std::size_t ReadRemain() const {
        return mCapacity - mReadOffset;
    }

    /**
     * Gets the number of bytes that must be written to complete the
     * packet
     */
    std::size_t WriteRemain() const {
        return mCapacity - mWriteOffset;
    }

    /**
     * Tests whether read operation has been completed
     */
    bool IsReadComplete() const {
        return ReadRemain() == 0;
    }

    /**
     * Tests whether write operation has been completed
     */
    bool IsWriteComplete() const {
        return WriteRemain() == 0;
    }

    /**
     * Tests whether the packet contains no data
     */
    bool IsEmpty() const {
        return mpBuffer == NULL || mCapacity == 0;
    }

private:
    // Packet data
    u8* mpBuffer;

    // Packet read offset
    std::size_t mReadOffset;
    // Packet write offset
    std::size_t mWriteOffset;
    // Packet capacity
    std::size_t mCapacity;

    // Packet sender (RecvFrom) or recipient (SendTo)
    SOSockAddr mAddress;
};

} // namespace kiwi

#endif
