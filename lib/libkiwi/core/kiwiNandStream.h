#ifndef LIBKIWI_CORE_NAND_STREAM_H
#define LIBKIWI_CORE_NAND_STREAM_H
#include <libkiwi/core/kiwiFileStream.h>
#include <libkiwi/k_types.h>
#include <revolution/NAND.h>

namespace kiwi {

/**
 * @brief NAND file stream
 */
class NandStream : public FileStream {
public:
    /**
     * @brief Constructor
     */
    explicit NandStream(EOpenMode mode) : FileStream(mode) {}

    /**
     * @brief Constructor
     *
     * @param path File path
     * @param mode Open mode
     */
    NandStream(const String& path, EOpenMode mode) : FileStream(mode) {
        Open(path);
    }

    /**
     * @brief Destructor
     */
    virtual ~NandStream() {
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
        return true;
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
        return 1;
    }
    virtual s32 GetBufferAlign() const {
        return 32;
    }

private:
    virtual s32 ReadImpl(void* dst, u32 size);
    virtual s32 WriteImpl(const void* src, u32 size);
    virtual s32 PeekImpl(void* dst, u32 size);
    virtual void SeekImpl(ESeekDir dir, s32 offset);

private:
    NANDFileInfo mFileInfo; // NAND handle
};

} // namespace kiwi

#endif
