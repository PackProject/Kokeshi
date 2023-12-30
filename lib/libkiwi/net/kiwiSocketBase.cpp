#include <libkiwi.h>

namespace kiwi {

namespace {

/**
 * Processes message (send/recv) result
 *
 * @param err LibSO result code
 * @param[out] outsize Message size, or -1 if error/blocking
 * @return Message success or would-be-blocking
 */
bool MsgResultProc(s32 err, std::size_t* outsize) {
    // Socket error
    if (err < 0) {
        // Cannot know message size
        if (outsize != NULL) {
            // Max unsigned value
            *outsize = -1;
        }

        // Only successful if error was from blocking
        return err == SO_EWOULDBLOCK;
    }

    // "Error code" is actually message size (when >= 0)
    if (outsize != NULL) {
        *outsize = static_cast<std::size_t>(err);
    }

    // Message was successful
    return true;
}

} // namespace

/**
 * Constructor
 *
 * @param family Socket protocol family
 * @param type Socket type
 */
SocketBase::SocketBase(SOProtoFamily family, SOSockType type)
    : mHandle(-1), mFamily(family), mType(type) {
    mHandle = LibSO::Socket(family, type);
    K_ASSERT_EX(mHandle >= 0, "Failed to create socket");
}

/**
 * Constructor
 *
 * @param socket Socket file descriptor
 * @param type Socket protocol family
 * @param type Socket type
 */
SocketBase::SocketBase(SOSocket socket, SOProtoFamily family, SOSockType type)
    : mHandle(socket), mFamily(family), mType(type) {
    K_ASSERT(mHandle >= 0);
}

/**
 * Destructor
 */
SocketBase::~SocketBase() {
    K_ASSERT(mHandle >= 0);
    Close();
}

/**
 * Gets the console's IP address
 *
 * @return Raw IPv4 address
 */
u32 SocketBase::GetHostAddr() {
    SOInAddr addr;

    if (LibSO::GetHostID(addr) >= SO_SUCCESS) {
        return addr.raw;
    }

    // Fallback to loopback address
    LibSO::INetPtoN("127.0.0.1", addr);
    return addr.raw;
}

/**
 * Binds socket to local address
 *
 * @param addr Local address
 * @return Success
 */
bool SocketBase::Bind(const SOSockAddr& addr) const {
    K_ASSERT(mHandle >= 0);
    K_ASSERT(mFamily == addr.in.family);

    return LibSO::Bind(mHandle, addr) >= SO_SUCCESS;
}

/**
 * Listens for incoming connections
 *
 * @param backlog Maximum pending connections
 * @return Success
 */
bool SocketBase::Listen(s32 backlog) const {
    K_ASSERT(mHandle >= 0);
    K_WARN(mType == SO_SOCK_DGRAM, "Listen won't do anything for dgram.");

    return LibSO::Listen(mHandle, backlog) >= SO_SUCCESS;
}

/**
 * Toggles socket blocking
 *
 * @param block Whether to enable blocking
 * @return Success
 */
bool SocketBase::SetBlocking(bool block) const {
    K_ASSERT(mHandle >= 0);

    s32 flags = LibSO::Fcntl(mHandle, SO_F_GETFL, 0);

    if (block) {
        flags &= ~SO_O_NONBLOCK;
    } else {
        flags |= SO_O_NONBLOCK;
    }

    return LibSO::Fcntl(mHandle, SO_F_SETFL, flags) >= 0;
}

/**
 * Stops socket from reading/writing
 *
 * @param how How to shutdown connection
 * @return Success
 */
bool SocketBase::Shutdown(SOShutdownType how) const {
    K_ASSERT(mHandle >= 0);
    K_ASSERT(how == SO_SHUT_RD || how == SO_SHUT_WR || how == SO_SHUT_RDWR);

    return LibSO::Shutdown(mHandle, how) >= SO_SUCCESS;
}

/**
 * Closes socket
 *
 * @return Success
 */
bool SocketBase::Close() {
    K_ASSERT(mHandle >= 0);

    // Invalidate socket descriptor
    const SOSocket fd = mHandle;
    mHandle = -1;

    return LibSO::Close(fd) >= SO_SUCCESS;
}

/**
 * Monitors status of socket descriptors
 *
 * @param fds Poll information
 * @param numfds Descriptor array length
 * @param timeout Timeout (OS time)
 * @return Success
 */
bool SocketBase::Poll(SOPollFD fds[], std::size_t numfds, s64 timeout) const {
    return LibSO::Poll(fds, numfds, timeout) >= SO_SUCCESS;
}

/**
 * Gets endpoint of socket
 *
 * @param[out] addr Socket address
 * @return Success
 */
bool SocketBase::GetSocketAddr(SOSockAddr& addr) const {
    K_ASSERT(mHandle >= 0);
    K_ASSERT(mFamily == addr.in.family);

    return LibSO::GetSockName(mHandle, addr) >= SO_SUCCESS;
}

/**
 * Gets endpoint of peer
 *
 * @param[out] addr Peer address
 * @return Success
 */
bool SocketBase::GetPeerAddr(SOSockAddr& addr) const {
    K_ASSERT(mHandle >= 0);
    K_ASSERT(mFamily == addr.in.family);

    return LibSO::GetPeerName(mHandle, addr) >= SO_SUCCESS;
}

/**
 * Tests if socket can receive data
 */
bool SocketBase::CanReceive() const {
    K_ASSERT(mHandle >= 0);

    SOPollFD fd[1];
    fd[0].fd = mHandle;
    fd[0].events = SO_POLLRDNORM;
    fd[0].revents = 0;

    bool success = Poll(fd, LENGTHOF(fd), 0);
    return success && fd[0].events == fd[0].revents;
}

/**
 * Tests if socket can send data
 */
bool SocketBase::CanSend() const {
    K_ASSERT(mHandle >= 0);

    SOPollFD fd[1];
    fd[0].fd = mHandle;
    fd[0].events = SO_POLLWRNORM;
    fd[0].revents = 0;

    bool success = Poll(fd, LENGTHOF(fd), 0);
    return success && fd[0].events == fd[0].revents;
}

/**
 * Receives bytes from bound connection
 *
 * @param buf Destination buffer
 * @param len Buffer size
 * @param[out] nrecv Number of bytes received, or -1 if error/blocking
 * @return Success or would-be-blocking
 */
bool SocketBase::RecieveBytes(void* buf, std::size_t len, std::size_t* nrecv) {
    K_ASSERT(mHandle >= 0);
    return MsgResultProc(RecieveImpl(buf, len, NULL), nrecv);
}

/**
 * Receives bytes from specified connection
 *
 * @param buf Destination buffer
 * @param len Buffer size
 * @param[out] addr Sender address
 * @param[out] nrecv Number of bytes received, or -1 if error/blocking
 * @return Success or would-be-blocking
 */
bool SocketBase::RecieveBytesFrom(void* buf, std::size_t len, SOSockAddr& addr,
                                  std::size_t* nrecv) {
    K_ASSERT(mHandle >= 0);
    return MsgResultProc(RecieveImpl(buf, len, &addr), nrecv);
}

/**
 * Sends bytes to bound connection
 *
 * @param buf Source buffer
 * @param len Buffer size
 * @param[out] nsend Number of bytes sent, or -1 if error/blocking
 * @return Success or would-be-blocking
 */
bool SocketBase::SendBytes(const void* buf, std::size_t len,
                           std::size_t* nsend) {
    K_ASSERT(mHandle >= 0);
    return MsgResultProc(SendImpl(buf, len, NULL), nsend);
}

/**
 * Sends bytes to specified connection
 *
 * @param buf Source buffer
 * @param len Buffer size
 * @param addr Destination address
 * @param[out] nsend Number of bytes sent, or -1 if error/blocking
 * @return Success or would-be-blocking
 */
bool SocketBase::SendBytesTo(const void* buf, std::size_t len,
                             const SOSockAddr& addr, std::size_t* nsend) {
    K_ASSERT(mHandle >= 0);
    return MsgResultProc(SendImpl(buf, len, &addr), nsend);
}

} // namespace kiwi
