#ifndef LIBKIWI_NET_ASYNC_SOCKET_H
#define LIBKIWI_NET_ASYNC_SOCKET_H
#include <libkiwi/core/kiwiLinkList.hpp>
#include <libkiwi/net/kiwiPacket.hpp>
#include <libkiwi/net/kiwiSocketBase.hpp>
#include <revolution/OS.h>
#include <types.h>

namespace kiwi {

/**
 * Asynchronous (non-blocking) IOS socket wrapper
 */
class AsyncSocket : public SocketBase {
public:
    /**
     * Socket connect callback
     *
     * @param s32 SOConnect result
     * @param arg User callback argument
     */
    typedef void (*ConnectCallback)(s32 result, void* arg);

    /**
     * Socket accept callback
     *
     * @param socket Peer socket object
     * @param peer Peer address
     * @param arg User callback argument
     */
    typedef void (*AcceptCallback)(SocketBase* socket, const SOSockAddr& peer,
                                   void* arg);

private:
    /**
     * Socket async task
     */
    enum Task { Task_None, Task_Connecting, Task_Accepting };

public:
    AsyncSocket(SOProtoFamily family, SOSockType type);
    virtual ~AsyncSocket();

    virtual bool Connect(const SOSockAddr& addr);
    virtual AsyncSocket* Accept();

    /**
     * Set async connect callback
     *
     * @param callback Callback function
     * @param arg Callback function argument
     */
    void SetConnectCallback(ConnectCallback callback, void* arg = NULL) {
        mpConnectCallback = callback;
        mpConnectCallbackArg = arg;
    }

    /**
     * Set async accept callback
     *
     * @param callback Callback function
     * @param arg Callback function argument
     */
    void SetAcceptCallback(AcceptCallback callback, void* arg = NULL) {
        mpAcceptCallback = callback;
        mpAcceptCallbackArg = arg;
    }

private:
    static void* ThreadFunc(void* arg);

    AsyncSocket(SOSocket socket, SOProtoFamily family, SOSockType type);

    virtual s32 RecieveImpl(void* dst, std::size_t len, SOSockAddr* addr);
    virtual s32 SendImpl(const void* src, std::size_t len,
                         const SOSockAddr* addr);

    void CalcRecv();
    void CalcSend();

    Packet* FindPacketForRecv();

private:
    // Socket thread stack size
    static const std::size_t scSocketThreadStackSize = 0x4000;

    // Socket async task
    Task mTask;
    // Socket peer address
    SOSockAddr mPeer;

    // Packets for receive operations
    TList<Packet> mRecvPackets;
    // Packets for receive operations
    TList<Packet> mSendPackets;

    // Socket connect callback
    ConnectCallback mpConnectCallback;
    // Socket connect callback argument
    void* mpConnectCallbackArg;

    // Socket accept callback
    AcceptCallback mpAcceptCallback;
    // Socket accept callback argument
    void* mpAcceptCallbackArg;

    // Thread for async socket operation
    static OSThread sSocketThread;
    // Async socket thread stack
    static u8 sSocketThreadStack[scSocketThreadStackSize];
    // List of all active async sockets
    static TList<AsyncSocket> sSocketList;
};

} // namespace kiwi

#endif
