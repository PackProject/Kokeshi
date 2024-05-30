#include <climits>
#include <libkiwi.h>

namespace kiwi {

/**
 * Connects to another socket
 *
 * @param addr Remote address
 * @param callback Connection callback
 * @param arg Callback user argument
 * @return Success
 */
bool SyncSocket::Connect(const SockAddr& addr, Callback callback, void* arg) {
    K_ASSERT(IsOpen());

    s32 result = LibSO::Connect(mHandle, addr);
    bool success = result == SO_SUCCESS || result == SO_EISCONN;

    if (callback != NULL) {
        callback(LibSO::GetLastError(), arg);
    }

    return success;
}

/**
 * Accepts remote connection
 *
 * @param callback Acceptance callback
 * @param arg Callback user argument
 * @return New socket
 */
SyncSocket* SyncSocket::Accept(AcceptCallback callback, void* arg) {
    K_ASSERT(IsOpen());

    SyncSocket* peer = NULL;
    kiwi::SockAddr4 addr; // TODO: Will forcing ipv4 cause problems?

    s32 fd = LibSO::Accept(mHandle, addr);

    // Result code is the peer descriptor
    if (fd > 0) {
        peer = new SyncSocket(fd, mFamily, mType);
        K_ASSERT(peer != NULL);
    }

    if (callback != NULL) {
        callback(LibSO::GetLastError(), peer, addr, arg);
    }

    return peer;
}

/**
 * Receives data and records sender address
 *
 * @param dst Destination buffer
 * @param len Buffer size
 * @param[out] nrecv Number of bytes received
 * @param[out] addr Sender address
 * @param callback Completion callback
 * @param arg Callback user argument
 * @return Socket library result
 */
SOResult SyncSocket::RecvImpl(void* dst, u32 len, u32& nrecv, SockAddr* addr,
                              Callback callback, void* arg) {
    K_ASSERT(IsOpen());
    K_ASSERT(dst != NULL);
    K_ASSERT(len > 0 && len < ULONG_MAX);

    s32 result;
    kiwi::SockAddr4 peer; // TODO: Will forcing ipv4 cause problems?

    nrecv = 0;
    while (nrecv < len) {
        result = LibSO::RecvFrom(mHandle, dst, len - nrecv, 0, peer);
        if (result <= 0) {
            goto _exit;
        }

        dst = AddToPtr(dst, result);
        nrecv += result;
    }

    K_ASSERT_EX(nrecv <= len, "Overflow???");

_exit:
    if (addr != NULL) {
        *addr = peer;
    }

    if (callback != NULL) {
        callback(LibSO::GetLastError(), arg);
    }

    // Successful if the last transaction resulted in some amount of bytes read
    return result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);
}

/**
 * Sends data to specified connection
 *
 * @param dst Destination buffer
 * @param len Buffer size
 * @param[out] nsend Number of bytes sent
 * @param[out] addr Sender address
 * @param callback Completion callback
 * @param arg Callback user argument
 * @return Socket library result
 */
SOResult SyncSocket::SendImpl(const void* src, u32 len, u32& nsend,
                              const SockAddr* addr, Callback callback,
                              void* arg) {
    K_ASSERT(IsOpen());
    K_ASSERT(src != NULL);
    K_ASSERT(len > 0 && len < ULONG_MAX);

    s32 result;

    nsend = 0;
    while (nsend < len) {
        if (addr != NULL) {
            result = LibSO::SendTo(mHandle, src, len - nsend, 0, *addr);
        } else {
            result = LibSO::Send(mHandle, src, len - nsend, 0);
        }

        if (result < 0) {
            goto _exit;
        }

        nsend += result;
    }

    K_ASSERT_EX(nsend <= len, "Overflow???");

_exit:
    if (callback != NULL) {
        callback(LibSO::GetLastError(), arg);
    }

    return result >= 0 ? SO_SUCCESS : static_cast<SOResult>(result);
}

} // namespace kiwi
