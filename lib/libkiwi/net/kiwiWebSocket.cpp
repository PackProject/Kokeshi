#include <libkiwi.h>

namespace kiwi {
namespace {

/**
 * @brief Frame opcode
 */
enum EOpcode {
    EOpcode_Continuation, //!< Intermediate frame of a fragmented message
    EOpcode_Text,         //!< UTF-8 encoded application text
    EOpcode_Binary,       //!< Application binary data
    EOpcode_Close = 8,    //!< Close frame
    EOpcode_Ping,         //!< Heartbeat/latency (first)
    EOpcode_Pong,         //!< Heartbeat/latency (second)
};

/**
 * @brief Data frame
 */
struct Frame {
    /* 0x00:0 */ u8 fin : 1;    //!< Final frame of a message
    /* 0x00:1 */ u8 rsv1 : 1;   //! Reserved, must be zero
    /* 0x00:2 */ u8 rsv2 : 1;   //! Reserved, must be zero
    /* 0x00:3 */ u8 rsv3 : 1;   //! Reserved, must be zero
    /* 0x00:4 */ u8 opcode : 4; //! Frame opcode
    /* 0x01:0 */ u8 mask : 1;   //! Frame is masked (and masking key is present)
    /* 0x01:1 */ u8 length : 7; //! Payload length

    union {
        //! Length is 7 + the following 16 bits
        union {
            struct {
                u16 length;
                u8 payload[1];
            } unmasked;

            struct {
                u16 length;
                u32 masking;
                u8 payload[1];
            } masked;
        } len16;

        //! Length is 7 + the following 64 bits
        union {
            struct {
                u64 length;
                u8 payload[1];
            } unmasked;

            struct {
                u64 length;
                u32 masking;
                u8 payload[1];
            } masked;
        } len64;

        union {
            struct {
                u8 payload[1];
            } unmasked;

