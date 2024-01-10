#include <libkiwi.h>

namespace kiwi {

/**
 * Constructor
 *
 * @param family Socket protocol family
 * @param type Socket type
 */
SyncSocket::SyncSocket(SOProtoFamily family, SOSockType type)
    : SocketBase(family, type) {}

/**
 * Constructor
 *
 * @param socket Socket file descriptor
 * @param type Socket protocol family
 * @param type Socket type
 */
SyncSocket::SyncSocket(SOSocket socket, SOProtoFamily family, SOSockType type)
    : SocketBase(socket, family, type) {}

/**
 * Destructor
 */
SyncSocket::~SyncSocket() {}

/**
 * Connects to another socket
 *
 * @param addr Remote address
 * @return Success
 */
bool SyncSocket::Connect(const SOSockAddr& addr) {
    K_ASSERT(mHandle >= 0);
    K_ASSERT(mFamily == addr.in.family);

    return LibSO::Connect(mHandle, addr) >= 0;
}

/**
 * Accepts remote connection
 *
 * @return New socket
 */
SyncSocket* SyncSocket::Accept() {
    K_ASSERT(mHandle >= 0);
    K_WARN(mType == SO_SOCK_DGRAM, "Accept won't do anything for dgram.");

    SOSockAddr addr;
    s32 result = LibSO::Accept(mHandle, addr);

    // Result >= 0 is the peer socket descriptor
    return result >= 0 ? new SyncSocket(result, mFamily, mType) : NULL;
}

/**
 * Receives data from specified connection
 *
 * @param dst Destination buffer
 * @param len Buffer size
 * @param[out] addr Sender address
 * @return Bytes received (< 0 if failure)
 */
s32 SyncSocket::RecieveImpl(void* dst, std::size_t len, SOSockAddr* addr) {
    K_ASSERT(mHandle >= 0);

    // No partial packet
    if (mRecvPacket.IsEmpty()) {
        // Try reading header for new packet
        Packet::Header header;
        s32 result = LibSO::Read(mHandle, &header, sizeof(Packet::Header));

        // Something here, but not a packet
        if (result != sizeof(Packet::Header)) {
            K_WARN_EX(result != 0, "Found extraneous data: %d bytes", result);
            // Discard the data
            return 0;
        }

        mRecvPacket.Set(header);
    }

    K_WARN_EX(mRecvPacket.WriteRemain() > len,
              "Requested %d bytes which is smaller than the packet (%d)", len,
              mRecvPacket.WriteRemain());

    // Read packet over socket
    while (!mRecvPacket.IsWriteComplete()) {
        mRecvPacket.Receive(mHandle);
    }

    // Copy out packet data
    return mRecvPacket.Read(dst, len);
}

/**
 * Sends data to specified connection
 *
 * @param src Source buffer
 * @param len Buffer size
 * @param addr Destination address
 * @return Bytes sent (< 0 if failure)
 */
s32 SyncSocket::SendImpl(const void* src, std::size_t len,
                         const SOSockAddr* addr) {
    K_ASSERT(mHandle >= 0);
    K_ASSERT(mSendPacket.IsEmpty());

    // Setup new packet
    Packet::Header header;
    header.capacity = len;
    mSendPacket.Set(header, addr);

    // Write packet data
    mSendPacket.Write(src, len);
    K_ASSERT(mSendPacket.IsWriteComplete());

    // Send packet over socket
    while (!mSendPacket.IsReadComplete()) {
        return mRecvPacket.Send(mHandle);
    }

    return len;
}

} // namespace kiwi
