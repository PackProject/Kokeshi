#ifndef LIBKIWI_NET_SOCKET_BASE_H
#define LIBKIWI_NET_SOCKET_BASE_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiOptional.h>
#include <libkiwi/support/kiwiLibSO.h>

namespace kiwi {
//! @addtogroup libkiwi_net
//! @{

/**
 * @brief IOS Berkeley socket wrapper
 */
class SocketBase : private NonCopyable {
public:
    /**
     * @brief Generic socket operation callback
     *
     * @param result Socket library result
     * @param pArg User callback argument
     */
    typedef void (*Callback)(SOResult result, void* pArg);

    /**
     * @brief Connection accept callback
     *
     * @param result Socket library result
     * @param pPeer Peer socket object
     * @param rAddr Peer address
     * @param pArg User callback argument
     */
    typedef void (*AcceptCallback)(SOResult result, SocketBase* pPeer,
                                   const SockAddrAny& rAddr, void* pArg);

public:
    /**
     * @brief Gets the console's IP address
     *
     * @param rAddr[out] IPv4 address
     */
    static void GetHostAddr(SockAddr4& rAddr);

    /**
     * @brief Constructor
     *
     * @param family Socket protocol family
     * @param type Socket type
     */
    SocketBase(SOProtoFamily family, SOSockType type);

    /**
     * @brief Destructor
     */
    virtual ~SocketBase();

    /**
     * @brief Tests whether the socket has a valid descriptor
     */
    bool IsOpen() const {
        return mHandle >= 0;
    }

    /**
     * @brief Connects to a peer
     *
     * @param rAddr Remote address
     * @param pCallback Connection callback
     * @param pArg Callback user argument
     * @return Success
     */
    virtual bool Connect(const SockAddrAny& rAddr, Callback pCallback = nullptr,
                         void* pArg = nullptr) = 0;

    /**
     * @brief Accepts a peer connection over a new socket
     *
     * @param pCallback Acceptance callback
     * @param pArg Callback user argument
     * @return New socket
     */
    virtual SocketBase* Accept(AcceptCallback pCallback = nullptr,
                               void* pArg = nullptr) = 0;

    /**
     * @brief Binds socket to local address
     * @note Bind to port zero for a random port (written out)
     *
     * @param rAddr[in,out] Local address
     * @return Success
     */
    bool Bind(SockAddrAny& rAddr = SockAddr4()) const;

    /**
     * @brief Listens for incoming connections
     *
     * @param backlog Maximum pending connections
     * @return Success
     */
    bool Listen(s32 backlog = SOMAXCONN) const;

    /**
     * @brief Tests whether socket is blocking
     */
    bool IsBlocking() const;

    /**
     * @brief Toggles socket blocking
     *
     * @param enable Whether to enable blocking
     * @return Success
     */
    bool SetBlocking(bool enable) const;

    /**
     * @brief Toggles port reuse
     *
     * @param enable Whether to enable port reuse
     * @return Success
     */
    bool SetReuseAddr(bool enable) const;

    /**
     * @brief Stops socket from reading/writing
     *
     * @param how How to shutdown connection
     * @return Success
     */
    bool Shutdown(SOShutdownType how) const;

    /**
     * @brief Closes socket
     *
     * @return Success
     */
    bool Close();

    /**
     * @brief Gets endpoint of socket
     *
     * @param[out] rAddr Socket address
     * @return Success
     */
    bool GetSocketAddr(SockAddrAny& rAddr) const;
    /**
     * @brief Gets endpoint of peer
     *
     * @param[out] rAddr Peer address
     * @return Success
     */
    bool GetPeerAddr(SockAddrAny& rAddr) const;

    /**
     * @brief Tests whether socket can receive data
     */
    bool CanRecv() const;
    /**
     * @brief Tests whether socket can send data
     */
    bool CanSend() const;

    /**
     * @name Receive data
     */
    /**@{*/
    /**
     * @brief Receives bytes from bound connection
     *
     * @param pDst Destination buffer
     * @param len Buffer size
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     * @return Number of bytes received
     */
    Optional<u32> RecvBytes(void* pDst, u32 len, Callback pCallback = nullptr,
                            void* pArg = nullptr);
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
    Optional<u32> RecvBytesFrom(void* pDst, u32 len, SockAddrAny& rAddr,
                                Callback pCallback = nullptr,
                                void* pArg = nullptr);

