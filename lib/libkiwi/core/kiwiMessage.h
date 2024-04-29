#ifndef KIWI_CORE_MESSAGE_H
#define KIWI_CORE_MESSAGE_H
#include <libkiwi/core/kiwiIBinary.h>
#include <types.h>

namespace kiwi {

/**
 * @brief Binary message file, similar to BMG
 */
class Message : IBinary {
private:
    static const u32 scKind = 'KMSG';
    static const u32 scVersion = KIWI_VERSION(1, 0);

    /**
     * @brief Message descriptor
     */
    struct DESCBlock : Block {
        static const u32 scKind = 'DESC';

        /* 0x08 */ u32 numMsg;
        /* 0x0C */ u32 msgOffsets[];
    };

    /**
     * @brief Message pool data
     */
    struct DATABlock : Block {
        static const u32 scKind = 'DATA';

        /* 0x08 */ u32 poolSize;
        /* 0x0C */ char poolData[];
    };

public:
    Message(const void* bin);
    virtual ~Message();

    /**
     * @brief Get the kind/magic of this object
     */
    virtual u32 GetBinaryKind() const {
        return scKind;
    }

    /**
     * @brief Get the serialized size of this object
     */
    virtual u32 GetBinarySize() const {
        K_ASSERT(mpDescBlock != NULL);
        K_ASSERT(mpDataBlock != NULL);

        return (sizeof(DESCBlock) + mpDescBlock->numMsg * sizeof(u32)) +
               (sizeof(DATABlock) + mpDataBlock->poolSize);
    }

    /**
     * @brief Get the expected version of this object
     */
    virtual u16 GetVersion() const {
        return scVersion;
    }

    const wchar_t* GetMessage(u32 id) const;

private:
    virtual void DeserializeImpl(const Header& header);
    virtual void SerializeImpl(Header& header) const;

private:
    // Binary file header
    Header* mpHeader;
    // Message descriptor block
    const DESCBlock* mpDescBlock;
    // Message data pool block
    const DATABlock* mpDataBlock;
};

} // namespace kiwi

#endif
