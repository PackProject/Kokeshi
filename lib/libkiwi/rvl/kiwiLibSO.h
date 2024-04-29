#ifndef LIBKIWI_RVL_LIBSO_H
#define LIBKIWI_RVL_LIBSO_H
#include <cstring>
#include <libkiwi/prim/kiwiString.h>
#include <revolution/SO.h>
#include <types.h>

namespace kiwi {

// Forward declarations
class SockAddr;
class SockAddr4;
class SockAddr6;

/**
 * SO library wrapper/extension
 */
class LibSO {
public:
    static void Initialize();
    static SOResult GetLastError();

    static s32 Socket(SOProtoFamily family, SOSockType type);
    static SOResult Close(SOSocket socket);
    static SOResult Listen(SOSocket socket, s32 backlog = SOMAXCONN);
    static s32 Accept(SOSocket socket, SockAddr& addr);
    static SOResult Bind(SOSocket socket, SockAddr& addr);
    static SOResult Connect(SOSocket socket, const SockAddr& addr);
    static SOResult GetSockName(SOSocket socket, SockAddr& addr);
    static SOResult GetPeerName(SOSocket socket, SockAddr& addr);

    static s32 Read(SOSocket socket, void* dst, u32 len);
    static s32 Recv(SOSocket socket, void* dst, u32 len, u32 flags);
    static s32 RecvFrom(SOSocket socket, void* dst, u32 len, u32 flags,
                        SockAddr& addr);

    static s32 Write(SOSocket socket, const void* src, u32 len);
    static s32 Send(SOSocket socket, const void* src, u32 len, u32 flags);
    static s32 SendTo(SOSocket socket, const void* src, u32 len, u32 flags,
                      const SockAddr& addr);

    static s32 Fcntl(SOSocket socket, SOFcntlCmd cmd, ...);
    static SOResult Shutdown(SOSocket socket, SOShutdownType how);
    static s32 Poll(SOPollFD fds[], u32 numfds, s64 timeout);

    static bool INetAtoN(const String& str, SockAddr4& addr);

    static bool INetPtoN(const String& str, SockAddr& addr);
    static String INetNtoP(const SockAddr& addr);

    static void GetHostID(SockAddr4& addr);

    static SOResult GetSockOpt(SOSocket socket, SOSockOptLevel level,
                               SOSockOpt opt, void* val, u32 len);
    static SOResult SetSockOpt(SOSocket socket, SOSockOptLevel level,
                               SOSockOpt opt, const void* val, u32 len);

    static void WaitForDHCP();

private:
    static s32 RecvImpl(SOSocket socket, void* dst, u32 len, u32 flags,
                        SockAddr* addr);
    static s32 SendImpl(SOSocket socket, const void* src, u32 len, u32 flags,
                        const SockAddr* addr);

private:
    // IOS IP device handle
    static s32 sDeviceHandle;
    // Last IOS error code
    static SOResult sLastError;
};

/**
 * @brief SO IP address wrapper
 */
struct SockAddr : public SOSockAddr {
    /**
     * @brief Constructor
     */
    SockAddr() {
        std::memset(this, 0, sizeof(SockAddr));
    }

    /**
     * @brief Constructor
     *
     * @param addr Socket address
     */
    SockAddr(const SockAddr& addr) {
        std::memcpy(this, &addr, addr.len);
    }

    SockAddr& operator=(const SockAddr& addr) {
        std::memcpy(this, &addr, addr.len);
        return *this;
    }

    /**
     * @brief Constructor
     *
     * @param addr Socket address
     */
    SockAddr(const SOSockAddr& addr) {
        std::memcpy(this, &addr, addr.len);
    }

    SockAddr& operator=(const SOSockAddr& addr) {
        std::memcpy(this, &addr, addr.len);
        return *this;
    }
};

/**
 * @brief SO IPv4 address wrapper to simplify upcasting
 */
struct SockAddr4 : public SOSockAddrIn {
    operator SockAddr&() {
        return reinterpret_cast<SockAddr&>(*this);
    }
    operator const SockAddr&() const {
        return reinterpret_cast<const SockAddr&>(*this);
    }

