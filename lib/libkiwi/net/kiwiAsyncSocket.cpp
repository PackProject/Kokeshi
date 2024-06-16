#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Socket manager thread
 */
OSThread AsyncSocket::sSocketThread;

/**
 * @brief Thread guard
 */
bool AsyncSocket::sSocketThreadCreated = false;

/**
 * @brief Thread stack
 */
u8 AsyncSocket::sSocketThreadStack[scThreadStackSize];

/**
 * @brief Open async sockets
 */
TList<AsyncSocket> AsyncSocket::sSocketList;

/**
 * @brief Async receive operation
 */
class AsyncSocket::RecvJob {
public:
    /**
     * @brief Constructor
     *
     * @param pPacket Packet for this job
     * @param pDst Destination address
     * @param[out] pPeer Peer address
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     */
    RecvJob(Packet* pPacket, void* pDst, SockAddrAny* pPeer,
            Callback pCallback = nullptr, void* pArg = nullptr)
        : mpPacket(pPacket),
          mpDst(pDst),
          mpPeer(pPeer),
          mpCallback(pCallback),
          mpArg(pArg) {
        K_ASSERT(mpPacket != nullptr);
        K_ASSERT(mpDst != nullptr);
    }

    /**
     * @brief Destructor
     */
    ~RecvJob() {
        delete mpPacket;
    }

    /**
     * @brief Tests whether the receive operation is complete
     */
    bool IsComplete() const {
        K_ASSERT(mpPacket != nullptr);
        return mpPacket->IsWriteComplete();
    }

    /**
     * @brief Updates job using the specified socket
     *
     * @param socket Socket descriptor
     * @return Whether the job is complete
     */
    bool Calc(SOSocket socket) {
        K_ASSERT(mpPacket != nullptr);
        K_ASSERT(socket >= 0);

        // Nothing left to do
        if (IsComplete()) {
            return true;
        }

        // Update
        mpPacket->Recv(socket);
        bool done = IsComplete();

        // Write out data
        if (done) {
            K_ASSERT(mpDst != nullptr);
            std::memcpy(mpDst, mpPacket->GetContent(),
                        mpPacket->GetContentSize());

            // Write peer information
            if (mpPeer != nullptr) {
                *mpPeer = mpPacket->GetPeer();
            }

            // Notify user
            if (mpCallback != nullptr) {
                mpCallback(LibSO::GetLastError(), mpArg);
            }
        }

        return done;
    }

private:
    Packet* mpPacket;   // Packet to complete
    void* mpDst;        // Where to store packet data
    SOSockAddr* mpPeer; // Where to store peer address

    Callback mpCallback; // Completion callback
    void* mpArg;         // Completion callback user argument
};

/**
 * @brief Async send operation
 */
class AsyncSocket::SendJob {
public:
    /**
     * @brief Constructor
     *
     * @param pPacket Packet for this job
     * @param pCallback Completion callback
     * @param pArg Callback user argument
     */
    SendJob(Packet* pPacket, Callback pCallback = nullptr, void* pArg = nullptr)
        : mpPacket(pPacket), mpCallback(pCallback), mpArg(pArg) {
        K_ASSERT(mpPacket != nullptr);
    }

    /**
     * @brief Destructor
     */
    ~SendJob() {
        delete mpPacket;
    }

    /**
     * @brief Tests whether the send operation is complete
     */
    bool IsComplete() const {
        K_ASSERT(mpPacket != nullptr);
        return mpPacket->IsReadComplete();
    }

    /**
     * @brief Updates job using the specified socket
     *
     * @param socket Socket descriptor
     * @return Whether the job is complete
     */
    bool Calc(SOSocket socket) {
        K_ASSERT(mpPacket != nullptr);
        K_ASSERT(socket >= 0);

        // Nothing left to do
        if (IsComplete()) {
            return true;
        }

        // Update
        mpPacket->Send(socket);
        bool done = IsComplete();

        // Fire callback
        if (done && mpCallback != nullptr) {
            mpCallback(LibSO::GetLastError(), mpArg);
        }

        return done;
    }

private:
    Packet* mpPacket; // Packet to complete

    Callback mpCallback; // Completion callback
    void* mpArg;         // Completion callback user argument
};

/**
 * @brief Socket thread function
 *
 * @param pArg Thread function argument
 */
void* AsyncSocket::ThreadFunc(void* pArg) {
#pragma unused(pArg)

    // Update all open sockets
    while (true) {
        for (TList<AsyncSocket>::Iterator it = sSocketList.Begin();
             it != sSocketList.End(); it++) {
            K_ASSERT(it->IsOpen());
            it->Calc();
        }
    }

    return nullptr;
}

/**
 * @brief Constructor
 *
 * @param family Socket protocol family
 * @param type Socket type
 */
AsyncSocket::AsyncSocket(SOProtoFamily family, SOSockType type)
    : SocketBase(family, type),
      mState(EState_Thinking),
      mpConnectCallback(nullptr),
      mpConnectCallbackArg(nullptr),
      mpAcceptCallback(nullptr),
      mpAcceptCallbackArg(nullptr) {
    Initialize();
}

/**
 * @brief Constructor
 *
 * @param socket Socket file descriptor
 * @param type Socket protocol family
 * @param type Socket type
 */
