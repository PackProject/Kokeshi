#ifndef LIBKIWI_NET_ASYNC_SOCKET_H
#define LIBKIWI_NET_ASYNC_SOCKET_H
#include <libkiwi/net/kiwiPacket.h>
#include <libkiwi/net/kiwiSocketBase.h>
#include <libkiwi/prim/kiwiLinkList.h>
#include <libkiwi/prim/kiwiOptional.h>
#include <revolution/OS.h>
#include <types.h>

namespace kiwi {

/**
 * Asynchronous (non-blocking) socket
 */
class AsyncSocket : public SocketBase {
public:
    AsyncSocket(SOProtoFamily family, SOSockType type);
    virtual ~AsyncSocket();

    virtual bool Connect(const SockAddr& addr, Callback callback = NULL,
                         void* arg = NULL);
    virtual AsyncSocket* Accept(AcceptCallback callback = NULL,
                                void* arg = NULL);

private:
    /**
     * Async state
     */
    enum EState { EState_Thinking, EState_Connecting, EState_Accepting };

    class RecvJob;
    class SendJob;

private:
    static void* ThreadFunc(void* arg);

    AsyncSocket(SOSocket socket, SOProtoFamily family, SOSockType type);

    void Initialize();
    void Calc();
    void CalcRecv();
    void CalcSend();

    virtual SOResult RecvImpl(void* dst, u32 len, u32& nrecv, SockAddr* addr,
                              Callback callback, void* arg);
    virtual SOResult SendImpl(const void* src, u32 len, u32& nsend,
                              const SockAddr* addr, Callback callback,
                              void* arg);

private:
    static const u32 THREAD_STACK_SIZE = 0x4000;

    // Current async task
    volatile EState mState;
    // Peer address
    SockAddr mPeer;

    // Active packet jobs
    TList<RecvJob> mRecvJobs;
    TList<SendJob> mSendJobs;

    // Connect callback
    Callback mpConnectCallback;
    void* mpConnectCallbackArg;

    // Accept callback
    AcceptCallback mpAcceptCallback;
    void* mpAcceptCallbackArg;

    // Thread for async socket operation
    static OSThread sSocketThread;
    static bool sSocketThreadCreated;
    static u8 sSocketThreadStack[THREAD_STACK_SIZE];

    // Active async sockets
    static TList<AsyncSocket> sSocketList;
};

} // namespace kiwi

#endif
