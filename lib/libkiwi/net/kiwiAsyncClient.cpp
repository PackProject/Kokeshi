#include <libkiwi.h>

namespace kiwi {

/**
 * Client thread function (TCP protocol)
 */
void* AsyncClient::ThreadFuncTCP(void* arg) {
    K_ASSERT(arg != NULL);

    // Thread argument is this object
    AsyncClient* self = static_cast<AsyncClient*>(arg);

    while (true) {
        K_ASSERT(self->mpMsgBuffer != NULL);

        // Read packet
        std::size_t nrecv;
        if (self->RecieveBytes(self->mpMsgBuffer, Packet::MAX_SIZE, &nrecv)) {
            // 0 bytes means we disconnected from the server
            if (nrecv == 0 && self->mpDisconnectCallback != NULL) {
                self->mpDisconnectCallback(self->mpDisconnectCallbackArg);
            }
            // -1 bytes means the socket would have blocked
            else if (nrecv != -1 && self->mpReceiveCallback != NULL) {
                self->mpReceiveCallback(self->mpMsgBuffer, nrecv,
                                        self->mpReceiveCallbackArg);
            }
        }
    }
}

/**
 * Client thread function (UDP protocol)
 */
void* AsyncClient::ThreadFuncUDP(void* arg) {
    K_ASSERT(arg != NULL);
    K_ASSERT_EX(false, "Not implemented");
}

/**
 * Constructor
 *
 * @param family Protocol family
 * @param type Socket type
 */
AsyncClient::AsyncClient(SOProtoFamily family, SOSockType type)
    : AsyncSocket(family, type),
      mpMsgBuffer(NULL),
      mpReceiveCallback(NULL),
      mpReceiveCallbackArg(NULL),
      mpDisconnectCallback(NULL) {
    // Allocate full size buffer so we can receive any packet fully
    mpMsgBuffer = new u8[Packet::MAX_SIZE];

    // Determine thread function from socket type
    OSThreadFunc func = type == SO_SOCK_STREAM ? ThreadFuncTCP : ThreadFuncUDP;

    OSCreateThread(&mThread, func, this, mThreadStack + sizeof(mThreadStack),
                   sizeof(mThreadStack), OS_PRIORITY_MAX, 0);
    OSResumeThread(&mThread);
}

/**
 * Destructor
 */
AsyncClient::~AsyncClient() {
    if (OSIsThreadTerminated(&mThread)) {
        return;
    }

    OSDetachThread(&mThread);
    OSCancelThread(&mThread);

    delete mpMsgBuffer;
    mpMsgBuffer = NULL;
}

} // namespace kiwi
