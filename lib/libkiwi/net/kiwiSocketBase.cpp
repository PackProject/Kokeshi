#include <libkiwi.h>

namespace kiwi {

/**
 * Constructor
 *
 * @param family Socket protocol family
 * @param type Socket type
 */
SocketBase::SocketBase(SOProtoFamily family, SOSockType type)
    : mHandle(-1), mFamily(family), mType(type) {
    // Create IOS socket
    mHandle = LibSO::Socket(mFamily, mType);
    K_ASSERT_EX(IsOpen(), "Failed to create socket");

    // By default, enable port reuse
    bool success = SetReuseAddr(true);
    K_ASSERT(success);
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
    K_ASSERT_EX(IsOpen(), "Invalid socket descriptor provided");
}

/**
 * Destructor
 */
SocketBase::~SocketBase() {
    K_ASSERT(IsOpen());

    bool success = Close();
    K_ASSERT(success);
}

/**
 * Gets the console's IP address
 *
 * @param addr[out] IPv4 address
 */
void SocketBase::GetHostAddr(SockAddr4& addr) {
    LibSO::GetHostID(addr);
}

/**
 * Binds socket to local address
 *
 * @note Bind to port zero for a random port (written out)
 * @param addr[in,out] Local address
 * @return Success
 */
bool SocketBase::Bind(SockAddr& addr) const {
    K_ASSERT(IsOpen());
    K_ASSERT(mFamily == addr.in.family);

    // Choose a random port in the private range
    if (addr.port == 0) {
        // Retry up to 10 times in case the random port is in use
        for (int i = 0; i < 10; i++) {
            addr.port = Random().NextU32(49152, 65535);

            if (LibSO::Bind(mHandle, addr) == SO_SUCCESS) {
                return true;
            }
        }
    }

    return LibSO::Bind(mHandle, addr) == SO_SUCCESS;
}

/**
 * Listens for incoming connections
 *
 * @param backlog Maximum pending connections
 * @return Success
 */
bool SocketBase::Listen(s32 backlog) const {
    K_ASSERT(IsOpen());
    K_WARN(mType == SO_SOCK_DGRAM, "Listen won't do anything for dgram.");

    return LibSO::Listen(mHandle, backlog) == SO_SUCCESS;
}

/**
 * Toggles socket blocking
 *
 * @param enable Whether to enable blocking
 * @return Success
 */
bool SocketBase::SetBlocking(bool enable) const {
    K_ASSERT(IsOpen());

    s32 flags = LibSO::Fcntl(mHandle, SO_F_GETFL, 0);

    if (enable) {
        flags &= ~SO_O_NONBLOCK;
    } else {
        flags |= SO_O_NONBLOCK;
    }

    return LibSO::Fcntl(mHandle, SO_F_SETFL, flags) >= 0;
}

/**
 * Toggle port reuse
 *
 * @param enable Whether to enable port reuse
 * @return Success
 */
bool SocketBase::SetReuseAddr(bool enable) const {
    s32 value = enable;
    return LibSO::SetSockOpt(mHandle, SO_SOL_SOCKET, SO_SO_REUSEADDR, &value,
                             sizeof(value)) == SO_SUCCESS;
}

/**
 * Stops socket from reading/writing
 *
 * @param how How to shutdown connection
 * @return Success
 */
bool SocketBase::Shutdown(SOShutdownType how) const {
    K_ASSERT(IsOpen());
    K_ASSERT(how == SO_SHUT_RD || how == SO_SHUT_WR || how == SO_SHUT_RDWR);

    return LibSO::Shutdown(mHandle, how) == SO_SUCCESS;
}

/**
 * Closes socket
 *
 * @return Success
 */
bool SocketBase::Close() {
    K_ASSERT(IsOpen());

    // Attempt to close
    if (LibSO::Close(mHandle) < 0) {
        return false;
    }

    // Invalidate socket descriptor
    mHandle = -1;
    return true;
}

/**
 * Gets endpoint of socket
 *
 * @param[out] addr Socket address
 * @return Success
 */
bool SocketBase::GetSocketAddr(SockAddr& addr) const {
    K_ASSERT(IsOpen());
    K_ASSERT(mFamily == addr.in.family);

    return LibSO::GetSockName(mHandle, addr) == SO_SUCCESS;
}

/**
 * Gets endpoint of peer
 *
 * @param[out] addr Peer address
 * @return Success
 */
bool SocketBase::GetPeerAddr(SockAddr& addr) const {
    K_ASSERT(IsOpen());
    K_ASSERT(mFamily == addr.in.family);

    return LibSO::GetPeerName(mHandle, addr) == SO_SUCCESS;
}

/**
 * Tests if socket can receive data
 */
bool SocketBase::CanRecv() const {
    K_ASSERT(IsOpen());

    SOPollFD fd[1];
    fd[0].fd = mHandle;
    fd[0].events = SO_POLLRDNORM;
    fd[0].revents = 0;

    bool success = LibSO::Poll(fd, LENGTHOF(fd), 0);
    return success && fd[0].events == fd[0].revents;
}

/**
 * Tests if socket can send data
 */
bool SocketBase::CanSend() const {
    K_ASSERT(IsOpen());

    SOPollFD fd[1];
    fd[0].fd = mHandle;
    fd[0].events = SO_POLLWRNORM;
    fd[0].revents = 0;

    bool success = LibSO::Poll(fd, LENGTHOF(fd), 0);
    return success && fd[0].events == fd[0].revents;
}

/**
 * Receives bytes from bound connection
 *
 * @param buf Destination buffer
 * @param len Buffer size
 * @param callback Completion callback
 * @param arg Callback user argument
 * @return Number of bytes received
 */
Optional<u32> SocketBase::RecvBytes(void* buf, u32 len, Callback callback,
                                    void* arg) {
    K_ASSERT(IsOpen());
    K_ASSERT(buf != NULL);
    K_ASSERT(len > 0);

    // Implementation version is responsible for using the callback
    u32 nrecv = 0;
    SOResult result = RecvImpl(buf, len, nrecv, NULL, callback, arg);

    // Success, return bytes read
    if (result == SO_SUCCESS) {
        return nrecv;
    }

    // Blocking is OK, just say zero bytes
    if (result == SO_EWOULDBLOCK) {
        return 0;
    }

    // Something went wrong!
    return kiwi::nullopt;
}

/**
 * Receives bytes and records sender address
 *
 * @param buf Destination buffer
 * @param len Buffer size
 * @param addr[out] Sender address
 * @param callback Completion callback
 * @param arg Callback user argument
 * @return Number of bytes received
 */
Optional<u32> SocketBase::RecvBytesFrom(void* buf, u32 len, SockAddr& addr,
                                        Callback callback, void* arg) {
    K_ASSERT(IsOpen());
    K_ASSERT(buf != NULL);
    K_ASSERT(len > 0);

    // Implementation version is responsible for using the callback
    u32 nrecv = 0;
    SOResult result = RecvImpl(buf, len, nrecv, &addr, callback, arg);

    // Success, return bytes read
    if (result == SO_SUCCESS) {
        return nrecv;
    }

    // Blocking is OK, just say zero bytes
    if (result == SO_EWOULDBLOCK) {
        return 0;
    }

    // Something went wrong!
    return kiwi::nullopt;
}

/**
 * Sends bytes to bound connection
 *
 * @param buf Source buffer
 * @param len Buffer size
 * @param callback Completion callback
 * @param arg Callback user argument
 * @return Number of bytes sent
 */
Optional<u32> SocketBase::SendBytes(const void* buf, u32 len, Callback callback,
                                    void* arg) {
    K_ASSERT(IsOpen());
    K_ASSERT(buf != NULL);
    K_ASSERT(len > 0);

    // Implementation version is responsible for using the callback
    u32 nsend = 0;
    SOResult result = SendImpl(buf, len, nsend, NULL, callback, arg);

    // Success, return bytes read
    if (result == SO_SUCCESS) {
        return nsend;
    }

    // Blocking is OK, just say zero bytes
    if (result == SO_EWOULDBLOCK) {
        return 0;
    }

    // Something went wrong!
    return kiwi::nullopt;
}

/**
 * Sends bytes to specified connection
 *
 * @param buf Source buffer
 * @param len Buffer size
 * @param addr Destination address
 * @param callback Completion callback
 * @param arg Callback user argument
 * @return Number of bytes sent
 */
Optional<u32> SocketBase::SendBytesTo(const void* buf, u32 len,
                                      const SockAddr& addr, Callback callback,
                                      void* arg) {
    K_ASSERT(IsOpen());
    K_ASSERT(buf != NULL);
    K_ASSERT(len > 0);

    // Implementation version is responsible for using the callback
    u32 nsend = 0;
    SOResult result = SendImpl(buf, len, nsend, &addr, callback, arg);

    // Success, return bytes read
    if (result == SO_SUCCESS) {
        return nsend;
    }

    // Blocking is OK, just say zero bytes
    if (result == SO_EWOULDBLOCK) {
        return 0;
    }

    // Something went wrong!
    return kiwi::nullopt;
}

} // namespace kiwi
