#ifndef LIBKIWI_NET_SYNC_SOCKET_H
#define LIBKIWI_NET_SYNC_SOCKET_H
#include <libkiwi/net/kiwiSocketBase.h>
#include <types.h>

namespace kiwi {

/**
 * @brief Synchronous (blocking) socket
 */
class SyncSocket : public SocketBase {
public:
    /**
     * Constructor
     *
     * @param family Socket protocol family
     * @param type Socket type
     */
    SyncSocket(SOProtoFamily family, SOSockType type)
        : SocketBase(family, type) {}

    /**
     * @brief Destructor
     */
    virtual ~SyncSocket() {}

    virtual bool Connect(const SockAddr& addr, Callback callback = NULL,
                         void* arg = NULL);
    virtual SyncSocket* Accept(AcceptCallback callback = NULL,
                               void* arg = NULL);

private:
    /**
     * Constructor
     *
     * @param socket Socket descriptor
     * @param family Socket protocol family
     * @param type Socket type
     */
    SyncSocket(SOSocket socket, SOProtoFamily family, SOSockType type)
        : SocketBase(socket, family, type) {}

    virtual SOResult RecvImpl(void* dst, u32 len, u32& nrecv, SockAddr* addr,
                              Callback callback, void* arg);
    virtual SOResult SendImpl(const void* src, u32 len, u32& nsend,
                              const SockAddr* addr, Callback callback,
                              void* arg);
};

} // namespace kiwi

#endif