    /**
     * @brief Constructor
     */
    SockAddr4() {
        len = sizeof(SOSockAddrIn);
        family = SO_AF_INET;
        port = 0;
        addr.raw = 0;
    }

    /**
     * @brief Constructor
     *
     * @param host IPv4 address OR hostname
     * @param _port Port
     */
    SockAddr4(const String& host, u16 _port = 0) {
        len = sizeof(SOSockAddrIn);
        family = SO_AF_INET;
        port = _port;

        bool success = LibSO::INetAtoN(host, *this);
        K_ASSERT_EX(success, "Could not resolve hostname: %s", host.CStr());
    }

    /**
     * @brief Constructor
     *
     * @param _addr IPv4 address
     * @param _port Port
     */
    SockAddr4(u32 _addr, u16 _port) {
        len = sizeof(SOSockAddrIn);
        family = SO_AF_INET;
        port = _port;
        addr.raw = _addr;
    }

    /**
     * @brief Constructor
     *
     * @param _port Port
     */
    explicit SockAddr4(u16 _port) {
        len = sizeof(SOSockAddrIn);
        family = SO_AF_INET;
        port = _port;
        addr.raw = 0;
    }

    /**
     * @brief Constructor
     *
     * @param addr Socket address
     */
    SockAddr4(const SockAddr& addr) {
        K_ASSERT_EX(addr.len == sizeof(SockAddr4), "Not for this class");
        std::memcpy(this, &addr, addr.len);
    }

    SockAddr4& operator=(const SockAddr& addr) {
        K_ASSERT_EX(addr.len == sizeof(SockAddr4), "Not for this class");
        std::memcpy(this, &addr, addr.len);
        return *this;
    }
};

/**
 * SO IPv6 address wrapper to simplify upcasting
 */
struct SockAddr6 : public SOSockAddrIn6 {
    operator SockAddr&() {
        return reinterpret_cast<SockAddr&>(*this);
    }
    operator const SockAddr&() const {
        return reinterpret_cast<const SockAddr&>(*this);
    }

    /**
     * @brief Constructor
     */
    SockAddr6() {
        len = sizeof(SOSockAddrIn6);
        family = SO_AF_INET6;
        port = 0;
        flowinfo = 0;
        std::memset(&addr, 0, sizeof(SOInAddr6));
        scope = 0;
    }

    /**
     * @brief Constructor
     *
     * @param _addr IPv6 address (string)
     * @param _port Port
     */
    SockAddr6(const String& _addr, u16 _port = 0) {
        len = sizeof(SOSockAddrIn6);
        family = SO_AF_INET6;
        port = _port;
        flowinfo = 0;

        bool success = LibSO::INetPtoN(_addr, *this);
        K_ASSERT(success);

        scope = 0;
    }

    /**
     * @brief Constructor
     *
     * @param _port Port
     */
    explicit SockAddr6(u16 _port) {
        len = sizeof(SOSockAddrIn6);
        family = SO_AF_INET6;
        port = _port;
        flowinfo = 0;
        std::memset(&addr, 0, sizeof(SOInAddr6));
        scope = 0;
    }

    /**
     * @brief Constructor
     *
     * @param addr Socket address
     */
    SockAddr6(const SockAddr& addr) {
        K_ASSERT_EX(addr.len == sizeof(SockAddr6), "Not for this class");
        std::memcpy(this, &addr, addr.len);
    }

    SockAddr6& operator=(const SockAddr& addr) {
        K_ASSERT_EX(addr.len == sizeof(SockAddr6), "Not for this class");
        std::memcpy(this, &addr, addr.len);
        return *this;
    }
};

/**
 * @brief Convert socket address to string
 */
inline String ToString(const SockAddr& t) {
    return Format("%s:%d", LibSO::INetNtoP(t).CStr(), t.port);
}

K_STATIC_ASSERT(sizeof(SockAddr) == sizeof(SOSockAddr));
K_STATIC_ASSERT(sizeof(SockAddr4) == sizeof(SOSockAddrIn));
K_STATIC_ASSERT(sizeof(SockAddr6) == sizeof(SOSockAddrIn6));

} // namespace kiwi

#endif
