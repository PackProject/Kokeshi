#include <cstdio>
#include <libkiwi.h>
#include <revolution/IPC.h>

namespace kiwi {

/**
 * @brief IOS I/O control codes
 */
enum {
    // dev/net/ip/top
    Ioctl_SOAccept = 1,
    Ioctl_SOBind = 2,
    Ioctl_SOClose = 3,
    Ioctl_SOConnect = 4,
    Ioctl_SOFcntl = 5,
    Ioctl_SOGetPeerName = 6,
    Ioctl_SOGetSocketName = 7,
    Ioctl_SOSetSockOpt = 9,
    Ioctl_SOListen = 10,
    Ioctl_SOPoll = 11,
    Ioctl_SORecvFrom = 12,
    Ioctl_SOSendTo = 13,
    Ioctl_SOShutdown = 14,
    Ioctl_SOCreate = 15,
    Ioctl_SOGetHostID = 16,
    Ioctl_SOINetAtoN = 21,
    Ioctl_SOStartup = 31,

    // dev/net/ncd/manage
    IoctlV_NCDGetLinkStatus = 7,

    // dev/net/kd/request
    Ioctl_NWC24iStartupSocket = 6,
};

struct NCDLinkStatus {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 linkState;
    /* 0x08 */ u8 padding[32 - 0x08];
};
struct NWC24Result {
    /* 0x00 */ s32 result;
    /* 0x04 */ s32 exResult;
    /* 0x08 */ u8 padding[32 - 0x08];
};

IosDevice LibSO::sDevNetIpTop;
SOResult LibSO::sLastError = SO_SUCCESS;

/**
 * @brief Accesses IOS IP device for socket operation
 *
 * @note Please call this before other LibSO functions
 */
void LibSO::Initialize() {
    // Prevent double initialization
    if (sDevNetIpTop.IsOpen()) {
        return;
    }

    // 1. Check network config link status
    {
        IosDevice ncd_manage;
        ncd_manage.Open("/dev/net/ncd/manage", 1000);
        K_ASSERT(ncd_manage.IsOpen());

        TVector<IosVector> input;
        TVector<IosVector> output;

        IosObject<NCDLinkStatus> linkStatus;
        output.PushBack(linkStatus);

        s32 result = ncd_manage.IoctlV(IoctlV_NCDGetLinkStatus, input, output);
        K_ASSERT_EX(result >= 0, "NCDGetLinkStatus failed (%d)", result);

        K_ASSERT(linkStatus->linkState >= 0);
    }

    // 2. NWC24 socket startup
    {
        IosDevice kd_request;
        kd_request.Open("/dev/net/kd/request", 1000);
        K_ASSERT(kd_request.IsOpen());

        IosVector dummy;
        IosObject<NWC24Result> commonResult;

        s32 result =
            kd_request.Ioctl(Ioctl_NWC24iStartupSocket, dummy, commonResult);
        K_ASSERT_EX(result >= 0, "NWC24iStartupSocket failed (%d)", result);

        K_ASSERT(commonResult->result >= 0);
        K_ASSERT(commonResult->exResult >= 0);
    }

    // 3. SO startup
    {
        sDevNetIpTop.Open("/dev/net/ip/top", IPC_OPEN_NONE);
        K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Couldn't open /dev/net/ip/top");

        s32 result = sDevNetIpTop.Ioctl(Ioctl_SOStartup);
        K_ASSERT_EX(result >= 0, "SOStartup failed (%d)", result);
    }

    // 4. Wait to obtain console IP address
    WaitForDHCP();
}

/**
 * @brief Determine the most recent error code
 */
SOResult LibSO::GetLastError() {
    return sLastError;
}

struct SOSocketArgs {
    /* 0x00 */ s32 family;
    /* 0x04 */ s32 type;
    /* 0x08 */ s32 protocol;
};
/**
 * @brief Creates a socket
 *
 * @param family Protocol family
 * @param type Socket type
 * @return New socket descriptor, or IOS error code
 */
s32 LibSO::Socket(SOProtoFamily family, SOSockType type) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    K_ASSERT_EX(family == SO_PF_INET || family == SO_PF_INET6,
                "Invalid protocol family (%d)", family);

    K_ASSERT_EX(type == SO_SOCK_STREAM || type == SO_SOCK_DGRAM,
                "Invalid socket type (%d)", type);

    IosObject<SOSocketArgs> args;
    IosVector dummy;

    args->family = family;
    args->type = type;
    // IOS must auto-detect protocol
    args->protocol = SO_IPPROTO_IP;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOCreate, args, dummy);
    sLastError = result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);

    return result;
}

