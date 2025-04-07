#ifndef LIBKIWI_NET_WEB_SOCKET_H
#define LIBKIWI_NET_WEB_SOCKET_H
#include <libkiwi/k_types.h>
#include <libkiwi/net/kiwiAsyncSocket.h>
#include <libkiwi/prim/kiwiString.h>

namespace kiwi {
//! @addtogroup libkiwi_net
//! @{

/**
 * @brief WebSocket message
 */
struct WebSockMessage {};

/**
 * @brief Web socket (asynchronous)
 */
class WebSocket {
public:
    /**
     * @brief WebSocket result codes
     */
    enum EResult {
        EResult_Success,     //!< OK
        EResult_CantConnect, //!< Can't connect to the server
        EResult_CantUpgrade, //!< Can't upgrade the HTTP connection
        EResult_CantAccept,  //!< Can't authenticate server Accept field
    };

    /**
     * @brief Generic socket operation callback
     *
     * @param result Socket result
     * @param pArg User callback argument
     */
    typedef void (*Callback)(EResult result, void* pArg);

    /**
     * @brief Message receive callback
     * @note If you need the message data after the callback function, make a
     * copy of it before the function returns.
     *
     * @param pMessage Message data
     * @param pArg User callback argument
     */
    typedef void (*MessageCallback)(const WebSockMessage* pMessage, void* pArg);

public:
    /**
     * @brief Constructor
     */
    WebSocket();
    /**
     * @brief Destructor
     */
    ~WebSocket();

    /**
     * @brief Sets the message receive callback function
     *
     * @param pCallback Callback function
     * @param pArg Callback user argument
     */
    void SetMessageCallback(MessageCallback pCallback, void* pArg = nullptr) {
        mpMessageCallback = pCallback;
        mpMessageCallbackArg = pArg;
    }

    /**
     * @brief Connects to the specified server hostname
     *
     * @param rHost Server hostname
     * @param pCallback Connection callback
     * @param pArg Callback user argument
     */
    void Connect(const String& rHost, Callback pCallback, void* pArg = nullptr);
    /**
     * @brief Connects to the specified server address
     *
     * @param rAddr Server address
     * @param pCallback Connection callback
     * @param pArg Callback user argument
     */
    void Connect(const SockAddrAny& rAddr, Callback pCallback,
                 void* pArg = nullptr);

private:
    /**
     * @brief Asynchronous state
     */
    enum EState {
        EState_None,
        EState_Connecting,
        EState_Receiving,
    };

private:
    /**
     * @brief Socket callback function
     *
     * @param result Socket library result
     * @param pArg User callback argument
     */
    static void SocketCallbackFunc(SOResult result, void* pArg);

    /**
     * @brief Request response callback function
     *
     * @param rResp Request response
     * @param pArg Callback user argument
     */
    static void RequestCallback(const HttpResponse& rResp, void* pArg);

private:
    EState mState;          //!< Asynchronous state
    AsyncSocket* mpSocket;  //!< TCP socket
    HttpRequest* mpRequest; //!< HTTP request

    Callback mpConnectCallback; //!< Connect callback
    void* mpConnectCallbackArg; //!< Connect callback user argument

    MessageCallback mpMessageCallback; //!< Message receive callback
    void* mpMessageCallbackArg;        //!< Message callback user argument
};

//! @}
} // namespace kiwi

#endif
