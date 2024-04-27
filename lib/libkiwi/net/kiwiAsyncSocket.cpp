#include <climits>
#include <libkiwi.h>

namespace kiwi {

/**
 * Async receive operation
 */
class AsyncSocket::RecvJob {
public:
    /**
     * @brief Constructor
     *
     * @param _packet Packet for this job
     * @param _dst Destination address
     * @param[out] _peer Peer address
     * @param _callback Completion callback
     * @param _arg Callback user argument
     */
    RecvJob(Packet* _packet, void* _dst, SockAddr* _peer,
            Callback _callback = NULL, void* _arg = NULL)
        : packet(_packet), dst(_dst), callback(_callback), arg(_arg) {
        K_ASSERT(packet != NULL);
        K_ASSERT(dst != NULL);
    }

    /**
     * @brief Destructor
     */
    ~RecvJob() {
        delete packet;
        packet = NULL;
    }

    /**
     * @brief Check whether the receive is complete
     */
    bool IsComplete() const {
        K_ASSERT(packet != NULL);
        return packet->IsWriteComplete();
    }

    /**
     * @brief Update job
     *
     * @param socket Socket descriptor
     * @return Whether the job is complete
     */
    bool Calc(SOSocket socket) {
        K_ASSERT(packet != NULL);

        // Nothing left to do
        if (IsComplete()) {
            return true;
        }

        // Update
        packet->Recv(socket);
        bool done = IsComplete();

        // Write out data
        if (done) {
            K_ASSERT(dst != NULL);
            std::memcpy(dst, packet->GetContent(), packet->GetContentSize());

            // Write peer information
            if (peer != NULL) {
                *peer = packet->GetPeer();
            }

            // Notify user
            if (callback != NULL) {
                callback(LibSO::GetLastError(), arg);
            }
        }

        return done;
    }

private:
    // Packet associated with this job
    Packet* packet;
    // Original read address
    void* dst;
    // Peer address
    SOSockAddr* peer;

    // Completion callback
    Callback callback;
    void* arg;
};

/**
 * Async send operation
 */
class AsyncSocket::SendJob {
public:
    /**
     * @brief Constructor
     *
     * @param _packet Packet for this job
     * @param _callback Completion callback
     * @param _arg Callback user argument
     */
    SendJob(Packet* _packet, Callback _callback = NULL, void* _arg = NULL)
        : packet(_packet), callback(_callback), arg(_arg) {
        K_ASSERT(packet != NULL);
    }

    /**
     * @brief Destructor
     */
    ~SendJob() {
        delete packet;
        packet = NULL;
    }

    /**
     * @brief Check whether the send is complete
     */
    bool IsComplete() const {
        K_ASSERT(packet != NULL);
        return packet->IsReadComplete();
    }

    /**
     * @brief Update job
     *
     * @param socket Socket descriptor
     * @return Whether the job is complete
     */
    bool Calc(SOSocket socket) {
        K_ASSERT(packet != NULL);

        // Nothing left to do
        if (IsComplete()) {
            return true;
        }

        // Update
        packet->Send(socket);
        bool done = IsComplete();

        // Fire callback
        if (done && callback != NULL) {
            callback(LibSO::GetLastError(), arg);
        }

        return done;
    }

private:
    // Packet associated with this job
    Packet* packet;