/**
 * @brief Closes a socket descriptor
 *
 * @param socket Socket descriptor
 * @return IOS error code
 */
SOResult LibSO::Close(SOSocket socket) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    IosObject<s32> fd(socket);
    IosVector dummy;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOClose, fd, dummy);
    sLastError = static_cast<SOResult>(result);

    return sLastError;
}

struct SOListenArgs {
    /* 0x00 */ s32 fd;
    /* 0x04 */ s32 backlog;
};
/**
 * @brief Listens for socket connections
 *
 * @param socket Socket descriptor
 * @param backlog Maximum pending connections (default 5)
 * @return IOS error code
 */
SOResult LibSO::Listen(SOSocket socket, s32 backlog) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    IosObject<SOListenArgs> args;
    IosVector dummy;

    args->fd = socket;
    args->backlog = backlog;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOListen, args, dummy);
    sLastError = static_cast<SOResult>(result);

    return sLastError;
}

/**
 * @brief Accepts a new connection on a socket
 *
 * @param socket Socket descriptor
 * @param[in,out] addr Remote address
 * @return Socket descriptor or IOS error code
 */
s32 LibSO::Accept(SOSocket socket, SockAddrAny& addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    K_ASSERT_EX(addr.len == sizeof(SockAddr4) || addr.len == sizeof(SockAddr6),
                "Invalid address length (%d)", addr.len);

    IosObject<s32> fd(socket);

    // Address length is specified by input
    IosObject<SockAddrAny> out;
    out->len = addr.len;

    // Result >= 0 == peer descriptor
    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOAccept, fd, out);
    sLastError = result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);

    if (result >= 0) {
        std::memcpy(&addr, &*out, out->len);
    }

    return result;
}

struct SOBindArgs {
    /* 0x00 */ s32 fd;
    /* 0x04 */ BOOL hasDest;
    /* 0x08 */ SockAddrAny dest;
};
/**
 * @brief Binds a name to a socket
 *
 * @param socket Socket descriptor
 * @param addr[in,out] Local address (zero for random port)
 * @return IOS error code
 */
SOResult LibSO::Bind(SOSocket socket, SockAddrAny& addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    K_ASSERT_EX(addr.len == sizeof(SockAddr4) || addr.len == sizeof(SockAddr6),
                "Invalid address length (%d)", addr.len);

    K_ASSERT_EX(addr.port != 0, "Port auto-detect not supported by IOS");

    IosObject<SOBindArgs> args;
    IosVector dummy;

    args->fd = socket;
    args->hasDest = TRUE;
    args->dest = addr;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOBind, args, dummy);
    sLastError = static_cast<SOResult>(result);

    return sLastError;
}

struct SOConnectArgs {
    /* 0x00 */ s32 fd;
    /* 0x04 */ BOOL hasDest;
    /* 0x08 */ SockAddrAny dest;
};
/**
 * @brief Connects a socket
 *
 * @param socket Socket descriptor
 * @param addr Remote address
 * @return IOS error code
 */
SOResult LibSO::Connect(SOSocket socket, const SockAddrAny& addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    K_ASSERT_EX(addr.len == sizeof(SockAddr4) || addr.len == sizeof(SockAddr6),
                "Invalid address length (%d)", addr.len);

    IosObject<SOConnectArgs> args;
    IosVector dummy;

    args->fd = socket;
    args->hasDest = TRUE;
    args->dest = addr;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOConnect, args, dummy);
    sLastError = static_cast<SOResult>(result);

    return sLastError;
}

/**
 * @brief Gets the socket name
 *
 * @param socket Socket descriptor
 * @param[in,out] addr Local address
 * @return IOS error code
 */
SOResult LibSO::GetSockName(SOSocket socket, SockAddrAny& addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    K_ASSERT_EX(addr.len == sizeof(SockAddr4) || addr.len == sizeof(SockAddr6),
                "Invalid address length (%d)", addr.len);

    IosObject<s32> fd(socket);

    IosObject<SockAddrAny> self;
    self->len = addr.len;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOGetSocketName, fd, self);
    sLastError = static_cast<SOResult>(result);

    if (result >= 0) {
        std::memcpy(&addr, &*self, self->len);
    }

    return sLastError;
}

/**
 * @brief Gets the peer socket name
 *
 * @param socket Socket descriptor
 * @param[in,out] addr Remote address
 * @return IOS error code
 */
