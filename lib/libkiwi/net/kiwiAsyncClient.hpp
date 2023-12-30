#ifndef LIBKIWI_NET_TCP_CLIENT_H
#define LIBKIWI_NET_TCP_CLIENT_H
#include <libkiwi/net/kiwiAsyncSocket.hpp>
#include <types.h>

namespace kiwi {

/**
 * Async networking client
 */
class AsyncClient : private AsyncSocket {
public:
    /**
     * Disconnection from server callback
     *
     * @param arg User callback argument
     */
    typedef void (*DisconnectCallback)(void* arg);

    /**
     * Client message receive callback
     *
     * @param msg Message data
     * @param len Message data length
     * @param arg User callback argument
     */
    typedef void (*ReceiveCallback)(const u8* msg, std::size_t len, void* arg);

public:
    AsyncClient(SOProtoFamily family, SOSockType type);
    ~AsyncClient();

    /**
     * Set server disconnection callback
     *
     * @param callback Callback function
     * @param arg Callback function argument
     */
    void SetDisconnectCallback(DisconnectCallback callback, void* arg = NULL) {
        mpDisconnectCallback = callback;
        mpDisconnectCallbackArg = arg;
    }

    /**
     * Set message receive callback
     *
     * @param callback Callback function
     * @param arg Callback function argument
     */
    void SetReceiveCallback(ReceiveCallback callback, void* arg = NULL) {
        mpReceiveCallback = callback;
        mpReceiveCallbackArg = arg;
    }

    // Only expose supported socket methods
    using AsyncSocket::Connect;
    using AsyncSocket::SetConnectCallback;

    using AsyncSocket::Send;
    using AsyncSocket::SendBytes;

    using AsyncSocket::Close;

private:
    static void* ThreadFuncTCP(void* arg);
    static void* ThreadFuncUDP(void* arg);

private:
    // Client thread stack size
    static const std::size_t scThreadStackSize = 0x4000;

    // Server address
    SOSockAddr mServer;

    // Buffer for received data
    u8* mpMsgBuffer;

    // Message receive callback
    ReceiveCallback mpReceiveCallback;
    // Message receive callback argument
    void* mpReceiveCallbackArg;

    // Server disconnection callback
    DisconnectCallback mpDisconnectCallback;
    // Server disconnection callback argument
    void* mpDisconnectCallbackArg;

    // Thread to manage client socket
    OSThread mThread;
    // Client thread stack
    u8 mThreadStack[scThreadStackSize];
};

} // namespace kiwi

#endif