AsyncSocket::AsyncSocket(SOSocket socket, SOProtoFamily family, SOSockType type)
    : SocketBase(socket, family, type),
      mState(EState_Thinking),
      mpConnectCallback(nullptr),
      mpConnectCallbackArg(nullptr),
      mpAcceptCallback(nullptr),
      mpAcceptCallbackArg(nullptr) {
    Initialize();
}

/**
 * @brief Prepares socket for async operation
 */
void AsyncSocket::Initialize() {
    // Make socket non-blocking
    bool success = SetBlocking(false);
    K_ASSERT(success);

    // Thread needs to see this socket
    sSocketList.PushBack(this);

    // Thread must exist if there is an open socket
    if (!sSocketThreadCreated) {
        OSCreateThread(&sSocketThread, ThreadFunc, nullptr,
                       sSocketThreadStack + sizeof(sSocketThreadStack),
                       sizeof(sSocketThreadStack), OS_PRIORITY_MAX, 0);

        sSocketThreadCreated = true;
        OSResumeThread(&sSocketThread);
    }
}

/**
 * @brief Connects to a peer
 *
 * @param rAddr Remote address
 * @param pCallback Connection callback
 * @param pArg Callback user argument
 * @return Success
 */
bool AsyncSocket::Connect(const SockAddrAny& rAddr, Callback pCallback,
                          void* pArg) {
    K_ASSERT(IsOpen());

    mState = EState_Connecting;
    mPeer = rAddr;

    mpConnectCallback = pCallback;
    mpConnectCallbackArg = pArg;

    // Connect doesn't actually happen on this thread
    return false;
}

/**
 * @brief Accepts a peer connection over a new socket
 *
 * @param pCallback Acceptance callback
 * @param pArg Callback user argument
 * @return New socket
 */
AsyncSocket* AsyncSocket::Accept(AcceptCallback pCallback, void* pArg) {
    K_ASSERT(IsOpen());

    mState = EState_Accepting;

    mpAcceptCallback = pCallback;
    mpAcceptCallbackArg = pArg;

    // Accept doesn't actually happen on this thread
    return nullptr;
}

/**
 * @brief Processes pending socket tasks
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
            AsyncSocket* pSocket = nullptr;

            // Result code is the peer descriptor
            if (result >= 0) {
                pSocket = new AsyncSocket(result, mFamily, mType);
                K_ASSERT(pSocket != nullptr);
            }

            mState = EState_Thinking;
            mpAcceptCallback(LibSO::GetLastError(), pSocket, mPeer,
                             mpAcceptCallbackArg);
        }
        break;
    }
}

/**
 * @brief Receives packet data over socket
 */
void AsyncSocket::CalcRecv() {
    K_ASSERT(IsOpen());

    // Nothing to do
    if (mRecvJobs.Empty()) {
        return;
    }

    // Find next incomplete job (FIFO)
    RecvJob& rJob = mRecvJobs.Front();
    K_ASSERT_EX(!rJob.IsComplete(), "Completed job should be removed");

    // Attempt to complete job
    if (rJob.Calc(mHandle)) {
        // Remove from queue
        mRecvJobs.PopFront();
        delete &rJob;
    }
}

/**
 * @brief Sends packet data over socket
 */
void AsyncSocket::CalcSend() {
    K_ASSERT(IsOpen());

    // Nothing to do
    if (mSendJobs.Empty()) {
        return;
    }

    // Find next incomplete job (FIFO)
    SendJob& rJob = mSendJobs.Front();
    K_ASSERT_EX(!rJob.IsComplete(), "Completed job should be removed");

    // Attempt to complete job
    if (rJob.Calc(mHandle)) {
        // Remove from queue
        mSendJobs.PopFront();
        delete &rJob;
    }
}

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
SOResult AsyncSocket::RecvImpl(void* pDst, u32 len, u32& rRecv,
                               SockAddrAny* pAddr, Callback pCallback,
                               void* pArg) {
    K_ASSERT(IsOpen());
    K_ASSERT(pDst != nullptr);
    K_ASSERT_EX(!PtrUtil::IsStack(pDst), "Don't use stack memory for async");

    // Packet to hold incoming data
    Packet* pPacket = new Packet(len);
    K_ASSERT(pPacket != nullptr);

    // Asynchronous job
    RecvJob* pJob = new RecvJob(pPacket, pDst, pAddr, pCallback, pArg);
    K_ASSERT(pJob != nullptr);
    mRecvJobs.PushBack(pJob);

    // Receive doesn't actually happen on this thread
    rRecv = 0;
    return SO_EWOULDBLOCK;
}

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
SOResult AsyncSocket::SendImpl(const void* pSrc, u32 len, u32& rSend,
                               const SockAddrAny* pAddr, Callback pCallback,
                               void* pArg) {
    K_ASSERT(IsOpen());
    K_ASSERT(pSrc != nullptr);
    K_ASSERT_EX(!PtrUtil::IsStack(pSrc), "Don't use stack memory for async");

    // Packet to hold incoming data
    Packet* pPacket = new Packet(len, pAddr);
    K_ASSERT(pPacket != nullptr);

    // Store data inside packet
    pPacket->Write(pSrc, len);

    // Asynchronous job
    SendJob* pJob = new SendJob(pPacket, pCallback, pArg);
    K_ASSERT(pJob != nullptr);
    mSendJobs.PushBack(pJob);

    // Send doesn't actually happen on this thread
    rSend = 0;
    return SO_EWOULDBLOCK;
}

} // namespace kiwi
