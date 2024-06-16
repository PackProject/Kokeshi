#ifndef LIBKIWI_NET_ASYNC_SOCKET_H
#define LIBKIWI_NET_ASYNC_SOCKET_H
#include <libkiwi/k_types.h>
#include <libkiwi/net/kiwiSocketBase.h>
#include <libkiwi/prim/kiwiLinkList.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_net
//! @{

/**
 * @brief Asynchronous (non-blocking) socket
 */
class AsyncSocket : public SocketBase {
public:
    /**
     * @brief Constructor
     *
     * @param family Socket protocol family
     * @param type Socket type
     */
    AsyncSocket(SOProtoFamily family, SOSockType type);

    /**
     * @brief Destructor
     */
    virtual ~AsyncSocket() {
        sSocketList.Remove(this);
    }

    /**
     * @brief Connects to a peer
     *
     * @param rAddr Remote address
     * @param pCallback Connection callback
     * @param pArg Callback user argument
     * @return Success
     */
    virtual bool Connect(const SockAddrAny& rAddr, Callback pCallback,
                         void* pArg);

    /**
     * @brief Accepts a peer connection over a new socket
     *
     * @param pCallback Acceptance callback
     * @param pArg Callback user argument
     * @return New socket
     */
    virtual AsyncSocket* Accept(AcceptCallback pCallback, void* pArg);

private:
    /**
     * @brief Async state
     */
    enum EState { EState_Thinking, EState_Connecting, EState_Accepting };

    // Async receive operation
    class RecvJob;
    // Async send operation
    class SendJob;

private:
    /**
     * @brief Socket thread function
     *
     * @param pArg Thread function argument
     */
    static void* ThreadFunc(void* pArg);

    /**
     * @brief Constructor
     *
     * @param socket Socket file descriptor
     * @param type Socket protocol family
     * @param type Socket type
     */
    AsyncSocket(SOSocket socket, SOProtoFamily family, SOSockType type);

    /**
     * @brief Prepares socket for async operation
     */
    void Initialize();
    /**
     * @brief Processes pending socket tasks
     */
    void Calc();

    /**
     * @brief Receives packet data over socket
     */
    void CalcRecv();
    /**
     * @brief Sends packet data over socket
     */
    void CalcSend();

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
                              void* pArg);

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
                              void* pArg);

private:
    static const u32 scThreadStackSize = 0x4000;

    volatile EState mState; // Current async task
    SockAddrAny mPeer;      // Peer address

    TList<RecvJob> mRecvJobs; // Active receive jobs
    TList<SendJob> mSendJobs; // Active send jobs

    Callback mpConnectCallback; // Connect callback
    void* mpConnectCallbackArg; // Connect callback user argument

    AcceptCallback mpAcceptCallback; // Accept callback
    void* mpAcceptCallbackArg;       // Accept callback user argument

    static OSThread sSocketThread;                   // Socket manager thread
    static bool sSocketThreadCreated;                // Thread guard
    static u8 sSocketThreadStack[scThreadStackSize]; // Thread stack
    static TList<AsyncSocket> sSocketList;           // Open async sockets
};

//! @}
} // namespace kiwi

#endif
