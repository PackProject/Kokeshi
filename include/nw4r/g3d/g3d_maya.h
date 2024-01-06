#ifndef NW4R_G3D_MAYA_H
#define NW4R_G3D_MAYA_H
#include "g3d_anmtexsrt.h"
#include "types_nw4r.h"

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {
bool CalcTexMtx_Maya(math::MTX34*, bool, const TexSrt&, TexSrt::Flag);
}
} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif
