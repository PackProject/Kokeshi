#include <libkiwi.h>

namespace kiwi {

OSThread AsyncSocket::sSocketThread;
u8 AsyncSocket::sSocketThreadStack[scSocketThreadStackSize];
TList<AsyncSocket> AsyncSocket::sSocketList;

/**
 * Socket thread function
 */
void* AsyncSocket::ThreadFunc(void* arg) {
#pragma unused(arg)
    s32 result;

    // Operate all open sockets
    while (true) {
        for (TList<AsyncSocket>::Iterator it = sSocketList.Begin();
             it != sSocketList.End(); it++) {
            K_ASSERT_EX(it->IsOpen(),
                        "Closed socket shouldn't be in the active list");

            switch (it->mTask) {
            case Task_None:
                it->CalcRecv();
                it->CalcSend();
                break;

            case Task_Connecting:
                result = LibSO::Connect(it->mHandle, it->mPeer);

                // Ignore async return codes
                if (result != SO_EWOULDBLOCK || result != SO_EINPROGRESS) {
                    // Done, report result to user
                    it->mTask = Task_None;
                    it->mpConnectCallback(result, it->mpConnectCallbackArg);
                }
                break;

            case Task_Accepting:
                result = LibSO::Accept(it->mHandle, it->mPeer);

                // Ignore async return codes
                if (result != SO_EWOULDBLOCK) {
                    // Peer socket
                    AsyncSocket* socket =
                        result >= 0
                            ? new AsyncSocket(result, it->mFamily, it->mType)
                            : NULL;

                    // If there was no error, confirm successful allocation
                    K_ASSERT(socket != NULL || result < 0);

                    // Done, report result to user
                    it->mTask = Task_None;
                    it->mpAcceptCallback(socket, it->mPeer,
                                         it->mpAcceptCallbackArg);
                }
                break;
            }
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
      mTask(Task_None),
      mpConnectCallback(NULL),
      mpConnectCallbackArg(NULL),
      mpAcceptCallback(NULL),
      mpAcceptCallbackArg(NULL) {
    // Make socket non-blocking
    bool success = SetBlocking(false);
    K_ASSERT(success);

    // Add socket to global list
    sSocketList.PushBack(this);

    OSCreateThread(&sSocketThread, ThreadFunc, NULL,
                   sSocketThreadStack + sizeof(sSocketThreadStack),
                   sizeof(sSocketThreadStack), OS_PRIORITY_MAX, 0);
    OSResumeThread(&sSocketThread);
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
      mTask(Task_None),
      mpConnectCallback(NULL),
      mpConnectCallbackArg(NULL),
      mpAcceptCallback(NULL),
      mpAcceptCallbackArg(NULL) {
    // Make socket non-blocking
    bool success = SetBlocking(false);
    K_ASSERT(success);

    // Add socket to global list
    sSocketList.PushBack(this);
}

/**
 * Destructor
 */
AsyncSocket::~AsyncSocket() {
    // Erase socket from list
    sSocketList.Remove(this);
}

/**
 * Connects to another socket
 *
 * @param addr Remote address
 * @return Success
 */
bool AsyncSocket::Connect(const SOSockAddr& addr) {
    mPeer = addr;
    mTask = Task_Connecting;
    // Connect doesn't actually happen on this thread
    return false;
}

/**
 * Accepts remote connection
 *
 * @return New socket
 */
AsyncSocket* AsyncSocket::Accept() {
    mTask = Task_Accepting;
    // Accept doesn't actually happen on this thread
    return NULL;
}

/**
 * Receives data from specified connection
 *
 * @param dst Destination buffer
 * @param len Buffer size
 * @param[out] addr Sender address
 * @return Bytes received (< 0 if failure)
 */
s32 AsyncSocket::RecieveImpl(void* dst, std::size_t len, SOSockAddr* addr) {
    // No packets available
    if (mRecvPackets.Empty()) {
        return SO_EWOULDBLOCK;
    }

    // Next packet
    Packet& packet = mRecvPackets.Back();

    // Packet not yet received
    if (!packet.IsWriteComplete()) {
        return SO_EWOULDBLOCK;
    }

    // Copy out packet data
    std::size_t bytes = packet.Read(dst, len);

    // Remove packet if completely read
    if (packet.IsReadComplete()) {
        mRecvPackets.PopBack();
        delete &packet;
    }

    return bytes;
}

/**
 * Sends data to specified connection
 *
 * @param src Source buffer
 * @param len Buffer size
 * @param addr Destination address
 * @return Bytes sent (< 0 if failure)
 */
s32 AsyncSocket::SendImpl(const void* src, std::size_t len,
                          const SOSockAddr* addr) {
    // Create new packet
    Packet::Header header;
    header.capacity = len;
    Packet* packet = new Packet(header);
    mSendPackets.PushFront(packet);

    // Copy in data
    return packet->Write(src, len);
}

/**
 * Searches for the next packet which should be received from the socket
 */
Packet* AsyncSocket::FindPacketForRecv() {
    for (TList<Packet>::Iterator it = mRecvPackets.Begin();
         it != mRecvPackets.End(); it++) {
        // Incomplete packet means more data can be received
        if (!it->IsWriteComplete()) {
            return &*it;
        }
    }

    // All packets are complete
    return NULL;
}

/**
 * Receives packet data over socket
 */
void AsyncSocket::CalcRecv() {
    while (true) {
        // Receive data for the next packet
        Packet* packet = FindPacketForRecv();
        if (packet != NULL) {
            packet->Receive(mHandle);
            continue;
        }

        // No existing packets, attempt to read new packet header
        Packet::Header header;
        s32 result = LibSO::Read(mHandle, &header, sizeof(Packet::Header));

        // Packet header found, enqueue new packet
        if (result == sizeof(Packet::Header)) {
            mRecvPackets.PushFront(new Packet(header));
            continue;
        }

        K_WARN_EX(result > 0, "Found extraneous data: %d bytes", result);

        // No new packet found, stop receiving more packets
        return;
    }
}

/**
 * Sends packet data over socket
 */
void AsyncSocket::CalcSend() {
    while (!mSendPackets.Empty()) {
        Packet& packet = mSendPackets.Back();
        packet.Send(mHandle);

        // Remove packet if completely sent
        if (packet.IsWriteComplete()) {
            mSendPackets.PopBack();
            delete &packet;
        }
    }
}

} // namespace kiwi
