#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Connects to a peer
 *
 * @param rAddr Remote address
 * @param pCallback Connection callback
 * @param pArg Callback user argument
 * @return Success
 */
bool SyncSocket::Connect(const SockAddrAny& rAddr, Callback pCallback,
                         void* pArg) {
    K_ASSERT(IsOpen());

    s32 result = LibSO::Connect(mHandle, rAddr);
    bool success = result == SO_SUCCESS || result == SO_EISCONN;

    if (pCallback != nullptr) {
        pCallback(LibSO::GetLastError(), pArg);
    }

    return success;
}

/**
 * @brief Accepts a peer connection over a new socket
 *
 * @param pCallback Acceptance callback
 * @param pArg Callback user argument
 * @return New socket
 */
SyncSocket* SyncSocket::Accept(AcceptCallback pCallback, void* pArg) {
    K_ASSERT(IsOpen());

    SyncSocket* pPeer = nullptr;
    SockAddr4 addr; // TODO: Will forcing ipv4 cause problems?

    s32 fd = LibSO::Accept(mHandle, addr);

    // Result code is the peer descriptor
    if (fd > 0) {
        pPeer = new SyncSocket(fd, mFamily, mType);
        K_ASSERT(pPeer != nullptr);
    }

    if (pCallback != nullptr) {
        pCallback(LibSO::GetLastError(), pPeer, addr, pArg);
    }

    return pPeer;
}

/**
 * @brief Receives data and records sender address (internal implementation)
 *
 * @param pDst Destination buffer
 * @param len Buffer size
 * @param[out] rRecv Number of bytes received
 * @param[out] pAddr Sender address
 * @param pCallback Completion callback
 * @param pArg Callback user argument
 * @return Socket library result
 */
SOResult SyncSocket::RecvImpl(void* pDst, u32 len, u32& rRecv,
                              SockAddrAny* pAddr, Callback pCallback,
                              void* pArg) {
    K_ASSERT(IsOpen());
    K_ASSERT(pDst != nullptr);
    K_ASSERT(OSIsMEM2Region(pDst));
    K_ASSERT(len > 0);

    rRecv = 0;
    SockAddr4 addr;

    s32 result = LibSO::RecvFrom(mHandle, pDst, len, 0, addr);
    if (result > 0) {
        rRecv += result;
    }

    if (pAddr != nullptr) {
        *pAddr = addr;
    }

    if (pCallback != nullptr) {
        pCallback(LibSO::GetLastError(), pArg);
    }

    // Successful if some amount of bytes read
    return result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);
}

/**
 * @brief Sends data to specified connection (internal implementation)
 *
 * @param pSrc Source buffer
 * @param len Buffer size
 * @param[out] rSend Number of bytes sent
 * @param pAddr Sender address
 * @param pCallback Completion callback
 * @param pArg Callback user argument
 * @return Socket library result
 */
SOResult SyncSocket::SendImpl(const void* pSrc, u32 len, u32& rSend,
                              const SockAddrAny* pAddr, Callback pCallback,
                              void* pArg) {
    K_ASSERT(IsOpen());
    K_ASSERT(pSrc != nullptr);
    K_ASSERT(OSIsMEM2Region(pSrc));
    K_ASSERT(len > 0);

    rSend = 0;

    s32 result;
    if (pAddr != nullptr) {
        result = LibSO::SendTo(mHandle, pSrc, len - rSend, 0, *pAddr);
    } else {
        result = LibSO::Send(mHandle, pSrc, len - rSend, 0);
    }

    if (result > 0) {
        rSend += result;
    }

    if (pCallback != nullptr) {
        pCallback(LibSO::GetLastError(), pArg);
    }

    // Successful if some amount of bytes sent
    return result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);
}

} // namespace kiwi
