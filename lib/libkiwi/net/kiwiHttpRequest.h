#ifndef LIBKIWI_NET_HTTP_REQUEST_H
#define LIBKIWI_NET_HTTP_REQUEST_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiHashMap.h>
#include <libkiwi/prim/kiwiOptional.h>
#include <libkiwi/prim/kiwiString.h>

namespace kiwi {
//! @addtogroup libkiwi_net
//! @{

// Forward declarations
class SyncSocket;

/**
 * @brief HTTP error
 */
enum EHttpErr {
    EHttpErr_Success,     // OK
    EHttpErr_CantConnect, // Can't connect to the server
    EHttpErr_BadResponse, // Malformed server response
    EHttpErr_TimedOut,    // Connection timed out
    EHttpErr_Closed,      // Connection closed
    EHttpErr_Socket,      // Misc. socket error
    EHttpErr_Usage,       // Invalid usage
};

/**
 * @brief HTTP status code
 */
enum EHttpStatus {
    // TODO: Determine which additional codes are useful here
    EHttpStatus_None = -1,

    // Informational
    EHttpStatus_SwitchProto = 101, // Switching Protocols

    // Successful
    EHttpStatus_OK = 200, // OK
    EHttpStatus_Created,  // Created
    EHttpStatus_Accepted, // Accepted

    // Client error
    EHttpStatus_BadReq = 400, // Bad Request
    EHttpStatus_NotAuth,      // Unauthorized
    EHttpStatus_Payment,      // Payment Required
    EHttpStatus_Forbidden,    // Forbidden
    EHttpStatus_NotFound,     // Not Found
    EHttpStatus_Method,       // Method Not Allowed

    // Server error
    EHttpStatus_ServErr = 500, // Internal Server Error
    EHttpStatus_NotImpl,       // Not Implemented
    EHttpStatus_BadGateway,    // Bad Gateway
    EHttpStatus_ServDown,      // Service Unavailable
};

/**
 * @brief HTTP request response
 */
struct HttpResponse {
    /**
     * @brief Constructor
     */
    HttpResponse()
        : error(EHttpErr_Success), exError(0), status(EHttpStatus_None) {}

    EHttpErr error;              //!< Error code
    s32 exError;                 //!< Internal error code
    EHttpStatus status;          //!< Status code
    TMap<String, String> header; //!< Response header
    String body;                 //!< Response body/payload
};

/**
 * @brief HTTP (1.1) request wrapper
 */
class HttpRequest {
public:
    /**
     * @brief Request method
     */
    enum EMethod {
        // TODO: Should we support more?
        EMethod_GET,
        EMethod_POST,

        EMethod_Max
    };

    /**
     * @brief Request response callback
     *
     * @param rResp Request response
     * @param pArg Callback user argument
     */
    typedef void (*Callback)(const HttpResponse& rResp, void* pArg);

public:
    /**
     * @brief Constructor
     *
     * @param rHost Server hostname
     * @param port Connection port
     */
    HttpRequest(const String& rHost, u16 port = DEFAULT_PORT);

    /**
     * @brief Constructor
     * @note The provided socket will outlive the HTTP request
     *
     * @param pSocket Socket connected to the server
     */
    explicit HttpRequest(SocketBase* pSocket);

    /**
     * @brief Destructor
     */
    ~HttpRequest();

    /**
     * @brief Sends request synchronously
     *
     * @param method Request method
     * @return Server response
     */
    const HttpResponse& Send(EMethod method = EMethod_GET);

    /**
     * @brief Sends request asynchronously
     *
     * @param pCallback Response callback
     * @param pArg Callback user argument
     * @param method Request method
     */
    void SendAsync(Callback pCallback, void* pArg = nullptr,
                   EMethod method = EMethod_GET);

    /**
     * @brief Sets the maximum state duration before timeout
     *
     * @param timeOut Time-out period, in milliseconds
     */
    void SetTimeOut(u32 timeOut) {
        mTimeOut = OS_MSEC_TO_TICKS(timeOut);
    }

    /**
     * @brief Accesses a request header field (if it exists)
     *
     * @param rName Field name
     */
    Optional<String> GetHeaderField(const String& rName) const {
        String* pValue = mHeader.Find(rName);
        return pValue != nullptr ? MakeOptional(*pValue) : kiwi::nullopt;
    }

    /**
     * @brief Adds/updates a request header field
     *
     * @param rName Field name
     * @param rValue Field value
     */
    template <typename T>
    void SetHeaderField(const String& rName, const T& rValue) {
        mHeader.Insert(rName, kiwi::ToString(rValue));
    }

    /**
     * @brief Accesses a URL parameter (if it exists)
     *
     * @param rName Parameter name
     */
    Optional<String> GetParameter(const String& rName) const {
        String* pValue = mParams.Find(rName);
        return pValue != nullptr ? MakeOptional(*pValue) : kiwi::nullopt;
    }

    /**
     * @brief Adds/updates a URL parameter
     *
     * @param rName Parameter name
     * @param rValue Parameter value
     */
    template <typename T>
    void SetParameter(const String& rName, const T& rValue) {
        mParams.Insert(rName, kiwi::ToString(rValue));
    }

    /**
     * @brief Changes the requested resource
     *
     * @param rURI URI value
     */
    void SetURI(const String& rURI) {
        mResource = rURI;
    }

private:
    /**
     * @brief Performs common initialization
     */
    void Init();

    /**
     * @brief Sends request (internal implementation)
     */
    void SendImpl();

    /**
     * @brief Sends request data
     *
     * @return Success
     */
    bool Request();
    /**
     * @brief Receives response data
     *
     * @return Successs
     */
    bool Receive();

private:
    //! Default port for HTTP connections
    static const u16 DEFAULT_PORT = 80;
    //! Default connection timeout, in milliseconds
    static const u32 DEFAULT_TIMEOUT = 10000;
    //! Size of temporary buffer when receiving a response
    static const int TEMP_BUFFER_SIZE = 512;

    //! HTTP request method names
    static const String METHOD_NAMES[EMethod_Max];
    //! HTTP protocol version
    static const String PROTOCOL_VERSION;

private:
    bool mIsSent; //!< Whether this request object has been used

    String mHost; //!< Server host name
    u16 mPort;    //!< Server port

    EMethod mMethod;        //!< Request method
    String mResource;       //!< Requested resource
    HttpResponse mResponse; //!< Server response
    u32 mTimeOut;           //!< Connection timeout

    SocketBase* mpSocket; //!< Connection to server
    bool mIsUserSocket;   //!< Whether the socket is owned by the user

    TMap<String, String> mParams; //!< URL parameters
    TMap<String, String> mHeader; //!< Header fields

    Callback mpCallback; //!< Response callback
    void* mpCallbackArg; //!< Callback user argument
};

//! @}
} // namespace kiwi

#endif
