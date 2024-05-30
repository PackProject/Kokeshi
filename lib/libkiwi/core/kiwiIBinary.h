#ifndef LIBKIWI_CORE_I_BINARY_H
#define LIBKIWI_CORE_I_BINARY_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>

/**
 * @brief Helper for creating version numbers
 */
#define K_VERSION(major, minor) ((major << 8) | minor)

namespace kiwi {

/**
 * @brief Interface for binary file serialization/deserialization
 */
class IBinary {
public:
    /**
     * @brief Base structure for all binary blocks
     */
    struct Block {
        /**
         * @brief Interpret kind as a string
         */
        String Kind() const {
            return String(reinterpret_cast<char*>(kind), sizeof(u32));
        }

        /* 0x00 */ u32 kind;
        /* 0x04 */ u32 size;
    };

    /**
     * @brief Base structure for all libkiwi file formats
     */
    struct Header {
        /* 0x00 */ Block block;
        /* 0x08 */ u16 version;
        /* 0x0A */ u16 numBlocks;
    };

public:
    /**
     * @brief Destructor
     */
    virtual ~IBinary() {}

    /**
     * @brief Get the kind/magic of this object
     */
    virtual u32 GetBinaryKind() const = 0;
    /**
     * @brief Get the serialized size of this object
     */
    virtual u32 GetBinarySize() const = 0;
    /**
     * @brief Get the expected version of this object
     */
    virtual u16 GetVersion() const = 0;

    /**
     * @brief Common behavior before internal deserialization (check kind,
     * version, etc.)
     *
     * @param bin Binary
     */
    void Deserialize(const void* bin) {
        K_ASSERT(bin != NULL);
        const Header& header = *static_cast<const Header*>(bin);

        // Check header kind
        K_ASSERT_EX(GetBinaryKind() == header.block.kind,
                    "Not for this class. Got %08X (%s)", header.block.kind,
                    header.block.Kind());

        // Check header version
        K_ASSERT_EX(GetVersion() == header.version,
                    "Not for this version. Expected %04X, got %04X",
                    GetVersion(), header.version);

        DeserializeImpl(header);
    }

    /**
     * @brief Common behavior before internal serialization (write kind,
     * version, etc.)
     *
     * @param bin Binary
     */
    void Serialize(void* bin) const {
        K_ASSERT(bin != NULL);
        Header& header = *static_cast<Header*>(bin);

        header.version = GetVersion();
        header.block.kind = GetBinaryKind();

        SerializeImpl(header);
    }

private:
    virtual void DeserializeImpl(const Header& header) = 0;
    virtual void SerializeImpl(Header& header) const = 0;
};

} // namespace kiwi

#endif
