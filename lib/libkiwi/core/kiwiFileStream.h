#ifndef LIBKIWI_CORE_FILE_STREAM_H
#define LIBKIWI_CORE_FILE_STREAM_H
#include <libkiwi/core/kiwiIStream.h>
#include <libkiwi/k_types.h>

namespace kiwi {

/**
 * @brief File access
 */
enum EOpenMode { EOpenMode_Read, EOpenMode_Write, EOpenMode_RW };

/**
 * @brief Stream to a physical file
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
     * @brief Destructor
     */
    virtual ~FileStream() {}

    /**
     * @brief Tests whether the stream has hit the end-of-file
     */
    virtual bool IsEOF() const {
        return mPosition >= GetSize();
    }

    virtual u32 GetSize() const = 0;

protected:
    EOpenMode mOpenMode; // File access type
};

} // namespace kiwi

#endif
