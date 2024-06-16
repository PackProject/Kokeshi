#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Constructor
 *
 * @param family Socket protocol family
 * @param type Socket type
 */
SocketBase::SocketBase(SOProtoFamily family, SOSockType type)
    : mHandle(-1), mFamily(family), mType(type) {
    // Create IOS socket
    mHandle = LibSO::Socket(mFamily, mType);
    K_ASSERT_EX(IsOpen(), "Failed to create socket (%d)",
                LibSO::GetLastError());

    // By default, enable port reuse
    bool success = SetReuseAddr(true);
    K_ASSERT(success);
}

/**
 * @brief Constructor
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
 * @brief Destructor
 */
SocketBase::~SocketBase() {
    K_ASSERT(IsOpen());

    bool success = Close();
    K_ASSERT(success);
}

/**
 * @brief Gets the console's IP address
 *
 * @param rAddr[out] IPv4 address
 */
void SocketBase::GetHostAddr(SockAddr4& rAddr) {
    LibSO::GetHostID(rAddr);
}

/**
 * @brief Binds socket to local address
 * @note Bind to port zero for a random port (written out)
 *
 * @param rAddr[in,out] Local address
 * @return Success
 */
bool SocketBase::Bind(SockAddrAny& rAddr) const {
    K_ASSERT(IsOpen());
    K_ASSERT(mFamily == rAddr.in.family);

    // Choose a random port in the private range
    if (rAddr.port == 0) {
        // Retry up to 10 times in case the random port is in use
        for (int i = 0; i < 10; i++) {
            rAddr.port = Random().NextU32(49152, 65535);

            if (LibSO::Bind(mHandle, rAddr) == SO_SUCCESS) {
                return true;
            }
        }
    }

    return LibSO::Bind(mHandle, rAddr) == SO_SUCCESS;
}

/**
 * @brief Listens for incoming connections
 *
 * @param backlog Maximum pending connections
 * @return Success
 */
bool SocketBase::Listen(s32 backlog) const {
    K_ASSERT(IsOpen());
    K_WARN_EX(mType == SO_SOCK_DGRAM, "Listen won't do anything for dgram.\n");

    return LibSO::Listen(mHandle, backlog) == SO_SUCCESS;
}

/**
 * @brief Tests whether socket is blocking
 */
bool SocketBase::IsBlocking() const {
    K_ASSERT(IsOpen());

    s32 flags = LibSO::Fcntl(mHandle, SO_F_GETFL, 0);
    return (flags & SO_O_NONBLOCK) == 0;
}

/**
 * @brief Toggles socket blocking
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
 * @brief Toggles port reuse
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
 * @brief Stops socket from reading/writing
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
 * @brief Closes socket
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
 * @brief Gets endpoint of socket
 *
 * @param[out] rAddr Socket address
 * @return Success
 */
bool SocketBase::GetSocketAddr(SockAddrAny& rAddr) const {
    K_ASSERT(IsOpen());
    K_ASSERT(mFamily == rAddr.in.family);

    return LibSO::GetSockName(mHandle, rAddr) == SO_SUCCESS;
}

/**
 * @brief Gets endpoint of peer
 *
 * @param[out] rAddr Peer address
 * @return Success
 */
bool SocketBase::GetPeerAddr(SockAddrAny& rAddr) const {
    K_ASSERT(IsOpen());
    K_ASSERT(mFamily == rAddr.in.family);

    return LibSO::GetPeerName(mHandle, rAddr) == SO_SUCCESS;
}

/**
 * @brief Tests whether socket can receive data
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
 * @brief Tests whether socket can send data
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
 * @brief Receives bytes from bound connection
 *
 * @param pDst Destination buffer
 * @param len Buffer size
 * @param pCallback Completion callback
 * @param pArg Callback user argument
 * @return Number of bytes received
 */
Optional<u32> SocketBase::RecvBytes(void* pDst, u32 len, Callback pCallback,
                                    void* pArg) {
    K_ASSERT(IsOpen());
    K_ASSERT(pDst != nullptr);
    K_ASSERT(len > 0);

    // Implementation version is responsible for using the callback
    u32 nrecv = 0;
    SOResult result = RecvImpl(pDst, len, nrecv, nullptr, pCallback, pArg);

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
 * @brief Receives bytes and records sender address
 *
 * @param pDst Destination buffer
 * @param len Buffer size
 * @param rAddr[out] Sender address
 * @param pCallback Completion callback
 * @param pArg Callback user argument
 * @return Number of bytes received
 */
Optional<u32> SocketBase::RecvBytesFrom(void* pDst, u32 len, SockAddrAny& rAddr,
                                        Callback pCallback, void* pArg) {
    K_ASSERT(IsOpen());
    K_ASSERT(pDst != nullptr);
    K_ASSERT(len > 0);

    // Implementation version is responsible for using the callback
    u32 nrecv = 0;
    SOResult result = RecvImpl(pDst, len, nrecv, &rAddr, pCallback, pArg);

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
 * @brief Sends bytes to bound connection
 *
 * @param pSrc Source buffer
 * @param len Buffer size
 * @param pCallback Completion callback
 * @param pArg Callback user argument
 * @return Number of bytes sent
 */
Optional<u32> SocketBase::SendBytes(const void* pSrc, u32 len,
                                    Callback pCallback, void* pArg) {
    K_ASSERT(IsOpen());
    K_ASSERT(pSrc != nullptr);
    K_ASSERT(len > 0);

    // Implementation version is responsible for using the callback
    u32 nsend = 0;
    SOResult result = SendImpl(pSrc, len, nsend, nullptr, pCallback, pArg);

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
 * @brief Sends bytes to specified connection
 *
 * @param pSrc Source buffer
 * @param len Buffer size
 * @param rAddr Destination address
 * @param pCallback Completion callback
 * @param pArg Callback user argument
 * @return Number of bytes sent
 */
Optional<u32> SocketBase::SendBytesTo(const void* pSrc, u32 len,
                                      const SockAddrAny& rAddr,
                                      Callback pCallback, void* pArg) {
    K_ASSERT(IsOpen());
    K_ASSERT(pSrc != nullptr);
    K_ASSERT(len > 0);

    // Implementation version is responsible for using the callback
    u32 nsend = 0;
    SOResult result = SendImpl(pSrc, len, nsend, &rAddr, pCallback, pArg);

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
