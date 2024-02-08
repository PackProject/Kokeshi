#ifndef LIBKIWI_CORE_NAND_STREAM_H
#define LIBKIWI_CORE_NAND_STREAM_H
#include <libkiwi/core/kiwiFileStream.hpp>
#include <revolution/NAND.h>
#include <types.h>

namespace kiwi {

/**
 * @brief NAND file stream
 */
class NandStream : public FileStream {
public:
    /**
     * @brief Constructor
     */
    NandStream() {}

    /**
     * @brief Constructor
     *
     * @param path File path
     * @param mode Open mode
     * @param create Create file if it doesn't exist
     */
    NandStream(const char* path, EOpenMode mode, bool create = false)
        : FileStream(mode) {
        Open(path, create);
    }

    /**
     * @brief Destructor
     */
    virtual ~NandStream() {
        if (mIsOpen) {
            Close();
        }
    }

    bool Open(const char* path, bool create = false);
    virtual void Close();

    virtual void Seek(ESeekDir dir, s32 offset);
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

    virtual s32 GetAlign() const {
        return 32;
    }

private:
    virtual s32 ReadImpl(void* dst, u32 size);
    virtual s32 WriteImpl(const void* src, u32 size);
    virtual s32 PeekImpl(void* dst, u32 size);

private:
    // NAND handle
    NANDFileInfo mFileInfo;
};

} // namespace kiwi

#endif