    /**
     * @brief Receives object from bound connection
     *
     * @param rDst Destination object
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     * @return Number of bytes received
     */
    template <typename T>
    Optional<u32> Recv(T& rDst, Callback pCallback = nullptr,
                       void* pArg = nullptr) {
        return RecvBytes(&rDst, sizeof(T), pCallback, pArg);
    }
    /**
     * @brief Receives object and records sender address
     *
     * @param rDst Destination object
     * @param rAddr[out] Sender address
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     * @return Number of bytes received
     */
    template <typename T>
    Optional<u32> RecvFrom(T& rDst, SockAddrAny& rAddr,
                           Callback pCallback = nullptr, void* pArg = nullptr) {
        return RecvBytesFrom(&rDst, sizeof(T), rAddr, pCallback, pArg);
    }
    /**@}*/

    /**
     * @name Send data
     */
    /**@{*/
    /**
     * @brief Sends bytes to bound connection
     *
     * @param pSrc Source buffer
     * @param len Buffer size
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     * @return Number of bytes sent
     */
    Optional<u32> SendBytes(const void* pSrc, u32 len,
                            Callback pCallback = nullptr, void* pArg = nullptr);
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
    Optional<u32> SendBytesTo(const void* pSrc, u32 len,
                              const SockAddrAny& rAddr,
                              Callback pCallback = nullptr,
                              void* pArg = nullptr);

    /**
     * @brief Sends object to bound connection
     *
     * @param rSrc Source object
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     * @return Number of bytes sent
     */
    template <typename T>
    Optional<u32> Send(const T& rSrc, Callback pCallback = nullptr,
                       void* pArg = nullptr) {
        return SendBytes(&rSrc, sizeof(T), pCallback, pArg);
    }
    /**
     * @brief Sends object to specified connection
     *
     * @param rSrc Source object
     * @param rAddr Destination address
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     * @return Number of bytes sent
     */
    template <typename T>
    Optional<u32> SendTo(const T& rSrc, const SockAddrAny& rAddr,
                         Callback pCallback = nullptr, void* pArg = nullptr) {
        return SendBytesTo(&rSrc, sizeof(T), rAddr, pCallback, pArg);
    }

    /**
     * @brief Sends string to bound connection
     *
     * @param rSrc Source string
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     * @return Number of bytes sent
     */
    template <typename T>
    Optional<u32> Send(const StringImpl<T>& rSrc, Callback pCallback = nullptr,
                       void* pArg = nullptr) {
        return SendBytes(rSrc.CStr(), rSrc.Length() * sizeof(T), pCallback,
                         pArg);
    }
    /**
     * @brief Sends string to specified connection
     *
     * @param rSrc Source string
     * @param rAddr Destination address
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     * @return Number of bytes sent
     */
    template <typename T>
    Optional<u32> SendTo(const StringImpl<T>& rSrc, const SockAddrAny& rAddr,
                         Callback pCallback = nullptr, void* pArg = nullptr) {
        return SendBytesTo(rSrc.CStr(), rSrc.Length() * sizeof(T), rAddr,
                           pCallback, pArg);
    }
    /**@}*/

protected:
    /**
     * @brief Constructor
     *
     * @param socket Socket file descriptor
     * @param type Socket protocol family
     * @param type Socket type
     */
    SocketBase(SOSocket socket, SOProtoFamily family, SOSockType type);

private:
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
    virtual SOResult RecvImpl(void* pDst, u32 len, u32& rRecv,
                              SockAddrAny* pAddr, Callback pCallback,
                              void* pArg) = 0;

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
    virtual SOResult SendImpl(const void* pSrc, u32 len, u32& rSend,
                              const SockAddrAny* pAddr, Callback pCallback,
                              void* pArg) = 0;

protected:
    SOSocket mHandle;      // File descriptor
    SOProtoFamily mFamily; // Protocol family
    SOSockType mType;      // Socket type
};

//! @}
} // namespace kiwi

#endif
