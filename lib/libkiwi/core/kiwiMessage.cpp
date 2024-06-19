#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Constructor
 *
 * @param pBin Binary file (KMSG)
 */
Message::Message(const void* pBin) {
    K_ASSERT(pBin != nullptr);
    Deserialize(pBin);
}

/**
 * @brief Gets the serialized size of this object
 */
u32 Message::GetBinarySize() const {
    K_ASSERT(mpDescBlock != nullptr);
    K_ASSERT(mpDataBlock != nullptr);

    return (sizeof(DESCBlock) + mpDescBlock->numMsg * sizeof(u32)) +
           (sizeof(DATABlock) + mpDataBlock->poolSize);
}

/**
 * @brief Deserializes binary contents (internal implementation)
 *
 * @param rHeader Binary file header
 */
void Message::DeserializeImpl(const Header& rHeader) {
    // Find first block
    const Block* block = AddToPtr<const Block>(&rHeader, rHeader.block.size);

    // Parse blocks
    for (int i = 0; i < rHeader.numBlocks; i++) {
        // Check block kind
        switch (block->kind) {
        case DESCBlock::KIND:
            mpDescBlock = static_cast<const DESCBlock*>(block);
            break;
        case DATABlock::KIND:
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
 * @brief Serializes binary contents (internal implementation)
 *
 * @param rHeader Binary file header
 */
void Message::SerializeImpl(Header& rHeader) const {
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
