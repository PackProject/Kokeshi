#ifndef LIBKIWI_NET_PACKET_H
#define LIBKIWI_NET_PACKET_H
#include <climits>
#include <libkiwi/k_types.h>
#include <libkiwi/math/kiwiAlgorithm.h>
#include <libkiwi/prim/kiwiOptional.h>
#include <libkiwi/support/kiwiLibSO.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_net
//! @{

/**
 * @brief Network packet wrapper
 */
class Packet {
public:
    /**
     * @brief Constructor
     *
     * @param size Packet buffer size
     * @param pAddr Packet recipient
     */
    Packet(u32 size, const SockAddrAny* pAddr = nullptr)
        : mpBuffer(nullptr), mBufferSize(0), mReadOffset(0), mWriteOffset(0) {
        OSInitMutex(&mBufferMutex);
        Alloc(size);

        if (pAddr != nullptr) {
            mAddress = *pAddr;
        } else {
            mAddress = SockAddr4();
        }
    }

    /**
     * @brief Destructor
     */
    ~Packet() {
        Free();
    }

    /**
     * @brief Gets the current size of the message buffer
     */
    virtual u32 GetBufferSize() const {
        return mBufferSize;
    }
    /**
     * @brief Gets the maximum size of the message buffer
     */
    virtual u32 GetMaxBuffer() const {
        return ULONG_MAX;
    }

    /**
     * @brief Gets the current size of the message payload
     */
    virtual u32 GetContentSize() const {
        return GetBufferSize() - GetOverhead();
    }
    /**
     * @brief Gets the maximum size of the message payload
     */
    virtual u32 GetMaxContent() const {
        return GetMaxBuffer() - GetOverhead();
    }
    /**
     * @brief Accesses the message payload
     */
    const void* GetContent() const {
        return mpBuffer + GetOverhead();
    }

    /**
     * @brief Gets the size of the message buffer overhead
     */
    virtual u32 GetOverhead() const {
        return 0;
    }

    /**
     * @brief Tests whether the packet contains no data
     */
    bool IsEmpty() const {
        return mpBuffer == nullptr || GetContentSize() == 0;
    }

    /**
     * @brief Gets the number of bytes that must be read to complete the packet
     */
    u32 ReadRemain() const {
        return Max<s64>(GetContentSize() - mReadOffset, 0);
    }
    /**
     * @brief Gets the number of bytes that must be written to complete the
     * packet
     */
    u32 WriteRemain() const {
        return Max<s64>(GetContentSize() - mWriteOffset, 0);
    }

    /**
     * @brief Tests whether read operation has been completed
     */
    bool IsReadComplete() const {
        return ReadRemain() == 0;
    }
    /**
     * @brief Tests whether write operation has been completed
     */
    bool IsWriteComplete() const {
        return WriteRemain() == 0;
    }

    /**
     * @brief Accesses peer socket address (read-only)
     */
    const SockAddrAny& GetPeer() const {
        return mAddress;
    }

    /**
     * @brief Allocates message buffer of the specified size
     *
     * @param size Packet size
     */
    void Alloc(u32 size);

    /**
     * @brief Reads data from message buffer
     *
     * @param pDst Data destination
     * @param n Data size
     *
     * @return Number of bytes read
     */
    u32 Read(void* pDst, u32 n);
    /**
     * @brief Writes data to message buffer
     *
     * @param pSrc Data source
     * @param n Data size
     *
     * @return Number of bytes written
     */
    u32 Write(const void* pSrc, u32 n);

    /**
     * @brief Receives message data from socket
     *
     * @param socket Socket descriptor
     *
     * @return Number of bytes received
     */
    Optional<u32> Send(SOSocket socket);
    /**
     * @brief Writes message data to socket
     *
     * @param socket Socket descriptor
     *
     * @return Number of bytes sent
     */
    Optional<u32> Recv(SOSocket socket);

protected:
    /**
     * @brief Releases message buffer
     */
    void Free();

    /**
     * @brief Clears existing state
     */
    void Clear();

protected:
    u8* mpBuffer;         // Message buffer
    u32 mBufferSize;      // Message buffer size
    OSMutex mBufferMutex; // Buffer access mutex

    s32 mReadOffset;  // Buffer read index
    s32 mWriteOffset; // Buffer write index

    SockAddrAny mAddress; // Sender (recv) or recipient (send)
};

//! @}
} // namespace kiwi

#endif
