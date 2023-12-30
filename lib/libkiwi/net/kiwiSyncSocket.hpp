#ifndef LIBKIWI_NET_SYNC_SOCKET_H
#define LIBKIWI_NET_SYNC_SOCKET_H
#include <libkiwi/net/kiwiPacket.hpp>
#include <libkiwi/net/kiwiSocketBase.hpp>
#include <types.h>

namespace kiwi {

/**
 * Synchronous (blocking) IOS socket wrapper
 */
class SyncSocket : public SocketBase {
public:
    SyncSocket(SOProtoFamily family, SOSockType type);
    virtual ~SyncSocket();

    virtual bool Connect(const SOSockAddr& addr);
    virtual SyncSocket* Accept();

private:
    SyncSocket(SOSocket socket, SOProtoFamily family, SOSockType type);

    virtual s32 RecieveImpl(void* dst, std::size_t len, SOSockAddr* addr);
    virtual s32 SendImpl(const void* src, std::size_t len,
                         const SOSockAddr* addr);

private:
    // Packet currently being received
    Packet mRecvPacket;
    // Packet currently being sent
    Packet mSendPacket;
};

} // namespace kiwi

#endif
