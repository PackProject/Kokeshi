#include <cstdio>
#include <libkiwi.h>
#include <revolution/IPC.h>

namespace kiwi {

s32 LibSO::sDeviceHandle = -1;

/**
 * IOS socket control codes
 */
enum {
    Ioctl_Accept = 1,
    Ioctl_Bind = 2,
    Ioctl_Close = 3,
    Ioctl_Connect = 4,
    Ioctl_Fcntl = 5,
    Ioctl_GetPeerName = 7,
    Ioctl_GetSocketName = 7,
    Ioctl_Listen = 10,
    Ioctl_Poll = 11,
    Ioctl_ReceiveFrom = 12,
    Ioctl_SendTo = 13,
    Ioctl_Shutdown = 14,
    Ioctl_Create = 15,
    Ioctl_GetHostID = 16,
};

/**
 * Accesses IOS IP device for socket operation
 * @note Please call this before other LibSO functions
 */
void LibSO::Initialize() {
    // Prevent double initialization
    if (sDeviceHandle >= 0) {
        return;
    }

    sDeviceHandle = IOS_Open("/dev/net/ip/top", IPC_OPEN_NONE);
    K_ASSERT_EX(sDeviceHandle >= 0, "Could not access IOS IP device");
}

/**
 * Creates a socket
 *
 * @param family Protocol family
 * @param type Socket type
 * @return New socket descriptor, or IOS error code
 */
s32 LibSO::Socket(SOProtoFamily family, SOSockType type) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT_EX(family == SO_PF_INET || family == SO_PF_INET6,
                "Unsupported protocol family");
    K_ASSERT_EX(type == SO_SOCK_STREAM || type == SO_SOCK_DGRAM,
                "Unsupported socket type");

    // Setup args for ioctl
    struct {
        s32 family;   // at 0x0
        s32 type;     // at 0x4
        s32 protocol; // at 0x8
    } args ALIGN(32);

    args.family = family;
    args.type = type;
    // Only IPPROTO_IP is allowed by IOS
    args.protocol = SO_IPPROTO_IP;

    return IOS_Ioctl(sDeviceHandle, Ioctl_Create, &args, sizeof(args), NULL, 0);
}

/**
 * Closes a socket descriptor
 *
 * @param socket Socket descriptor
 * @return IOS error code
 */
s32 LibSO::Close(SOSocket socket) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");

    // Setup args for ioctl
    s32 fd ALIGN(32) = socket;

    return IOS_Ioctl(sDeviceHandle, Ioctl_Close, &fd, sizeof(fd), NULL, 0);
}

/**
 * Listens for socket connections
 *
 * @param socket Socket descriptor
 * @param backlog Maximum pending connections (default 5)
 * @return IOS error code
 */
s32 LibSO::Listen(SOSocket socket, s32 backlog) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");

    // Setup args for ioctl
    struct {
        s32 fd;      // at 0x0
        s32 backlog; // at 0x4
    } args ALIGN(32);

    args.fd = socket;
    args.backlog = backlog;

    return IOS_Ioctl(sDeviceHandle, Ioctl_Listen, &args, sizeof(args), NULL, 0);
}

/**
 * Accepts a new connection on a socket
 *
 * @param socket Socket descriptor
 * @param[in,out] addr Remote address
 * @return Socket descriptor or IOS error code
 */
s32 LibSO::Accept(SOSocket socket, SOSockAddr& addr) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT_EX(addr.len == sizeof(SOSockAddrIn) ||
                    addr.len == sizeof(SOSockAddrIn6),
                "Unsupported address size specified");

    // Setup args for ioctl
    s32 fd ALIGN(32) = socket;
    SOSockAddr peer ALIGN(32);
    peer.len = addr.len;

    // Result >= 0 == peer descriptor
    s32 result = IOS_Ioctl(sDeviceHandle, Ioctl_Accept, &fd, sizeof(fd), &peer,
                           peer.len);

    // Copy out results (if no error)
    if (result >= 0) {
        std::memcpy(&addr, &peer, peer.len);
    }

    return result;
}

/**
 * Binds a name to a socket
 *
 * @param socket Socket descriptor
 * @param addr Local address
 * @return IOS error code
 */
s32 LibSO::Bind(SOSocket socket, const SOSockAddr& addr) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT_EX(addr.len == sizeof(SOSockAddrIn) ||
                    addr.len == sizeof(SOSockAddrIn6),
                "Unsupported address size specified");

    // Setup args for ioctl
    struct {
        s32 fd;          // at 0x0
        BOOL hasDest;    // at 0x4
        SOSockAddr dest; // at 0x8
    } args ALIGN(32);

    args.fd = socket;
    args.hasDest = TRUE;

    // Copy in arguments
    std::memcpy(&args.dest, &addr, addr.len);

    return IOS_Ioctl(sDeviceHandle, Ioctl_Bind, &args, sizeof(args), NULL, 0);
}

