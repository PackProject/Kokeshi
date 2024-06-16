#ifndef LIBKIWI_CORE_FILE_STREAM_H
#define LIBKIWI_CORE_FILE_STREAM_H
#include <libkiwi/core/kiwiIStream.h>
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief File access type
 */
enum EOpenMode { EOpenMode_Read, EOpenMode_Write, EOpenMode_RW };

/**
 * @brief Stream to a file
 */
class FileStream : public IStream {
public:
    /**
     * @brief Constructor
     *
     * @param mode Open mode
     */
    explicit FileStream(EOpenMode mode) : mOpenMode(mode) {}

    /**
     * @brief Gets the size of the currently open file
     */
    virtual u32 GetSize() const = 0;

    /**
     * @brief Tests whether the stream has hit the end-of-file
     */
    virtual bool IsEOF() const {
        return mPosition >= GetSize();
    }

protected:
    EOpenMode mOpenMode; //!< File access type
};

//! @}
} // namespace kiwi

#endif
