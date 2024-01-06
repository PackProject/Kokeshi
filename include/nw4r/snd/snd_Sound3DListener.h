#ifndef NW4R_SND_SOUND_3D_LISTENER_H
#define NW4R_SND_SOUND_3D_LISTENER_H
#include "math_types.h"
#include "types_nw4r.h"

namespace nw4r {
namespace snd {
struct Sound3DListener {
    math::MTX34 mMatrix; // at 0x0

    float mInteriorSize;      // at 0x30
    float mMaxVolumeDistance; // at 0x34
    float mUnitDistance;      // at 0x38

    Sound3DListener();
};
} // namespace snd
} // namespace nw4r

#endif