SOResult LibSO::GetPeerName(SOSocket socket, SockAddrAny& addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    K_ASSERT_EX(addr.len == sizeof(SockAddr4) || addr.len == sizeof(SockAddr6),
                "Invalid address length (%d)", addr.len);

    IosObject<s32> fd(socket);

    IosObject<SockAddrAny> peer;
    peer->len = addr.len;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOGetPeerName, fd, peer);
    sLastError = static_cast<SOResult>(result);

    if (result >= 0) {
        std::memcpy(&addr, &*peer, peer->len);
    }

    return sLastError;
}

/**
 * @brief Receives a message from a connected socket
 *
 * @param socket Socket descriptor
 * @param dst Destination buffer
 * @param len Number of bytes to read
 * @return Number of bytes read, or IOS error code
 */
s32 LibSO::Read(SOSocket socket, void* dst, u32 len) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(dst != nullptr);

    return Recv(socket, dst, len, 0);
}

/**
 * @brief Receives a message from a connected socket
 *
 * @param socket Socket descriptor
 * @param dst Destination buffer
 * @param len Number of bytes to read
 * @param flags Operation flags
 * @return Number of bytes read, or IOS error code
 */
s32 LibSO::Recv(SOSocket socket, void* dst, u32 len, u32 flags) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(dst != nullptr);

    return RecvImpl(socket, dst, len, flags, nullptr);
}

/**
 * @brief Receives a message from a specified address
 *
 * @param socket Socket descriptor
 * @param dst Destination buffer
 * @param len Number of bytes to read
 * @param flags Operation flags
 * @param[out] addr Sender address
 * @return Number of bytes read, or IOS error code
 */
s32 LibSO::RecvFrom(SOSocket socket, void* dst, u32 len, u32 flags,
                    SockAddrAny& addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(dst != nullptr);

    K_ASSERT_EX(addr.len == sizeof(SockAddr4) || addr.len == sizeof(SockAddr6),
                "Invalid address length (%d)", addr.len);

    return RecvImpl(socket, dst, len, flags, &addr);
}

/**
 * @brief Sends a message on a socket
 *
 * @param socket Socket descriptor
 * @param src Source buffer
 * @param len Number of bytes to write
 * @return Number of bytes written, or IOS error code
 */
s32 LibSO::Write(SOSocket socket, const void* src, u32 len) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(src != nullptr);

    return Send(socket, src, len, 0);
}

/**
 * @brief Sends a message on a socket
 *
 * @param socket Socket descriptor
 * @param src Source buffer
 * @param len Number of bytes to write
 * @param flags Operation flags
 * @return Number of bytes written, or IOS error code
 */
s32 LibSO::Send(SOSocket socket, const void* src, u32 len, u32 flags) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(src != nullptr);

    return SendImpl(socket, src, len, flags, nullptr);
}

/**
 * @brief Sends a message to a specified address
 *
 * @param socket Socket descriptor
 * @param src Source buffer
 * @param len Number of bytes to write
 * @param flags Operation flags
 * @param addr[in] Destination address
 * @return Number of bytes written, or IOS error code
 */
s32 LibSO::SendTo(SOSocket socket, const void* src, u32 len, u32 flags,
                  const SockAddrAny& addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(src != nullptr);

    K_ASSERT_EX(addr.len == sizeof(SockAddr4) || addr.len == sizeof(SockAddr6),
                "Invalid address length (%d)", addr.len);

    return SendImpl(socket, src, len, flags, &addr);
}

struct SORecvArgs {
    /* 0x00 */ s32 fd;
    /* 0x04 */ u32 flags;
};
/**
 * @brief Receives a message and records its sender
 *
 * @param socket Socket descriptor
 * @param dst Destination buffer
 * @param len Number of bytes to read
 * @param flags Operation flags
 * @param[out] addr Sender address
 * @return Number of bytes read, or IOS error code
 */
s32 LibSO::RecvImpl(SOSocket socket, void* dst, u32 len, u32 flags,
                    SockAddrAny* addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(dst != nullptr);

    K_ASSERT_EX(addr == nullptr || addr->len == sizeof(SockAddr4) ||
                    addr->len == sizeof(SockAddr6),
                "Invalid address length (%d)", addr->len);

    TVector<IosVector> input;
    TVector<IosVector> output;

    // Input vector 1: Ioctl args
    IosObject<SORecvArgs> args;
    args->fd = socket;
    args->flags = flags;
    input.PushBack(args);

    // Output vector 1: Destination buffer
    IosVector buffer;
    buffer.Set(dst, len);
    output.PushBack(buffer);

    // Output vector 2: Source address
    IosObject<SockAddrAny> from;
    IosVector dummy;
    if (addr != nullptr) {
        *from = *addr;
        output.PushBack(from);
    } else {
        output.PushBack(dummy);
    }

    s32 result = sDevNetIpTop.IoctlV(Ioctl_SORecvFrom, input, output);
    sLastError = result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);

    return result;
}

