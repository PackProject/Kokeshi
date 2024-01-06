#ifndef NW4R_G3D_DRAW1_MAT1_SHP_H
#define NW4R_G3D_DRAW1_MAT1_SHP_H
#include "g3d_resmat.h"
#include "g3d_resshp.h"
#include "types_nw4r.h"

namespace nw4r {
namespace g3d {
struct Draw1Mat1ShpSwap {};

void Draw1Mat1ShpDirectly(ResMat, ResShp, const math::MTX34*,
                          const math::MTX34*, u32, Draw1Mat1ShpSwap*,
                          G3DState::IndMtxOp*);
} // namespace g3d
} // namespace nw4r

#endif
