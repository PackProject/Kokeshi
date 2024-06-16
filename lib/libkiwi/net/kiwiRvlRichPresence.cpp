#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Constructor
 *
 * @param rClient Client app ID
 */
RvlRichPresence::RvlRichPresence(const String& rClient)
    : IRichPresence(rClient) {
    K_LOG("Not yet implemented\n");
}

/**
 * @brief Whether there is an ongoing connection the RPC host
 */
bool RvlRichPresence::IsConnected() const {
    K_LOG("Not yet implemented\n");
    return true;
}

/**
 * @brief Retreive the current Unix epoch time (in seconds)
 */
u64 RvlRichPresence::GetTimeNow() const {
    K_LOG("Not yet implemented\n");
    return 0;
}

/**
 * @brief Update Discord client/app ID
 */
void RvlRichPresence::UpdateClient() const {
    K_LOG("Not yet implemented\n");
}

/**
 * @brief Update Discord presence status
 */
void RvlRichPresence::UpdatePresence() const {
    K_LOG("Not yet implemented\n");
}

} // namespace kiwi