struct SOSendArgs {
    /* 0x00 */ s32 fd;
    /* 0x04 */ u32 flags;
    /* 0x08 */ BOOL hasDest;
    /* 0x0C */ SockAddrAny dest;
};
/**
 * @brief Sends a message to a specified address
 *
 * @param socket Socket descriptor
 * @param src Source buffer
 * @param len Number of bytes to write
 * @param flags Operation flags
 * @param addr Recipient address
 * @return Number of bytes written, or IOS error code
 */
s32 LibSO::SendImpl(SOSocket socket, const void* src, u32 len, u32 flags,
                    const SockAddrAny* addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(src != nullptr);

    K_ASSERT_EX(addr == nullptr || addr->len == sizeof(SockAddr4) ||
                    addr->len == sizeof(SockAddr6),
                "Invalid address length (%d)", addr->len);

    TVector<IosVector> input;
    TVector<IosVector> output;

    // Input vector 1: Source buffer
    IosVector buffer;
    buffer.Set(src, len);
    input.PushBack(buffer);

    // Input vector 2: Ioctl args
    IosObject<SOSendArgs> args;
    args->fd = socket;
    args->flags = flags;
    input.PushBack(args);

    // Copy in destination address
    if (addr != nullptr) {
        args->hasDest = TRUE;
        args->dest = *addr;
    } else {
        args->hasDest = FALSE;
    }

    // Request send
    s32 result = sDevNetIpTop.IoctlV(Ioctl_SOSendTo, input, output);
    sLastError = result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);

    return result;
}

struct SOFcntlArgs {
    /* 0x00 */ s32 fd;
    /* 0x04 */ s32 cmd;
    /* 0x08 */ void* arg;
};
/**
 * @brief Controls socket file descriptor
 *
 * @param socket Socket descriptor
 * @param cmd Command
 * @param ... Command argument
 * @return Command return value, or IOS error code
 */
s32 LibSO::Fcntl(SOSocket socket, SOFcntlCmd cmd, ...) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    std::va_list list;
    va_start(list, cmd);
    void* arg = va_arg(list, void*);
    va_end(list);

    IosObject<SOFcntlArgs> args;
    IosVector dummy;

    args->fd = socket;
    args->cmd = cmd;
    args->arg = arg;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOFcntl, args, dummy);
    sLastError = result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);

    return result;
}

struct SOShutdownArgs {
    /* 0x00 */ s32 fd;
    /* 0x04 */ s32 type;
};
/**
 * @brief Shuts down specified part of socket connection
 *
 * @param socket Socket descriptor
 * @param how How to shutdown connection
 * @return IOS error code
 */
SOResult LibSO::Shutdown(SOSocket socket, SOShutdownType how) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    IosObject<SOShutdownArgs> args;
    IosVector dummy;

    args->fd = socket;
    args->type = how;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOShutdown, args, dummy);
    sLastError = static_cast<SOResult>(result);

    return sLastError;
}

/**
 * @brief Wait for events on socket file descriptors
 *
 * @param[in,out] fds Socket descriptor array
 * @param numfds Socket descriptor array size
 * @param timeout Timeout for blocking
 * @return Number of socket results written out, or IOS error code
 */
s32 LibSO::Poll(SOPollFD fds[], u32 numfds, s64 timeout) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(fds != nullptr);
    K_ASSERT(numfds > 0);

    IosObject<s64> msec(OS_TICKS_TO_MSEC(timeout));

    IosBuffer<SOPollFD> results(numfds);
    std::memcpy(results.Ptr(), fds, numfds * sizeof(SOPollFD));

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOPoll, msec, results);
    sLastError = result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);

    // Output provides search results
    if (result >= 0) {
        std::memcpy(fds, results.Ptr(), numfds * sizeof(SOPollFD));
    }

    return result;
}

/**
 * @brief Convert hostname to IPv4 address
 *
 * @param name Hostname
 * @param[out] addr Socket address
 * @return IOS error code
 */
bool LibSO::INetAtoN(const String& name, SockAddr4& addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    IosString<char> input(name);
    IosObject<SOInAddr> output;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOINetAtoN, input, output);
    sLastError = result == 1 ? SO_SUCCESS : SO_EINVAL;

    if (result == 1) {
        addr.addr.raw = output->raw;
    }

    return result == 1;
}

/**
 * @brief Converts a string to a socket address
 *
 * @param str Address string
 * @param[out] addr Socket address
 * @return Success
 */
