#ifndef RP_UTILITY_MAP_DATA_H
#define RP_UTILITY_MAP_DATA_H
#include <Pack/RPTypes.h>

#include <egg/core.h>

//! @addtogroup rp_system
//! @{

class RPUtlMapObject {
public:
    u32 getID() const {
        return mId;
    }

private:
    u32 mId; // at 0x0
    // . . .
};

/**
 * @brief Pack map data (PMP)
 */
class RPUtlMapData {
public:
    static RPUtlMapData* mount(void* pData, u16 id, EGG::Heap* pHeap);

    RPUtlMapObject* getMapObject(u16 i) const;
    u32 getMapObjectNum() const;

private:
    void unpack();
};

//! @}

#endif
