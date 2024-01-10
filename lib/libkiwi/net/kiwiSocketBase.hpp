#ifndef LIBKIWI_NET_SOCKET_BASE_H
#define LIBKIWI_NET_SOCKET_BASE_H
#include <libkiwi/rvl/kiwiLibSO.hpp>
#include <libkiwi/util/kiwiNonCopyable.hpp>
#include <types.h>

namespace kiwi {

/**
 * IOS socket wrapper, base class
 */
class SocketBase : private NonCopyable {
public:
    static u32 GetHostAddr();

    SocketBase(SOProtoFamily family, SOSockType type);
    virtual ~SocketBase();

    virtual bool Connect(const SOSockAddr& addr) = 0;
    virtual SocketBase* Accept() = 0;

    /**
     * Conversion for use as socket descriptor
     */
    operator SOSocket() const {
        return mHandle;
    }

    /**
     * Tests whether the socket has a valid descriptor
     */
    bool IsOpen() const {
        return mHandle >= 0;
    }

    bool Bind(const SOSockAddr& addr) const;
    bool Listen(s32 backlog = 5) const;
    bool SetBlocking(bool enable) const;
    bool Shutdown(SOShutdownType how) const;
    bool Close();

    bool GetSocketAddr(SOSockAddr& addr) const;
    bool GetPeerAddr(SOSockAddr& addr) const;

    bool CanReceive() const;
    bool CanSend() const;

    bool RecieveBytes(void* buf, std::size_t len, std::size_t* nrecv = NULL);
    bool RecieveBytesFrom(void* buf, std::size_t len, SOSockAddr& addr,
                          std::size_t* nrecv = NULL);

    bool SendBytes(const void* buf, std::size_t len, std::size_t* nsend = NULL);
    bool SendBytesTo(const void* buf, std::size_t len, const SOSockAddr& addr,
                     std::size_t* nsend = NULL);

    template <typename T> bool Receive(T& dst, std::size_t* nrecv = NULL) {
        return RecieveBytes(&dst, sizeof(T), nrecv);
    }
    template <typename T>
    bool ReceiveFrom(T& dst, SOSockAddr& addr, std::size_t* nrecv = NULL) {
        return RecieveBytesFrom(&dst, sizeof(T), addr, nrecv);
    }

    template <typename T> bool Send(const T& src, std::size_t* nsend = NULL) {
        return SendBytes(&src, sizeof(T), nsend);
    }
    template <typename T>
    bool SendTo(const T& src, const SOSockAddr& addr,
                std::size_t* nsend = NULL) {
        return SendBytesTo(&src, sizeof(T), addr, nsend);
    }

protected:
    SocketBase(SOSocket socket, SOProtoFamily family, SOSockType type);

    bool Poll(SOPollFD fds[], std::size_t numfds, s64 timeout) const;

    virtual s32 RecieveImpl(void* dst, std::size_t len, SOSockAddr* addr) = 0;
    virtual s32 SendImpl(const void* src, std::size_t len,
                         const SOSockAddr* addr) = 0;

protected:
    // Socket file descriptor
    SOSocket mHandle;
    // Socket protocol family
    SOProtoFamily mFamily;
    // Socket type
    SOSockType mType;
};

} // namespace kiwi

#endif