bool LibSO::INetPtoN(const String& str, SockAddrAny& addr) {
    bool success;

    switch (addr.len) {
    case sizeof(SockAddr4):
        success = std::sscanf(str, "%d.%d.%d.%d", &addr.in.addr.octets[0],
                              &addr.in.addr.octets[1], &addr.in.addr.octets[2],
                              &addr.in.addr.octets[3]) ==
                  LENGTHOF(addr.in.addr.octets);
        break;

    case sizeof(SockAddr6):
        success =
            std::sscanf(str, "%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx",
                        &addr.in6.addr.groups[0], &addr.in6.addr.groups[1],
                        &addr.in6.addr.groups[2], &addr.in6.addr.groups[3],
                        &addr.in6.addr.groups[4], &addr.in6.addr.groups[5],
                        &addr.in6.addr.groups[6], &addr.in6.addr.groups[7]) ==
            LENGTHOF(addr.in6.addr.groups);
        break;

    default:
        success = false;
        K_ASSERT_EX(false, "Invalid SockAddrAny length (%d)", addr.len);
        break;
    }

    sLastError = success ? SO_SUCCESS : SO_EINVAL;
    return success;
}

/**
 * @brief Converts a socket address to a string
 *
 * @param addr Socket address
 * @return Address string
 */
String LibSO::INetNtoP(const SockAddrAny& addr) {
    switch (addr.len) {
    case sizeof(SockAddr4):
        sLastError = SO_SUCCESS;
        return Format("%d.%d.%d.%d", addr.in.addr.octets[0],
                      addr.in.addr.octets[1], addr.in.addr.octets[2],
                      addr.in.addr.octets[3]);

    case sizeof(SockAddr6):
        sLastError = SO_SUCCESS;
        return Format("%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx",
                      addr.in6.addr.groups[0], addr.in6.addr.groups[1],
                      addr.in6.addr.groups[2], addr.in6.addr.groups[3],
                      addr.in6.addr.groups[4], addr.in6.addr.groups[5],
                      addr.in6.addr.groups[6], addr.in6.addr.groups[7]);

    default:
        sLastError = SO_EINVAL;
        K_ASSERT_EX(false, "Invalid SockAddrAny length (%d)", addr.len);
        return "";
    }
}

/**
 * @brief Gets the host machine's IPv4 address
 *
 * @param[out] addr Host address
 */
void LibSO::GetHostID(SockAddr4& addr) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOGetHostID);
    addr.addr.raw = static_cast<u32>(result);
    addr.port = 0;

    sLastError = SO_SUCCESS;
}

/**
 * @brief Get socket option
 *
 * @param socket Socket descriptor
 * @param level Option level
 * @param opt Option name
 * @param[out] val Option value
 * @param len Buffer size
 * @return IOS error code
 */
SOResult LibSO::GetSockOpt(SOSocket socket, SOSockOptLevel level, SOSockOpt opt,
                           void* val, u32 len) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    K_ASSERT_EX(false, "Not implemented");
    sLastError = SO_SUCCESS;
    return SO_SUCCESS;
}

struct SOSetSockOptArgs {
    /* 0x00 */ s32 fd;
    /* 0x04 */ s32 level;
    /* 0x08 */ s32 opt;
    /* 0x0C */ const void* val;
    /* 0x10 */ u32 len;
};
/**
 * @brief Set socket option
 *
 * @param socket Socket descriptor
 * @param level Option level
 * @param opt Option name
 * @param val Option value
 * @param len Buffer size
 * @return IOS error code
 */
SOResult LibSO::SetSockOpt(SOSocket socket, SOSockOptLevel level, SOSockOpt opt,
                           const void* val, u32 len) {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");
    K_ASSERT(socket >= 0);
    K_ASSERT(val != nullptr);
    K_ASSERT(len > 0);

    IosObject<SOSetSockOptArgs> args;
    IosVector dummy;

    args->fd = socket;
    args->level = level;
    args->opt = opt;
    args->val = val;
    args->len = len;

    s32 result = sDevNetIpTop.Ioctl(Ioctl_SOSetSockOpt, args, dummy);
    sLastError = static_cast<SOResult>(result);

    return sLastError;
}

/**
 * @brief Wait until the local IP address has been assigned
 */
void LibSO::WaitForDHCP() {
    K_ASSERT_EX(sDevNetIpTop.IsOpen(), "Please call LibSO::Initialize");

    SockAddr4 addr;
    GetHostID(addr);

    while (addr.addr.raw == 0) {
        OSSleepTicks(OS_MSEC_TO_TICKS(10));
        GetHostID(addr);
    }

    sLastError = SO_SUCCESS;
}

} // namespace kiwi