            struct {
                u32 masking;
                u8 payload[1];
            } masked;
        };
    };
};

/**
 * @brief Websocket protocol version
 */
static const int WEBSOCKET_VERSION = 13;

/**
 * @brief Server accept key constant
 */
static const char* WEBSOCKET_KEY_CONST = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

/**
 * @brief Generates a random 16-byte key for the WebSocket protocol
 */
String GenerateKey() {
    Random r;
    u8 key[16];

    for (int i = 0; i < LENGTHOF(key); i++) {
        key[i] = static_cast<u8>(r.NextU32());
    }

    return B64Encode(key, LENGTHOF(key));
}

/**
 * @brief Generates the expected 'Sec-WebSocket-Accept' value from the given key
 *
 * @param rKey Key ('Sec-WebSocket-Key')
 */
String GenerateAccept(const String& rKey) {
    String expected = rKey + WEBSOCKET_KEY_CONST;
    expected = SHA1Hash(expected, expected.Length());
    expected = B64Encode(expected, expected.Length());
    return expected;
}

} // namespace

/**
 * @brief Constructor
 */
WebSocket::WebSocket()
    : mState(EState_None),
      mpSocket(nullptr),
      mpRequest(nullptr),
      mpConnectCallback(nullptr),
      mpConnectCallbackArg(nullptr),
      mpMessageCallback(nullptr),
      mpMessageCallbackArg(nullptr) {}

/**
 * @brief Destructor
 */
WebSocket::~WebSocket() {
    // TODO: Will we need a close function?
    delete mpSocket;
    mpSocket = nullptr;

    delete mpRequest;
    mpRequest = nullptr;
}

/**
 * @brief Connects to the specified server hostname
 *
 * @param rHost Server hostname
 * @param pCallback Connection callback
 * @param pArg Callback user argument
 */
void WebSocket::Connect(const String& rHost, Callback pCallback, void* pArg) {
    // Assume port 80
    SockAddr4 addr;
    if (LibSO::ResolveHostName(addr, rHost, "80")) {
        Connect(addr, pCallback, pArg);
        return;
    }

    if (pCallback != nullptr) {
        pCallback(EResult_CantConnect, pArg);
    }
}

/**
 * @brief Connects to the specified server address
 *
 * @param rAddr Server address
 * @param pCallback Connection callback
 * @param pArg Callback user argument
 */
void WebSocket::Connect(const SockAddrAny& rAddr, Callback pCallback,
                        void* pArg) {
    K_ASSERT(rAddr.IsValid());
    K_WARN_EX(rAddr.port != 80,
              "WebSocket connections are expected to use port 80.");

    K_WARN_EX(pCallback == nullptr,
              "You probably want to use a callback function.");

    // Close existing connection
    if (mpSocket != nullptr) {
        // TODO: Will we need a close function?
        delete mpSocket;
    }

    SOProtoFamily family =
        rAddr.len == sizeof(SOSockAddrIn) ? SO_PF_INET : SO_PF_INET6;

    // WebSocket uses TCP
    mpSocket = new AsyncSocket(family, SO_SOCK_STREAM);
    K_ASSERT(mpSocket != nullptr);

    K_LOG("Connecting...\n");

    mState = EState_Connecting;
    mpConnectCallback = pCallback;
    mpConnectCallbackArg = pArg;
    mpSocket->Connect(rAddr, SocketCallbackFunc, this);
}

/**
 * @brief Socket callback function
 *
 * @param result Socket library result
 * @param pArg User callback argument
 */
void WebSocket::SocketCallbackFunc(SOResult result, void* pArg) {
    K_ASSERT(pArg != nullptr);

    K_LOG("SocketCallbackFunc...\n");

    // User argument is this object
    WebSocket* p = static_cast<WebSocket*>(pArg);

    // Dispatch state machine
    switch (p->mState) {
    case EState_Connecting: {
        // Connection to the server failed
        if (result != SO_SUCCESS && result != SO_EISCONN) {
            if (p->mpConnectCallback != nullptr) {
                p->mpConnectCallback(EResult_CantConnect,
                                     p->mpConnectCallbackArg);
            }

            p->mState = EState_None;
            return;
        }

        K_LOG("Connected, upgrading...\n");

        // Request connection upgrade to WebSocket
        p->mpRequest = new HttpRequest(p->mpSocket);
        K_ASSERT(p->mpRequest != nullptr);

        p->mpRequest->SetHeaderField("Connection", "Upgrade");
        p->mpRequest->SetHeaderField("Upgrade", "websocket");
        p->mpRequest->SetHeaderField("Sec-WebSocket-Version",
                                     WEBSOCKET_VERSION);
        p->mpRequest->SetHeaderField("Sec-WebSocket-Key", GenerateKey());
        p->mpRequest->SendAsync(RequestCallback, p);
        break;
    }

    case EState_Receiving: {
        // TODO: Receive frame and then dispatch to proper state
        break;
    }
    }
}

/**
 * @brief Request response callback
 *
 * @param rResp Request response
 * @param pArg Callback user argument
 */
void WebSocket::RequestCallback(const HttpResponse& rResp, void* pArg) {
    K_ASSERT(pArg != nullptr);

    // User argument is this object
    WebSocket* p = static_cast<WebSocket*>(pArg);
    K_ASSERT(p->mpRequest != nullptr);

    // Validate upgrade-related fields
    if (rResp.error != EHttpErr_Success ||
        rResp.status != EHttpStatus_SwitchProto ||
        rResp.header.Get("Connection") != "Upgrade" ||
        rResp.header.Get("Upgrade") != "websocket") {

        if (p->mpConnectCallback != nullptr) {
            p->mpConnectCallback(EResult_CantUpgrade, p->mpConnectCallbackArg);
        }

        p->mState = EState_None;
        return;
    }

    // Validate server secret
    if (rResp.header.Get("Sec-WebSocket-Accept") !=
        GenerateAccept(*p->mpRequest->GetHeaderField("Sec-WebSocket-Key"))) {

        if (p->mpConnectCallback != nullptr) {
            p->mpConnectCallback(EResult_CantAccept, p->mpConnectCallbackArg);
        }

        p->mState = EState_None;
        return;
    }

    if (p->mpConnectCallback != nullptr) {
        p->mpConnectCallback(EResult_Success, p->mpConnectCallbackArg);
    }

    p->mState = EState_Receiving;
    SocketCallbackFunc(SO_SUCCESS, p);
}

} // namespace kiwi
