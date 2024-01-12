#ifndef LIBKIWI_RVL_LIBSO_HPP
#define LIBKIWI_RVL_LIBSO_HPP
#include <libkiwi/core/kiwiString.hpp>
#include <revolution/SO.h>
#include <types.h>

namespace kiwi {

/**
 * SO library wrapper/extension
 */
class LibSO {
public:
    static void Initialize();

    static s32 Socket(SOProtoFamily family, SOSockType type);
    static s32 Close(SOSocket socket);
    static s32 Listen(SOSocket socket, s32 backlog = 5);
    static s32 Accept(SOSocket socket, SOSockAddr& addr);
    static s32 Bind(SOSocket socket, const SOSockAddr& addr);
    static s32 Connect(SOSocket socket, const SOSockAddr& addr);
    static s32 GetSockName(SOSocket socket, SOSockAddr& addr);
    static s32 GetPeerName(SOSocket socket, SOSockAddr& addr);

    static s32 Read(SOSocket socket, void* dst, u32 n);
    static s32 Recv(SOSocket socket, void* dst, u32 n, u32 flags);
    static s32 RecvFrom(SOSocket socket, void* dst, u32 n, u32 flags,
                        SOSockAddr& addr);

    static s32 Write(SOSocket socket, const void* src, u32 n);
    static s32 Send(SOSocket socket, const void* src, u32 n, u32 flags);
    static s32 SendTo(SOSocket socket, const void* src, u32 n,
                      u32 flags, const SOSockAddr& addr);

    static s32 Fcntl(SOSocket socket, s32 cmd, ...);
    static s32 Shutdown(SOSocket socket, SOShutdownType how);
    static s32 Poll(SOPollFD fds[], u32 numfds, s64 timeout);

    static bool INetPtoN(String str, SOInAddr& addr);
    static bool INetPtoN(String str, SOInAddr6& addr);
    static String INetNtoP(const SOInAddr& addr);
    static String INetNtoP(const SOInAddr6& addr);

    static s32 GetHostID(SOInAddr& addr);

    static s32 GetSockOpt(SOSocket socket, SOSockOptLevel level, SOSockOpt opt,
                          void* val, u32 len);
    static s32 SetSockOpt(SOSocket socket, SOSockOptLevel level, SOSockOpt opt,
                          const void* val, u32 len);

private:
    static s32 RecvImpl(SOSocket socket, void* dst, u32 n, u32 flags,
                        SOSockAddr* addr);
    static s32 SendImpl(SOSocket socket, const void* src, u32 n,
                        u32 flags, const SOSockAddr* addr);

private:
    // IOS IP device handle
    static s32 sDeviceHandle;
};

/**
 * SO IPv4 address wrapper to simplify upcasting
 */
struct SockAddr4 : public SOSockAddr {
    SockAddr4() {
        len = sizeof(SOSockAddrIn);
        in.family = SO_AF_INET;
        in.addr.raw = 0;
        in.port = 0;
    }

    SockAddr4(String ip, u16 port) {
        len = sizeof(SOSockAddrIn);
        in.family = SO_AF_INET;
        LibSO::INetPtoN(ip, in.addr);
        in.port = port;
    }

    SockAddr4(u32 ip, u16 port) {
        len = sizeof(SOSockAddrIn);
        in.family = SO_AF_INET;
        in.addr.raw = ip;
        in.port = port;
    }

    SockAddr4(u16 port) {
        SockAddr4("0.0.0.0", port);
    }
};

/**
 * SO IPv6 address wrapper to simplify upcasting
 */
struct SockAddr6 : public SOSockAddr {
    SockAddr6() {
        len = sizeof(SOSockAddrIn6);
        std::memset(&in6, 0, sizeof(SOSockAddrIn6));
        in6.family = SO_AF_INET6;
    }

    SockAddr6(String ip, u16 port) {
        len = sizeof(SOSockAddrIn6);
        in6.family = SO_AF_INET6;
        LibSO::INetPtoN(ip, in6.addr);
        in6.port = port;
    }

    SockAddr6(u16 port) {
        SockAddr6("0000:0000:0000:0000:0000:0000:0000:0000", port);
    }
};

} // namespace kiwi

#endif
