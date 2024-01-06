#ifndef EGG_CORE_ARCHIVE_H
#define EGG_CORE_ARCHIVE_H
#include "eggAssert.h"
#include "eggDisposer.h"
#include "types_egg.h"
#include "ut_list.h"

#include <revolution/ARC/arc.h>
#include <string.h>

namespace EGG {
struct Archive : Disposer {
    struct FileInfo {
        u32 offset;
        u32 length;
    };

    inline Archive() {
        INT_0x14 = 1;
        INT_0x10 = 0;
        memset(&mHandle, 0, sizeof(mHandle));

        if (!sIsArchiveListInitialized) {
            nw4r::ut::List_Init(&sArchiveList, offsetof(Archive, mNode));
            sIsArchiveListInitialized = true;
        }

        appendList(this);
    }

    inline static void appendList(Archive* p) {
        nw4r::ut::List_Append(&sArchiveList, p);
    }

    inline static void removeList(Archive* p) {
        nw4r::ut::List_Remove(&sArchiveList, p);
    }

    virtual ~Archive(); // at 0x8
    static Archive* mount(void*, Heap*, int);
    void* getFile(const char*, FileInfo*);
    bool initHandle(void*); // inlined

    int INT_0x10;
    int INT_0x14;
    ARCHandle mHandle;    // at 0x18
    nw4r::ut::Node mNode; // at 0x34

    static nw4r::ut::List sArchiveList;
    static bool sIsArchiveListInitialized;
};
} // namespace EGG

#endif
