#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Constructor
 *
 * @param client Client app ID
 */
RvlRichPresence::RvlRichPresence(const String& client) : IRichPresence(client) {
    K_LOG("RvlRichPresence::RvlRichPresence");
    UpdateClient();
}

/**
 * @brief Destructor
 */
RvlRichPresence::~RvlRichPresence() {
    K_LOG("RvlRichPresence::~RvlRichPresence");
}

/**
 * @brief Whether there is an ongoing connection the RPC host
 */
bool RvlRichPresence::IsConnected() const {
    K_LOG("RvlRichPresence::IsConnected");
    return true;
}

/**
 * @brief Retreive the current Unix epoch time (in seconds)
 */
u64 RvlRichPresence::GetTimeNow() const {
    K_LOG("RvlRichPresence::IsConnected");
    return 0;
}

/**
 * @brief Update Discord client/app ID
 */
void RvlRichPresence::UpdateClient() const {
    K_LOG("RvlRichPresence::UpdateClient");

    if (!IsConnected()) {
        K_LOG("Not connected to rvl_rpc_host!");
        return;
    }
}

/**
 * @brief Update Discord presence status
 */
void RvlRichPresence::UpdatePresence() const {
    K_LOG("RvlRichPresence::UpdatePresence");

    if (!IsConnected()) {
        K_LOG("Not connected to rvl_rpc_host!");
        return;
    }
}

} // namespace kiwi
