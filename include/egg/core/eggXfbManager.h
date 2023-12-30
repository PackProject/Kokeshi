#ifndef EGG_CORE_XFBMANAGER_H
#define EGG_CORE_XFBMANAGER_H
#include "eggXfb.h"
#include "types_egg.h"

namespace EGG {
class XfbManager {
public:
    XfbManager(Heap* heap) {}
    ~XfbManager() {}

    bool isReadytoCopy() const {
        return mListTail != mShowXfb && mListTail != mListHead;
    }

    bool attach(Xfb*);
    void copyEFB(bool);
    void setNextFrameBuffer();
    void postVRetrace();

private:
    Xfb* mListHead;  // at 0x0
    Xfb* mListTail;  // at 0x4
    Xfb* mCopiedXfb; // at 0x8
    Xfb* mShowXfb;   // at 0xC
};
} // namespace EGG

#endif
