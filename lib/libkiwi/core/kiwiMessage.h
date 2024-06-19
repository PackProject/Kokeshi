#ifndef LIBKIWI_CORE_MESSAGE_H
#define LIBKIWI_CORE_MESSAGE_H
#include <libkiwi/core/kiwiIBinary.h>
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief Binary message file
 * @note Does not own file memory
 */
class Message : IBinary {
public:
    /**
     * @brief Constructor
     *
     * @param pBin Binary file (KMSG)
     */
    explicit Message(const void* pBin);

    /**
     * @brief Gets the kind/magic of this object
     */
    virtual u32 GetBinaryKind() const {
        return 'KMSG';
    }

    /**
     * @brief Gets the serialized size of this object
     */
    virtual u32 GetBinarySize() const;

    /**
     * @brief Gets the expected version of this object
     */
    virtual u16 GetVersion() const {
        return K_VERSION(1, 0);
    }

    /**
     * @brief Gets message text by ID
     *
     * @param id Message ID
     * @return Message text
     */
    const wchar_t* GetMessage(u32 id) const;

private:
    /**
     * @brief Message descriptor block
     */
    struct DESCBlock : Block {
        //! Block signature
        static const u32 KIND = 'DESC';

        /* 0x08 */ u32 numMsg;       //!< Number of offsets
        /* 0x0C */ u32 msgOffsets[]; //!< Message offsets
    };

    /**
     * @brief Message data block
     */
    struct DATABlock : Block {
        //! Block signature
        static const u32 KIND = 'DATA';

        /* 0x08 */ u32 poolSize;    //!< String pool data size
        /* 0x0C */ char poolData[]; //!< String pool data
    };

private:
    /**
     * @brief Deserializes binary contents (internal implementation)
     *
     * @param rHeader Binary file header
     */
    virtual void DeserializeImpl(const Header& rHeader);
    /**
     * @brief Serializes binary contents (internal implementation)
     *
     * @param rHeader Binary file header
     */
    virtual void SerializeImpl(Header& rHeader) const;

private:
    Header* mpHeader;             //!< Binary file header
    const DESCBlock* mpDescBlock; //!< Message descriptor block
    const DATABlock* mpDataBlock; //!< Message data pool block
};

//! @}
} // namespace kiwi

#endif
