#ifndef LIBKIWI_CORE_DVD_STREAM_H
#define LIBKIWI_CORE_DVD_STREAM_H
#include <libkiwi/core/kiwiFileStream.h>
#include <libkiwi/k_types.h>
#include <revolution/DVD.h>

namespace kiwi {

/**
 * @brief DVD file stream
 */
class DvdStream : public FileStream {
public:
    /**
     * @brief Constructor
     */
    DvdStream() : FileStream(EOpenMode_Read) {}

    /**
     * @brief Constructor
     *
     * @param path File path
     * @param mode Open mode
     * @param create Create file if it doesn't exist
     */
    explicit DvdStream(const String& path) : FileStream(EOpenMode_Read) {
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

    bool Open(const String& path);
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

    /**
     * Required byte-alignment
     */
    virtual s32 GetSizeAlign() const {
        return 32;
    }
    virtual s32 GetOffsetAlign() const {
        return 4;
    }
    virtual s32 GetBufferAlign() const {
        return 32;
    }

private:
    virtual void SeekImpl(ESeekDir dir, s32 offset);
    virtual s32 ReadImpl(void* dst, u32 size);
    virtual s32 WriteImpl(const void* src, u32 size);
    virtual s32 PeekImpl(void* dst, u32 size);

private:
    DVDFileInfo mFileInfo; // DVD handle
};

} // namespace kiwi

#endif
