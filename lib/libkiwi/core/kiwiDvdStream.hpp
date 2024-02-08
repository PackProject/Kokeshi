#ifndef LIBKIWI_CORE_DVD_STREAM_H
#define LIBKIWI_CORE_DVD_STREAM_H
#include <libkiwi/core/kiwiFileStream.hpp>
#include <revolution/DVD.h>
#include <types.h>

namespace kiwi {

/**
 * @brief DVD file stream
 */
class DvdStream : public FileStream {
public:
    /**
     * @brief Constructor
     */
    DvdStream() {}

    /**
     * @brief Constructor
     *
     * @param path File path
     * @param mode Open mode
     * @param create Create file if it doesn't exist
     */
    DvdStream(const char* path) : FileStream(EOpenMode_Read) {
        Open(path);
    }

    /**
     * @brief Destructor
     */
    virtual ~DvdStream() {
        if (mIsOpen) {
            Close();
        }
    }

    bool Open(const char* path);
    virtual void Close();

    virtual u32 GetSize() const;

    virtual bool CanSeek() const {
        return true;
    }
    virtual bool CanRead() const {
        return true;
    }
    virtual bool CanWrite() const {
        return false;
    }
    virtual bool CanPeek() const {
        return true;
    }

    virtual s32 GetAlign() const {
        return 32;
    }

private:
    virtual void SeekImpl(ESeekDir dir, s32 offset);
    virtual s32 ReadImpl(void* dst, u32 size);
    virtual s32 WriteImpl(const void* src, u32 size);
    virtual s32 PeekImpl(void* dst, u32 size);

private:
    // DVD handle
    DVDFileInfo mFileInfo;

    // Buffered seek parameters
    ESeekDir mSeekDir;
    s32 mSeekOffset;
};

} // namespace kiwi

#endif
