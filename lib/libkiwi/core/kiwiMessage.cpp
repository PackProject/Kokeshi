#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Constructor
 *
 * @param bin Binary file (KMSG)
 */
Message::Message(const void* bin) {
    K_ASSERT(bin != NULL);
    Deserialize(bin);
}

/**
 * @brief Destructor
 */
Message::~Message() {}

/**
 * @brief Deserialize binary message file
 *
 * @param bin Binary message (KMSG)
 */
void Message::DeserializeImpl(const Header& bin) {
    // Find first block
    const Block* block = AddToPtr<const Block>(&bin, bin.block.size);

    // Parse blocks
    for (int i = 0; i < bin.numBlocks; i++) {
        // Check block kind
        switch (block->kind) {
        case DESCBlock::scKind:
            mpDescBlock = static_cast<const DESCBlock*>(block);
            break;
        case DATABlock::scKind:
            mpDataBlock = static_cast<const DATABlock*>(block);
            break;
        default:
            K_ASSERT_EX(false, "Unknown block kind: %s (%08X)", block->Kind(),
                        block->kind);
            break;
        }

        // Advance block pointer
        block = AddToPtr<const Block>(block, block->size);
    }
}

/**
 * @brief Serialize binary message file
 *
 * @param bin Binary message (KMSG)
 */
void Message::SerializeImpl(Header& bin) const {
    K_ASSERT_EX(false, "Not supported.");
}

/**
 * @brief Get message text by ID
 *
 * @param id Message ID
 * @return Message text
 */
const wchar_t* Message::GetMessage(u32 id) const {
    K_ASSERT(id < mpDescBlock->numMsg);

    return AddToPtr<const wchar_t>(mpDataBlock->poolData,
                                   mpDescBlock->msgOffsets[id]);
}

} // namespace kiwi