/**
 * Connects a socket
 *
 * @param socket Socket descriptor
 * @param addr Remote address
 * @param len Address length (sock addr structure size)
 * @return IOS error code
 */
s32 LibSO::Connect(SOSocket socket, const SOSockAddr& addr) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT_EX(addr.len == sizeof(SOSockAddrIn) ||
                    addr.len == sizeof(SOSockAddrIn6),
                "Unsupported address size specified");

    // Setup args for ioctl
    struct {
        s32 fd;          // at 0x0
        BOOL hasDest;    // at 0x4
        SOSockAddr dest; // at 0x8
    } args ALIGN(32);

    args.fd = socket;
    args.hasDest = TRUE;

    // Copy in arguments
    std::memcpy(&args.dest, &addr, addr.len);

    return IOS_Ioctl(sDeviceHandle, Ioctl_Connect, &args, sizeof(args), NULL,
                     0);
}

/**
 * Gets the socket name
 *
 * @param socket Socket descriptor
 * @param[in,out] addr Local address
 * @param len Address length (sock addr structure size)
 * @return IOS error code
 */
s32 LibSO::GetSockName(SOSocket socket, SOSockAddr& addr) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT_EX(addr.len == sizeof(SOSockAddrIn) ||
                    addr.len == sizeof(SOSockAddrIn6),
                "Unsupported address size specified");

    // Setup args for ioctl
    s32 fd ALIGN(32) = socket;
    SOSockAddr self ALIGN(32);

    s32 result = IOS_Ioctl(sDeviceHandle, Ioctl_GetSocketName, &fd, sizeof(fd),
                           &self, addr.len);

    // Copy out results (if no error)
    if (result >= 0) {
        std::memcpy(&addr, &self, addr.len);
    }

    return result;
}

/**
 * Gets the peer socket name
 *
 * @param socket Socket descriptor
 * @param[in,out] addr Remote address
 * @param len Address length (sock addr structure size)
 * @return IOS error code
 */
s32 LibSO::GetPeerName(SOSocket socket, SOSockAddr& addr) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT_EX(addr.len == sizeof(SOSockAddrIn) ||
                    addr.len == sizeof(SOSockAddrIn6),
                "Unsupported address size specified");

    // Setup args for ioctl
    s32 fd ALIGN(32) = socket;
    SOSockAddr peer ALIGN(32);
    peer.len = addr.len;

    s32 result = IOS_Ioctl(sDeviceHandle, Ioctl_GetPeerName, &fd, sizeof(fd),
                           &peer, peer.len);

    // Copy out results (if no error)
    if (result >= 0) {
        std::memcpy(&addr, &peer, peer.len);
    }

    return result;
}

/**
 * Receives a message from a connected socket
 *
 * @param socket Socket descriptor
 * @param dst Destination buffer
 * @param n Number of bytes to read
 * @return Number of bytes read, or IOS error code
 */
s32 LibSO::Read(SOSocket socket, void* dst, std::size_t n) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT(dst != NULL);

    return Recv(socket, dst, n, 0);
}

/**
 * Receives a message from a connected socket
 *
 * @param socket Socket descriptor
 * @param dst Destination buffer
 * @param n Number of bytes to read
 * @param flags Operation flags
 * @return Number of bytes read, or IOS error code
 */
s32 LibSO::Recv(SOSocket socket, void* dst, std::size_t n, u32 flags) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT(dst != NULL);

    return RecvImpl(socket, dst, n, flags, NULL);
}

/**
 * Receives a message from a specified address
 *
 * @param socket Socket descriptor
 * @param dst Destination buffer
 * @param n Number of bytes to read
 * @param flags Operation flags
 * @param[out] addr Sender address
 * @return Number of bytes read, or IOS error code
 */
s32 LibSO::RecvFrom(SOSocket socket, void* dst, std::size_t n, u32 flags,
                    SOSockAddr& addr) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT(dst != NULL);
    K_ASSERT_EX(addr.len == sizeof(SOSockAddrIn) ||
                    addr.len == sizeof(SOSockAddrIn6),
                "Unsupported address size specified");

    return RecvImpl(socket, dst, n, flags, &addr);
}

/**
 * Sends a message on a socket
 *
 * @param socket Socket descriptor
 * @param src Source buffer
 * @param n Number of bytes to write
 * @return Number of bytes written, or IOS error code
 */
s32 LibSO::Write(SOSocket socket, const void* src, std::size_t n) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT(src != NULL);

    return Send(socket, src, n, 0);
}

/**
 * Sends a message on a socket
 *
 * @param socket Socket descriptor
 * @param src Source buffer
 * @param n Number of bytes to write
 * @param flags Operation flags
 * @return Number of bytes written, or IOS error code
 */