    // Completion callback
    Callback callback;
    void* arg;
};

OSThread AsyncSocket::sSocketThread;
bool AsyncSocket::sSocketThreadCreated = false;
u8 AsyncSocket::sSocketThreadStack[THREAD_STACK_SIZE];
TList<AsyncSocket> AsyncSocket::sSocketList;

/**
 * Socket thread function
 */
void* AsyncSocket::ThreadFunc(void* arg) {
#pragma unused(arg)

    // Update all open sockets
    while (true) {
        for (TList<AsyncSocket>::Iterator it = sSocketList.Begin();
             it != sSocketList.End(); it++) {
            K_ASSERT(it->IsOpen());
            it->Calc();
        }
    }

    return NULL;
}

/**
 * Constructor
 *
 * @param family Socket protocol family
 * @param type Socket type
 */
AsyncSocket::AsyncSocket(SOProtoFamily family, SOSockType type)
    : SocketBase(family, type),
      mState(EState_Thinking),
      mpConnectCallback(NULL),
      mpConnectCallbackArg(NULL),
      mpAcceptCallback(NULL),
      mpAcceptCallbackArg(NULL) {
    Initialize();
}

/**
 * Constructor
 *
 * @param socket Socket file descriptor
 * @param type Socket protocol family
 * @param type Socket type
 */
AsyncSocket::AsyncSocket(SOSocket socket, SOProtoFamily family, SOSockType type)
    : SocketBase(socket, family, type),
      mState(EState_Thinking),
      mpConnectCallback(NULL),
      mpConnectCallbackArg(NULL),
      mpAcceptCallback(NULL),
      mpAcceptCallbackArg(NULL) {
    Initialize();
}

/**
 * Prepares socket for async operation
 */
void AsyncSocket::Initialize() {
    // Make socket non-blocking
    bool success = SetBlocking(false);
    K_ASSERT(success);

    // Thread needs to see this socket
    sSocketList.PushBack(this);

    // Thread must exist if there is an open socket
    if (!sSocketThreadCreated) {
        OSCreateThread(&sSocketThread, ThreadFunc, NULL,
                       sSocketThreadStack + sizeof(sSocketThreadStack),
                       sizeof(sSocketThreadStack), OS_PRIORITY_MAX, 0);

        sSocketThreadCreated = true;
        OSResumeThread(&sSocketThread);
    }
}

/**
 * Destructor
 */
AsyncSocket::~AsyncSocket() {
    sSocketList.Remove(this);
}

/**
 * Connects to another socket
 *
 * @param addr Remote address
 * @param callback Connection callback
 * @param arg Callback user argument
 * @return Success
 */
bool AsyncSocket::Connect(const SockAddr& addr, Callback callback, void* arg) {
    K_ASSERT(IsOpen());

    mState = EState_Connecting;
    mPeer = addr;

    mpConnectCallback = callback;
    mpConnectCallbackArg = arg;

    // Connect doesn't actually happen on this thread
    return false;
}

/**
 * Accepts remote connection
 *
 * @param callback Acceptance callback
 * @param arg Callback user argument
 * @return New socket
 */
AsyncSocket* AsyncSocket::Accept(AcceptCallback callback, void* arg) {
    K_ASSERT(IsOpen());

    mState = EState_Accepting;

    mpAcceptCallback = callback;
    mpAcceptCallbackArg = arg;

    // Accept doesn't actually happen on this thread
    return NULL;
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
SOResult AsyncSocket::RecvImpl(void* dst, u32 len, u32& nrecv, SockAddr* addr,
                               Callback callback, void* arg) {
    K_ASSERT(IsOpen());
    K_ASSERT(dst != NULL);
    K_ASSERT(len > 0 && len < ULONG_MAX);
    K_ASSERT_EX(callback != NULL, "Please provide a receive callback");

    // Packet to hold incoming data
    Packet* packet = new Packet(len);
    K_ASSERT(packet != NULL);

    // Asynchronous job
    RecvJob* job = new RecvJob(packet, dst, addr, callback, arg);
    K_ASSERT(job != NULL);
    mRecvJobs.PushBack(job);

    // Receive doesn't actually happen on this thread
    nrecv = 0;
    return SO_EWOULDBLOCK;
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
SOResult AsyncSocket::SendImpl(const void* src, u32 len, u32& nsend,
                               const SockAddr* addr, Callback callback,
                               void* arg) {
    K_ASSERT(IsOpen());
    K_ASSERT(src != NULL);
    K_ASSERT(len > 0 && len < ULONG_MAX);

    // Packet to hold incoming data
    Packet* packet = new Packet(len, addr);
    K_ASSERT(packet != NULL);

    // Store data inside packet
    packet->Write(src, len);

    // Asynchronous job
    SendJob* job = new SendJob(packet, callback, arg);
    K_ASSERT(job != NULL);
    mSendJobs.PushBack(job);

    // Send doesn't actually happen on this thread
    nsend = 0;
    return SO_EWOULDBLOCK;
}

/**
 * Process pending tasks
 */
void AsyncSocket::Calc() {
    s32 result;

    K_ASSERT(IsOpen());

    switch (mState) {
    case EState_Thinking:
        CalcRecv();
        CalcSend();
        break;

    case EState_Connecting:
        result = LibSO::Connect(mHandle, mPeer);

        // Blocking, try again
        if (result == SO_EINPROGRESS || result == SO_EALREADY) {
            break;
        }

        // Connection complete (looking for EISCONN here)
        mState = EState_Thinking;
        mpConnectCallback(result == SO_EISCONN ? SO_SUCCESS
                                               : LibSO::GetLastError(),
                          mpConnectCallbackArg);

        break;

    case EState_Accepting:
        result = LibSO::Accept(mHandle, mPeer);

        // Report non-blocking results
        if (result != SO_EWOULDBLOCK) {
            // Peer connection
            AsyncSocket* socket = NULL;

            // Result code is the peer descriptor
            if (result >= 0) {
                AsyncSocket* socket = new AsyncSocket(result, mFamily, mType);
                K_ASSERT(socket != NULL);
            }

            mState = EState_Thinking;
            mpAcceptCallback(LibSO::GetLastError(), socket, mPeer,
                             mpAcceptCallbackArg);
        }
        break;
    }
}

/**
 * Receives packet data over socket
 */
void AsyncSocket::CalcRecv() {
    K_ASSERT(IsOpen());

    // Nothing to do
    if (mRecvJobs.Empty()) {
        return;
    }

    // Find next incomplete job (FIFO)
    RecvJob& job = mRecvJobs.Front();
    K_ASSERT_EX(!job.IsComplete(), "Completed job should be removed");

    // Attempt to complete job
    if (job.Calc(mHandle)) {
        // Remove from queue
        mRecvJobs.PopFront();
        delete &job;
    }
}

/**
 * Sends packet data over socket
 */
void AsyncSocket::CalcSend() {
    K_ASSERT(IsOpen());

    // Nothing to do
    if (mSendJobs.Empty()) {
        return;
    }

    // Find next incomplete job (FIFO)
    SendJob& job = mSendJobs.Front();
    K_ASSERT_EX(!job.IsComplete(), "Completed job should be removed");

    // Attempt to complete job
    if (job.Calc(mHandle)) {
        // Remove from queue
        mSendJobs.PopFront();
        delete &job;
    }
}

} // namespace kiwi
