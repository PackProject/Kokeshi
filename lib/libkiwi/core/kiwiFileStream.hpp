#ifndef LIBKIWI_CORE_FILE_STREAM_H
#define LIBKIWI_CORE_FILE_STREAM_H
#include <libkiwi/core/kiwiIStream.hpp>
#include <types.h>

namespace kiwi {

/**
 * @brief File access
 */
enum EOpenMode {
    EOpenMode_Invalid,
    EOpenMode_Read,
    EOpenMode_Write,
    EOpenMode_RW
};

/**
 * @brief Stream to a physical file
 */
class FileStream : public IStream {
public:
    /**
     * @brief Constructor
     */
    FileStream() : mOpenMode(EOpenMode_Invalid) {}

    /**
     * @brief Constructor
     *
     * @param mode Open mode
     */
    FileStream(EOpenMode mode) : mOpenMode(mode) {
        K_ASSERT(mode != EOpenMode_Invalid);
    }

    /**
     * @brief Destructor
     */
    virtual ~FileStream() {}

    virtual u32 GetSize() const = 0;

protected:
    // File access type
    EOpenMode mOpenMode;
};

} // namespace kiwi

#endif
