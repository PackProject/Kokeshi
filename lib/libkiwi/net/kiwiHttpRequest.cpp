#include <libkiwi.h>

namespace kiwi {

typedef TMap<String, String>::ConstIterator ParamIterator;
typedef TMap<String, String>::ConstIterator HeaderIterator;

/**
 * @brief HTTP request method names
 */
const char* HttpRequest::sMethodNames[EMethod_Max] = {"GET", "POST"};
/**
 * @brief HTTP protocol version
 */
const char* HttpRequest::sProtocolVer = "1.1";

/**
 * @brief Constructor
 *
 * @param rHost Server hostname
 */
HttpRequest::HttpRequest(const String& rHost)
    : mHostName(rHost),
      mURI("/"),
      mpSocket(nullptr),
      mTimeOut(OS_MSEC_TO_TICKS(scDefaultTimeOut)),
      mpResponseCallback(nullptr),
      mpResponseCallbackArg(nullptr) {
    mpSocket = new SyncSocket(SO_PF_INET, SO_SOCK_STREAM);
    K_ASSERT(mpSocket != nullptr);

    // Need non-blocking so timeout can be enforced
    bool success = mpSocket->SetBlocking(false);
    K_ASSERT(success);

    // Bind to any local port
    success = mpSocket->Bind();
    K_ASSERT(success);

    // Hostname required by HTTP 1.1
    mHeader["Host"] = rHost;
    // Identify libkiwi requests by user agent
    mHeader["User-Agent"] = "libkiwi";
}

/**
 * @brief Sends request synchronously
 *
 * @param method Request method
 * @return Server response
 */
const HttpResponse& HttpRequest::Send(EMethod method) {
    K_ASSERT(method < EMethod_Max);
    K_ASSERT(mpSocket != nullptr);

    mMethod = method;
    mpResponseCallback = nullptr;
    mpResponseCallbackArg = nullptr;

    // Call on this thread
    SendImpl();
    return mResponse;
}

/**
 * @brief Sends request asynchronously
 *
 * @param pCallback Response callback
 * @param pArg Callback user argument
 * @param method Request method
 */
void HttpRequest::SendAsync(ResponseCallback pCallback, void* pArg,
                            EMethod method) {
    K_ASSERT_EX(pCallback != nullptr, "You will lose the reponse!");
    K_ASSERT(method < EMethod_Max);
    K_ASSERT(mpSocket != nullptr);

    mMethod = method;
    mpResponseCallback = pCallback;
    mpResponseCallbackArg = pArg;

    // Call on new thread
    Thread t(SendImpl, *this);
}

/**
 * @brief Sends request (internal implementation)
 */
void HttpRequest::SendImpl() {
    K_ASSERT(mMethod < EMethod_Max);
    K_ASSERT(mpSocket != nullptr);

    // HTTP connections use port 80
    SockAddr4 addr(mHostName, 80);

    Watch w;
    w.Start();

    // Establish connection with server
    while (true) {
        // Successful connection
        if (mpSocket->Connect(addr)) {
            (void)Request();
            (void)Receive();
            break;
        }

        // Timeout while connecting
        if (w.Elapsed() >= mTimeOut) {
            mResponse.error = EHttpErr_CantConnect;
            break;
        }
    }

    // User callback
    if (mpResponseCallback != nullptr) {
        mpResponseCallback(mResponse, mpResponseCallbackArg);
    }

#ifndef NDEBUG
    // Signal to destructor
    mpResponseCallback = nullptr;
#endif
}

/**
 * @brief Sends request data
 *
 * @return Success
 */
bool HttpRequest::Request() {
    K_ASSERT(mMethod < EMethod_Max);
    K_ASSERT(mpSocket != nullptr);

    // Build URI & URL parameter string
    String request = mURI;
    for (ParamIterator it = mParams.Begin(); it != mParams.End(); ++it) {
        // Parameters delimited by ampersand
        String fmt = it == mParams.Begin() ? "?%s=%s" : "&%s=%s";
        request += Format(fmt, it.Key().CStr(), it.Value().CStr());
    }

    // Build request line
    request = Format("%s %s HTTP/%s\n", sMethodNames[mMethod], request.CStr(),
                     sProtocolVer);

    // Build header fields
    for (HeaderIterator it = mHeader.Begin(); it != mHeader.End(); ++it) {
        request += Format("%s: %s\n", it.Key().CStr(), it.Value().CStr());
    }

    // Request ends with double-line
    request += "\n";

    // Send request data
    Optional<u32> sent = mpSocket->Send(request);
    return sent && *sent == request.Length();
}

/**
 * @brief Receives response data
 *
 * @return Successs
 */
bool HttpRequest::Receive() {
    K_ASSERT(mMethod < EMethod_Max);
    K_ASSERT(mpSocket != nullptr);

    // Beginning timestamp
    Watch w;
    w.Start();

    /**
     *
     * Receive response headers
     *
     */

    // Need non-blocking because we greedily receive data
    bool success = mpSocket->SetBlocking(false);
    K_ASSERT(success);

    // Read header string (ends in double newline)
    String work = "";
    size_t end = String::npos;
    while (end == String::npos) {
        char buffer[512 + 1] = "";
        Optional<u32> nrecv = mpSocket->RecvBytes(buffer, sizeof(buffer) - 1);

        // Check for error
        if (!nrecv) {
            mResponse.error = EHttpErr_Socket;
            return false;
        }

        // Continue to append data
        if (nrecv) {
            work += buffer;
            end = work.Find("\r\n\r\n");

            // Server has terminated the connection
            if (*nrecv == 0 && LibSO::GetLastError() != SO_EWOULDBLOCK) {
                mResponse.error = EHttpErr_Closed;
                return false;
            }
        }

        // Timeout check
        if (w.Elapsed() >= mTimeOut) {
            mResponse.error = EHttpErr_TimedOut;
            return false;
        }
    }

    if (end == String::npos) {
        mResponse.error = EHttpErr_BadResponse;
        return false;
    }

    // Point index at end of sequence instead of start
    end += sizeof("\r\n\r\n") - 1;
    String headers = work.SubStr(0, end);

    /**
     *
     * Build header dictionary
     *
     */

    // Must at least have one line (status code)
    TVector<String> lines = headers.Split("\r\n");
    if (lines.Size() < 1) {
        mResponse.error = EHttpErr_BadResponse;
        return false;
    }

    // Extract status code
    K_ASSERT(lines[0].StartsWith("HTTP/1.1"));
    int num = std::sscanf(lines[0], "HTTP/1.1 %d", &mResponse.status);
    if (num != 1) {
        mResponse.error = EHttpErr_BadResponse;
        return false;
    }

    // Other lines contain key/value pairs
    for (int i = 1; i < lines.Size(); i++) {
        // Use Find over Split in case the value also contains a colon
        u32 pos = lines[i].Find(": ");
        u32 after = pos + sizeof(": ") - 1;

        // Malformed line (or part of \r\n\r\n ending)
        if (pos == String::npos) {
            // If this isn't one of the trailing newlines, we have a problem
            if (lines[i] != "") {
                mResponse.error = EHttpErr_BadResponse;
                return false;
            }

            continue;
        }

        // Create key/value pair
        String key = lines[i].SubStr(0, pos);
        String value = lines[i].SubStr(after);
        mResponse.header.Insert(key, value);
    }

    /**
     *
     * Receive response body
     *
     */

    // If we were given the length, we can be 100% sure
    Optional<u32> len;
    if (mResponse.header.Contains("Content-Length")) {
        len = ksl::strtoul(*mResponse.header.Find("Content-Length"));
    }

    // We may have read *some* of it earlier
    if (end != work.Length()) {
        mResponse.body = work.SubStr(end);
    }

    // Receive the rest of the body
    while (true) {
        char buffer[512 + 1] = "";
        Optional<u32> nrecv = mpSocket->RecvBytes(buffer, sizeof(buffer) - 1);

        // Check for error
        if (!nrecv) {
            mResponse.error = EHttpErr_Socket;
            return false;
        }

        // Continue appending data
        if (nrecv) {
            mResponse.body += buffer;

            // Server is likely done and has terminated the connection
            if (*nrecv == 0 && LibSO::GetLastError() != SO_EWOULDBLOCK) {
                // This is only okay if we've read enough of the body
                if (!len || mResponse.body.Length() >= *len) {
                    break;
                }

                mResponse.error = EHttpErr_Closed;
                return false;
            }
        }

        // Timeout check
        if (w.Elapsed() >= mTimeOut) {
            // May be the only way to end the body, so not a failure
            break;
        }
    };

    mResponse.error = EHttpErr_Success;
    return true;
}

} // namespace kiwi