s32 LibSO::Send(SOSocket socket, const void* src, std::size_t n, u32 flags) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT(src != NULL);

    return SendImpl(socket, src, n, flags, NULL);
}

/**
 * Sends a message to a specified address
 *
 * @param socket Socket descriptor
 * @param src Source buffer
 * @param n Number of bytes to write
 * @param flags Operation flags
 * @param addr Destination address
 * @return Number of bytes written, or IOS error code
 */
s32 LibSO::SendTo(SOSocket socket, const void* src, std::size_t n, u32 flags,
                  const SOSockAddr& addr) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT(src != NULL);
    K_ASSERT_EX(addr.len == sizeof(SOSockAddrIn) ||
                    addr.len == sizeof(SOSockAddrIn6),
                "Unsupported address size specified");

    return SendImpl(socket, src, n, flags, &addr);
}

/**
 * Receives a message and records its sender
 *
 * @param socket Socket descriptor
 * @param dst Destination buffer
 * @param n Number of bytes to read
 * @param flags Operation flags
 * @param[out] addr Sender address
 * @return Number of bytes read, or IOS error code
 */
s32 LibSO::RecvImpl(SOSocket socket, void* dst, std::size_t n, u32 flags,
                    SOSockAddr* addr) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT(dst != NULL);
    K_ASSERT_EX(addr == NULL || addr->len == sizeof(SOSockAddrIn) ||
                    addr->len == sizeof(SOSockAddrIn6),
                "Unsupported address size specified");

    // Setup vectors for ioctl
    enum {
        V_ARGS,   // I/O vector for ioctl args
        V_BUFFER, // I/O vector for user buffer
        V_FROM,   // I/O vector for source address
        V_MAX,    // Total I/O vector count

        V_NUM_IN = 1, // Total input I/O vector count
        V_NUM_OUT = 2 // Total output I/O vector count
    };
    IPCIOVector* vectors = new (32) IPCIOVector[V_MAX];

    // Setup args for ioctl
    struct {
        s32 fd;    // at 0x0
        u32 flags; // at 0x4
    } args ALIGN(32);

    args.fd = socket;
    args.flags = flags;

    // Input vector 1: Ioctl args
    vectors[V_ARGS].base = &args;
    vectors[V_ARGS].length = sizeof(args);

    // Output vector 1: Destination buffer
    vectors[V_BUFFER].base = dst;
    vectors[V_BUFFER].length = n;

    // Output vector 2: Source address
    SOSockAddr from ALIGN(32);
    if (addr != NULL) {
        std::memcpy(&from, addr, addr->len);
        vectors[V_FROM].base = &from;
        vectors[V_FROM].length = addr->len;
    } else {
        vectors[V_FROM].base = NULL;
        vectors[V_FROM].length = 0;
    }

    s32 result = IOS_Ioctlv(sDeviceHandle, Ioctl_ReceiveFrom, V_NUM_IN,
                            V_NUM_OUT, vectors);

    delete[] vectors;
    return result;
}

/**
 * Sends a message to a specified address
 *
 * @param socket Socket descriptor
 * @param src Source buffer
 * @param n Number of bytes to write
 * @param flags Operation flags
 * @param addr Recipient address
 * @return Number of bytes written, or IOS error code
 */
s32 LibSO::SendImpl(SOSocket socket, const void* src, std::size_t n, u32 flags,
                    const SOSockAddr* addr) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");
    K_ASSERT(src != NULL);
    K_ASSERT_EX(addr == NULL || addr->len == sizeof(SOSockAddrIn) ||
                    addr->len == sizeof(SOSockAddrIn6),
                "Unsupported address size specified");

    // Setup vectors for ioctl
    enum {
        V_BUFFER, // I/O vector for user buffer
        V_ARGS,   // I/O vector for ioctl args
        V_MAX,    // Total I/O vector count
    };
    IPCIOVector* vectors = new (32) IPCIOVector[V_MAX];

    // Setup args for ioctl
    struct {
        s32 fd;          // at 0x0
        u32 flags;       // at 0x4
        BOOL hasDest;    // at 0x8
        SOSockAddr dest; // at 0xC
    } args ALIGN(32);

    args.fd = socket;
    args.flags = flags;

    // Input vector 2: Ioctl args
    vectors[V_ARGS].base = &args;
    vectors[V_ARGS].length = sizeof(args);

    // Copy in destination address
    if (addr != NULL) {
        args.hasDest = TRUE;
        std::memcpy(&args.dest, addr, addr->len);
    } else {
        args.hasDest = FALSE;
    }

    // Input vector 1: Source buffer
    vectors[V_BUFFER].base = const_cast<void*>(src);
    vectors[V_BUFFER].length = n;

    // Request send
    s32 result = IOS_Ioctlv(sDeviceHandle, Ioctl_SendTo, V_MAX, 0, vectors);

    delete[] vectors;
    return result;
}

