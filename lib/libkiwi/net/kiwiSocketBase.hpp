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

    bool CanRecv() const;
    bool CanSend() const;

    bool RecvBytes(void* buf, u32 len, s32* nrecv = NULL);
    bool RecvBytesFrom(void* buf, u32 len, SOSockAddr& addr, s32* nrecv = NULL);

    bool SendBytes(const void* buf, u32 len, s32* nsend = NULL);
    bool SendBytesTo(const void* buf, u32 len, const SOSockAddr& addr,
                     s32* nsend = NULL);

    template <typename T> bool Recv(T& dst, s32* nrecv = NULL) {
        return RecvBytes(&dst, sizeof(T), nrecv);
    }
    template <typename T>
    bool RecvFrom(T& dst, SOSockAddr& addr, s32* nrecv = NULL) {
        return RecvBytesFrom(&dst, sizeof(T), addr, nrecv);
    }

    template <typename T> bool Send(const T& src, s32* nsend = NULL) {
        return SendBytes(&src, sizeof(T), nsend);
    }
    template <typename T>
    bool SendTo(const T& src, const SOSockAddr& addr, s32* nsend = NULL) {
        return SendBytesTo(&src, sizeof(T), addr, nsend);
    }

protected:
    SocketBase(SOSocket socket, SOProtoFamily family, SOSockType type);

    bool Poll(SOPollFD fds[], u32 numfds, s64 timeout) const;

    virtual s32 RecvImpl(void* dst, u32 len, SOSockAddr* addr) = 0;
    virtual s32 SendImpl(const void* src, u32 len, const SOSockAddr* addr) = 0;

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
