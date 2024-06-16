#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Deserializes binary contents after ensuring they belong to this
 * class
 *
 * @param pBin Binary contents
 */
void IBinary::Deserialize(const void* pBin) {
    K_ASSERT(pBin != nullptr);
    const Header& rHeader = *static_cast<const Header*>(pBin);

    // Check header kind
    K_ASSERT_EX(GetBinaryKind() == rHeader.block.kind,
                "Not for this class. Got %08X (%s)", rHeader.block.kind,
                rHeader.block.Kind());

    // Check header version
    K_ASSERT_EX(GetVersion() == rHeader.version,
                "Not for this version. Expected %04X, got %04X", GetVersion(),
                rHeader.version);

    DeserializeImpl(rHeader);
}

/**
 * @brief Serializes binary contents
 *
 * @param pBin Binary contents
 */
void IBinary::Serialize(void* pBin) const {
    K_ASSERT(pBin != nullptr);
    Header& rHeader = *static_cast<Header*>(pBin);

    rHeader.version = GetVersion();
    rHeader.block.kind = GetBinaryKind();

    SerializeImpl(rHeader);
}

} // namespace kiwi