/**
 * Controls socket file descriptor
 *
 * @param socket Socket descriptor
 * @param cmd Command
 * @param ... Command argument
 * @return Command return value, os IOS error code
 */
s32 LibSO::Fcntl(SOSocket socket, s32 cmd, ...) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");

    // Get argument
    std::va_list list;
    va_start(list, cmd);
    void* arg = va_arg(list, void*);
    va_end(list);

    // Setup args for ioctl
    struct {
        s32 fd;    // at 0x0
        s32 cmd;   // at 0x4
        void* arg; // at 0x8
    } args ALIGN(32);

    args.fd = socket;
    args.cmd = cmd;
    args.arg = arg;

    return IOS_Ioctl(sDeviceHandle, Ioctl_Fcntl, &args, sizeof(args), NULL, 0);
}

/**
 * Shuts down specified part of socket connection
 *
 * @param socket Socket descriptor
 * @param how How to shutdown connection
 * @return IOS error code
 */
s32 LibSO::Shutdown(SOSocket socket, SOShutdownType how) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");

    // Setup args for ioctl
    struct {
        s32 fd;   // at 0x0
        s32 type; // at 0x4
    } args ALIGN(32);

    args.fd = socket;
    args.type = how;

    return IOS_Ioctl(sDeviceHandle, Ioctl_Shutdown, &args, sizeof(args), NULL,
                     0);
}

/**
 * Wait for specific events on specified socket file descriptors
 *
 * @param[in, out] fds Socket descriptor array
 * @param numfds Socket descriptor array size
 * @param timeout Timeout for blocking
 * @return Number of socket results written out, or IOS error code
 */
s32 LibSO::Poll(SOPollFD fds[], std::size_t numfds, s64 timeout) {
    K_ASSERT_EX(sDeviceHandle >= 0, "Please call LibSO::Initialize");

    // Setup args for ioctl
    s64 msec ALIGN(32) = OS_TICKS_TO_MSEC(timeout);
    SOPollFD* buffer ALIGN(32) = new (32) SOPollFD[numfds];

    // Copy in specified socket descriptors
    std::memcpy(buffer, fds, numfds * sizeof(SOPollFD));

    s32 result = IOS_Ioctl(sDeviceHandle, Ioctl_Poll, &msec, sizeof(msec),
                           buffer, numfds * sizeof(SOPollFD));

    // Copy out poll result if successful
    if (result >= 0) {
        std::memcpy(fds, buffer, numfds * sizeof(SOPollFD));
    }

    delete buffer;
    return result;
}

/**
 * Converts a string to an IPv4 address
 *
 * @param str Address string
 * @param[out] addr Address binary
 * @return Success
 */
bool LibSO::INetPtoN(String str, SOInAddr& addr) {
    return std::sscanf(str, "%d.%d.%d.%d", &addr.octets[0], &addr.octets[1],
                       &addr.octets[2],
                       &addr.octets[3]) == LENGTHOF(addr.octets);
}

/**
 * Converts a string to an IPv6 address
 *
 * @param str Address string
 * @param[out] addr Address binary
 * @return Success
 */
bool LibSO::INetPtoN(String str, SOInAddr6& addr) {
    return std::sscanf(str, "%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx", &addr.groups[0],
                       &addr.groups[1], &addr.groups[2], &addr.groups[3],
                       &addr.groups[4], &addr.groups[5], &addr.groups[6],
                       &addr.groups[7]) == LENGTHOF(addr.groups);
}

/**
 * Converts an IPv4 address to a string
 *
 * @param addr Address binary
 * @return Address string
 */
String LibSO::INetNtoP(const SOInAddr& addr) {
    return Format("%d.%d.%d.%d", addr.octets[0], addr.octets[1], addr.octets[2],
                  addr.octets[3]);
}

/**
 * Converts an IPv6 address to a string
 *
 * @param addr Address binary
 * @return Address string
 */
String LibSO::INetNtoP(const SOInAddr6& addr) {
    return Format("%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx", addr.groups[0],
                  addr.groups[1], addr.groups[2], addr.groups[3],
                  addr.groups[4], addr.groups[5], addr.groups[6],
                  addr.groups[7]);
}

/**
 * Gets the host machine's IPv4 address
 *
 * @param[out] addr Host address
 * @return IOS error code
 */
s32 LibSO::GetHostID(SOInAddr& addr) {
    s32 result = IOS_Ioctl(sDeviceHandle, Ioctl_GetHostID, NULL, 0, NULL, 0);

    // Copy out IP if successful
    if (result >= 0) {
        addr.raw = static_cast<u32>(result);
    }

    return result;
}

} // namespace kiwi
